
const int buttonPin = 8 ;     // the number of the pushbutton pin
//const int buttonPin = 18;     // the number of the pushbutton pin
const int ledPin =  5;        // the number of the LED pin

                             // variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);    // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT);  // initialize the pushbutton pin as an input:
}




void loop() {
  buttonState = digitalRead(buttonPin);   // read the state of the pushbutton value:

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
     Serial.println("-  Hola - D "); 
     digitalWrite(ledPin, HIGH); // turn LED on:
    }
  else 
    {
     Serial.println("-  Hola -  2"); 
     digitalWrite(ledPin, LOW); // turn LED off:
    }
}
