using System;
using System.Net.Sockets;
using System.Net;

namespace testProtoCCS
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			const string IP = "192.168.250.50";

			TcpClient client = new TcpClient ();
			client.Connect (IP, 1001);
			int scelta = 99;
			byte[] msg;
			string sr;
			string param1, param2, param3;

			while (scelta != 0) {
				Console.WriteLine ("0) esci");
				Console.WriteLine ("1) add");
				Console.WriteLine ("2) log");
				Console.WriteLine ("3) checked-add");
				Console.WriteLine ("4) check");
				Console.WriteLine ("5) config");
				Console.WriteLine ("6) shutdown connessione");
				Console.WriteLine ("7) reconnect");
				scelta = int.Parse (Console.ReadLine ());

				switch (scelta) {				
					case 0:
						client.Close ();
						break;
					case 1:
						Console.Write ("Linea: ");
						param1 = Console.ReadLine ();
						Console.Write ("Barcode: ");
						param2 = Console.ReadLine ();
						sr = string.Format ("$ADD::{0}::{1}!", param1, param2);
						msg = System.Text.Encoding.ASCII.GetBytes (sr);
						client.Client.Send (msg);
						break;
					case 2:
						Console.Write ("Linea: ");
						param1 = Console.ReadLine ();
						Console.Write ("Posizione: ");
						param2 = Console.ReadLine ();
						Console.Write ("MSG: ");
						param3 = Console.ReadLine ();
						sr = string.Format ("$LOG::{0}::{1}::{2}!", param1, param2, param3);
						msg = System.Text.Encoding.ASCII.GetBytes (sr);
						client.Client.Send (msg);
						break;
					case 3:
						Console.Write ("Linea: ");
						param1 = Console.ReadLine ();
						Console.Write ("Posizione: ");
						param2 = Console.ReadLine ();
						Console.Write ("Barcode: ");
						param3 = Console.ReadLine ();			
						sr = string.Format ("$CHECKED-ADD::{0}::{1}::{2}!", param1, param2, param3);
						msg = System.Text.Encoding.ASCII.GetBytes (sr);
						client.Client.Send (msg);
						break;

					case 4:
						Console.Write ("Barcode: ");
						param1 = Console.ReadLine ();
							
						sr = string.Format ("$CHECK::{0}!", param1);
						msg = System.Text.Encoding.ASCII.GetBytes (sr);
						client.Client.Send (msg);
						//TODO: TOFIX

						while (client.Available == 0) { }

						msg = new byte[client.Client.Available];
						client.Client.Receive (msg);
						sr = System.Text.Encoding.ASCII.GetString (msg);
						Console.WriteLine(sr);
						break;
					case 5:
						Console.Write ("IDStazione: ");
						param1 = Console.ReadLine ();
						
						sr = string.Format ("$CONFIG::{0}!", param1);
						msg = System.Text.Encoding.ASCII.GetBytes (sr);
						client.Client.Send (msg);
						//todo:tofix
						while (client.Available == 0) { }

						msg = new byte[client.Client.Available];
						client.Client.Receive (msg);
						sr = System.Text.Encoding.ASCII.GetString (msg);
						Console.WriteLine(sr);
						break;

					case 6:
						client.Client.Shutdown (SocketShutdown.Both);
						break;
					case 7:
						client.Client.Shutdown (SocketShutdown.Both);
						client = new TcpClient ();
						client.Connect (IP, 1001);
						break;
				}

			}


		}
	}
}
