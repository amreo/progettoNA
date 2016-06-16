/*
 * Legge i vari parametri sull'SD, un file per ogni parametro
 */
 
#include <SPI.h>
#include <SD.h>
#include "Ethernet.h"
const int chipSelect = 4;

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

String readFileString(char filename[])
{
 
  File dataFile = SD.open(filename);
  if (dataFile)
  {
    String str;
    str = dataFile.readString();
    dataFile.close();
    return str;  
  }
  else
  {
    Serial.print("Errore apertura file: ");
    Serial.println(filename);
    return "";
  } 
}

void readFileMac(char filename[], int result[])
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

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  Serial.print("Initializing SD card...");
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  Serial.println(readFileIP("localip.txt"));
  Serial.println(readFileIP("serverip.txt"));
  Serial.println(readFileInt("serverport.txt"));
  Serial.println(readFileString("username.txt"));
  Serial.println(readFileString("password.txt"));
  Serial.println(readFileInt("lineaproduzione.txt"));
  int mac[6];
  readFileMac("mac.txt", mac);
  for (int i=0; i<6; i++)
    Serial.print(mac[i]);
  Serial.println("");
}

void loop() {
}

