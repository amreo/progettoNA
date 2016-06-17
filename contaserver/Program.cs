using System;
using MySql.Data.MySqlClient;
using System.IO;
using System.Net;
using System.Collections.Generic;
using System.Net.Sockets;

namespace contaserver
{
	//
	class MainClass
	{

		public static void sendSQLCommand(MySqlConnection conn, string query)
		{
			MySqlCommand cmd = new MySqlCommand (query, conn);
			cmd.ExecuteNonQuery ();
		}
		public static int countProduct(MySqlConnection conn, int barcode)
		{
			MySqlCommand cmd = new MySqlCommand (string.Format("SELECT COUNT(*) FROM dati_produzione.output_catena WHERE ID_prodotto = {0};", barcode), conn);
			object result = cmd.ExecuteScalar();
			if (result != null)
			{
				int r = Convert.ToInt32(result);
				return r;
			}		
		}


		public static void Main (string[] args)
		{
			System.Net.Sockets.TcpListener listener = new System.Net.Sockets.TcpListener (new IPAddress(0), 10000);
			List<TcpClient> clients = new List<TcpClient>();
			listener.Start ();
			MySqlConnection conn = new MySqlConnection ();
			conn.ConnectionString = "Server=127.0.0.1;Port=1234;Database=dati_produzione;Uid=arduino;Pwd=arduino4you;";
			conn.Open ();
			while (true) 
			{
				if (listener.Pending())
				{
					clients.Add(listener.AcceptTcpClient());
					Console.WriteLine("Rilevato client: {0}", clients[clients.Count-1].Client.RemoteEndPoint);
				}

				foreach (TcpClient client in clients) 
				{
					if (client.Available > 0) {
						StreamReader reader = new StreamReader(client.GetStream ());
						string msg = reader.ReadToEnd ();
						Console.WriteLine (msg);
						msg = msg.Substring (1, msg.Length - 2);
						string[] info = msg.Split(new String[] {"::"}, StringSplitOptions.None);
				
						if (info[0] == "LOG")
						{
							
							Console.WriteLine ("Messaggio di log: LINEA={0} POSIZIONE={1} MSG={2}", info[1], info[2], info[3]);
							sendSQLCommand(conn, String.Format("INSERT INTO dati_produzione.log_eventi (Linea, Posizione, Info) VALUES ({0}, {1}, '{2}');",  
								info[1], info[2], info[3]));
						} 
						else if (info[0] == "ADD") 
						{
							Console.WriteLine ("Messaggio di aggiunta: LINEA={0} BARCODE={1}", info[1], info[2]);
							sendSQLCommand(conn, String.Format("UPDATE dati_produzione.output_catena SET numProdotti = numProdotti + 1 WHERE ID_prodotto = {0};",  
								info[2]));
							sendSQLCommand(conn, String.Format("UPDATE dati_produzione.contatori_dati SET numProdotti = numProdotti + 1 WHERE ID_prodotto = {0} AND ID = {1};",  
								info[1], info[2]));	
						} 
						else if (info[0] == "CHECK") 
						{
							Console.WriteLine ("Messaggio di controllo barcode BARCODE={0}", info[1]);
							int n = countProduct (conn, info [1]);
							StreamWriter writer = new StreamWriter (client.GetStream ());
							writer.WriteLine ("${0}!", n == 0 ? 'F' : 'T');
							writer.Close ();
						}

						reader.Close ();
					}
				
				}
			}
		}




	}
}
