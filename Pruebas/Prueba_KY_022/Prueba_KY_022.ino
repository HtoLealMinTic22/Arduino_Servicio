
#include <IRremote.h>    // importa libreria IRremote

// 40100929 - Panasonic - Ok
// 40100080 - Panasonic - 01       


// 1074791106 Arriba - Ok 
// 1074793162 Abajo  - Ok


// FF38C7 - Ok 
// FFA25D - Boton (1)
// FF629D - Boton (2)
// FFE21D - Boton (3)
// FF5AA5 - Flecha Derecha
// FF10EF - Flecha Izquierda







int SENSOR = 11;    // sensor KY-022 a pin digital 11  
IRrecv irrecv(SENSOR);    // establece al 11 para objeto irrecv
decode_results codigo;    // crea objeto codigo de la clase decode_results

void setup() { 
  Serial.begin(9600);     // inicializa comunicacion serie a 9600 bps
  irrecv.enableIRIn();    // inicializa recepcion de datos
} 

void loop() { 
  //  Serial.println("Hola");
  if (irrecv.decode(&codigo)) {   // si existen datos ya decodificados
    //Serial.println(codigo.value, HEX);  // imprime valor en hexadecimal en monitor
      Serial.println(codigo.value, DEC);  // imprime valor en decimal en monitor
    irrecv.resume();      // resume la adquisicion de datos
  }
  delay (800);        // breve demora de 100 ms.
}
