#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <Wire.h>

LiquidCrystal lcd(12, 11, 7, 6, 5, 4) ;
volatile int rpmcont ;
volatile int Calc;
int pin_sensor = 2 ;

volatile int horas  = 0 ; // variable dentro y fuera de una interupcion
volatile int minutos= 0 ;
volatile int segundos=0 ;
volatile int hto=0 ;
volatile int NumeroCiclo=0 ;
volatile boolean swbomba2HIGH = true ;
volatile int swinicializar= 0 ;

int bomba1    = 8 ; 
int bomba2    = 10  ;  
int bomba3    = 9 ;  
int bomba4    = 11 ;  

void rpm ()
{
 rpmcont++;
}



void setup(){
   Serial.begin(9600); 
   pinMode(bomba1,OUTPUT);    
   pinMode(bomba2,OUTPUT);    
   pinMode(bomba3,OUTPUT);    
   pinMode(bomba4,OUTPUT);    

   digitalWrite(bomba1, HIGH);
   digitalWrite(bomba2, HIGH);
   digitalWrite(bomba3, HIGH);  
   digitalWrite(bomba4, HIGH);
}


void loop(){
  hto++ ; 
//  uno();
//  dos();
//  tres();
  cuatro();
}

void uno()
{
  Serial.print(hto);
  Serial.println(" - Rele UNO "); 
  lcd.clear();
  lcd.setCursor(0,0) ;
  lcd.print("Motor UNO") ;

  Serial.println("Prendido ");
  digitalWrite(bomba1, LOW); 
  digitalWrite(bomba4, HIGH);
  delay(3000);

  Serial.println("Apagado ");
  digitalWrite(bomba1, HIGH); 
  digitalWrite(bomba4, HIGH);
  delay(3000);

}

void dos()
{
  hto++ ; 
  Serial.print(hto);
  Serial.println(" - Rele DOS ");  
  lcd.clear();
  lcd.setCursor(0,0) ;
  lcd.print("Motor DOS") ;

  Serial.println("Prendido ");
  digitalWrite(bomba2, LOW); 
  digitalWrite(bomba4, HIGH);
  delay(5000);

  Serial.println("Apagado ");
  digitalWrite(bomba2, HIGH);
  digitalWrite(bomba4, HIGH);
  delay(5000);
}


void tres()
{
  hto++ ; 
  Serial.print(hto);
  Serial.println(" - Rele TRES");

  lcd.clear();
  lcd.setCursor(0,0) ;
  lcd.print("Motor TRES") ;

  Serial.println("Prendido ");
  digitalWrite(bomba3, LOW); 
  digitalWrite(bomba4, HIGH);  
  delay(5000);

  Serial.println("Apagado ");
  digitalWrite(bomba3, HIGH); 
  digitalWrite(bomba4, HIGH);  
  delay(5000);
}

void cuatro()
{
  hto++ ; 
  Serial.print(hto);
  Serial.println(" - Rele CUATRO");

  lcd.clear();
  lcd.setCursor(0,0) ;
  lcd.print("Motor CUATRO") ;

  Serial.println("Prendido ");
  digitalWrite(bomba4, LOW); 
  delay(5000);

  Serial.println("Apagado ");
  digitalWrite(bomba4, HIGH);
  delay(5000);
}
