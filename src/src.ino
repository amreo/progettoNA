/*Nexapp project : When the sensor notice a box arduino modify the infrared value
in true then the barcode scanner read the barcode on the box, arduino send it to
the MySQL database that compare it with the barcodes saved in its memory, if the 
barcode is valid arduino modify the value of producted items on the database; there
are different kinds of products so every barcode is relative to a product.
If the sensor notice a box but the barcode is not readable or isn't in the database
the program will notify an error*/

//variable for read the return of the database
boolean found = false;

const int outputIR = 5; 




void setup()
{
	Serial.begin(9600);
	pinMode(outputIR, INPUT);
	
}

void loop()
{
 digitalRead(outputIR);
 if (outputIR == LOW)
 {
 	
 	// read barcode by the scanner
 	/*........................*/
 	
    // send barcode to the database
 	/*........................*/
 	
 	 // if the barcode is in the database found variable becomes true
 	/* found = true; */
 	
 	//if the barcode hasn't been found the found variable keep false
 	
 	if(found == true)
 	{
 		/*ask to the database the kind of product corresponding to the barcode*/
 		/*....................*/
 		
 		/*modify the number of items produced*/
 		/*..................*/
 		
 	}
 	
 	else
 	{
 		/*send an error*/
 		/*........................*/
 	}
 }
 	
 
}
