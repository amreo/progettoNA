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
			System.IO.StreamWriter writer = new System.IO.StreamWriter (client.GetStream ());
			client.Connect ("127.0.0.1", 10000);
			int scelta = 99;
			string param1, param2, param3;

			while (scelta != 0) {
				Console.WriteLine ("0) esci");
				Console.WriteLine ("1) add");
				Console.WriteLine ("2) log");
				Console.WriteLine ("3) checked-add");

				scelta = int.Parse (Console.ReadLine ());

				switch (scelta) {				
					case 0:
						break;
					case 1:
						Console.Write("Linea: ");
						param1 = Console.ReadLine ();
						Console.Write("Barcode: ");
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
						writer.Write ("$LOG::{0}::{1}::{2}!", param1, param2, param3);
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
				}

			}


		}
	}
}
