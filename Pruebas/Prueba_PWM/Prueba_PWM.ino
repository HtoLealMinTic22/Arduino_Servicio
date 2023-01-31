#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <Wire.h>

#define Bomba 10
//#define Bomba 3
const int POT = A2;
volatile int conteo=0 ;


void setup(){
   Serial.begin(9600); 
   pinMode(Bomba,OUTPUT);    
}


void loop(){
   delay(500);
   Serial.print("Conteo ..:  ");  
   Serial.println(conteo);

   if (conteo!=20) 
     {
      conteo=conteo+1;
      // analogWrite(Bomba, 10);      // Nivel minimo de velocidad - muy lento 
      // Ok analogWrite(Bomba, 50);   // Entre menos sea el numero disminuye la velocidad
      // Ok analogWrite(Bomba, 200);
      // Ok analogWrite(Bomba, 500);        // Nivel maximo de velocidad - Rapido 
      analogWrite(Bomba, analogRead(POT/8));
     }      
   else
     {
      for (int i=150; i!=0;  i--)
        {
         analogWrite(Bomba,LOW);
         Serial.print("Apagado ..:  ");  
         Serial.println(i);
        }
      conteo=0;  
     }  
} 


      





//     for (i=0;  i<=10; i++)
//       {
//        analogWrite(Bomba, analogRead(POT/8));
//        Serial.print("Conteo ..:  ");  
//        Serial.println(i);
//       }
