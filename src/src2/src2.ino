/*Progetto Nexapp:
Quando il sensore rileva una scatola Arduino legge il codice a barre su di essa
e chiede al database se lo conosce, se risponde si aumenta la quantità in magazzino
del prodotto corrispondente al codice a barre, se invece il codice a barre non viene
trovato nel database arduino manda un segnale di errore che sarà poi elaborato*/

#include "SPI.h"
#include "Ethernet.h"
#include "PS2Keyboard.h"
#include "SD.h"
#define PIN_INPUT_IR 5
#define PIN_DATA_BR 2
#define PIN_CLOCK_BR 3
#define PIN_SELECT 4
PS2Keyboard keyboard;

//Variabili che contengono i risultati dello scan
boolean scanCorrect = true;
long int scannedInt = 0;

//variabile per leggere il check di ritorno del database
byte irState;

byte mac[6];
IPAddress ip;
IPAddress serverIP;
int port;

//Client di connessione remota a contaserver
EthernetClient client;
//Query da inviare
const char LOG_QUERY[] = "$LOG::%d::%d::%s!";
//const char ADD_QUERY[] = "$ADD::%d::%d!";
//const char CHECK_QUERY[] = "$CHECK::%d!";
const char CHECKED_ADD_QUERY[] = "$CHECKED-ADD::%d::%d::%d!"

//Credenziali
//char user[30];
//char password[30];

//Time out della lettura barcode
int TIMEOUT_READING_BARCODE; //5 sec, timeout da quando inizia a vedere la scatola
int TIMEOUT_WAITING_RESPONSE; //2 sec, timeout max di aspettare la risposta dal contaserver
byte MAX_RETRY_CONNECT = 3; //Numero di massime volte di riprovare la connessione

unsigned long now;  //tempo da quando arduino è partito	 		
bool timeExpired = false; //true se il tempo di lettura barcode è scaduto
bool found;
//Scatole viste
long positionBox = 0;
byte LINEA;

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
	byte retry = 0; 
  	bool success = false;
  	//Ripete la connessione al massimo MAX_RETRY_CONNECT
  	while (retry < MAX_RETRY_CONNECT || success)
  	{
   		retry++;
    	//si connette al contaserver
    	if (client.connect(serverIP, port))
   		{
      		Serial.println("OK!");
      		//invia una query di test 
			sendLog(LINEA,positionBox, "Connesso al contaserver");
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

//Funzione che invia un messaggio di log al server contaserver
void sendLog(int linea, int position, char msg[])
{
	char query[128];
	sprintf(query, LOG_QUERY, linea, position, msg);
	
	Serial.println(query);	
	
	//Verifica che è connesso e eventualmente si ricconnette di nuovo	
	if (!client.connected())
		connect();
	client.print(query);

}

////Funzione che invia una notifica di rilevamento al server contaserver
//void sendProductAdd(int linea, int barcode)
//{
//	char query[128];
//	sprintf(query, ADD_QUERY, linea, barcode);
//	
//	Serial.println(query);	
//	
//	//Verifica che è connesso e eventualmente si ricconnette di nuovo	
//	if (!client.connected())
//		connect();
//	client.print(query);
//}

//Funzione che invia una notifica di rilevamento al server contaserver
void sendProductCheckedAdd(int linea, int position, int barcode)
{
	char query[128];
	sprintf(query, CHECKED_ADD_QUERY, linea, position, barcode);
	
	Serial.println(query);	
	
	//Verifica che è connesso e eventualmente si ricconnette di nuovo	
	if (!client.connected())
		connect();
	client.print(query);
}


//Restituisce true se il prodotto esiste nel database
//boolean sendCheckProduct(int linea, int position, int barcode)
//{
//	int i=0;
//	char query[128];
//	bool loopEnd = false;
//	char temp;
//	bool result;
//	sprintf(query, CHECK_QUERY, barcode);
//	
//	//Verifica che è connesso e eventualmente si ricconnette di nuovo	
//	if (!client.connected())
//		connect();
//	client.print(query);
//
//	//ottiene il tempo di adesso
//	now = millis();
//	
//	//Aspetta che è arrivato un messaggio
//	while (!loopEnd) { 
//		//se da adesso fino a adesso di tempo fa c'è una differenza troppo grande
//		//La scatole potrebbe non avere l'etichetta quindi errore
//		if (millis() - now >= TIMEOUT_READING_BARCODE)
//		{
//			timeExpired = true;
//			loopEnd = true;
//			sendLog(linea, position, "Risposta non ricevuta");
//			result = false;		
//		}
//		//Se sono disponibili almeno 3 caratteri leggibili (quindi presubilmente si ha ricevuto $F! o $T!) leggere i caratteri
//		if (client.available() >= 3)
//		{
//			//legge il carattere
//			temp = client.read();
//			//se è $ significa che è inizio del protocollo, quindi legge gli altri due
//			if (temp == '$')
//			{
//				temp = client.read();
//				if (temp == 'F')
//					result = false;
//				else if (temp == 'T')
//					result = true;
//				client.read();
//				loopEnd = true;	
//			}
//		}
//	}	
//
//	return result;
//}	

//Funzione che restituisce l'indirizzo IP contenuto nel file filename
IPAddress readFileIP(char filename[])
{
  File dataFile = SD.open(filename);
  byte numeri[4];
  if (dataFile)
  {
    for (byte i=0; i<4; i++)
    {
      numeri[i] = dataFile.parseInt();  
    }
    dataFile.close();
    return IPAddress(numeri[0], numeri[1], numeri[2], numeri[3]);
  }
  else
  {
    Serial.print("Errore apertura file: ");
    Serial.println(filename);
    return IPAddress(99,99,99,99);
  }
}

//Funzione che restituisce l'intero contenuto nel file filename
int readFileInt(char filename[])
{
  File dataFile = SD.open(filename);
  if (dataFile)
  {
    int n=0;
    while (dataFile.available())
      n = n*10 + (dataFile.read() - '0');
    dataFile.close();
   return n;  
  }
  else
  {
    Serial.print("Errore apertura file: ");
    Serial.println(filename);
    return -1;
  }
}

//Funzione che restituisce la stringa contenuta nel filename
void readFileString(char filename[], char str[])
{
 
  File dataFile = SD.open(filename);
  if (dataFile)
  {
    int i=0;
    while (dataFile.available())
    {
      str[i] = dataFile.read();
      i++;  
    }
    dataFile.close();
  }
  else
  {
    Serial.print("Errore apertura file: ");
    Serial.println(filename);
    str = "";
  } 
}

//Funzione che restituisce l'indirizzo MAC contenuto nel file filename
void readFileMac(char filename[], byte result[])
{
  File dataFile = SD.open(filename);
  if (dataFile)
  {
    char temp;
    byte i;
    for (byte i=0; i<6; i++)
      result[i] = 0;
    for (byte i=0; i<12; i++)
    {
      temp = dataFile.read();
      if (temp >= '0' && temp <= '9')
      {
        result[i/2] = result[i/2]*10 + (temp - '0');
      } else if (temp >= 'a' && temp <= 'f') {
        result[i/2] = result[i/2]*10 + (temp - 'a');
      } else if (temp >= 'A' && temp <= 'F') {
        result[i/2] = result[i/2]*10 + (temp - 'A');
      } else {
        i--; //Rifare il giro;  
      }
    }
  }
  else
  {
    Serial.print("Errore apertura file: ");
    Serial.println(filename);
    result[0] = 0x11;
    result[1] = 0x22;
    result[2] = 0x33;
    result[3] = 0x44;
    result[4] = 0x55;
    result[5] = 0x66;
  } 
}


void setup()
{
	//messaggio seriale per varificare il corretto settaggio del barcode
	Serial.println("Barcode settato correttamente");
	
	pinMode(PIN_INPUT_IR, INPUT);
	Serial.begin(9600);

	Serial.print("Inizializzando SD...");
	//Inizializza SD
	if (!SD.begin(PIN_SELECT)) {
		Serial.println("SD non presente. Configurazione di FALLBACK");
	}
	Serial.println("OK");

	readFileMac("mac.txt", mac);
	ip = readFileIP("localip.txt");
	serverIP = readFileIP("serverip.txt");
	port = readFileInt("serverport.txt");
	//readFileString("username.txt", user);
	//readFileString("password.txt", password);
	//LINEA = readFileInt("lineaproduzione.txt");
	MAX_RETRY_CONNECT = readFileInt("max-retry-connect.txt");
	//TIMEOUT_READING_BARCODE = readFileInt("barcode-reading-timeout.txt");
  
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
			if (millis() - now >= TIMEOUT_READING_BARCODE)
			{
				timeExpired = true;
				break;
			}
		}
		
		//Se è stato superato il timeout invia un messaggio di errore
		if (timeExpired)
		{
			sendLog(LINEA, positionBox, "Scatola con codice insesistente o corrotto");
			sendProductCheckedAdd(LINEA,positionBox, 1);
		} else {
			// manda il codice a barre al database
			sendProductCheckedAdd(LINEA, positionBox, scannedInt);
		}
    	while (digitalRead(PIN_INPUT_IR) == LOW) {}
	} 	
}
