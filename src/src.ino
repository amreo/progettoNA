/*Progetto Nexapp:
Quando il sensore rileva una scatola Arduino legge il codice a barre su di essa
e chiede al database se lo conosce, se risponde si aumenta la quantità in magazzino
del prodotto corrispondente al codice a barre, se invece il codice a barre non viene
trovato nel database arduino manda un segnale di errore che sarà poi elaborato*/


#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"
#define PIN_INPUT_IR 5


//variabile per leggere il check di ritorno del database
boolean found = false; 
byte irState;

byte mac[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }; //TODO: impostare mac address corretto
IPAddress ip(11,22,33,44); //TODO: impostare ip di arduino
IPAddress server_addr(11, 22, 33, 44); //TODO: impostare ip del server

Connector my_conn; //connettore verso server mysql per inviare comandi SQL

//Credenziali
char user[] = "arduino";
char password[] = "arduino4you";

//Query da inviare
const char logQuery[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (-1, \'arduino si è connesso\');";
const char logQueryModel[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (%d, \'%s\');";
const char updateQueryModel[] = "UPDATE dati_produzione.output_catena SET numProdotti = numProdotti + 1 WHERE ID_prodotto = %d;";

//Funzione che instaura la connessione con il server mysql
void stabilisciConnessione()
{
	//si connette al database
	if (my_conn.mysql_connect(server_addr, 3306, user, password)) 
	{
		Serial.println("OK!");
		//invia una query di test	
		if (my_conn.cmd_query(logQuery))
		{
			Serial.println("Query inviata con successo");
		} else {
			Serial.println("Query fallita");
		};
	} 
	else 
	{	 	
		//blocca l'arduino
		while (true) {
			Serial.println("Fallito.");
		}
	}

}

//Funzione che invia un messaggio di log al server mysql
void sendLog(int posizione, char messaggio[])
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//sostituisce %d e %s con la relativa parte		
	sprintf(query, logQueryModel, posizione, messaggio);
	//invia stringa		
	if (my_conn.cmd_query(query))
	{
		Serial.println("Query inviata con successo");
	} else {
		Serial.println("Query fallita");
		stabilisciConnessione();
	};
}

//Funzione che incrementa il numero di oggetti prodotti all'ID/Barcode
void sendProductUpdate(int barcode)
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//compone la query
	sprintf(query, updateQueryModel, barcode);
	//la invia al database
	if (my_conn.cmd_query(query))
	{
		Serial.print("Query inviata con successo: ");
		Serial.println(barcode);
	} else {
		Serial.println("Query fallita");
		stabilisciConnessione();
	};
}

void setup()
{
	pinMode(PIN_INPUT_IR, INPUT);
	Serial.begin(9600);
	Ethernet.begin(mac, ip);

	Serial.println(Ethernet.localIP());

	Serial.println("Connettendo...");
	stabilisciConnessione();
}
void loop()
{
	irState = digitalRead(PIN_INPUT_IR);
 	if (irState == LOW)
 	{
 	
 		// read barcode by the scanner
 		/*........................*/
 	
	    	// send barcode to the database
	 	/*........................*/
 	
 		// if the barcode is in the database found variable becomes true
 		/* found = true; */
 	
 		//if the barcode hasn't been found the found variable keep false
 	
	 	if(found == true)
 		{
 			/*ask to the database the kind of product corresponding to the barcode*/
 			/*....................*/
 			
 			/*modify the number of items produced*/
 			/*..................*/
 		
 		}
 	
	 	else
	 	{
	 		/*send an error*/
	 		/*........................*/
	 	}
	 } 	
}
