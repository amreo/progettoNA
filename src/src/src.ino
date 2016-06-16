/*Progetto Nexapp:
Quando il sensore rileva una scatola Arduino legge il codice a barre su di essa
e chiede al database se lo conosce, se risponde si aumenta la quantità in magazzino
del prodotto corrispondente al codice a barre, se invece il codice a barre non viene
trovato nel database arduino manda un segnale di errore che sarà poi elaborato*/

#include "SPI.h"
#include "Ethernet.h"
#include "SD.h"
#define PIN_INPUT_IR 5
#define PIN_CLOCK_BR 3
#define PIN_DATA_BR 2
#define PIN_SELECT 4

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

byte mac[6]; 
IPAddress ip; 
IPAddress serverIP;
int port;

//Credenziali
char user[30];
char password[30];

//Time out della lettura barcode
int TIMEOUT_READING_BARCODE; //5 sec, timeout da quando inizia a vedere la scatola
int MAX_RETRY_CONNECT; //Numero di massime volte di riprovare la connessione

unsigned long now;  //tempo da quando arduino è partito	 		
bool timeExpired = false; //true se il tempo di lettura barcode è scaduto
bool found;
//Scatole viste
int positionBox = 0;
int LINEA;

//funzione per leggere il codice a barre
//restituisce true se letto con successo
boolean readBarcode(){
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
void connect()
{
  byte retry = 0; 
  bool success = false;
  //Ripete la connessione al massimo MAX_RETRY_CONNECT
  while (retry < MAX_RETRY_CONNECT || success)
  {
    retry++;
    //si connette al database
    if (true) //TODO: 
    {
      Serial.println("OK!");
      //invia una query di test 
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

  Serial.print(position);
  Serial.print(" ");
  Serial.println(msg);
}

//Funzione che incrementa il numero di oggetti prodotti all'ID/Barcode
void sendProductUpdate(int barcode)
{

}


//Restituisce true se il prodotto esiste nel database
bool checkProduct(int barcode)
{

}

//Funzione che restituisce l'indirizzo IP contenuto nel file filename
IPAddress readFileIP(char filename[])
{
  File dataFile = SD.open(filename);
  int numeri[4];
  if (dataFile)
  {
    for (int i=0; i<4; i++)
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
    int i;
    for (int i=0; i<6; i++)
      result[i] = 0;
    for (int i=0; i<12; i++)
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
	//pin di collegamento del barcode reader
	pinMode(PIN_CLOCK_BR, INPUT_PULLUP);
    	pinMode(PIN_DATA_BR, INPUT_PULLUP);
	
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
  readFileString("username.txt", user);
  readFileString("password.txt", password);
  LINEA = readFileInt("lineaproduzione.txt");
  MAX_RETRY_CONNECT = readFileInt("max-retry-connect.txt");
  TIMEOUT_READING_BARCODE = readFileInt("barcode-reading-timeout.txt");
  
	Ethernet.begin(mac, ip);
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
			sendLog(positionBox, "Scatola con codice insesistente o corrotto");
      sendProductUpdate(1);		
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
		WAITHIGH(PIN_INPUT_IR);		
	} 	
}
