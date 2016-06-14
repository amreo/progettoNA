/*Code for test the barcode scanner*/

#define WAITLOW(pin) while (digitalRead(pin) != 0);
#define WAITHIGH(pin) while (digitalRead(pin) != 1);

int clockPin = 3;
int dataPin = 4;
static volatile uint8_t head;
#define BUFFER_SIZE 45
static volatile uint8_t buffer[BUFFER_SIZE];
unsigned long lastScan = 0;
boolean scanCorrect = true;
int scannedInt = 0;
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

void setup() {
  pinMode(clockPin, INPUT_PULLUP);
  pinMode(dataPin, INPUT_PULLUP);
  
  Serial.begin(9600);
  Serial.println("smart kart");
  delay(2000);
}

void loop() {
  WAITLOW(clockPin);
  WAITHIGH(clockPin);
  unsigned char keycode = 0;
  for (uint8_t i = 0; i < 8; i++) {
    WAITLOW(clockPin);
    keycode >>= 1;
    if (digitalRead(dataPin)) {
      keycode |= 0x80;
    }
    WAITHIGH(clockPin);
  }
  buffer[head++] = keycode;
  WAITLOW(clockPin);
  WAITHIGH(clockPin);
  WAITLOW(clockPin);
  WAITHIGH(clockPin);
  unsigned long time = millis();
  scanCorrect = true;
  if (head == 5 && lastScan - time > 2000) {
    scannedInt = keymap[buffer[3]] - '0';
    if (scannedInt > 0) {
      Serial.println();
      Serial.println("***** Detected Scan *******");
      
      /*scanned int is the int to send to the database*/
      Serial.println(scannedInt);
      Serial.println("*******");
    } else {
      scanCorrect = false;
    }
    head = 0;
    lastScan = time;
    for (int i = 0; i < 5; i++) buffer[i] = 0;
  }
}
