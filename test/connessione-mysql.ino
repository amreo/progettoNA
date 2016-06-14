#include "SPI.h"
#include "Ethernet.h"
#include "sha1.h"
#include "mysql.h"

byte mac[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }; //TODO: impostare mac address corretto
IPAddress ip(11,22,33,44); //TODO: impostare ip di arduino
IPAddress server_addr(11, 22, 33, 44); //TODO: impostare ip del server

Connector my_conn; //connettore verso server mysql per inviare comandi SQL

char user[] = "arduino";
char password[] = "arduino4you";

char logQuery[] = "INSERT INTO dati_produzione.log_eventi (Posizione, Info) VALUES (-1, \"arduino si è connesso\");"

void setup()
{
	Serial.begin(9600);
	Ethernet.begin(mac, ip);

	Serial.println(Ethernet.localIP());

	Serial.println("Connettendo...");
	if (my_conn.mysql_connect(server_addr, 3306, user, password)) 
	{
		Serial.println("OK!");
		my_conn.cmd_query(logQuery);
	} 
	else 
	{	 	
		while (true) {
			Serial.println("Fallito.");
		}
	}
}

void loop()
{

}
