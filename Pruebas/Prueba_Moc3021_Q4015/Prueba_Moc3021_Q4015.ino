// no funciona cuando es un bombillo de luz - LED
//#include <LiquidCrystal.h>
//#include <TimerOne.h>
//#include <Wire.h>

#define PinFlota2  4  

//LiquidCrystal lcd(12, 11, 7, 6, 5, 4) ;
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


const int bomba1    = 8 ;            // D0 - GPIO16 Luz entrada - Rele
//    int bomba1    = 6  ; // Bomba1 llenado desde el tanque auxiliar  
//    int bomba2    = 9  ; // Bomba2-ElectroValvula Llenado desde el acueducto
volatile int vr_PinFlota2 ;

void rpm ()
{
 rpmcont++;
}



void setup(){
   Serial.begin(115200); 
   pinMode(bomba1,OUTPUT);    // Bomba1 llenado desde el tanque auxiliar  (8)

// pinMode(bomba2,OUTPUT);    // Bomba2-ElectroValvula Llenado desde el acueducto(9)
}


void loop(){
  hto++ ; 
  Serial.print(hto);
  uno();
  Serial.println("------------------------ ");
  
  //dos();
  //Serial.println("Motor DOS ");
}

void uno()
{
  Serial.println("  Apaga   --  Motor UNO ");
  digitalWrite(bomba1, LOW); // Apaga - Bomba1 llenado desde el tanque auxiliar  (4)
  delay(5000);
  
  Serial.println("   Prende  --  Motor UNO ");
  digitalWrite(bomba1, HIGH); // Prende - Bomba1 llenado desde el tanque auxiliar  (4)  
  delay(3000);

}


void dos()
{
  //digitalWrite(bomba1, LOW); // Bomba1 llenado desde el tanque auxiliar  (4)
  //digitalWrite(bomba2, HIGH);  // Bomba2-ElectroValvula Llenado desde el acueducto(7)

//  lcd.clear();  
//  lcd.setCursor(0,1) ;
//  lcd.print("Motor DOS") ;
//  
//  lcd.setCursor(10,1) ;
//  lcd.print("No.:") ;
//  lcd.setCursor(14,1) ;
//  lcd.print(hto) ;

  delay(5000);



//lcd.noDisplay();
//delay(500);
//Turn on the display:
//lcd.display();
//delay(500);


}




void pruebaLcd()
{
  for (int thisChar = 0; thisChar < 11; thisChar++) {
    //lcd.print(thisChar);
    delay(500);
    
    }
    // Turn on the blinking cursor:
  
    //lcd.setCursor(16,1);
    // set the display to automatically scroll:
    //lcd.autoscroll();
    // print from 0 to 9:
    for (int thisChar = 0; thisChar < 10; thisChar++) {
    //lcd.print(thisChar);
    delay(500);
    }
    //lcd.noAutoscroll();
}
