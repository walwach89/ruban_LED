
#include <ESP8266WiFi.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 64

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2
#define CLOCK_PIN 13

bool led1, led2, led3, led4, led5, led6, led7, led8;

// Define the array of leds
CRGB leds[NUM_LEDS];
String ledsColors[NUM_LEDS];

String codeVersion = "Version 1.0  Aug 2016 by TonesB";

// WiFi Router Login - change these to your router settings
//const char* SSID = "ESGI";
//const char* password = "Reseau-GES";

// WiFi Router Login - change these to your router settings
const char* SSID = "ssidRobin";
const char* password = "12345678";

//const char* SSID = "Xana";
//const char* password = "lyoko2468";

// Create the ESP Web Server on port 80
WiFiServer WebServer(80);
// Web Client
WiFiClient client;
int cpt = 0;

void setup() {

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( 30 );

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  // Connect to WiFi network
  Serial.println();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to WiFi");


  //  if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
  //    Serial.println("Error setting up MDNS responder!");
  //  }
  //  Serial.println("mDNS responder started");


  // Start the Web Server
  WebServer.begin();
  Serial.println("Web Server started");

  // Print the IP address
  Serial.print("You can connect to the ESP8266 at this URL: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {

  // Check if a user has connected
  client = WebServer.available();
  if (!client) {//restart loop
    return;
  }

  // Wait until the user sends some data
  Serial.println("New User");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r\n');
  Serial.println("la request");
  Serial.println(request);
  client.flush();
int accr=0;
for (int cptReq=1; cptReq<65; cptReq++){
  // get x, y, value from request

  String xVal = getValue(request, ',', accr);
  Serial.println("x :"+xVal);
  accr++;
  String hexVal = getValue(request, ',', accr);
  Serial.println("hexa :"+hexVal);
  accr++;

  hexVal = getValue(hexVal, ' ', 0);

  int xvalue = xVal.toInt();


  String value;
  value = hexVal;
  Serial.println("HEXA :"+value);

  String r = hexVal.substring(0, 2);
  int rInt = hexToDec(r);

  String g = hexVal.substring(2, 4);
  int gInt = hexToDec(g);

  String b = hexVal.substring(4, 6);
  int bInt = hexToDec(b);


  
  //Put the hex value on x y pos
  leds[cptReq].setRGB( rInt, gInt, bInt);

  FastLED.show();
}

  



  //

  //  for (int parc = 0; parc < 8; parc++) {
  //    Serial.println(ledsColors[parc]);
  //  }

  if (request.indexOf("/reset") != -1) {
    for (int cptReset = 0; cptReset < NUM_LEDS ; cptReset ++) {
      leds[cptReset].setRGB( 0, 0, 0);
      FastLED.show();
    }
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>ESP8266 Demo</title>");
  client.println("</head>");
  client.println("<body>");

  //Fonction javascript, recupere couleur, l'associe au bouton et appelle la page avec les bonens valeurs
  String fJS = (" <script> function func(id){ var color = (document.getElementById(id).value).substring(1, 7); window.location.href = \"http://192.168.43.135/led,\"+id+\",\"+color;}");
  client.println(fJS);


  client.println(" function env(){ var color ; var cpt = 0; var fullUrl=\"http://192.168.43.135/\";  console.log(cpt); console.log(fullUrl); ");
  client.println(" while (cpt < 64){ cpt++;  color = (document.getElementById(cpt).value).substring(1, 7); ");
  client.println(" console.log(color); fullUrl = fullUrl+cpt+\",\"+color+\",\"; console.log(fullUrl);  ");
  client.println("window.location.href = fullUrl; }}");
  client.println("</script>");


  int cptRow = 0;
  for (int i = 1; i <= NUM_LEDS ; i++) {


    String valeurColor;
    if (ledsColors[i - 1] == "") {
      valeurColor = "000000";
    }
    else {
      valeurColor = ledsColors[i - 1];
    }
    String button = ("<input id=\"");
    button.concat(i);
    button.concat("\" type=color \" style=\"background-color:#");
    button.concat(valeurColor);
    button.concat("\";>");
    client.print(button);
    cptRow++;
    if (cptRow > 7) {
      client.print("<br>");
      cptRow = 0;
    }

  }

  String footers = ("<a href=\"/reset\">RESET</a> </br> </br> </body> </html>");
  client.println(footers);

  client.println("<A  onclick=\"env()\">[[ENVOYER]]</A><BR>");

  FastLED.show();
}


String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int hexToDec(String hexString) {

  unsigned int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}
