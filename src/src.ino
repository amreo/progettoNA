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
#define PIN_CLOCK_BR 3
#define PIN_DATA_BR 2


//Macro per aspettare che il pin rimane in uno stato
#define WAITLOW(pin) while (digitalRead(pin) != 0);
#define WAITHIGH(pin) while (digitalRead(pin) != 1);

//dichiarazione variabili per leggere il codice a barre sulla scatola
static volatile uint8_t head;
#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
unsigned long lastScan = 0;
boolean scanCorrect = true;
long int scannedInt = 0;
byte keymap[] = {0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, '`', 0,
        0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
        0, 0, 'z', 's', 'a', 'w', '2', 0,
        0, 'c', 'x', 'd', 'e', '4', '3', 0,
        0, ' ', 'v', 'f', 't', 'r', '5', 0,
        0, 'n', 'b', 'h', 'g', 'y', '6', 0,
        0, 0, 'm', 'j', 'u', '7', '8', 0,
        0, ',', 'k', 'i', 'o', '0', '9', 0,
        0, '.', '/', 'l', ';', 'p', '-', 0,
        0, 0, '\'', 0, '[', '=', 0, 0,
        0 /*CapsLock*/, 0 /*Rshift*/, 0 /*Enter*/, ']', 0, '\\', 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, '1', 0, '4', '7', 0, 0, 0,
        '0', '.', '2', '5', '6', '8', 0, 0 /*NumLock*/,
        0, '+', '3', '-', '*', '9', 0, 0,
        0, 0, 0, 0 };


//variabile per leggere il check di ritorno del database
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
const char checkProductQueryModel[] = "SELECT ID_prodotto FROM dati_produzione.output_catena WHERE ID_prodotto = %d;";

//Time out della lettura barcode
const int TIMEOUT_LETTURA_BARCODE = 5000; //5 sec, timeout da quando inizia a vedere la scatola
const int MAX_RETRY_CONNECT = 3; //Numero di massime volte di riprovare la connessione

int adesso;  //tempo da quando arduino è partito	 		
bool tempoScaduto = false; //true se il tempo di lettura barcode è scaduto
bool found;
//Scatole viste
int posizioneScatola = 0;


//funzione per leggere il codice a barre
//restituisce true se letto con successo
boolean letturaBarcode(){
	WAITLOW(PIN_CLOCK_BR);
  	WAITHIGH(PIN_CLOCK_BR);
  	unsigned char keycode = 0;
  	for (uint8_t i = 0; i < 8; i++) {
		WAITLOW(PIN_CLOCK_BR);
  	  	keycode >>= 1;
  	  	if (digitalRead(PIN_DATA_BR)) {
      			keycode |= 0x80;
    	  	}
		WAITHIGH(PIN_CLOCK_BR);
  	}
  	buffer[head++] = keycode;
  	WAITLOW(PIN_CLOCK_BR);
 	WAITHIGH(PIN_CLOCK_BR);
  	WAITLOW(PIN_CLOCK_BR);
  	WAITHIGH(PIN_CLOCK_BR);
  	unsigned long time = millis();
  	scanCorrect = true;
  	if (head == 5 && lastScan - time > 2000) {
    		scannedInt = keymap[buffer[3]] - '0';
    		if (scannedInt > 0) {
      			Serial.println();
      			Serial.println("***** Detected Scan *******");
      
      			/*scanned int è il codice da mandare al database*/
      			Serial.println(scannedInt);
      			Serial.println("*******");
    		} else {
      			scanCorrect = false;
    		}
    		head = 0;
    		lastScan = time;
    		for (int i = 0; i < 5; i++) buffer[i] = 0;
  	}

	return scanCorrect;
}



//Funzione che instaura la connessione con il server mysql
void stabilisciConnessione()
{
	int retry = 0;	
	bool success = false;
	//Ripete la connessione al massimo MAX_RETRY_CONNECT
	while (retry < MAX_RETRY_CONNECT || success)
	{
		retry++;
		//si connette al database
		if (my_conn.mysql_connect(server_addr, 3306, user, password)) 
		{
			Serial.println("OK!");
			//invia una query di test	
			if (my_conn.cmd_query(logQuery))
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
void sendLog(int posizione, char messaggio[])
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//sostituisce %d e %s con la relativa parte		
	sprintf(query, logQueryModel, posizione, messaggio);
	//verifica che è connesso per inviare la query
	if (!my_conn.is_connected())
		stabilisciConnessione();
	//SPERIAMO che non si disconnetta qui <--
	//invia stringa		
	if (my_conn.cmd_query(query))
	{
		Serial.println("Query inviata con successo");
	} else {
		Serial.println("Query fallita");
	
	};

	Serial.print(posizione);
	Serial.print(" ");
	Serial.println(messaggio);
}

//Funzione che incrementa il numero di oggetti prodotti all'ID/Barcode
void sendProductUpdate(int barcode)
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//compone la query
	sprintf(query, updateQueryModel, barcode);
	//verifica che è connesso per inviare la query
	if (!my_conn.is_connected())
		stabilisciConnessione();
	//SPERIAMO che non si disconnetta qui <--
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


//Restituisce true se il prodotto esiste nel database
bool checkProduct(int barcode)
{
	//stringa temporanea
	char query[128]; //Lunghezza max della query
	//compone la query
	sprintf(query, checkProductQueryModel, barcode);
	//verifica che è connesso per inviare la query
	if (!my_conn.is_connected())
		stabilisciConnessione();
	//SPERIAMO che non si disconnetta qui <--
	//la invia al database
	if (my_conn.cmd_query(query))
	{
		Serial.print("Query inviata con successo: ");
		Serial.println(barcode);
		//Conta il numero di righe
		int nRighe = 1; //TODO: mettero 0		
		//my_conn.get_columns();
		//row_values *row = NULL;
		//while ((row = my_conn.get_next_row()) != NULL)
		//{
		//	nRighe++;		 	
		//	my_conn.free_row_buffer();
		//}
		//my_conn.free_columns_buffer();		
		Serial.print("prodotto: ");
		Serial.print(barcode);
		Serial.print(" R=");
		Serial.println(nRighe);			
		return nRighe == 1? true : false;
	} else {
		Serial.println("Query fallita");
		stabilisciConnessione();
	};
}

void setup()
{
	//pin di collegamento del barcode reader
	pinMode(PIN_CLOCK_BR, INPUT_PULLUP);
    	pinMode(PIN_DATA_BR, INPUT_PULLUP);
	
	//messaggio seriale per varificare il corretto settaggio del barcode
	 Serial.println("Barcode settato correttamente");
	
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
 		posizioneScatola++;
		adesso = millis();  //tempo da quando arduino è partito	 		
		tempoScaduto = false;
		
		// legge il codice a barre
 	    	while (!letturaBarcode()) { 
			//se da adesso fino a adesso di tempo fa c'è una differenza troppo grande
			//La scatole potrebbe non avere l'etichetta quindi errore	
			if (millis() - adesso >= TIMEOUT_LETTURA_BARCODE)
			{
				tempoScaduto = true;
				break;
			}
		}
		
		//Se è stato superato il timeout invia un messaggio di errore
		if (tempoScaduto)
		{
			sendLog(posizioneScatola, "Scatola senza etichetta presubilmente");
		} else {
			// manda il codice a barre al database
		 	// l'intero da mandare al database è scannedInt
			found = checkProduct(scannedInt);			 	
	 		// se il codice a barre è nel database la variabile found diventa true
	 		/* found = true; */
 			
 			//se il codice abarre non è sul database si mentiane la variabile found falsa			
	 		if(found)
 			{
 				/*chiede al database il prodotto corrispondente al codice a barre*/
				sendProductUpdate(scannedInt);
 		
 			}	
		 	else
			{
	 			/*manda un errore*/
	 			/*........................*/
				sendLog(posizioneScatola, "Scatola con barcode non registrato");
			}

		}
	} 	
}
