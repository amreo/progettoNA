using System;
using System.Net.Sockets;
using System.Net;

namespace testProtoCCS
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			TcpClient client = new TcpClient ();
			client.Connect ("127.0.0.1", 1001);
			System.IO.StreamWriter writer = new System.IO.StreamWriter (client.GetStream ());
			System.IO.StreamReader reader = new System.IO.StreamReader (client.GetStream ());
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
						writer.Write ("$ADD::{0}::{1}!", param1, param2);
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
						writer.Write ("$ADD::{0}::{1}::{2}!", param1, param2, param3);
						break;
					case 4:
						Console.Write ("Barcode: ");
						param1 = Console.ReadLine ();
						writer.Write ("$CHECK::{0}!", param1);
						while (reader.EndOfStream) { }
						Console.WriteLine(reader.ReadToEnd());
						break;
					case 5:
						Console.Write ("IDStazione: ");
						param1 = Console.ReadLine ();
						writer.Write ("$CONFIG::{0}!", param1);
						while (reader.EndOfStream) { }
						Console.WriteLine(reader.ReadToEnd());
						break;
				}

			}


		}
	}
}
