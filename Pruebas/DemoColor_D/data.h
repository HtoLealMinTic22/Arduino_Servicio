const char* ssid_1     = "Sistema Adin 2G";
const char* password_1 = "Leal083096";
const char* ssid_2     = "Sistema Adin 2G";
const char* password_2 = "Leal083096";


String Pagina = R"====(<!DOCTYPE html>
<html>
  <head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='ie=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <style>
      body,td,th { background-color: #a0cbf3; color:#036; }
      label1 {color: rgb(240,6,6); font-size: 23px; }
      label2 {color: rgb(250,246,9,0.952);}
      label3 {color: rgb(233, 82, 157);}
      #rangevalue{ font-size:5vw; color: white; }
      .pollo {position: relative; display: inline-block; width: 150px; height: 42px; background-color: #2196f3; border-radius: 34px;}
      .vivo .pollo {background-color: #2196f3;}
      .pollo:hover {}
      .huevo {position: absolute; content: ""; height: 38px; width: 60px; left: 1px; bottom: 2px; border-radius: 34px; background-color: white; }
      .vivo .huevo { background-color: rgb(245, 182, 8); color:rgb(189, 7, 7); left: 88px; }
      input { width:74%; height:35px;margin: 0.4rem;font-size: 25px;border-radius:5px;}
      input[type="range"] {
        // -webkit-appearance: none;     // no funciona en html
        width: 300px;height: 65px;border-radius: 5px;background-image: linear-gradient(#71b9f4, #2196f3);
        background-repeat: no-repeat;}
    </style>
  </head>

  <body>
     <div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:5px; width:490px; height:320px; display: inline-block; margin: 30px; ' > 
       <center>
         <form method='get' target='pantalla'>
            <h2><label1>- Definir Horarios -</label1></h2>
           
            <h2><label>Luz sala.........: </label>
            <input type='number' name='h1'     id='h1'    placeholder='hh'       min='0' max='24'  style='width:14%;height:35px '> 
            <input type='number' name='minu1'  id='minu1' placeholder='mm'       min='0' max='59'  style='width:14%;height:35px '> 
            <input type='number' name='dr1'    id='dr1'   placeholder='Duracion' min='0' max='720' style='width:15%;height:35px '><br></h2>

            <h2><label>Cascada/Luces</label>
            <input type='number' name='hora2' id='hora2' placeholder='hh'       min='0' max='24'  style='width:14%;height:35px'> 
            <input type='number' name='minu2' id='minu2' placeholder='mm'       min='0' max='59'  style='width:14%;height:35px'> 
            <input type='number' name='dura2' id='dura2' placeholder='Duracion' min='0' max='720' style='width:15%;height:35px'><br></h2>

            <h2><label>Figuras..........: </label>
            <input type='number' name='hora3' id='hora3' placeholder='hh'       min='0' max='24'  style='width:14%;height:35px'> 
            <input type='number' name='minu3' id='minu3' placeholder='mm'       min='0' max='59'  style='width:14%;height:35px'> 
            <input type='number' name='dura3' id='dura3' placeholder='Duracion' min='0' max='720' style='width:15%;height:35px'><br></h2>

            <h2><label>Luz entrada..: </label>
            <input type='number' name='hora4' id='hora4' placeholder='hh'       min='0' max='24'  style='width:14%;height:35px'> 
            <input type='number' name='minu4' id='minu4' placeholder='mm'       min='0' max='59'  style='width:14%;height:35px'> 
            <input type='number' name='dura4' id='dura4' placeholder='Duracion' min='0' max='720' style='width:15%;height:35px'>
           <br></h2>
         </form>
       </center>
     </div>


     <div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:5px; width:490px; height:400px; display: inline-block; margin: 30px; ' > 
       <center>
         <div id="botonSala" class="vivo"><p1><h2>Luz sala .............: <span class="pollo"><span class="huevo"></span></span></h2></p1></div>
         <div id="botonArcoiris" class="vivo"><p1><h2>Cascada y luces..: <span class="pollo"><span class="huevo"></span></span></h2></p1></div>
         <div id="botonFiguras" class="vivo"><p1><h2>Figuras ...............: <span class="pollo"><span class="huevo"></span></span></h2></p1></div>
         <div id="botonLuzEnt" class="vivo"><p1><h2>Luz Entrada........: <span class="pollo"><span class="huevo"></span></span></h2></p1></div>

         <div>
            <output id="rangevalue"> </output><br>
            <input type="range" min="0" max="255" id="rangeNivel" oninput="rangevalue.value=value">
         </div>
       </center>
     </div>
     
     <div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:5px; width:490px; height:210px; display: inline-block; margin: 30px; ' > 
      <center>
        <form method='get' target='pantalla'>
           <center>
           
             <h2><label1>- CONFIGURAR Fecha Actual -</label1><br></h2>

             <h2><label>Fecha.......: </label>
             <input type='number' name='diaDate'  id='diaDate' placeholder='Dia' style='width:15%;height:40px ;border-radius:10px'; align='center'>
             <input type='number' name='mesDate'  id='mesDate' placeholder='Mes' style='width:15%;height:40px ;border-radius:10px' >
             <input type='number' name='anoDate'  id='anoDate' placeholder='Año' style='width:15%;height:40px ;border-radius:10px' >
             <br>

             <h2><label>Hora ......: </label>
             <input type='number' name='horaDate' id='horaDate' placeholder='Hora'    style='width:20%;height:40px ;border-radius:10px'>
             <input type='number' name='minDate'  id='minDate'  placeholder='Minutos' style='width:20%;height:40px ;border-radius:10px'>
 
           </center>
        </form>
       </center>
     </div>
        
     <div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 10px; width: 480px; height:350px; display: inline-block; margin: 30px; ' > 
       <center>
         <h3> <label1>- PROGRAMAR ALARMAS -</label1></h3>
         <form method='get' target='pantalla'>
             <center>
                <fieldset align='center' style='border-style:solid; border-color:#336666; width:330px; height:205px;'
                  <br>

                  <h2><label>Alarma 1....: </label>
                  <input type='number' name='Almhora1' id='Almhora1' placeholder='Hora'    min='0' max='24'  style='width:15%;height:35px'>
                  <input type='number' name='Almminu1' id='Almminu1' placeholder='Minutos' min='0' max='59'  style='width:15%;height:35px'> 
                  <br></h2>

                  <h2><label>Alarma 2... : </label>
                  <input type='number' name='Almhora2' id='Almhora2' placeholder='Hora'    min='0' max='24'  style='width:15%;height:35px'>
                  <input type='number' name='Almminu2' id='Almminu2' placeholder='Minutos' min='0' max='59'  style='width:15%;height:35px'>
                  <br></h2>

                  <h2><label>Alarma 3....: </label>
                  <input type='number' name='Almhora3' id='Almhora3' placeholder='Hora'    min='0' max='24'  style='width:15%;height:35px'> 
                  <input type='number' name='Almminu3' id='Almminu3' placeholder='Minutos' min='0' max='59'  style='width:15%;height:35px'> 
                  <br></h2>
                </fieldset>   
             </center>
         </form>
       </center>
     </div>

    <script>
     
 
      const horLuz   = document.getElementById("h1");
      horLuz.addEventListener("change", cambiarHoraLuz);
      const minLuz   = document.getElementById("minu1");
      minLuz.addEventListener("change", cambiarMinuLuz);
      const durLuz   = document.getElementById("dr1");
      durLuz.addEventListener("change", cambiarDuraLuz);
      
      const horCas   = document.getElementById("hora2");  
      horCas.addEventListener("change", cambiarHoraCas);
      const minCas   = document.getElementById("minu2");
      minCas.addEventListener("change", cambiarMinuCas);
      const durCas   = document.getElementById("dura2");
      durCas.addEventListener("change", cambiarDuraCas);      

      const horario3 = document.getElementById("hora3");  
      horario3.addEventListener("change", cambiarHora3);
      const minutos3 = document.getElementById("minu3");  
      minutos3.addEventListener("change", cambiarMinu3);
      const duracio3 = document.getElementById("dura3");  
      duracio3.addEventListener("change", cambiarDura3);

      const horario4 = document.getElementById("hora4");  
      horario4.addEventListener("change", cambiarHora4);
      const minutos4 = document.getElementById("minu4");  
      minutos4.addEventListener("change", cambiarMinu4);
      const duracio4 = document.getElementById("dura4");  
      duracio4.addEventListener("change", cambiarDura4);

      const diaAct = document.getElementById("diaDate");            // Fecha Actual
      diaAct.addEventListener("change", cambiarDiaAct);
      const mesAct = document.getElementById("mesDate");            
      mesAct.addEventListener("change", cambiarMesAct);
      const anoAct = document.getElementById("anoDate");            
      anoAct.addEventListener("change", cambiarAnoAct);

      const horAct = document.getElementById("horaDate");            
      horAct.addEventListener("change", cambiarHorAct);
      const minAct = document.getElementById("minDate");            
      minAct.addEventListener("change", cambiarMinAct);

      const horaAlm1 = document.getElementById("Almhora1");         // Alarmas UNO 
      horaAlm1.addEventListener("change", camAlmHora1);
      const minuAlm1 = document.getElementById("Almminu1");
      minuAlm1.addEventListener("change", camAlmMinu1);
      
      const horaAlm2 = document.getElementById("Almhora2");         // Alarmas DOS
      horaAlm2.addEventListener("change", camAlmHora2);
      const minuAlm2 = document.getElementById("Almminu2");
      minuAlm2.addEventListener("change", camAlmMinu2);

      const horaAlm3 = document.getElementById("Almhora3");         // Alarmas TRES
      horaAlm3.addEventListener("change", camAlmHora3);
      const minuAlm3 = document.getElementById("Almminu3");
      minuAlm3.addEventListener("change", camAlmMinu3);
      
      const elementoEstado   = document.getElementById("botonSala");
      elementoEstado.addEventListener("click", cambiarEstado);

      const elementoArcoiris = document.getElementById("botonArcoiris");
      elementoArcoiris.addEventListener("click", cambiarArcoiris);

      const elementoFiguras = document.getElementById("botonFiguras");
      elementoFiguras.addEventListener("click", cambiarFiguras);

      const elementoLuzEnt  = document.getElementById("botonLuzEnt");
      elementoLuzEnt.addEventListener("click", cambiarLuzEnt);
      
      const elementoNivel = document.getElementById("rangeNivel");
      elementoNivel.addEventListener("change", cambiarNivel);

      function cambiarDiaAct(){                                         // Fecha Actual
        const diaAct = document.getElementById("diaDate");        
        consultaGET("http://%ip/diaActEnt?valor=" +  diaAct.value);}; 
      function cambiarMesAct(){
        const mesAct = document.getElementById("mesDate");
        consultaGET("http://%ip/mesActEnt?valor=" +  mesAct.value);}; 
      function cambiarAnoAct(){
        const anoAct = document.getElementById("anoDate");
        consultaGET("http://%ip/anoActEnt?valor=" +  anoAct.value);};         

      function cambiarHorAct(){
        const horAct = document.getElementById("horaDate"); 
        consultaGET("http://%ip/horActEnt?valor=" +  horAct.value);};         
      function cambiarMinAct(){
        const minAct = document.getElementById("minDate");
        consultaGET("http://%ip/minActEnt?valor=" +  minAct.value);};         

      function camAlmHora1(){                                           // Alarmas UNO 
        const horaAlm1 = document.getElementById("Almhora1");        
        consultaGET("http://%ip/hora1Alm?valor=" +  horaAlm1.value);}; 
      function camAlmMinu1(){
        const minuAlm1 = document.getElementById("Almminu1");
        consultaGET("http://%ip/minu1Alm?valor=" +  minuAlm1.value);}; 

      function camAlmHora2(){                                           // Alarmas DOS
        const horaAlm2 = document.getElementById("Almhora2");        
        consultaGET("http://%ip/hora2Alm?valor=" +  horaAlm2.value);}; 
      function camAlmMinu2(){
        const minuAlm2 = document.getElementById("Almminu2");
        consultaGET("http://%ip/minu2Alm?valor=" +  minuAlm2.value);}; 

      function camAlmHora3(){                                           // Alarmas TRES 
        const horaAlm3 = document.getElementById("Almhora3");        
        consultaGET("http://%ip/hora3Alm?valor=" +  horaAlm3.value);}; 
      function camAlmMinu3(){
        const minuAlm3 = document.getElementById("Almminu3");
        consultaGET("http://%ip/minu3Alm?valor=" +  minuAlm3.value);}; 

    
      function cambiarEstado(){
        console.log("%ip");
        const elementoEstado = document.getElementById("botonSala");
        if(elementoEstado.className == "vivo"){elementoEstado.className = "muerto";}
        else {elementoEstado.className = "vivo";}
        console.log("Cambiando boton a " + elementoEstado.className)
        consultaGET("http://%ip/" + elementoEstado.className);};

      function cambiarArcoiris(){
        const elementoEstado = document.getElementById("botonArcoiris");
        if(elementoEstado.className == "vivo"){elementoEstado.className = "muerto";} 
        else{elementoEstado.className = "vivo";}
        console.log("Cambiando Arcoiris a " + elementoEstado.className)
        consultaGET("http://%ip/arcoiris?estado=" + elementoEstado.className);} 
      function cambiarFiguras(){
        const elementoEstado = document.getElementById("botonFiguras");
        if(elementoEstado.className == "vivo"){elementoEstado.className = "muerto";} 
        else{elementoEstado.className = "vivo";}
        consultaGET("http://%ip/figuras?estado=" + elementoEstado.className);}         
      function cambiarLuzEnt(){
        const elementoEstado = document.getElementById("botonLuzEnt");
        if(elementoEstado.className == "vivo"){elementoEstado.className = "muerto";} 
        else{elementoEstado.className = "vivo";}
        consultaGET("http://%ip/luzentra?estado=" + elementoEstado.className);}         

      function cambiarHoraLuz(){
        const horLuz   = document.getElementById("h1");
        console.log("Cambiando HoraLuz a " + horLuz.value);
        consultaGET("http://%ip/horaLuz?valor=" +  horLuz.value);};
      function cambiarMinuLuz(){
        const minLuz   = document.getElementById("minu1");
        consultaGET("http://%ip/minuLuz?valor=" +  minLuz.value);};
      function cambiarDuraLuz(){
        const durLuz   = document.getElementById("dr1");
        consultaGET("http://%ip/duraLuz?valor=" +  durLuz.value);};
      function cambiarHoraCas(){
        const horCas   = document.getElementById("hora2");
        consultaGET("http://%ip/horaCas?valor=" +  horCas.value);};
      function cambiarMinuCas(){
        const minCas   = document.getElementById("minu2");
        consultaGET("http://%ip/minuCas?valor=" +  minCas.value);};
      function cambiarDuraCas(){
        const durCas   = document.getElementById("dura2");
        consultaGET("http://%ip/duraCas?valor=" +  durCas.value);};
      function cambiarHora3(){
        const horario3 = document.getElementById("hora3");
        consultaGET("http://%ip/hora3?valor=" +  horario3.value);};
      function cambiarMinu3(){
        const minutos3 = document.getElementById("minu3"); 
        consultaGET("http://%ip/minu3?valor=" +  minutos3.value);};
      function cambiarDura3(){
        const duracio3 = document.getElementById("dura3");  
        consultaGET("http://%ip/dura3?valor=" +  duracio3.value);};
      function cambiarHora4(){
        const horario4 = document.getElementById("hora4");
        consultaGET("http://%ip/hora4?valor=" +  horario4.value);};                        
      function cambiarMinu4(){
        const minutos4 = document.getElementById("minu4");
        consultaGET("http://%ip/minu4?valor=" +  minutos4.value);};                        
      function cambiarDura4(){
        const duracio4 = document.getElementById("dura4");  
        consultaGET("http://%ip/dura4?valor=" +  duracio4.value);}; 

      function cambiarNivel(){
        const elementoNivel = document.getElementById("rangeNivel");
        consultaGET("http://%ip/nivel?valor=" +  elementoNivel.value);
      };
       

      function consultaGET(consulta){
        const Http = new XMLHttpRequest();
        console.log(`Consultando la IP ${consulta}`)
        Http.open("GET", consulta);
        Http.send();

        Http.onreadystatechange = (e) => {
          console.log(`Este es el estatus..: ${Http.status}`)
          console.log(`Este es el responseText..: ${Http.responseText}`)
        };
      };
      
    </script>
  </body>
</html>
)====";

String FeActual    = R"====(<center><label1>Fecha Actual : </label1>)====";
String FinFeActual = R"====(</center>)====";
String Parametros  = R"====(<fieldset align='center' style='border-style:solid; margin: 30px; padding: 1px; border-color:#336666; width:490px; height:200px;')====";
String Titulo1     = R"====(</h2><h2></center><label1>Luz sala...............: </label1>)====";
String Titulo2     = R"====(</h2><h2></center><label1>Cascada y Luces..: </label1>)====";
String Titulo3     = R"====(</h2><h2></center><label1>Figuras.................: </label1>)====";
String Titulo4     = R"====(</h2><h2></center><label1>Luz entrada.........: </label1>)====";

String AlmTitulo1  = R"====(<center><h2><br><label1>Alarmas. . .: </label1>)====";

String PagWebHoras = R"====(<br></h2>)====";
String PagWebHora1 = R"====(<h2></form></h2>)====";
String PagWebHora2 = R"====(<h2></form></h2>)====";
String PagWebHora3 = R"====(<h2></form></h2>)====";
String PagWebHora4 = R"====(<h2></fieldset><br></center></h2>)====";
String pagAlarma   = R"====(</center>)====";  // fin String pagAlarma

String PagFecha = R"====(
<div style = 'box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding:10px; width:420px; height:210px; display: inline-block; margin:48px;'> 
<center>
<form method='get' target='pantalla'>
  <center>
    <fieldset align='center' style='border-style:solid; border-color:#336666; width:380px; height:180px;'
    <h2>
    <label1>- CONFIGURAR Fecha/Hora -</label1>
    <br><br>
    </h2>

    <label for='Fecha dd/mm/año'> Fecha.......: </label>
    <input class='input1' type='text' name='diaDate'  id='diaDate' placeholder='Dia' style='width:15%;height:40px ;border-radius:10px' ; align='center'    >
    <input class='input1' type='text' name='mesDate'  id='mesDate' placeholder='Mes' style='width:15%;height:40px ;border-radius:10px' >
    <input class='input1' type='text' name='anoDate'  id='anoDate' placeholder='Año' style='width:15%;height:40px ;border-radius:10px' >
    <br><br>

    <label for='Hora'> Hora.......: </label>
    <input class='input1' type='text' name='horaDate' id='horaDate' placeholder='Hora'    style='width:20%;height:40px ;border-radius:10px'>
    <input class='input1' type='text' name='minDate'  id='minDate'  placeholder='Minutos' style='width:20%;height:40px ;border-radius:10px'>
    
     
    </fieldset>
  </center>
  
</form>
<center>
</div>
)====";
