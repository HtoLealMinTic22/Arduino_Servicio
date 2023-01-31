
const int buttonPinD =  8 ;     // the number of the pushbutton pin
const int buttonPinC =  9 ;     // the number of the pushbutton pin
const int buttonPinB = 10 ;     // the number of the pushbutton pin
const int buttonPinA = 11 ;     // the number of the pushbutton pin


const int ledPin =  5;        // the number of the LED pin

                             // variables will change:
int buttonStateA = 0;         // variable for reading the pushbutton status
int buttonStateB = 0;         // variable for reading the pushbutton status
int buttonStateC = 0;         // variable for reading the pushbutton status
int buttonStateD = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);    // initialize the LED pin as an output:
  pinMode(buttonPinA, INPUT);  // initialize the pushbutton pin as an input:
  pinMode(buttonPinB, INPUT);  // initialize the pushbutton pin as an input:
  pinMode(buttonPinC, INPUT);  // initialize the pushbutton pin as an input:
  pinMode(buttonPinD, INPUT);  // initialize the pushbutton pin as an input:    
}




void loop() {
  buttonStateA = digitalRead(buttonPinA);   // read the state of the pushbutton value:
  buttonStateB = digitalRead(buttonPinB);   // read the state of the pushbutton value:
  buttonStateC = digitalRead(buttonPinC);   // read the state of the pushbutton value:  
  buttonStateD = digitalRead(buttonPinD);   // read the state of the pushbutton value:


  // Boton A
  if (buttonStateA == HIGH) {
     Serial.println("-  Hola - B ") ; 
     digitalWrite(ledPin, HIGH)     ; // turn LED on:
    }
  else
    {
     //Serial.println("-  Hola -  2"); 
     //digitalWrite(ledPin, LOW); // turn LED off:
    }

  
  // Boton B
  if (buttonStateB == HIGH) {
     Serial.println("-  Hola - C ") ; 
     digitalWrite(ledPin, HIGH)     ; // turn LED on:
    }
  else 
    {
     //Serial.println("-  Hola -  2"); 
     //digitalWrite(ledPin, LOW); // turn LED off:
    }

  
  // Boton C
  if (buttonStateC == HIGH) {
     Serial.println("-  Hola - A ") ; 
     digitalWrite(ledPin, HIGH)     ; // turn LED on:
    }
  else
    {
     //Serial.println("-  Hola -  2"); 
     //digitalWrite(ledPin, LOW); // turn LED off:
    }


  // Boton D
  if (buttonStateD == HIGH) {
     Serial.println("-  Hola - D ") ; 
     digitalWrite(ledPin, HIGH)     ; // turn LED on:
    }
  else
    {
     //Serial.println("-  Hola -  2"); 
     //digitalWrite(ledPin, LOW); // turn LED off:
    }





}
