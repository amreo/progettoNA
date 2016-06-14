/* Skecth for verify the behavior of the ir sensor*/

const int outputIR = 5; 

void setup()
{
	Serial.begin(9600);
	pinMode(outputIR, INPUT);
	
}

void loop()
{
 
 Serial.println(digitalRead(outputIR));
 
 digitalRead(outputIR);
 
 if (outputIR == LOW)
 {
 	
 Serial.println("Pacco rilevato");
 
 }
 	
 
}
