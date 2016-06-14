/**
 * Example: Hello, MySQL!
 *
 * This code module demonstrates how to create a simple database-enabled
 * sketch.
 */
#include <SPI.h>
#include <Ethernet.h>
#include <sha1.h>
#include <mysql.h>

/* Setup for Ethernet Library */
byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server_addr(10, 0, 1, 3); 

/* Setup for the Connector/Arduino */
Connector my_conn;        // The Connector/Arduino reference

char user[] = "root";
char password[] = "root";
char QUERY_SQL[] = "select @@version";

void setup() {  
  Ethernet.begin(mac_addr);
  Serial.begin(115200);
  delay(1000);
  Serial.print("Connecting...");
  if (my_conn.mysql_connect(server_addr, 3306, user, password)) {
    delay(500);
    Serial.println("Success!");
  } else {
    Serial.println("Connection failed.");
  }
}

int num_fails;
#define MAX_FAILED_CONNECTS 5

void soft_reset() {
  asm volatile("jmp 0");  
}  

void loop() {  
  delay(1000);
  if (my_conn.is_connected()) {
    my_conn.cmd_query(QUERY_SQL);
    my_conn.show_results();
    delay(1000);
    num_fails = 0;
  } else {
    my_conn.disconnect();
    Serial.println("Connecting...");
    if (my_conn.mysql_connect(server_addr, 3306, user, password)) {
      delay(500);
      Serial.println("Success!");
    } else {
      num_fails++;
      Serial.println("Connect failed!");
      if (num_fails == MAX_FAILED_CONNECTS) {
        Serial.println("Ok, that's it. I'm outta here. Rebooting...");
        delay(2000);
        soft_reset();
      }
    }
  }
}

