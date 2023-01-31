// Motoreductor
// Sensor RX-480 - RadioFrecuencia
// Variacion de velocidad con potenciometro
#define potenciometro A5

int IN1 = 4 ;
int IN2 = 3 ;
int ENA = 5 ;

volatile boolean swEnciendePaso  ;
volatile boolean swReloj         ;


// Remoto
const int buttonPinD =  8 ;     // the number of the pushbutton pin
const int buttonPinC =  9 ;     // the number of the pushbutton pin
const int buttonPinB = 10 ;     // the number of the pushbutton pin
const int buttonPinA = 11 ;     // the number of the pushbutton pin
//const int ledPin =  5;        // the number of the LED pin

int buttonStateA = 0;         // variable for reading the pushbutton status
int buttonStateB = 0;         // variable for reading the pushbutton status
int buttonStateC = 0;         // variable for reading the pushbutton status
int buttonStateD = 0;         // variable for reading the pushbutton status
int PotEnt ;

void setup() {
   Serial.begin(9600);
   pinMode(pinPrueba1, OUTPUT);
   pinMode(pinPrueba2, OUTPUT);
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);   
   pinMode(ENA, OUTPUT);   
   pinMode(potenciometro, INPUT);   


   // Remoto
   // pinMode(ledPin, OUTPUT);    // initialize the LED pin as an output:
   pinMode(buttonPinA, INPUT);  // initialize the pushbutton pin as an input:
   pinMode(buttonPinB, INPUT);  // initialize the pushbutton pin as an input:
   pinMode(buttonPinC, INPUT);  // initialize the pushbutton pin as an input:
   pinMode(buttonPinD, INPUT);  // initialize the pushbutton pin as an input:    
   
}

// the loop function runs over and over again forever
void loop()  
{

  buttonStateA = digitalRead(buttonPinA);   // read the state of the pushbutton value:
  buttonStateB = digitalRead(buttonPinB);   // read the state of the pushbutton value:
  buttonStateC = digitalRead(buttonPinC);   // read the state of the pushbutton value:  
  buttonStateD = digitalRead(buttonPinD);   // read the state of the pushbutton value:
  PotEnt = analogRead(potenciometro);
  PotEnt = PotEnt/4 ;

  Serial.print("Potenciometro de entrada : ") ; 
  Serial.println(PotEnt) ; 
  analogWrite(ENA, PotEnt);

  // Boton A
  if (buttonStateC == HIGH) {
    if (swEnciendePaso==false) {
      //Serial.println("-  Hola Prender - A1 ") ; 
      digitalWrite(IN1, HIGH); 
      digitalWrite(IN2, LOW); 
      swEnciendePaso = true;
      delay(400);     // Ok 
    }
    else 
    {
     //Serial.println("-  Hola Apagar   - A ") ; 
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW); 
     swEnciendePaso = false;
     delay(1000) ;
    }
  }

  // Boton B
  if (buttonStateA == HIGH) {
    //Serial.println("-  Hola - B ") ; 
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW); 
    swReloj = true;
    delay(500);
  }

  // Boton C
  if (buttonStateB == HIGH) {
    //Serial.println("-  Hola - C ") ; 
    digitalWrite(IN1, LOW); 
    digitalWrite(IN2, HIGH); 
    swReloj = false;
    delay(500);
  }
  
}
