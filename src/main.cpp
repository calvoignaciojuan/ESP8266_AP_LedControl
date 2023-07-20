
// Based on  https://gitlab.com/MrDIYca/code-samples/-/raw/master/esp8266_setup_portal.ino


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define OUT1 4
#define OUT2 5
#define OUT3 13
#define OUT4 12
#define OUT5 14

ESP8266WebServer    server(80);

long tiempo_inicial_efecto = 0;
long tiempo_transcurrido_efecto = 0;
long tiempo_total_efecto = 0;

long tiempo_inicial_intervalo = 0;
long tiempo_transcurrido_intervalo = 0;
long tiempo_total_intervalo = 0;
int intervalo_activo = 0;

long tiempo_inicial_paso = 0;
long tiempo_transcurrido_paso = 0;

long tiempo_pasoActual = 0;
int paso_activo = 1;

int s1=0;
int s2=0;
int s3=0;
int s4=0;
int s5=0;

struct settings {

  int s1t1;
  int s2t1;
  int s3t1;
  int s4t1;
  int s5t1;
  int s1t2;
  int s2t2;
  int s3t2;
  int s4t2;
  int s5t2;
  int s1t3;
  int s2t3;
  int s3t3;
  int s4t3;
  int s5t3;
  int s1t4;
  int s2t4;
  int s3t4;
  int s4t4;
  int s5t4;
  int s1t5;
  int s2t5;
  int s3t5;
  int s4t5;
  int s5t5;

  int s1t6;
  int s2t6;
  int s3t6;
  int s4t6;
  int s5t6;

  long t_pasoUno;
  long t_pasoDos;
  long t_pasoTres;
  long t_pasoCuatro;
  long t_pasoCinco;
  long t_efecto;
  long t_intervalo;

} user_config = {};

void handlePortal();

int tiempoEntrePasos = 0;
int tiempoEsperaReinicioEfecto = 0;

void iniciarPasos();

void setup() {
  
    EEPROM.begin(sizeof(struct settings) );
    EEPROM.get( 0, user_config );

    Serial.begin(115200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("Led Control Config", "12345678");
    server.on("/",  handlePortal);
    server.begin();

    pinMode(OUT1,OUTPUT);
    pinMode(OUT2,OUTPUT);
    pinMode(OUT3,OUTPUT);
    pinMode(OUT4,OUTPUT);
    pinMode(OUT5,OUTPUT);
   
    //arranco paso 1
    iniciarPasos();
}

void loop() {

    server.handleClient();

    tiempo_transcurrido_efecto = millis() -  tiempo_inicial_efecto;

    if(tiempo_transcurrido_efecto < tiempo_total_efecto){
        //continuo con los pasos del efecto
        tiempo_transcurrido_paso = millis()- tiempo_inicial_paso;

        if(tiempo_transcurrido_paso >= tiempo_pasoActual){
            
            //avanzo al siguiente paso
            tiempo_inicial_paso = millis();
            tiempo_transcurrido_paso = 0;
            paso_activo = paso_activo + 1;

            if(paso_activo == 6){
                paso_activo = 1;
            }          

            //tomo datos del paso
            switch (paso_activo)
            {
                case 1:
                    s1=user_config.s1t1;
                    s2=user_config.s2t1;
                    s3=user_config.s3t1;
                    s4=user_config.s4t1;
                    s5=user_config.s5t1;
                    tiempo_pasoActual = user_config.t_pasoUno;
                    break;
                case 2:
                    s1=user_config.s1t2;
                    s2=user_config.s2t2;
                    s3=user_config.s3t2;
                    s4=user_config.s4t2;
                    s5=user_config.s5t2;
                    tiempo_pasoActual = user_config.t_pasoDos;
                    break;
                case 3:
                    s1=user_config.s1t3;
                    s2=user_config.s2t3;
                    s3=user_config.s3t3;
                    s4=user_config.s4t3;
                    s5=user_config.s5t3;
                    tiempo_pasoActual = user_config.t_pasoTres;
                    break;
                case 4:
                    s1=user_config.s1t4;
                    s2=user_config.s2t4;
                    s3=user_config.s3t4;
                    s4=user_config.s4t4;
                    s5=user_config.s5t4;
                    tiempo_pasoActual = user_config.t_pasoCuatro;
                    break;
                case 5:
                    s1=user_config.s1t5;
                    s2=user_config.s2t5;
                    s3=user_config.s3t5;
                    s4=user_config.s4t5;
                    s5=user_config.s5t5;
                    tiempo_pasoActual = user_config.t_pasoCinco;
                break;        
            }
        
            digitalWrite(OUT1,s1);
            digitalWrite(OUT2,s2);
            digitalWrite(OUT3,s3);
            digitalWrite(OUT4,s4);
            digitalWrite(OUT5,s5);
        }
    }else{

        if(!intervalo_activo){
            intervalo_activo = 1;
            tiempo_inicial_intervalo = millis();
             //inicio intervalo
            s1=user_config.s1t6;
            s2=user_config.s2t6;
            s3=user_config.s3t6;
            s4=user_config.s4t6;
            s5=user_config.s5t6;
            digitalWrite(OUT1,s1);
            digitalWrite(OUT2,s2);
            digitalWrite(OUT3,s3);
            digitalWrite(OUT4,s4);
            digitalWrite(OUT5,s5);
        }     

        tiempo_transcurrido_intervalo = millis() - tiempo_inicial_intervalo;

        if(tiempo_transcurrido_intervalo >= tiempo_total_intervalo){
           
            iniciarPasos();
        }                 
    }
}

void iniciarPasos(){

    intervalo_activo = 0;  

    s1=user_config.s1t1;
    s2=user_config.s2t1;
    s3=user_config.s3t1;
    s4=user_config.s4t1;
    s5=user_config.s5t1;
    digitalWrite(OUT1,s1);
    digitalWrite(OUT2,s2);
    digitalWrite(OUT3,s3);
    digitalWrite(OUT4,s4);
    digitalWrite(OUT5,s5);

    tiempo_inicial_efecto = millis();
    tiempo_transcurrido_efecto = 0;
    tiempo_total_efecto = user_config.t_efecto;

    tiempo_total_intervalo = user_config.t_intervalo;

    tiempo_inicial_paso = millis();
    tiempo_transcurrido_paso = 0;
    paso_activo = 1;
    tiempo_pasoActual = user_config.t_pasoUno ; 
}

void handlePortal() {

    if (server.method() == HTTP_POST) {

        user_config.s1t1 = server.arg("s1t1") == "on" ? 1 : 0;
        user_config.s2t1 = server.arg("s2t1") == "on" ? 1 : 0;
        user_config.s3t1 = server.arg("s3t1") == "on" ? 1 : 0;
        user_config.s4t1 = server.arg("s4t1") == "on" ? 1 : 0;
        user_config.s5t1 = server.arg("s5t1") == "on" ? 1 : 0;

        user_config.s1t2 = server.arg("s1t2") == "on" ? 1 : 0;
        user_config.s2t2 = server.arg("s2t2") == "on" ? 1 : 0;
        user_config.s3t2 = server.arg("s3t2") == "on" ? 1 : 0;
        user_config.s4t2 = server.arg("s4t2") == "on" ? 1 : 0;
        user_config.s5t2 = server.arg("s5t2") == "on" ? 1 : 0;

        user_config.s1t3 = server.arg("s1t3") == "on" ? 1 : 0;
        user_config.s2t3 = server.arg("s2t3") == "on" ? 1 : 0;
        user_config.s3t3 = server.arg("s3t3") == "on" ? 1 : 0;
        user_config.s4t3 = server.arg("s4t3") == "on" ? 1 : 0;
        user_config.s5t3 = server.arg("s5t3") == "on" ? 1 : 0;

        user_config.s1t4 = server.arg("s1t4") == "on" ? 1 : 0;
        user_config.s2t4 = server.arg("s2t4") == "on" ? 1 : 0;
        user_config.s3t4 = server.arg("s3t4") == "on" ? 1 : 0;
        user_config.s4t4 = server.arg("s4t4") == "on" ? 1 : 0;
        user_config.s5t4 = server.arg("s5t4") == "on" ? 1 : 0;

        user_config.s1t5 = server.arg("s1t5") == "on" ? 1 : 0;
        user_config.s2t5 = server.arg("s2t5") == "on" ? 1 : 0;
        user_config.s3t5 = server.arg("s3t5") == "on" ? 1 : 0;
        user_config.s4t5 = server.arg("s4t5") == "on" ? 1 : 0;
        user_config.s5t5 = server.arg("s5t5") == "on" ? 1 : 0;

        user_config.s1t6 = server.arg("s1t6") == "on" ? 1 : 0;
        user_config.s2t6 = server.arg("s2t6") == "on" ? 1 : 0;
        user_config.s3t6 = server.arg("s3t6") == "on" ? 1 : 0;
        user_config.s4t6 = server.arg("s4t6") == "on" ? 1 : 0;
        user_config.s5t6 = server.arg("s5t6") == "on" ? 1 : 0;

        user_config.t_pasoUno = server.arg("t1").toInt();
        user_config.t_pasoDos = server.arg("t2").toInt();
        user_config.t_pasoTres = server.arg("t3").toInt();
        user_config.t_pasoCuatro = server.arg("t4").toInt();
        user_config.t_pasoCinco = server.arg("t5").toInt();
        user_config.t_efecto = server.arg("tefecto").toInt();
        user_config.t_intervalo = server.arg("t6").toInt();

        EEPROM.put(0, user_config);
        EEPROM.commit();

        iniciarPasos();
       
        server.send(200,  "text/html", "<!doctype html> <html lang='en'> <head> <meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>Wifi Setup</title> <style> body{ text-align:center; } .toggle { position: relative; display: block; width: 80px; height: 40px; padding: 3px; margin: auto; border-radius: 50px; cursor: pointer; } .toggle-input { position: absolute; top: 0; left: 0; opacity: 0; } .toggle-label { position: relative; display: block; height: inherit; font-size: 12px; background: red; border-radius: inherit; box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.12), inset 0 0 3px rgba(0, 0, 0, 0.15); } .toggle-label:before, .toggle-label:after { position: absolute; top: 50%; color: black; margin-top: -.5em; line-height: 1; } .toggle-label:before { content: attr(data-off); right: 11px; color: #fff; text-shadow: 0 1px rgba(255, 255, 255, 0.5); } .toggle-label:after { content: attr(data-on); left: 11px; color: #fff; text-shadow: 0 1px rgba(0, 0, 0, 0.2); opacity: 0; } .toggle-input:checked~.toggle-label { background: green; } .toggle-input:checked~.toggle-label:before { opacity: 0; } .toggle-input:checked~.toggle-label:after { opacity: 1; } .toggle-handle { position: absolute; top: 4px; left: 4px; width: 38px; height: 38px; background: linear-gradient(to bottom, #FFFFFF 40%, #f0f0f0); border-radius: 50%; } .toggle-handle:before { position: absolute; top: 50%; left: 50%; margin: -6px 0 0 -6px; width: 16px; height: 16px; } .toggle-input:checked~.toggle-handle { left: 44px; box-shadow: -1px 1px 5px rgba(0, 0, 0, 0.2); } /* Transition*/ .toggle-label, .toggle-handle { transition: All 0.3s ease; -webkit-transition: All 0.3s ease; -moz-transition: All 0.3s ease; -o-transition: All 0.3s ease; } .container-columnas{ position: relative; } .columa__item{ display: inline-block; background-color: gray; width: 100px; border-radius: 10px; /* position: absolute; */ left: 20px; } /* .columa__item p{ margin: 10px; } */ .columa__item input { margin-top: 5px; margin-bottom: 5px; width: 80px; height: 20px; border: blueviolet solid 2px; border-radius: 3px; text-align: center; } .columa__leyenda{ display: inline-block; border-radius: 10px; /* position: absolute; */ left: 20px; /* margin: 10px; */ width: 80px; background-color: papayawhip; } .columa__leyenda p{ display: inline-block; margin-top: 15px; margin-bottom: 12px; } .enviarButton{ background-color: rgb(184, 184, 236); width: 150px; height: 25px; border-radius: 10px; /* margin-top: 5px; */ } h1{ display: inline-block; font-size: 20px; margin: 0px; } .tituloColum{ font-size: 12px; margin-top: 5px; margin-bottom: 5px; } .columa__item--intervalo{ margin-left: 20px; background-color: blueviolet; } .tiempoEfecto{ display: inline-block; background-color: gray; width: 520px; border-radius: 10px; margin-right: 40px; margin-bottom: 10px; } .tiempoEfecto input{ width: 50px; height: 15px; margin: 2px; } </style> </head> <body> <main class='form-signin'> <h1>LED CONTROL</h1> <form action='/' method='post'> <div class='container-columnas'> <div class='columa__leyenda'> <p>Salida 1</p> <p>Salida 2</p> <p>Salida 3</p> <p>Salida 4</p> <p>Salida 5</p> <p>Durac[ms]</p> </div> <div class='columa__item'> <p class='tituloColum'>Paso 1</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t1' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t1'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t1' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t1' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t1'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t1' value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 2</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t2'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t2' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t2' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t2' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t2'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t2'  value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 3</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t3'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t3'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t3' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t3' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t3'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t3'  value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 4</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t4'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t4'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t4' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t4' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t4'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t4' value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 5</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t5'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t5'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t5' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t5' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t5'checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t5'  value='200'> </div> <div  class='columa__item columa__item--intervalo'> <p class='tituloColum'>Estado Intervalo</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t6'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t6'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t6' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t6' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t6'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t6'  value='20000'> </div> </div> <div class='tiempoEfecto'> <label for='tefecto'>Tiempo duracion efecto[ms]   </label> <input type='text' name='tefecto'  value='10000'> </div> <br> <input class='enviarButton' type='submit' value='Guardar configuracion'></input> </form> </main> </body> </html>" );
    } else {

          
        server.send(200,   "text/html", "<!doctype html> <html lang='en'> <head> <meta charset='utf-8'> <meta name='viewport' content='width=device-width, initial-scale=1'> <title>Wifi Setup</title> <style> body{ text-align:center; } .toggle { position: relative; display: block; width: 80px; height: 40px; padding: 3px; margin: auto; border-radius: 50px; cursor: pointer; } .toggle-input { position: absolute; top: 0; left: 0; opacity: 0; } .toggle-label { position: relative; display: block; height: inherit; font-size: 12px; background: red; border-radius: inherit; box-shadow: inset 0 1px 2px rgba(0, 0, 0, 0.12), inset 0 0 3px rgba(0, 0, 0, 0.15); } .toggle-label:before, .toggle-label:after { position: absolute; top: 50%; color: black; margin-top: -.5em; line-height: 1; } .toggle-label:before { content: attr(data-off); right: 11px; color: #fff; text-shadow: 0 1px rgba(255, 255, 255, 0.5); } .toggle-label:after { content: attr(data-on); left: 11px; color: #fff; text-shadow: 0 1px rgba(0, 0, 0, 0.2); opacity: 0; } .toggle-input:checked~.toggle-label { background: green; } .toggle-input:checked~.toggle-label:before { opacity: 0; } .toggle-input:checked~.toggle-label:after { opacity: 1; } .toggle-handle { position: absolute; top: 4px; left: 4px; width: 38px; height: 38px; background: linear-gradient(to bottom, #FFFFFF 40%, #f0f0f0); border-radius: 50%; } .toggle-handle:before { position: absolute; top: 50%; left: 50%; margin: -6px 0 0 -6px; width: 16px; height: 16px; } .toggle-input:checked~.toggle-handle { left: 44px; box-shadow: -1px 1px 5px rgba(0, 0, 0, 0.2); } /* Transition*/ .toggle-label, .toggle-handle { transition: All 0.3s ease; -webkit-transition: All 0.3s ease; -moz-transition: All 0.3s ease; -o-transition: All 0.3s ease; } .container-columnas{ position: relative; } .columa__item{ display: inline-block; background-color: gray; width: 100px; border-radius: 10px; /* position: absolute; */ left: 20px; } /* .columa__item p{ margin: 10px; } */ .columa__item input { margin-top: 5px; margin-bottom: 5px; width: 80px; height: 20px; border: blueviolet solid 2px; border-radius: 3px; text-align: center; } .columa__leyenda{ display: inline-block; border-radius: 10px; /* position: absolute; */ left: 20px; /* margin: 10px; */ width: 80px; background-color: papayawhip; } .columa__leyenda p{ display: inline-block; margin-top: 15px; margin-bottom: 12px; } .enviarButton{ background-color: rgb(184, 184, 236); width: 150px; height: 25px; border-radius: 10px; /* margin-top: 5px; */ } h1{ display: inline-block; font-size: 20px; margin: 0px; } .tituloColum{ font-size: 12px; margin-top: 5px; margin-bottom: 5px; } .columa__item--intervalo{ margin-left: 20px; background-color: blueviolet; } .tiempoEfecto{ display: inline-block; background-color: gray; width: 520px; border-radius: 10px; margin-right: 40px; margin-bottom: 10px; } .tiempoEfecto input{ width: 50px; height: 15px; margin: 2px; } </style> </head> <body> <main class='form-signin'> <h1>LED CONTROL</h1> <form action='/' method='post'> <div class='container-columnas'> <div class='columa__leyenda'> <p>Salida 1</p> <p>Salida 2</p> <p>Salida 3</p> <p>Salida 4</p> <p>Salida 5</p> <p>Durac[ms]</p> </div> <div class='columa__item'> <p class='tituloColum'>Paso 1</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t1' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t1'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t1' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t1' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t1'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t1' value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 2</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t2'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t2' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t2' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t2' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t2'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t2'  value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 3</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t3'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t3'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t3' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t3' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t3'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t3'  value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 4</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t4'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t4'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t4' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t4' checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t4'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t4' value='200'> </div> <div  class='columa__item'> <p class='tituloColum'>Paso 5</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t5'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t5'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t5' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t5' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t5'checked/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t5'  value='200'> </div> <div  class='columa__item columa__item--intervalo'> <p class='tituloColum'>Estado Intervalo</p> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s1t6'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s2t6'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s3t6' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s4t6' /> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <label class='toggle'> <input class='toggle-input' type='checkbox' name='s5t6'/> <span class='toggle-label' data-off='OFF' data-on='ON'></span> <span class='toggle-handle'></span> </label> <input type='text' name='t6'  value='20000'> </div> </div> <div class='tiempoEfecto'> <label for='tefecto'>Tiempo duracion efecto[ms]   </label> <input type='text' name='tefecto'  value='10000'> </div> <br> <input class='enviarButton' type='submit' value='Guardar configuracion'></input> </form> </main> </body> </html>" );
    }
}

