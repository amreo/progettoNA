/* Skecth for verify the behavior of the ir sensor*/
#define PIN_OUTPUT_IR 5 

byte irState;

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_OUTPUT_IR, INPUT);
}

void loop()
{
 	irState = digitalRead(PIN_OUTPUT_IR);
	Serial.println(irState); 

 	if (irState == LOW)
 	{
 		Serial.println("Pacco rilevato");
 	}
}
