const char* ssid_1     = "Sistema Adin 2G";
const char* password_1 = "Leal083096";
const char* ssid_2     = "Sistema Adin 2G";
const char* password_2 = "Leal083096";


//const
String Pagina =  R"====(<!DOCTYPE html>
<html>
  <head>
    <title>Servidor-70 </title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <style>
      
      body,td,th { background-color: #a0cbf3;
        color:#036; 
      }

      label1 {color: rgb(240,6,6);}
      label2 {color: rgb(250,246,9,0.952);}
      label3 {color: rgb(233, 82, 157);}

      h1, h2, h3 {
        background-color: #a0cbf3;
        color:#036; 
        padding: 10px;
      }
      
      #rangevalue{
        font-size:5vw;
        color: white;
      }

      .pollo {
        position: relative;
        display: inline-block;
        width: 150px;
        height: 42px;
        background-color: #2196f3;
        border-radius: 34px;
      }

      .vivo .pollo {
        background-color: #2196f3;
      }

      .pollo:hover {
   
      }

      .huevo {
        position: absolute;
        content: "";
        height: 38px;
        width: 60px;
        left: 1px;
        bottom: 2px;
        border-radius: 34px;
        background-color: white;
      }

      .vivo .huevo {
        background-color: rgb(245, 182, 8); 
        color:rgb(189, 7, 7); 
        left: 88px;
      }

      input {
        margin: 0.4rem;
      }

      input[type="range"] {
        -webkit-appearance: none;     // no funciona en html
        width: 1200px;
        height: 25px;
        border-radius: 5px;
        background-image: linear-gradient(#71b9f4, #2196f3);
        background-repeat: no-repeat;
      }
    </style>
  </head>

  <body>
     <div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 460px; height:650px; display: inline-block; margin: 30px; ' > 
       <center>
         <h3>Panel de Control </h3>
         <h3>Estado</h3>

         <div id="botonEstado" class="vivo">
           <span class="pollo">
             <span class="huevo"></span>
           </span>
         </div>


         <h3>
          <label>Luz sala.........: </label>
          <input type='number' name='hrel1' id='hrel1' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px' align='center'> 
          <input type='number' name='minu1' id='minu1' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> 
          <input type='number' name='dura1' id='dura1' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> 
          <br> 
         </h3>

         <input class='boton' id="botonGrabaUno"  type='submit' value='ACTUALIZAR' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px' >
      
         <h3>Nivel</h3>
         <output id="rangevalue">50</output><br>
         <input type="range" value="50" min="0" max="255" id="rangeNivel" oninput="rangevalue.value=value">
     
         <h3>Color</h3>
         <input type="color"  id="colorHTML" name="colorHTML" value="#ff0000" > </br>
      
         <h3>Arcoiris</h3>
         <div id="botonArcoiris" class="vivo">
           <span class="pollo">
             <span class="huevo"></span>
           </span>
         </div>
         </h2>
       </center>
     </div>


    <script>

      const elementoEstado   = document.getElementById("botonEstado");
      const elementoBoton1   = document.getElementById("botonGrabaUno");
      const elementoNivel    = document.getElementById("rangeNivel");
      const elementoColor    = document.getElementById("colorHTML");
      const elementoArcoiris = document.getElementById("botonArcoiris");
      
      elementoEstado.addEventListener("click", cambiarEstado);
      elementoBoton1.addEventListener("click", cambiarBot1);
      elementoNivel.addEventListener("click", cambiarNivel);
      elementoColor.addEventListener("change", cambiarColor);
      elementoArcoiris.addEventListener("click", cambiarArcoiris);


      function cambiarBot1(){
        console.log("%ip");
        const elementoBoton1 = document.getElementById("botonGrabaUno");

//        String horaEnt1 = (server.arg("hora1"));
//        String minuEnt1 = (server.arg("minu1"));
//        String duraEnt1 = (server.arg("dura1"));
//
//        // Horario Uno
//        if (horaEnt1 != ""){
//          h1  = horaEnt1.toInt() ;
          h1  = 16 ;
          EEPROM.write(100, h1); 
          EEPROM.commit();
          delay(10);
//        }

//        if (minuEnt1 != ""){
//          m1  = minuEnt1.toInt() ;
//          EEPROM.write(101, m1);  
//          EEPROM.commit();
//          delay(10);
//        }

//        if (duraEnt1 != ""){
//          dr1 = duraEnt1.toInt() ;
//          EEPROM.write(108, dr1);  
//          EEPROM.commit();
//          delay(10);
//        }

        console.log("Cambiando Horas1 " + elementoBoton1.className)
        consultaGET("http://%ip/nivel?valor=" + elementoBoton1.value);        
      }
        
      


      function cambiarEstado(){
        console.log("%ip");
        const elementoEstado = document.getElementById("botonEstado");

        if(elementoEstado.className == "vivo")
         {
           elementoEstado.className = "muerto";
         }
        else
         {
          elementoEstado.className = "vivo";
         }
        console.log("Cambiando boton a " + elementoEstado.className)
        consultaGET("http://%ip/" + elementoEstado.className);
      };


      function cambiarNivel(){
        const elementoNivel = document.getElementById("rangeNivel");
        console.log("Cambiando nivel a " + elementoNivel.value);
        consultaGET("http://%ip/nivel?valor=" +  elementoNivel.value);
      }

      function cambiarColor(){
        console.log("Cambiando color");
        console.log(colorHTML.value);
        colorActual = hexa_a_rgb(colorHTML.value);
        consultaGET("http://%ip/color?r=" + colorActual.r +"&g=" + colorActual.g + "&b=" + colorActual.b);
      };

      function hexa_a_rgb(hex) {
        var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
        return result
          ? {
              r: parseInt(result[1], 16),
              g: parseInt(result[2], 16),
              b: parseInt(result[3], 16),
            }
          : null;
      }

      function cambiarArcoiris(){
        const elementoEstado = document.getElementById("botonArcoiris");

        if(elementoEstado.className == "vivo"){
          elementoEstado.className = "muerto";
        } else{
          elementoEstado.className = "vivo";
        }
        console.log("Cambiando Arcoiris a " + elementoEstado.className)
        consultaGET("http://%ip/arcoiris?estado=" + elementoEstado.className);
      }
      
      function consultaGET(consulta){
        const Http = new XMLHttpRequest();
        console.log(`Consultando  ${consulta}`)
        Http.open("GET", consulta);
        Http.send();

        Http.onreadystatechange = (e) => {
          console.log(Http.status );
          // console.log(Http.responseText);
        };
      };
      
    </script>
  </body>
</html>
)====";


String FeActual = R"====(<h2>
<center>
<label1>Fecha Actual : </label1>
)====";

String FinFeActual = R"====(</h2>
</center>;
)====";

String PagWeb = R"====(
<!DOCTYPE html>
<html lang='es'>

<head>
  <meta charset='UTF-8'>"
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <meta http-equiv='X-UA-Compatible' content='ie=edge'>
  <title>Tempo-Gral.</title> 

  <script type='text/javascript'>   
    window.onload = FocoColor() ;      
    function FocoColor(x){
    x.style='width:75px; height:35px; background-color:rgba(247, 245, 137, 0.952); color:red; text-shadow:1px 1px 1px blue; text-align:center; border-radius:5px' ;
    }

    function FocoColor1(x){
    x.style='width:180px; height:35px; background-color:rgba(250,246,9,0.952); color:red; text-shadow:1px 1px 1px blue; text-align:center; border-radius:5px' ;
    "    
  </script>
 
</head>
<body>
</form>   
<center>
<br>

// Definicion de horarios
<fieldset align='center' style='border-style:solid; border-color:#336666; width:460px; height:300px; '

<form action ='guardar_horario' method='get'> 
<br>
)====";

String pagHoras = R"====(
<!DOCTYPE html>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1.0'>
  <meta http-equiv='X-UA-Compatible' content='ie=edge'>
  <style type ='text/css'> body,td,th {color:#036;} label1{color: rgb(240,6,6);} label2{color: rgb(250,246,9,0.952);} body{background-color:#a0cbf3;} 
  </style>
  <script>
    function envioHorarios() {
    
    
    }
  </script>
</head>

<body>
<center>
  <div  style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 460px; height:390px; display: inline-block; margin: 30px; ' > 
    <form action ='configHor' method='get' target='pantalla'>
       <center>
        <h2><label1>- PROGRAMAR HORARIOS -</label1></h2>
        <fieldset align='center' style='border-style:solid; border-color:#336666; width:430px; height:280px; '
         <br>

         <h2>
          <label>Luz sala.........: </label>
          <input type='number' name='hora1' id='hora1' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px' align='center'> 
          <input type='number' name='minu1' id='minu1' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> 
          <input type='number' name='dura1' id='dura1' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> 
          <br> 
         </h2>

         <h2>
          <label>Cascada/Luces</label>
          <input type='number' name='hora2' id='hora2' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> 
          <input type='number' name='minu2' id='minu2' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> 
          <input type='number' name='dura2' id='dura2' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> 
          <br> 
         </h2>

         <h2>
           <label>Figuras..........: </label>"  
           <input type='number' name='hora3' id='hora3' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> 
           <input type='number' name='minu3' id='minu3' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> 
           <input type='number' name='dura3' id='dura3' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> 
           <br>
         </h2>

         <h2>
           <label>Luz entrada..: </label>
           <input type='number' name='hora4' id='hora4' placeholder='Hora'     onfocus='FocoColor(this)' min='0' max='24'  style='width:12%;height:35px'> 
           <input type='number' name='minu4' id='minu4' placeholder='Minutos'  onfocus='FocoColor(this)' min='0' max='59'  style='width:12%;height:35px'> 
           <input type='number' name='dura4' id='dura4' placeholder='Duracion' onfocus='FocoColor(this)' min='0' max='720' style='width:12%;height:35px'> 
           <br>
         </h2>

         <br>      
         <input class='boton'  type='submit' value='ACTUALIZAR' style='width:70%;height:50px ;background-color:rgba(153,228,176,0.952) ;border-radius:10px'>
         <iframe id='pantaHor' name='pantaHor' src='' width:800px height:170px frameborder='0' scrolling='no' >  </iframe>
         <br><br>
        </fieldset>
   
     </center>
    </form>
  </div>
</center>


</body>
)===="; // fin String pagHoras
