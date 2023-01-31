#include <LiquidCrystal_I2C.h> // Debe descargar la Libreria que controla el I2C
#include<Wire.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
// ENTRADAS DIGITALES EXT 12V
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;
const int in5 = 6;
const int in6 = 7;
// ENTRADAS DIP SWITCH
const int in7 = 8;
const int in8 = 9;
// SALIDAS TRANSISTORES
const int OUT1 = 10;
const int OUT2 = 11;
const int OUT3 = 12;
const int OUT4 = 13;
// SALIDAS RELAYS
const int E_V   = 14;
const int BOMBA = 15;
const int OUT7  = 16;
const int OUT8  = 17;
int contador = 0;
volatile double flujo;
int S_F = 0;
int P_C = 0;
int OK  = 0;
int OP2 = 0;
int OP3 = 0;
int RV2 = 0;
int LAST_C_P = 0;
int filling = 0;
int TIME = 3000;
boolean TIEMPO = false;
byte soles = 0;
boolean E0, E1, E2 = false;
boolean T01, T12, T20 = false;
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("ELECTROALL.");
  delay(600);
  lcd.setCursor(2, 1);
  lcd.print("ELECTROALL..");
  delay(700);
  lcd.setCursor(3, 2);
  lcd.print("ELECTROALL...");
  delay(800);
  lcd.setCursor(4, 3);
  lcd.print("ELECTROALL...");
  delay(1000);
  lcd.clear();
  pinMode(in2, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  pinMode(in5, INPUT);
  pinMode(in6, INPUT);
  pinMode(OUT1, OUTPUT);
  pinMode(OUT2, OUTPUT);
  pinMode(OUT3, OUTPUT);
  pinMode(OUT4, OUTPUT);
  pinMode(E_V,   OUTPUT);
  pinMode(BOMBA, OUTPUT);
  pinMode(OUT7,  OUTPUT);
  pinMode(OUT8,  OUTPUT);
  flujo = 0;
  attachInterrupt(0, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
  //attachInterrupt(1, coin, RISING);  //DIGITAL Pin 3: Interrupt 1
}
void loop() {
  P_C = digitalRead(in2);
  OK  = digitalRead(in3);
  OP2 = digitalRead(in4);
  OP3 = digitalRead(in5);
  RV2 = digitalRead(in6);
  if (E0 == true) {
    if (P_C != LAST_C_P) {
      if (P_C == LOW) {
        if (soles < 3) {
          soles++;
          lcd.clear();
        }
      }
    }
  }
  LAST_C_P = P_C;
  //INICIALIZACION***************************
  if ((!E1 && !E2) == true) {
    E0 = true;
  }
  //TRANSITION*********************************
  //from
  if (E0 == true && (soles == 1 || soles == 2 || soles == 3) && OK == true) {
    lcd.clear();
    T01 = true;
    T20 = false;
  }
  if (E1 == true && (filling == 1 || filling == 2 || filling == 3) ) {
    lcd.clear();
    T12 = true;
    T01 = false;
  }
  if (E2 == true && TIEMPO == true ) {
    lcd.clear();
    T20 = true;
    T12 = false;
  }
  //ETAPA**************************************
  if (E0 && T01 == true) {
    E0 = false;
    E1 = true;
  }
  if (E1 && T12 == true) {
    E1 = false;
    E2 = true;
  }
  if (E2 && T20 == true) {
    E2 = false;
    E0 = true;
  }
  //ACTUADORES*********************************
  //Reposo
  if (E0 == true) {
    digitalWrite(E_V,   LOW);
    digitalWrite(BOMBA, LOW);
    flujo = 0;
    filling = 0;
    if (soles == 0) {
      lcd.setCursor(0, 0);
      lcd.print("BIENVENIDOS A H2O FT");
      lcd.setCursor(2, 1);
      lcd.print("CUANTOS L DESEA?");
      lcd.setCursor(0, 3);
      lcd.print("1L = 1 2L = 2 3L = 3");
    }
    if (soles == 1) {
      lcd.setCursor(4, 0);
      lcd.print("TIENE UN SALDO");
      lcd.setCursor(2, 1);
      lcd.print("DE: S/.");
      lcd.print(soles);
      lcd.setCursor(3, 3);
      lcd.print("1 litro de agua");
    }
    if (soles == 2) {
      lcd.setCursor(4, 0);
      lcd.print("TIENE UN SALDO");
      lcd.setCursor(2, 1);
      lcd.print("DE: S/.");
      lcd.print(soles);
      lcd.setCursor(3, 3);
      lcd.print("2 litro de agua");
    }
    if (soles == 3) {
      lcd.setCursor(4, 0);
      lcd.print("TIENE UN SALDO");
      lcd.setCursor(2, 1);
      lcd.print("DE: S/.");
      lcd.print(soles);
      lcd.setCursor(3, 3);
      lcd.print("3 litro de agua");
    }
  }
  // LLENANDO AGUA
  if (E1  == true) {
    digitalWrite(E_V,   HIGH);
    digitalWrite(BOMBA, HIGH);
    if (soles == 1) {
      lcd.setCursor(5, 0);
      lcd.print("LLENANDO...");
      lcd.setCursor(3, 1);
      lcd.print("1 Litro de agua");
      lcd.setCursor(0, 3);
      lcd.print("PROCESO Ltrs. = ");
      lcd.print(flujo);
      if (flujo >= 1.0) {
        filling = 1;
      }
    }
    if (soles == 2) {
      lcd.setCursor(5, 0);
      lcd.print("LLENANDO...");
      lcd.setCursor(3, 1);
      lcd.print("2 Litros de agua");
      lcd.setCursor(0, 3);
      lcd.print("PROCESO Ltrs. = ");
      lcd.print(flujo);
      if (flujo >= 2.0) {
        filling = 2;
      }
    }
    if (soles == 3) {
      lcd.setCursor(5, 0);
      lcd.print("LLENANDO...");
      lcd.setCursor(3, 1);
      lcd.print("3 Litros de agua");
      lcd.setCursor(0, 3);
      lcd.print("PROCESO Ltrs. = ");
      lcd.print(flujo);
      if (flujo >= 3.0) {
        filling = 3;
      }
    }
  }
  //COMPLETADO
  if (E2 == true) {
    digitalWrite(E_V,   LOW);
    digitalWrite(BOMBA, LOW);
    lcd.setCursor(6, 0);
    lcd.print("COMPLETO!");
    lcd.setCursor(1, 1);
    lcd.print("RETIRE EL PRODUCTO");
    lcd.setCursor(1, 3);
    lcd.print("MUCHAS GRACIAS...");
    soles = 0;
    delay(TIME);
    TIEMPO = true;
  }
}
void pulse() {
  flujo += 1.0 / 450.0;
}
/*
  void coin() {
  soles++;
  }*/
