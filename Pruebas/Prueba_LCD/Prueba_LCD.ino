// Prueba del L.C.D. - I2C
// A4 - SDA
// A5 - SCL
// Ene/23/2022 

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Servo.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <PololuLedStrip.h>

// A4 - SDA - Pantalla LCD
// A5 - SCL - Pantalla LCD

LiquidCrystal_I2C lcd(0x27,16,2);

volatile int tgContador = 0 ;


void setup()
{  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  delay(1000); 
  Serial.begin (115200);       // inicializa el puerto seria a 115200 baudios
}

void loop()
{
  tgContador = tgContador+1 ;
  delay(1000);
  DuracionBano() ;
}

void DuracionBano() {
   lcd.setCursor(0,0) ;
   lcd.print("Prueba   L.C.D.") ;   
  
   lcd.setCursor(2,1) ;
   lcd.print("Contador.: ") ; 
   lcd.print(tgContador,DEC) ; 
}
