// Abr/18/2022 - Se instalo el motor de Paso, para eliminar residuos


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Conecxion entre las dos tarjetas Arduino
// Tx-12 Trasmite del Celular a Arduino, Rx-13 trasmite de Arduino a Celular 
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Servo.h>
#include <EEPROM.h>
#include <RTClib.h>


SoftwareSerial PuertoUno(12,13);   // Rx-2 Trasmite del Celular a Arduino, Tx-7 trasmite de Arduino a Celular 
RTC_DS1307 RTC;

LiquidCrystal_I2C lcd(0x27,20,4); 

#define PinFlotador 4        // Pin del flotador
volatile int vr_LLenoTanque ;


void setup()
{
  pinMode(PinFlotador, INPUT);   // Pin del flotador
  Serial.begin (9600);           // inicializa el puerto seria a 9600 baudios
}


void loop()
{
  vr_LLenoTanque  = digitalRead(PinFlotador) ; 
  if (vr_LLenoTanque== LOW)  
    { Serial.println("  Tanque - LLENO - con agua (H2O) - LOW ");  
    }
  else
    { Serial.println("  Tanque flotador - VACIO - HIGH ");  
    }  
  delay(500) ;  
}
