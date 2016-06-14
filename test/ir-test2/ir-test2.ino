/* Skecth for verify the behavior of the ir sensor*/
#define PIN_INPUT_IR 5 
byte irState;

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_INPUT_IR, INPUT);
}

void loop()
{
	irState = digitalRead(PIN_INPUT_IR);	
	if (irState == LOW)
	{
		Serial.println("pacco rilevato (inizio)");		
		do {
			irState = digitalRead(PIN_INPUT_IR);
		} (irState == LOW);
		Serial.println("pacco rilevato (fine)");
	}

}
