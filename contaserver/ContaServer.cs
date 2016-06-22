using System;
using MySql.Data.MySqlClient;
using System.IO;
using System.Net;
using System.Collections.Generic;
using System.Net.Sockets;


namespace contaserver
{
	/// <summary>
	/// Classe che gestisce le funzioni da ContaServer
	/// Si occupa anche da MySQL client
	/// E la lettura delle info da /etc/contaserver.conf o ./contaserver.conf
	/// </summary>
	public class ContaServer
	{
		//File di configurazione
		//Di default è /etc/contaserver.conf
		private string configFile; 
		//Parametri di connessione a MySQL
		private short mysqlPort = 3306;
		private IPAddress mysqlIP = IPAddress.Parse ("127.0.0.1");
		private string username = "arduino";
		private string password = "arduino4you";
		//Parametri del contaserver
		private short localPort = 10000;
		public bool debugInfo = true;
	
		//Oggetti in uso
		private MySqlConnection conn; //mysql-contaserver
		private TcpListener listener; //contaserver-contatori
		private List<TcpClient> clientsList; //lista di client connessi

		//Query dei comandi
		const string LOG_QUERY = "INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info) VALUES ({0}, {1}, '{2}');";
		const string UPDATE_PRODUCT_QUERY = "UPDATE dati_produzione.output_catena SET numProdotti = numProdotti + 1 WHERE ID_prodotto = {0};";
		const string UPDATE_STATS_QUERY = "UPDATE dati_produzione.contatori_dati SET numProdotti = numProdotti + 1 WHERE ID = {0} AND ID_prodotto = {1};";
		const string READ_CONFIG_QUERY = "SELECT IDStazione, Lineaproduzione, Barcodetimeout FROM dati_produzione.settings WHERE IDStazione = {0};";

		//Modello stringa protocollo CCS
		const string BOOL_MSG = "${0}!";
		const string CONFIG_RETURN_MSG = "${0:D3}::{1:D5}!";

		/// <summary>
		/// Inizializza i parametri del client mysql e porta locale
		/// </summary>
		/// <param name="configFile">Percorso di configurazione file.</param>
		public ContaServer (string configFile)
		{
			//Imposta i parametri
			this.configFile = configFile;
		}

		/// <summary>
		/// Inizializza tutte le funzioni del server
		/// </summary>
		public void init()
		{
			//inizializza varie funzionalità
			initConfigurationFile ();
			initMysqlConnection();
			initServerListener ();

			//Spedisce messaggio di log di avvenuta inizializzazione
			sendLogMessage("0","0","Avvenuto avvio di contaserver");
		}
		/// <summary>
		/// Inizializza i parametri leggendo le info dal file
		/// </summary>
		public void initConfigurationFile()
		{ 
			StreamReader reader = null;
			int i = 0;

			//Verifica che esiste il file
			if (!File.Exists (configFile)) {
				Console.WriteLine ("Il file {0} non esiste", configFile);
				return; //Esce dal metodo
			}

			//Instanzia il lettore
			try {
				reader = new StreamReader (configFile);	
			} catch (Exception ex) {
				Console.WriteLine ("Errore nel aprire il file {0}:  {1}", configFile, ex.ToString ());
			}

			//Legge il file linea per linea fino alla fine dello stream
			while (!reader.EndOfStream) 
			{
				i++;
				//legge la linea
				string rawline = reader.ReadLine();
				string line = rawline;
				//scarta la parte con i commenti (da # in poi)
				if (line.IndexOf ('#') != -1)
					line = line.Remove (line.IndexOf ('#'));
				//messaggio di debug. linea letta
				if (debugInfo) Console.WriteLine(line);
				//quindi fa il trim della linea per rimuovere gli spazi laterali
				line = line.Trim();
				//dovrebbe contenere o nulla (solo commento) o due parola (opzione valore)
				if (line == "") { //linea vuota
					//nulla
				} else if (line.IndexOf (' ') == -1) { //non contiene nessun spazio quindi parola attacata
					Console.WriteLine ("Errore in file di configurazione alla linea {0}, contiene una sola parola: {1}", i, rawline); 
				} else if (line.IndexOf (' ') != line.LastIndexOf (' ')) { //gli indici di entrambi //gli spazi sono diversi quindi ci sono almeno due spazi
					Console.WriteLine ("Errore in file di configurazione alla linea {0}, contiene troppe proprietà o valori (solo uno spazio): {1}", i, rawline);
				} else if (line.IndexOf (' ') == line.LastIndexOf (' ')) { //Lo spazio è lo stesos. dovrebbe essere giusto
					//caso corretto, separa le due stringhe e imposta il valore
					string[] words = line.Split(' ');
					setOptions (words [0], words [1]);
				} else { //Errore generico che non si dovrebbe verificare
					Console.WriteLine ("Errore in file di configurazione sconosciuto alla linea {0}: {1}", i, rawline);
				}
			}

			//chiude lo stream
			reader.Close();
		}

		/// <summary>
		/// Inizializza la connessione verso mysql
		/// </summary>
		public void initMysqlConnection()
		{
			conn = new MySqlConnection ();
			conn.ConnectionString = string.Format("Server={0};Port={1};Database=dati_produzione;Uid={2};Pwd={3};", 
				mysqlIP, mysqlPort, username, password);
			conn.Open ();
		}

		/// <summary>
		/// Inizializza l'ascoltatore del server
		/// </summary>
		public void initServerListener()
		{
			listener = new TcpListener (new IPAddress(0), localPort);
			clientsList = new List<TcpClient>();
		}

		/// <summary>
		/// Imposta il parametro del contaserver usando le opzioni sotto
		/// </summary>
		/// <param name="key">Parametro da impostare</param>
		/// <param name="value">Valore da impostare</param>
		public void setOptions(string key, string value)
		{
			short tempInt;
			IPAddress tempIP;
			bool tempBool;
			
			key = key.ToLower();			

			switch (key) {
				case "mysql.port": //se key="mysql.port" imposta l'intero
					if (short.TryParse (value, out tempInt)) //verifica che sia una stringa e quindi la mette
						mysqlPort = tempInt;
					else
						Console.WriteLine ("Valore {0} non è valido per mysql.port", value);
					break;
				case "mysql.ip": //se key="mysql.ip" imposta l'ip di mysql
					if (IPAddress.TryParse (value, out tempIP)) //verifica che sia un indirizzo ip
						mysqlIP = tempIP;
					else
						Console.WriteLine ("Valore {0} non è valido per mysql.ip", value);
					break;
				case "mysql.username": //se key="mysql.username"
					username = value;
					break;
				case "mysql.password": //se key="mysql.password"
					password = value;
					break;
				case "contaserver.port": //se key="contaserver.port"
					if (short.TryParse (value, out tempInt)) //verifica che sia una stringa e quindi la mette
						localPort = tempInt;
					else
						Console.WriteLine ("Valore {0} non è valido per contaserver.port", value);
					break;
				case "contaserver.debug": //se key="contaserver.debug"
					if (bool.TryParse(value, out tempBool))
						debugInfo = tempBool;
					else
						Console.WriteLine ("Valore {0} non è valido per contaserver.debug", value);
					break;
				default: //Parametri non riconosciuti
					Console.WriteLine ("Il parametro {0} con valore {1} non è stato riconosciuto", key, value);
					break;
			}
		}

		/// <summary>
		/// Stampa i messaggi di 
		/// </summary>
		public void printInfo()
		{
			Console.WriteLine ("Config file = {0}", configFile);
			Console.WriteLine ("mysql.ip = {0}", mysqlIP);
			Console.WriteLine ("mysql.port = {0}", mysqlPort);
			Console.WriteLine ("mysql.username = {0}", username);
			Console.WriteLine ("mysql.password = {0}", password);
			Console.WriteLine ("contaserver.port = {0}", localPort);
		}

		/// <summary>
		/// Inizia il ciclo di esecuzione del server
		/// </summary>
		public int run()
		{
			bool running = true;
	
			listener.Start();
			//Ripete finchè è in esecuzione
			while (running) {
				//Verifica connessioni in ascolto
				checkPendingConnection ();
	
				//Verifica che non siano arrivati msg da gestire
				checkPendingMessage();
			}

			return 0;
		}

		/// <summary>
		/// Verifica se ci sono contatori che stanno cercando di connetersi al contaserver
		/// </summary>
		private void checkPendingConnection()
		{
			//Se il listener ha connessioni in attesa
			if (listener.Pending())
			{
				clientsList.Add(listener.AcceptTcpClient()); //Accetta il client
				sendLogMessage("0","0",string.Format("Connesso contatore {0}",  clientsList[clientsList.Count-1].Client.RemoteEndPoint));
				if (debugInfo) 
					Console.WriteLine("Connesso client: {0}", clientsList[clientsList.Count-1].Client.RemoteEndPoint);
			}
		}

		/// <summary>
		/// Verifica se ci sono messaggi in attesa da processare e quindi vengono processati
		/// </summary>
		private void checkPendingMessage()
		{
			
			//Controlla ogni client se ha messaggi in attesa
			foreach (TcpClient client in clientsList) 
			{
				Console.WriteLine ("client: {0} available: {1}", client.Client.RemoteEndPoint.ToString (), client.Available);

				//Se ha ricevuto bytes
				if (client.Available > 0) {
					Console.WriteLine("Sono entrato");
					//Legge il contenuto del bytes
					byte[] data = new byte[client.Client.Available];
					client.Client.Receive (data);
	
					string msg = System.Text.Encoding.ASCII.GetString (data);
					//e quindi chiude la connessione

					//messagio di debug
					if (debugInfo) Console.WriteLine (msg);

					//Individua il contenuto del protocollo
					parseMsgProtoCCS (msg, client);
				}
			}
		}

		/// <summary>
		/// Esegue l'azione associata al messaggio arrivato
		/// </summary>
		private void parseMsgProtoCCS(string msg, TcpClient client)
		{
			//Elimina dal messaggio il dollaro '$' iniziale e il punto esclamativo '!' finale
			msg = msg.Substring (1, msg.Length - 2);
			//Separa i componenti del messaggio per "::"
			string[] info = msg.Split(new String[] {"::"}, StringSplitOptions.None);
			//E quindi ne individua i componenti
			switch (info [0]) {
				case "LOG":
					runLogCommandCCS (info);
					break;
				case "ADD":
					runAddCommandCCS (info);
					break;
				case "CHECK":
					runCheckCommandCCS (info, client);
					break;
				case "CHECKED-ADD":
					runCheckedAddCommandCCS (info);
					break;
				case "CONFIG":
					runConfigCommandCCS (info, client);
					break;
			}
		}

		/// <summary>
		/// Esegue il comando LOG del protocollo CCS
		/// </summary>
		private void runLogCommandCCS(string[] info)
		{
			//Scrive un messagio di debug	
			if (debugInfo) 
				Console.WriteLine ("Messaggio di log: LINEA={0} POSIZIONE={1} MSG={2}", 
					info[1], info[2], info[3]);
			//Invia il comando SQL di log
			sendLogMessage(info[1], info[2], info[3]);
			Console.WriteLine("sono qui");
		}
		/// <summary>
		/// Esegue il comando Add del protocollo CCS
		/// </summary>
		private void runAddCommandCCS(string[] info)
		{
			//Scrive un messagio di debug	
			if (debugInfo) 
				Console.WriteLine ("Messaggio di aggiunta: LINEA={0} BARCODE={1}", 
					info[1], info[2]);
			//Invia i comandi SQL
			sendSQLCommand(String.Format(UPDATE_PRODUCT_QUERY, info[2]));
			sendSQLCommand(String.Format(UPDATE_STATS_QUERY, info[1], info[2]));	
		}
		/// <summary>
		/// Esegue il comando Check del protocollo CCS
		/// </summary>
		private void runCheckCommandCCS(string[] info, TcpClient client)
		{
			//Scrive un messagio di debug
			if (debugInfo) 
				Console.WriteLine ("Messaggio di controllo barcode BARCODE={0}", 
					info[1]);
			//Conta il numero di prodotti
			int n = countProduct (info [1]);
			//Scrive in output il risultato
			StreamWriter writer = new StreamWriter (client.GetStream ());
			writer.WriteLine (BOOL_MSG, n == 0 ? 'F' : 'T');
			writer.Close ();
		}
		/// <summary>
		/// Esegue il comando Checked-add del protocollo CCS
		/// </summary>
		private void runCheckedAddCommandCCS(string[] info)
		{
			//Scrive un messagio di debug	
			if (debugInfo) 
				Console.WriteLine ("Messaggio di aggiunta sicura: LINEA={0} POSIZIONE={1} BARCODE={2}", 
					info[1], info[2], info[3]);
			//Conta il numero di prodotti
			int n = countProduct (info [3]);

			//Se n=0 significa non presente e quindi errore
			if (n == 0) {
				sendLogMessage (info [1], info [2], 
					string.Format ("Barcode non esistente o corrotto: {0}", info [3]));
				sendSQLCommand (String.Format (UPDATE_PRODUCT_QUERY, 1));
				sendSQLCommand (String.Format (UPDATE_STATS_QUERY, info [1], 1));					
			} else {
				sendSQLCommand (String.Format (UPDATE_PRODUCT_QUERY, info[3]));
				sendSQLCommand (String.Format (UPDATE_STATS_QUERY, info [1], info[3]));					
			}
		}
		/// <summary>
		/// Esegue il comando Check del protocollo CCS
		/// </summary>
		private void runConfigCommandCCS(string[] info, TcpClient client)
		{
			//Scrive un messagio di debug
			if (debugInfo) 
				Console.WriteLine ("Messaggio di configurazione stazione IDStazione={0}", 
					info[1]);
			//Conta il numero di prodotti
			string[] rows = sendSQLTableCommandOneRow(string.Format(READ_CONFIG_QUERY, info[1]), 3);
			//Scrive in output il risultato
			StreamWriter writer = new StreamWriter (client.GetStream ());
			writer.WriteLine (CONFIG_RETURN_MSG, rows[1].PadLeft(3,'0'), rows[2].PadLeft(5,'0'));
			writer.Close ();
		}


		/// <summary>
		/// Invia un messaggio di log al server mysql
		/// </summary>
		public void sendLogMessage(string linea, string position, string msg)
		{
			sendSQLCommand (string.Format (LOG_QUERY, linea, position, msg));
		}

		/// <summary>
		/// Invia il comando SQL query e lo fa eseguire
		/// </summary>
		public void sendSQLCommand(string query)
		{
			MySqlCommand cmd = new MySqlCommand (query);
			cmd.Connection = conn;
			cmd.ExecuteNonQuery ();
		}
		/// <summary>
		/// Conta il numero di prodotti con quel barcode
		/// </summary>
		public int countProduct(string barcode)
		{
			MySqlCommand cmd = new MySqlCommand (string.Format("SELECT COUNT(*) FROM dati_produzione.output_catena WHERE ID_prodotto = {0};", barcode), conn);
			object result = cmd.ExecuteScalar();
			if (result != null) {
				int r = Convert.ToInt32 (result);
				return r;
			} else {
				return 0;
			}
		}
		/// <summary>
		/// Invia il comando SQL query e ne restituisce la prima riga
		/// </summary>
		public string[] sendSQLTableCommandOneRow(string query, int n)
		{
			MySqlCommand cmd = new MySqlCommand (query);
			cmd.Connection = conn;
			cmd.CommandType = System.Data.CommandType.TableDirect;
			MySqlDataReader reader = cmd.ExecuteReader ();
			string[] result = new string[n];
			reader.Read ();
			for (int i = 0; i < result.Length; i++)
				result [i] = reader.GetString (i);
			reader.Close ();

			return result;
		}

	}
}

