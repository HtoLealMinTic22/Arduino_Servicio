// ----------------------------------
// Abr/27/2022 - Prueba de reloj 1307
// ----------------------------------

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Servo.h>
#include <EEPROM.h>
#include <RTClib.h>

RTC_DS1307 RTC;
LiquidCrystal_I2C lcd(0x27,20,4); 

// SDA        A4 - RELOJ DS 1307
// SCL        A5 - RELOJ DS 1307

volatile long anomov =0 ;
volatile int mesmov =0 ;
volatile int diamov =0 ;
volatile int horamov=0 ;
volatile int minumov=0 ; 
volatile int vrLimpia=0 ; 

void setup()
{
   Serial.begin (9600); 

   lcd.init();                      // initialize the lcd 
   lcd.backlight();
 
   // Reloj
   Wire.begin();
   RTC.begin();
   delay(100) ;
   
   // Asigna la fecha y hora del sistema   
   RTC.adjust(DateTime(__DATE__, __TIME__));

   anomov  = EEPROM.read(0) ; // 
   mesmov  = EEPROM.read(1) ; // 
   diamov  = EEPROM.read(2) ; // 
   horamov = EEPROM.read(6) ; // 
   minumov = EEPROM.read(7) ; // 
   vrLimpia= EEPROM.read(8) ; // 
}


void loop()
{
   verTime();
   verhour() ;
  
   anomov  = EEPROM.read(0) ; // 
   mesmov  = EEPROM.read(1) ; // 
   diamov  = EEPROM.read(2) ; // 
   horamov = EEPROM.read(6) ; // 
   minumov = EEPROM.read(7) ; // 
   vrLimpia= EEPROM.read(8) ; // 
   delay(500) ;
}

void verTime() {
  DateTime now = RTC.now();
  Serial.print(" Fecha : ") ;
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  
  lcd.setCursor(0,0) ;
  lcd.print("Fecha:") ;
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
}

void verhour() {
  DateTime now = RTC.now();
  Serial.print("   Hora : ");
  Serial.print(now.hour(),DEC);
  Serial.print(':');
  Serial.print(now.minute(),DEC);
  Serial.print(':');
  Serial.println(now.second(),DEC);
  
  lcd.setCursor(0,1) ;
  lcd.print("Hora :");
  lcd.print(now.hour(),DEC);
  lcd.print(':');
  lcd.print(now.minute(),DEC);
}
