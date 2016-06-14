<<<<<<< HEAD
#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"

byte mac[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }; //TODO: impostare mac address corretto
IPAddress ip(11,22,33,44); //TODO: impostare ip di arduino
IPAddress server_addr(11, 22, 33, 44); //TODO: impostare ip del server

Connector my_conn; //connettore verso server mysql per inviare comandi SQL

//Credenziali
const char user[] = "arduino";
const char password[] = "arduino4you";

//Query da inviare
const char logQuery[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (-1, \'arduino si è connesso\');";
const char logQueryModel[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (%d, \'%s\');";
const char updateQueryModel[] = "UPDATE dati_produzione.output_catena SET numProdotti = numProdotti + 1 WHERE ID_prodotto = %d;";

//Funzione che instaura la connessione con il server mysql
void stabilisciConnessione()
{
	if (my_conn.mysql_connect(server_addr, 3306, user, password)) 
	{
		Serial.println("OK!");
		if (my_conn.cmd_query(logQuery))
		{
			Serial.println("Query inviata con successo");
		} else {
			Serial.println("Query fallita");
		};
	} 
	else 
	{	 	
		while (true) {
			Serial.println("Fallito.");
		}
	}

}

//Funzione che invia un messaggio di log al server mysql
void sendLog(int posizione, char messaggio[])
{
	char query[128]; //Lunghezza max della query
	sprintf(query, logQueryModel, posizione, messaggio);
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
	char query[128]; //Lunghezza max della query
	sprintf(query, updateQueryModel, barcode);
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
	Serial.begin(9600);
	Ethernet.begin(mac, ip);

	Serial.println(Ethernet.localIP());

	Serial.println("Connettendo...");
	stabilisciConnessione();
}

void loop()
{
=======
/*Nexapp project : When the sensor notice a box arduino modify the infrared value
in true then the barcode scanner read the barcode on the box, arduino send it to
the MySQL database that compare it with the barcodes saved in its memory, if the 
barcode is valid arduino modify the value of producted items on the database; there
are different kinds of products so every barcode is relative to a product.
If the sensor notice a box but the barcode is not readable or isn't in the database
the program will notify an error*/

//variable for read the return of the database
boolean found = false;

const int outputIR = 5; 


>>>>>>> e8ee3e78f3369183302368fb95ce39dde0d16715


void setup()
{
	Serial.begin(9600);
	pinMode(outputIR, INPUT);
	
}

void loop()
{
 digitalRead(outputIR);
 if (outputIR == LOW)
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
