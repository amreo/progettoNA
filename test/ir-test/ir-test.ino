/* Skecth for verify the behavior of the ir sensor*/
#define PIN_INPUT_IR 5 
#define PIN_LED 13
byte irState;

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_INPUT_IR, INPUT);
	pinMode(PIN_LED, OUTPUT);
}

void loop()
{
 	irState = digitalRead(PIN_INPUT_IR);
	//Serial.println(irState); 
	digitalWrite(PIN_LED, irState);
}
