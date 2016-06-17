#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //TODO: impostare mac
IPAddress ip(192,168,1,177);	//TODO: impostare IP

IPAddress server(1,1,1,1); //TODO: impostare IP

//Client di connessione remota a contaserver
EthernetClient client;

const char LOG_QUERY[] = "$LOG::%d::%d::%s!";


//Funzione che spedisce il messaggio di log al server e seriale
bool sendLog(int linea, int posizione, const char msg[])
{
	char query[128];
	sprintf(query, LOG_QUERY, linea, posizione, msg);
	
	Serial.println(query);	
	
	//Verifica che è connesso e quindi invia la query
	if (client.connected()) {
    	client.print(query); 
		return true;    
	} else {
		return false;
	}
}

void setup() {
  	// start the Ethernet connection:
  	Ethernet.begin(mac, ip);
  	// Open serial communications and wait for port to open:
  	Serial.begin(9600);  

  	// give the Ethernet shield a second to initialize:
  	delay(1000);
  	Serial.println("connecting...");

  	// if you get a connection, report back via serial:
  	if (client.connect(server, 10000)) {
 		Serial.println("connected");
		sendLog(-1,-1, "arduino connesso");
  	}	 
  	else {
    	// if you didn't get a connection to the server:
   	 	Serial.println("connection failed");
 	}	
}

oid loop()
{
	// if there are incoming bytes available 
  	// from the server, read them and print them:
  	if (client.available()) {
   		char c = client.read();
    	Serial.print(c);
  	}

  	// as long as there are bytes in the serial queue,
 	// read them and send them out the socket if it's open:
  	while (Serial.available() > 0) {
    	char inChar = Serial.read();
    	if (client.connected()) {
      		client.print(inChar); 
    	}
  	}

  	// if the server's disconnected, stop the client:
  	if (!client.connected()) {
    	Serial.println();
    	Serial.println("disconnecting.");
    	client.stop();
    	// do nothing:
    	while(true);
  	}
}
