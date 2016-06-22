using System;
using MySql.Data.MySqlClient;
using System.IO;
using System.Net;
using System.Collections.Generic;
using System.Net.Sockets;

namespace contaserver
{
	class MainClass
	{
		public static void Main(string[] args)
		{
			//Ottiene il nome del config file dai parametri, se non c'è usare /etc/contaserver.conf
			String conf;
			ContaServer serv;

			if (args.Length > 0)
				conf = args [0]; //Seconda stringa
			else 
				conf = "/etc/contaserver.conf";

			//Crea l'instanza del server e lo inizializza subito
			serv = new ContaServer (conf);
			serv.init ();

			//Stampa info di debug
			if (serv.debugInfo) serv.printInfo();

			//Inizia il ciclo di esecuzione del server
			serv.run ();
		}
	}
}
