int led = 13;
int contador=0 ;
char dato;



void setup() {
   pinMode(13,OUTPUT);
   Serial.begin(9600);
}

void loop() {
  
  contador = contador+1;
  Serial.println(contador);
  delay(500);   // No mover

  if (Serial.available()>0 )
    {
      dato = Serial.read();
      if (dato== '1')
        {
          digitalWrite(13,HIGH) ;
          Serial.println("prende");          
        }      

      if (dato== 'A')
        {
          digitalWrite(13,LOW) ;
          Serial.println("apaga");          
        }   

    delay(5000);             
           
    }
     
}
