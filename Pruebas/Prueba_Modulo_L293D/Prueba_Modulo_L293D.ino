#define pinPrueba1 2
#define pinPrueba2 3

int IN1 = 8 ;
int IN2 = 7 ;
int ENA = 9 ;


void setup() {
   pinMode(pinPrueba1, OUTPUT);
   pinMode(pinPrueba2, OUTPUT);
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);   
   pinMode(ENA, OUTPUT);   
}

// the loop function runs over and over again forever
void loop()  
{
  // Movimiento Horario
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW); 
  analogWrite(ENA, 100); // Despacio
  delay(10000);  


  // Movimiento Anti-Horario
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255); // Rapido
  delay(10000);  

  // Apagado 
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  delay(10000);  

  
//  // Movimiento Horario
//  digitalWrite(pinPrueba1, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(5000);                       // wait for a second
//  digitalWrite(pinPrueba1, LOW);    // turn the LED off by making the voltage LOW
//  delay(500);  
//  
//  // Movimiento Anti-Horario
//  digitalWrite(pinPrueba2, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(5000);                       // wait for a second
//  digitalWrite(pinPrueba2, LOW);    // turn the LED off by making the voltage LOW
//  delay(500);  
  
}
