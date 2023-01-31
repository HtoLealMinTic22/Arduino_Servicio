// Funciona con el  Motor 42HD4027-01
// Sensor RX-480 - RadioFrecuencia

volatile boolean swEnciendePaso  ;
volatile boolean swReloj         ;
#define EncienPaso  13 
#define steps   A2
#define direccion A3

// 23/08/22
//volatile int     veloPaso   = 300  ;  // NO Arranca
//volatile int     veloPaso   = 350  ;  // NO Arranca
//volatile int     veloPaso   = 380 ;

// 26/Ago/2022
//volatile int     veloPaso   = 500 ;  // Ok 
volatile int     veloPaso   = 700 ;  // Ok 


//volatile int     veloPaso   = 400  ; // 400-10 / Ok  A mayor numero menor velocidad, mas Torque
//volatile int     veloPaso   = 800  ; // 800-10  A mayor numero menor velocidad, mas Torque
//volatile int     veloPaso   = 800  ; // 800-10  A mayor numero menor velocidad, mas Torque



//volatile int     veloPaso   = 1100 ; // A mayor numero menor velocidad, mas Torque
//volatile int     veloPaso   = 1000 ;
//volatile int     veloPaso   = 1450 ;
//volatile int     veloPaso   = 500 ;  // 500-10 Rapido - Ok Perfecto Velocidad motor paso Mayor numero menor velocidad

//volatile int     veloPaso   = 1500  ; // Velocidad motor paso Mayor numero menor velocidad
//volatile int     veloPaso   = 2000  ; // Velocidad motor paso 
// volatile int     veloPaso   = 1500-480 Se bloquea;
//volatile int     veloPaso   = 1000;       // 1000 Mas lento
//volatile int     veloPaso   = 950-480 ; 
//volatile int     veloPaso   = 950-600 ;     // Ok Rapido 
//volatile int     veloPaso   = 900 ;     // 900-480 Ok Rapido 
//volatile int     veloPaso   = 850 ;     // 850-480 Ok Rapido 
//volatile int     veloPaso   = 800 ;     // 800-480 Ok Rapido 
//volatile int     veloPaso   = 700 ;     // 700-480 Ok Rapido 
//volatile int     veloPaso   = 700 ;     //  700-300
//volatile int     veloPaso   = 600 ;     //  600-10  NO arranca
//volatile int     veloPaso   = 650 ;     // Vibra 
//volatile int     veloPaso   = 640 ;     // 640-10
//volatile int     veloPaso   = 610 ;     // 610-10 NO arranca 
//volatile int     veloPaso   = 630 ;     // 630-10 ok Perfecto
//volatile int     veloPaso   = 620 ;     // 620-10 ok Perfecto
//volatile int     veloPaso   = 200 ;
//volatile int     veloPaso   = 100 ;
//volatile int     veloPaso   = 300 ;
//volatile int     veloPaso   = 400 ;   // 400-10 No Arranca
//volatile int     veloPaso   = 350 ;   // 350-10 No Arranca
//volatile int     veloPaso   = 300 ;   // 300-10 No Arranca
//volatile int     veloPaso   = 200 ;  // NO arranca


// Remoto
const int buttonPinD =  8 ;     // the number of the pushbutton pin
const int buttonPinC =  9 ;     // the number of the pushbutton pin
const int buttonPinB = 10 ;     // the number of the pushbutton pin
const int buttonPinA = 11 ;     // the number of the pushbutton pin
const int ledPin =  5;        // the number of the LED pin

int buttonStateA = 0;         // variable for reading the pushbutton status
int buttonStateB = 0;         // variable for reading the pushbutton status
int buttonStateC = 0;         // variable for reading the pushbutton status
int buttonStateD = 0;         // variable for reading the pushbutton status

void setup ()
{
  Serial.begin(9600);
  pinMode(direccion, OUTPUT);
  pinMode(steps, OUTPUT);
  pinMode(EncienPaso, OUTPUT);
  digitalWrite(EncienPaso, LOW)  ; // Apagar

  // Remoto
  pinMode(ledPin, OUTPUT);    // initialize the LED pin as an output:
  pinMode(buttonPinA, INPUT);  // initialize the pushbutton pin as an input:
  pinMode(buttonPinB, INPUT);  // initialize the pushbutton pin as an input:
  pinMode(buttonPinC, INPUT);  // initialize the pushbutton pin as an input:
  pinMode(buttonPinD, INPUT);  // initialize the pushbutton pin as an input:    
  
}

void loop()
{
  buttonStateA = digitalRead(buttonPinA);   // read the state of the pushbutton value:
  buttonStateB = digitalRead(buttonPinB);   // read the state of the pushbutton value:
  buttonStateC = digitalRead(buttonPinC);   // read the state of the pushbutton value:  
  buttonStateD = digitalRead(buttonPinD);   // read the state of the pushbutton value:

 Serial.println("-  Hola ") ; 
  // Boton A
  if (buttonStateC == HIGH) {
    if (swEnciendePaso==false) {
      Serial.println("-  Hola Prender - A ") ; 
      digitalWrite(EncienPaso, HIGH); 
      swEnciendePaso = true;
      //delay(200);  // NO arranca
      delay(400);     // Ok 
      //delay(1000);  // Muy Demorado
    }
    else 
    {
     Serial.println("-  Hola Apagar   - A ") ; 
     digitalWrite(EncienPaso, LOW); 
     swEnciendePaso = false;
     delay(1000) ;
    }
  }


  // Boton B
  if (buttonStateA == HIGH) {
    //if (swReloj == false){
       Serial.println("-  Hola - B ") ; 
       digitalWrite(direccion, LOW);   // Direccion Horario
       swReloj = true;
    // }
    }


  // Boton C
  if (buttonStateB == HIGH) {
    Serial.println("-  Hola - C ") ; 
//    if (swReloj == true){
//       Serial.println("-  Hola - C ") ; 
       digitalWrite(direccion, HIGH);  // Direccion Anti-Horario
       swReloj = false;
//     }
  }


  // Boton D - DesIntalado
  // if (buttonStateD == HIGH) {
  //  Serial.println("-  Hola - D ") ; 
  //  }
  
   
   // Motor de paso   
   //digitalWrite(EncienPaso, HIGH); // Prender
   //Serial.print("Velocidad..: ");
   //Serial.println(veloPaso);

   for (int i=0; i<10; i++){      // 
     digitalWrite(steps,HIGH);    // siempre debe estar los step en "HIGH""
     delayMicroseconds(veloPaso); // velocidad / mayor numero menos(-) velocidad
     digitalWrite(steps,LOW);     // siempre debe estar los step en ""LOW"
     delayMicroseconds(veloPaso);       
   }
}
