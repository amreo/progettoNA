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
		public bool errorInfo = true;

		//Oggetti in uso
		private MySqlConnection conn; //mysql-contaserver
		private TcpListener listener; //contaserver-contatori
		private List<TcpClient> clientsList; //lista di client connessi

		//Query dei comandi
		const string LOG_QUERY = "INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info) VALUES ({0}, {1}, '{2}');";
		const string UPDATE_PRODUCT_QUERY = "UPDATE dati_produzione.output_catena SET numProdotti = numProdotti + 1 WHERE ID_prodotto = {0} AND linea={1};";
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
		public bool init()
		{
			//inizializza varie funzionalità
			initConfigurationFile ();
			if (!initMysqlConnection ())
				return false;
			initServerListener ();

			//Spedisce messaggio di log di avvenuta inizializzazione
			sendLogMessage("0","0","Avvenuto avvio di contaserver");
		
			//Restituisce true - successo
			return true;
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
				if (errorInfo) 
					Console.WriteLine ("Il file {0} non esiste", configFile);
				return; //Esce dal metodo
			}

			//Instanzia il lettore
			try {
				reader = new StreamReader (configFile);	
			} catch (Exception ex) {
				if (errorInfo) 
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
				//if (debugInfo) Console.WriteLine(line);
				//quindi fa il trim della linea per rimuovere gli spazi laterali
				line = line.Trim();
				//dovrebbe contenere o nulla (solo commento) o due parola (opzione valore)
				if (line == "") { //linea vuota
					//nulla
				} else if (line.IndexOf (' ') == -1) { //non contiene nessun spazio quindi parola attacata
					if (errorInfo) Console.WriteLine ("Errore in file di configurazione alla linea {0}, contiene una sola parola: {1}", i, rawline); 
				} else if (line.IndexOf (' ') != line.LastIndexOf (' ')) { //gli indici di entrambi //gli spazi sono diversi quindi ci sono almeno due spazi
					if (errorInfo) Console.WriteLine ("Errore in file di configurazione alla linea {0}, contiene troppe proprietà o valori (solo uno spazio): {1}", i, rawline);
				} else if (line.IndexOf (' ') == line.LastIndexOf (' ')) { //Lo spazio è lo stesos. dovrebbe essere giusto
					//caso corretto, separa le due stringhe e imposta il valore
					string[] words = line.Split(' ');
					setOptions (words [0], words [1]);
				} else { //Errore generico che non si dovrebbe verificare
					if (errorInfo) Console.WriteLine ("Errore in file di configurazione sconosciuto alla linea {0}: {1}", i, rawline);
				}
			}

			//chiude lo stream
			reader.Close();
		}

		/// <summary>
		/// Inizializza la connessione verso mysql
		/// </summary>
		public bool initMysqlConnection()
		{
			conn = new MySqlConnection ();
			conn.ConnectionString = string.Format("Server={0};Port={1};Database=dati_produzione;Uid={2};Pwd={3};", 
				mysqlIP, mysqlPort, username, password);

			try {
				conn.Open ();
				return true;
			} catch (Exception ex) {
				Console.WriteLine ("Errore nell'apertura della connessione verso mysql, {0}, connection string: {1}", ex.ToString (), conn.ConnectionString);
				return false;
			}
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
						if (errorInfo) Console.WriteLine ("Valore {0} non è valido per mysql.port", value);
					break;
				case "mysql.ip": //se key="mysql.ip" imposta l'ip di mysql
					if (IPAddress.TryParse (value, out tempIP)) //verifica che sia un indirizzo ip
						mysqlIP = tempIP;
					else
						if (errorInfo) Console.WriteLine ("Valore {0} non è valido per mysql.ip", value);
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
						if (errorInfo) Console.WriteLine ("Valore {0} non è valido per contaserver.port", value);
					break;
				case "contaserver.debug": //se key="contaserver.debug"
					if (bool.TryParse(value, out tempBool))
						debugInfo = tempBool;
					else
						if (errorInfo) Console.WriteLine ("Valore {0} non è valido per contaserver.debug", value);
					break;
				case "contaserver.error": //se key="contaserver.error"
					if (bool.TryParse(value, out tempBool))
						errorInfo = tempBool;
					else
						if (errorInfo) Console.WriteLine ("Valore {0} non è valido per contaserver.error", value);
					break;				

				default: //Parametri non riconosciuti
					if (errorInfo) Console.WriteLine ("Il parametro {0} con valore {1} non è stato riconosciuto", key, value);
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
		public bool run()
		{
			bool running = true;
			//Comincia a ascoltare
			try {
				listener.Start();	
			} catch (Exception ex) {
				if (errorInfo)
					Console.WriteLine ("Impossibili aprire la porta {0}, {1}", localPort, ex.ToString());
				return false;
			}

			//Ripete finchè è in esecuzione
			while (running) {
				//Verifica connessioni chiuse
				checkClosedConnection();
				//Verifica connessioni in ascolto
				checkPendingConnection ();
	
				//Verifica che non siano arrivati msg da gestire
				checkPendingMessage();
			}

			return true;
		}

		/// <summary>
		/// Verifica le connessioni di tutti i client se sono aperte o chiuse
		/// </summary>
		private void checkClosedConnection()
		{
			//Per ogni client nella lista
			for (int i = 0; i < clientsList.Count; i++) {
				//Se non è connesso...
				if (!clientsList [i].Connected) {
					//Chiude la connessione
					clientsList [i].Client.Shutdown (SocketShutdown.Both);
					//Rimuove il client dalla lista
					clientsList.RemoveAt (i);
				}
					
			}
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
				//Se ha ricevuto bytes
				if (client.Available > 0) {
					//Legge il contenuto del bytes
					string msg = readMsg(client);
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
			//Verifiche
			if (!msg.StartsWith ("$")) {
				if (errorInfo) Console.WriteLine ("Messaggio non valido. Non comincia per $: {0}", msg);
				return;
			} else if (!msg.EndsWith ("!")) {
				if (errorInfo) Console.WriteLine ("Messaggio non valido. Non finisce per !: {0}", msg);
				return;
			} 
			//Elimina dal messaggio il dollaro '$' iniziale e il punto esclamativo '!' finale
			msg = msg.Substring (1, msg.Length - 2);
			//Separa i componenti del messaggio per "::"
			string[] info = msg.Split(new String[] {"::"}, StringSplitOptions.None);
			//Verifica la quantità di elementi in info per irregolarità
			if (info.Length == 0) {
				if (errorInfo)
					Console.WriteLine ("Messaggio non valido. Non ci sono informazioni (no '::') : {0}", msg);
				return;				
			} else if (info.Length == 1) {
				if (errorInfo)
					Console.WriteLine ("Messaggio non valido. Non c'è solo una informazione ({1}) : {0}", msg, info [1]);
				return;
			} else if (info.Length >= 5) {
				if (errorInfo)
					Console.WriteLine ("Messaggio non valido. Ci sono più di 5 informazioni : {0}", msg);
				return;
			}

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
				default:
					if (errorInfo) Console.WriteLine ("Comando non valido {0} in {1}", info[0], msg);
					break;
			}
		}

		/// <summary>
		/// Esegue il comando LOG del protocollo CCS
		/// </summary>
		private void runLogCommandCCS(string[] info)
		{
			uint temp; // variabile utilizzata solo per tryparse

			//verifica il numero di informazioni
			if (info.Length != 4) {
				if (errorInfo)
					Console.WriteLine ("Ci sono troppe info per il comando LOG: {0}", string.Join ("::", info));
				return;
			} else if (!uint.TryParse (info [1], out temp)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro della linea non è un numero intero positivo: {0}", info [1]);
				return;			
			} else if (!uint.TryParse (info [2], out temp)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro della posizione non è un numero intero positivo: {0}", info [2]);
				return;
			}

			//Scrive un messagio di debug	
			if (debugInfo) 
				Console.WriteLine ("Messaggio di log: LINEA={0} POSIZIONE={1} MSG={2}", 
					info[1], info[2], info[3]);
			//Invia il comando SQL di log
			sendLogMessage(info[1], info[2], info[3]);
		}
		/// <summary>
		/// Esegue il comando Add del protocollo CCS
		/// </summary>
		private void runAddCommandCCS(string[] info)
		{
			uint temp; // variabile utilizzata solo per tryparse
			int temp2; // variabile utilizzato solo per tryparse

			//verifica il numero di informazioni
			if (info.Length != 3) {
				if (errorInfo)
					Console.WriteLine ("Ci sono troppe info per il comando ADD: {0}", string.Join ("::", info));
				return;
			} else if (!uint.TryParse (info [1], out temp)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro della linea non è un numero intero positivo: {0}", info [1]);
				return;			
			} else if (!int.TryParse (info [2], out temp2)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro del barcode non è un numero intero: {0}", info [2]);
				return;
			}		

			//Scrive un messagio di debug	
			if (debugInfo) 
				Console.WriteLine ("Messaggio di aggiunta: LINEA={0} BARCODE={1}", 
					info[1], info[2]);
			//Invia i comandi SQL
			sendSQLCommand(String.Format(UPDATE_PRODUCT_QUERY, info[2], info[1]));
		}
		/// <summary>
		/// Esegue il comando Check del protocollo CCS
		/// </summary>
		private void runCheckCommandCCS(string[] info, TcpClient client)
		{
			int temp2; // variabile utilizzato solo per tryparse

			//verifica il numero di informazioni
			if (info.Length != 2) {
				if (errorInfo)
					Console.WriteLine ("Ci sono troppe info per il comando CHECK: {0}", string.Join ("::", info));
				return;		
			} else if (!int.TryParse (info [1], out temp2)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro del barcode non è un numero intero: {0}", info [1]);
				return;
			}

			//Scrive un messagio di debug
			if (debugInfo) 
				Console.WriteLine ("Messaggio di controllo barcode BARCODE={0}", 
					info[1]);
			//Conta il numero di prodotti
			int n = countProduct (info [1]);
			//Scrive in output il risultato
			sendMsg(client, string.Format (BOOL_MSG, n == 0 ? 'F' : 'T')); 
		}
		/// <summary>
		/// Esegue il comando Checked-add del protocollo CCS
		/// </summary>
		private void runCheckedAddCommandCCS(string[] info)
		{
			uint temp; // variabile utilizzata solo per tryparse
			int temp2; // variabile utilizzato solo per tryparse

			//verifica il numero di informazioni
			if (info.Length != 4) {
				if (errorInfo)
					Console.WriteLine ("Ci sono troppe info per il comando CHECKED-ADD: {0}", string.Join ("::", info));
				return;
			} else if (!uint.TryParse (info [1], out temp)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro della linea non è un numero intero positivo: {0}", info [1]);
				return;			
			} else if (!uint.TryParse (info [2], out temp)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro del posizione non è un numero intero positivo: {0}", info [2]);
				return;
			} else if (!int.TryParse (info [3], out temp2)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro del posizione non è un numero intero: {0}", info [3]);
				return;
			}

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
				sendSQLCommand (String.Format (UPDATE_PRODUCT_QUERY, 1, 0));				
			} else {
				sendSQLCommand (String.Format (UPDATE_PRODUCT_QUERY, info[3], info[1]));					
			}
		}
		/// <summary>
		/// Esegue il comando Check del protocollo CCS
		/// </summary>
		private void runConfigCommandCCS(string[] info, TcpClient client)
		{
			uint temp; // variabile utilizzata solo per tryparse 

			//Esegue la verifica delle informazioni
			if (info.Length != 2) {
				if (errorInfo)
					Console.WriteLine ("Ci sono troppe info per il comando CONFIG: {0}", string.Join ("::", info));
				return;
			} else if (!uint.TryParse (info [1], out temp)) {
				if (errorInfo)
					Console.WriteLine ("Il parametro IDStazione non è un numero intero positivo: {0}", info [1]);
				return;		
			}

			//Scrive un messagio di debug
			if (debugInfo) 
				Console.WriteLine ("Messaggio di configurazione stazione IDStazione={0}", 
					info[1]);
			//Conta il numero di prodotti
			Console.WriteLine(READ_CONFIG_QUERY, info[1]);
			string[] rows = sendSQLTableCommandOneRow(string.Format(READ_CONFIG_QUERY, info[1]), 3);
			Console.WriteLine("dopo sendSQLTableCommandOneRow");
			//Scrive in output il risultato
			sendMsg(client, string.Format (CONFIG_RETURN_MSG, rows[1].PadLeft(3,'0'), rows[2].PadLeft(5,'0'))); 
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
			//Crea una nuova instanza di MySqlCommand 
			MySqlCommand cmd = new MySqlCommand (string.Format("SELECT COUNT(*) FROM dati_produzione.output_catena WHERE ID_prodotto = {0};", barcode), conn);
			//Esegue il comando e resituisce il valore
			object result = cmd.ExecuteScalar();
			//lo converte in un numero intero
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
			//Crea una nuova instanza di MySqlCommand che contiene la query
			MySqlCommand cmd = new MySqlCommand (query);
			cmd.Connection = conn;
			cmd.CommandType = System.Data.CommandType.Text;
			try {
				Console.WriteLine("Eseguendo cmd.ExecuteReader()");
				//Esegue il comando e ne restituisce un lettore di dati
				MySqlDataReader reader = cmd.ExecuteReader ();
				Console.WriteLine("dopo cmd.ExecuteReader()");
				string[] result = new string[n];
				//legge la prima riga e imposta le celle di result i valori corrispondenti
				reader.Read ();
				for (int i = 0; i < result.Length; i++)
					result [i] = reader.GetString (i);
				//chiude la connessione e restituisce il risultato
				reader.Close ();
				return result;
			} catch (Exception ex) {
				Console.WriteLine("{0}", ex.ToString());
				return null;
			}

		}

		/// <summary>
		/// Legge il pacchetto ricevuto dal client
		/// </summary>
		public string readMsg(TcpClient client)
		{
			//Legge il contenuto del pacchetto in bytes
			byte[] data = new byte[client.Client.Available];
			client.Client.Receive (data);
			//Converte i bytes in una stringa secondo la codifica ASCII in stringa e ne restituisce
			return System.Text.Encoding.ASCII.GetString (data);
		}
		/// <summary>
		/// Spedisce un messaggio al client
		/// </summary>
		public void sendMsg(TcpClient client, string str)
		{
			//Converte il messaggio (stringa) in un array di byte
			byte[] msg = System.Text.Encoding.ASCII.GetBytes (str);
			//Spedisce i byte
			client.Client.Send (msg);
		}
	}
}

