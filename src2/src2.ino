/*Progetto Nexapp:
Quando il sensore rileva una scatola Arduino legge il codice a barre su di essa
e chiede al database se lo conosce, se risponde si aumenta la quantità in magazzino
del prodotto corrispondente al codice a barre, se invece il codice a barre non viene
trovato nel database arduino manda un segnale di errore che sarà poi elaborato*/

#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"
#include "PS2Keyboard.h"
#define PIN_INPUT_IR 5
#define PIN_DATA_BR 2
#define PIN_CLOCK_BR 3

PS2Keyboard keyboard;

//Variabili che contengono i risultati dello scan
boolean scanCorrect = true;
long int scannedInt = 0;

//variabile per leggere il check di ritorno del database
byte irState;

byte mac[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }; //TODO: impostare mac address corretto
IPAddress ip(11,22,33,44); //TODO: impostare ip di arduino
IPAddress serverIP(11, 22, 33, 44); //TODO: impostare ip del server

Connector myConn; //connettore verso server mysql per inviare comandi SQL

//Credenziali
char user[] = "arduino";
char password[] = "arduino4you";

//Query da inviare
const char LOG_QUERY[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (-1, \'arduino si è connesso\');";
const char LOG_QUERY_MODEL[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (%d, \'%s\');";
const char UPDATE_QUERY_MODEL[] = "UPDATE dati_produzione.output_catena SET numProdotti = numProdotti + 1 WHERE ID_prodotto = %d;";
const char CHECK_PRODUCT_QUERY_MODEL[] = "SELECT ID_prodotto FROM dati_produzione.output_catena WHERE ID_prodotto = %d;";

//Time out della lettura barcode
const int TIMEOUT_READING_BARCODE = 5000; //5 sec, timeout da quando inizia a vedere la scatola
const int MAX_RETRY_CONNECT = 3; //Numero di massime volte di riprovare la connessione

int now;  //tempo da quando arduino è partito	 		
bool timeExpired = false; //true se il tempo di lettura barcode è scaduto
bool found;
//Scatole viste
int positionBox = 0;


//funzione per leggere il codice a barre
//restituisce true se letto con successo
boolean readBarcode()
{	
	//verifica che ci sono dati in arrivo
	if (!keyboard.available())
	{
		scanCorrect = false;
	} else {
		byte dat, val;
		scannedInt = 0;
		//Continua a leggere le cifre finche incontra l'ENTER
		while ((dat = keyboard.read()) != PS2_ENTER)
		{
			val = dat - '0';
			scannedInt = scannedInt * 10 + val; //Sposta le cifre precedente a dx e ne aggiunge val
		}
		Serial.print("letto: ");
		Serial.println(scannedInt);
		scanCorrect = true;	
	}
	return scanCorrect;
}



//Funzione che instaura la connessione con il server mysql
void connect()
{
	int retry = 0;	
	bool success = false;
	//Ripete la connessione al massimo MAX_RETRY_CONNECT
	while (retry < MAX_RETRY_CONNECT || success)
	{
		retry++;
		//si connette al database
		if (myConn.mysql_connect(serverIP, 3306, user, password)) 
		{
			Serial.println("OK!");
			//invia una query di test	
			if (myConn.cmd_query(LOG_QUERY))
			{
				Serial.println("Query inviata con successo");
				success = true;							
			} else {
				Serial.println("Query fallita");
			};
		} 
	}
	
	if (!success)
	{
		//blocca l'arduino
		while (true) {
			Serial.println("Fallito.");
		}
	}
}

//Funzione che invia un messaggio di log al server mysql
void sendLog(int position, char msg[])
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//sostituisce %d e %s con la relativa parte		
	sprintf(query, LOG_QUERY_MODEL, position, msg);
	//verifica che è connesso per inviare la query
	if (!myConn.is_connected())
		connect();
	//SPERIAMO che non si disconnetta qui <--
	//invia stringa		
	if (myConn.cmd_query(query))
	{
		Serial.println("Query inviata con successo");
	} else {
		Serial.println("Query fallita");
	
	};

	Serial.print(position);
	Serial.print(" ");
	Serial.println(msg);
}

//Funzione che incrementa il numero di oggetti prodotti all'ID/Barcode
void sendProductUpdate(int barcode)
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//compone la query
	sprintf(query, UPDATE_QUERY_MODEL, barcode);
	//verifica che è connesso per inviare la query
	if (!myConn.is_connected())
		connect();
	//SPERIAMO che non si disconnetta qui <--
	//la invia al database
	if (myConn.cmd_query(query))
	{
		Serial.print("Query inviata con successo: ");
		Serial.println(barcode);
	} else {
		Serial.println("Query fallita");
		connect();
	};
}


//Restituisce true se il prodotto esiste nel database
bool checkProduct(int barcode)
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//compone la query
	sprintf(query, CHECK_PRODUCT_QUERY_MODEL, barcode);
	//verifica che è connesso per inviare la query
	if (!myConn.is_connected())
		connect();
	//SPERIAMO che non si disconnetta qui <--
	//la invia al database
	if (myConn.cmd_query(query))
	{
		Serial.print("Query inviata con successo: ");
		Serial.println(barcode);
		//Conta il numero di righe
		int nRighe = 0;	
		myConn.get_columns();
		row_values *row = NULL;
		while ((row = myConn.get_next_row()) != NULL)
		{
			nRighe++;		 	
			myConn.free_row_buffer();
		}
		myConn.free_columns_buffer();		
		Serial.print("prodotto: ");
		Serial.print(barcode);
		Serial.print(" R=");
		Serial.println(nRighe);			
		return nRighe == 1? true : false;
	} else {
		Serial.println("Query fallita");
		connect();
	};
}

void setup()
{
	//messaggio seriale per varificare il corretto settaggio del barcode
	Serial.println("Barcode settato correttamente");
	
	pinMode(PIN_INPUT_IR, INPUT);
	Serial.begin(9600);
	Ethernet.begin(mac, ip);
  keyboard.begin(PIN_DATA_BR, PIN_CLOCK_BR);
	
	Serial.println(Ethernet.localIP());

	Serial.println("Connettendo...");
	connect();
}
void loop()
{
	irState = digitalRead(PIN_INPUT_IR);
 	if (irState == LOW)
 	{
 		positionBox++;
 		//E' probabile che la variabile di riferimento now sia da indicare come tipo unsigned long dato che la 
 		//funzione millis() conta i millisecondi da quando arduino è acceso
		now = millis();  //tempo da quando arduino è partito	 		
		timeExpired = false;
		
		// legge il codice a barre
 	    	while (!readBarcode()) { 
			//se da adesso fino a adesso di tempo fa c'è una differenza troppo grande
			//La scatole potrebbe non avere l'etichetta quindi errore
			//TODO: Verificare il funzionamento dato che la funzione millis() non si resetta al ripartire del loop 
			//nel caso sia così sarà da sostituire con un ciclo for
			if (millis() - now >= TIMEOUT_READING_BARCODE)
			{
				timeExpired = true;
				break;
			}
		}
		
		//Se è stato superato il timeout invia un messaggio di errore
		if (timeExpired)
		{
			sendLog(positionBox, "Scatola con codice insesistente o corrotto");
		} else {
			// manda il codice a barre al database
		 	// l'intero da mandare al database è scannedInt
			// se il codice a barre è nel database la variabile found diventa true			
			found = checkProduct(scannedInt);			 	

 			//se il codice a barre non è sul database la variabile found è falsa				
	 		if(found)
 			{
 				/*chiede al database il prodotto corrispondente al codice a barre*/
				sendProductUpdate(scannedInt);
 		
 			}	
		 	else
			{
	 			/*manda un errore*/
				sendLog(positionBox, "Scatola con barcode non registrato");
			}

		}
    while (digitalRead(PIN_INPUT_IR) == LOW) {}
	} 	
}
