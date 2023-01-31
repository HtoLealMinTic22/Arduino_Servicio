#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <Wire.h>
#define PinFlotador 2 
#define Bomba 10


const int POT = A2;
volatile int conteo=0 ;
volatile int vr_LLenoTanque ;

void setup(){
   Serial.begin(9600); 
   pinMode(Bomba,OUTPUT); 
   analogWrite(Bomba,LOW);
   pinMode(PinFlotador, INPUT);     
}


void loop(){
   vr_LLenoTanque  = digitalRead(PinFlotador) ; 
   if (vr_LLenoTanque== LOW)  
     { 
      Serial.println("  Tanque - LLENO - con agua (H2O) - LOW ");
      analogWrite(Bomba,LOW);
     }
   else
     { 
      Serial.println("  Tanque flotador - VACIO - HIGH "); 
      analogWrite(Bomba, analogRead(POT/8));
     }  
} 
