
const char* ssid_1     = "Sistema Adin 2G";
const char* password_1 = "Leal083096";
const char* ssid_2     = "Sistema Adin 2G";
const char* password_2 = "Leal083096";

//const String Pagina =  R"====(HTTP/1.1 200 OK
//Content-Type: text/html
//
//<!DOCTYPE HTML>
//    <title>Servidor-90 </title>
//    <h1>-- Ip_estatica --</h1>
//<html>
//)====";


String Pagina =  R"====(<!DOCTYPE html>
<html>
  <head>
    <title>Servidor-90 </title>
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
      const elementoNivel    = document.getElementById("rangeNivel");
      const elementoColor    = document.getElementById("colorHTML");
      const elementoArcoiris = document.getElementById("botonArcoiris");
      
      elementoEstado.addEventListener("click", cambiarEstado);
      elementoNivel.addEventListener("click", cambiarNivel);
      elementoColor.addEventListener("change", cambiarColor);
      elementoArcoiris.addEventListener("click", cambiarArcoiris);

      function cambiarEstado(){
        console.log("%ip");
        const elementoEstado = document.getElementById("botonEstado");

        //Serial.print("elementoEstado .. : ");
        //Serial.println(elementoEstado);
        // cliente.print(Estado ? "Encendida2" : "Apagada2");

        LineaActual="GET /encender"
        //Serial.print("-----Linea enviada -------- :");
        //Serial.println(LineaActual);  // GET /encender HTTP/1.1
                                      // GET /apagar HTTP/1.1
   
        //VerificarMensaje(LineaActual);
        LineaActual = "";





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
