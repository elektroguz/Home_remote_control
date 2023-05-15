#include <UIPEthernet.h>
#include <dht11.h> // dht11 kütüphanesini ekliyoruz.

dht11 DHT11;

int led1 = 24;
int led2 = 22;
int led3 = 26;
int led4 = 28;
byte mac[] = {
  0x74,
  0x69,
  0x69,
  0x2D,
  0x30,
  0x31
}; //physical mac address
byte ip[] = {
  192,
  168,
  2,
  180
}; // ip in lan (that's what you need to use in your browser. 
byte gateway[] = {
  192,
  168,
  2,
  1
}; // internet access via router
byte subnet[] = {
  255,
  255,
  255,
  0
}; //subnet mask
EthernetServer server(8085); //server port     
String readString;
bool elektrik = false;
bool dinamo1 = false;
bool dinamo2 = false;
bool yazici = false;

void setup() {
  // Open serial communications and wait for port to open:
  DHT11.attach(2);
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // Create a client connection
  
  EthernetClient client = server.available();
  int chk = DHT11.read();

  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
        }
        //if HTTP request has ended
        if (c == '\n') {
          Serial.println(readString); //print to serial monitor for debuging
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println("");
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<meta charset='UTF-8'>");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no\">");
          client.println("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta/css/bootstrap.min.css' integrity='sha384-/Y6pD6FV/Vv2HJnA6t+vslU6fwYXjCFtcEpHbNJ0lyAFsXTsjBbfaDjzALeQsN6M' crossorigin='anonymous'>");
          client.println("<style>");
          client.println("body{background-image:url(https://i.pinimg.com/originals/c4/cc/0d/c4cc0d9a28ef1bab0784325cffbb840b.jpg);background-repeat:repeat-y repeat-x}");
          client.println("</style>");
          client.println("<script>function startTime(){var today=new Date(); var h=today.getHours(); var m=today.getMinutes(); var s=today.getSeconds(); m=checkTime(m); s=checkTime(s); document.getElementById('txt').innerHTML= 'Saat : ' + h + ':' + m + ':' + s; var t=setTimeout(startTime, 500);}function checkTime(i){if (i < 10){i='0' + i}; return i;}</script>");
          client.println("<TITLE>ÜNSAL KONTROL</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY onload='startTime()'>");
          client.println("<CENTER>");
          client.println("<H1>ÜNSAL DEVRE KONTROL SİSTEMİ</H1>");
          client.println("<HR/>");
          if (readString.indexOf("sifre=rusenhatice") > 0) {
            if (readString.indexOf("?elektrik=acik") > 0) {
              elektrik = true;
              digitalWrite(led1, LOW);
            }
            if (readString.indexOf("?elektrik=kapali") > 0) {
              elektrik = false;
              digitalWrite(led1, HIGH);
            }
            if (readString.indexOf("?dinamo1=acik") > 0) {
              dinamo1 = true;
              digitalWrite(led2, LOW);
            }
            if (readString.indexOf("?dinamo1=kapali") > 0) {
              dinamo1 = false;
              digitalWrite(led2, HIGH);
            }
            if (readString.indexOf("?dinamo2=acik") > 0) {
              dinamo2 = true;
              digitalWrite(led3, LOW);
            }
            if (readString.indexOf("?dinamo2=kapali") > 0) {
              dinamo2 = false;
              digitalWrite(led3, HIGH);
            }
            if (readString.indexOf("?yazici=acik") > 0) {
              yazici = true;
              digitalWrite(led4, LOW);
            }
            if (readString.indexOf("?yazici=kapali") > 0) {
              yazici = false;
              digitalWrite(led4, HIGH);
            }
 
            client.println("<H4 id='txt'></H4>");
            client.println("<H4>Sicaklik : ");
            client.println(DHT11.temperature);
            client.println("&#8451;</H4>");
            client.println("<H4>Nem : %");
            client.println(DHT11.humidity);
            client.println("</H4>");
            if (elektrik) {
              client.println("<a class='btn btn-danger' href=\"/?elektrik=kapali&sifre=rusenhatice\" role='button'>DÜKKAN ELEKTRİĞİ KAPAT</a>");
            } else {
              client.println("<a class='btn btn-danger' href=\"/?elektrik=acik&sifre=rusenhatice\" role='button'>DÜKKAN ELEKTRİĞİ AÇ</a>");
            }
            client.println("<HR/>");
            if (dinamo1) {
              client.println("<a class='btn btn-warning' href=\"/?dinamo1=kapali&sifre=rusenhatice\">1. DİNAMO KAPAT</a>");
            } else {
              client.println("<a class='btn btn-warning' href=\"/?dinamo1=acik&sifre=rusenhatice\">1. DİNAMO AÇ</a>");
            }
            client.println("<HR/>");
            if (dinamo2) {
              client.println("<a class='btn btn-info' href=\"/?dinamo2=kapali&sifre=rusenhatice\">2. DİNAMO KAPAT</a>");
            } else {
              client.println("<a class='btn btn-info' href=\"/?dinamo2=acik&sifre=rusenhatice\">2. DİNAMO AÇ</a>");
            }
            client.println("<HR/>");
  
            if (yazici) {
              client.println("<a class='btn btn-primary' href=\"/?yazici=kapali&sifre=rusenhatice\">YAZICI KAPAT</a>");
            } else {
              client.println("<a class='btn btn-primary' href=\"/?yazici=acik&sifre=rusenhatice\">YAZICI AÇ</a>");
            }
            client.println("<HR/>");
 
          }
          else {
            client.println("<form action='/'><div class='input-group'> <input type='password' class='form-control' name='sifre' id='password' placeholder='Lütfen şifreyi giriniz...'> <span class='input-group-btn'><button type='submit' class='btn btn-secondary'>Tamam</button></span> </div></form>");
          }

          client.println("<BR/></center></BODY></HTML>");

          

          delay(10);
          //stopping client
          client.stop();

          //clearing string for next read
          readString = "";

        }
      }
    }
  }
}
