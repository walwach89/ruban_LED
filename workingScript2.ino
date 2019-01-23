
#include <ESP8266WiFi.h>
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 8

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

const char* SSID = "Xana";
const char* password = "lyoko2468";

// Create the ESP Web Server on port 80
WiFiServer WebServer(80);
// Web Client
WiFiClient client;
int cpt = 0;

void setup() {

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( 150 );

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



  // get x, y, value from request

  String xVal = getValue(request, ',', 1);
  String yVal = getValue(request, ',', 2);
  String hexVal = getValue(request, ',', 3);

  hexVal = getValue(hexVal, ' ', 0);

  int xvalue = xVal.toInt();
  int yvalue = yVal.toInt();


  String value;
  value = hexVal;

  String r = hexVal.substring(0, 2);
  int rInt = hexToDec(r);

  String g = hexVal.substring(2, 4);
  int gInt = hexToDec(g);

  String b = hexVal.substring(4, 6);
  int bInt = hexToDec(b);



  //Put the hex value on x y pos
  leds[xvalue - 1].setRGB( rInt, gInt, bInt);
  ledsColors[xvalue - 1] = value ;
  FastLED.show();

  for (int parc = 0; parc < 8; parc++) {
    Serial.println(ledsColors[parc]);
  }


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

  client.println("</br></br>");

  client.println("");

  for (int j = 1; j <= 5; j++) {
    for (int i = 1; i <= NUM_LEDS ; i++) {
      //
      //      String url = "<a href=\"/led,";
      //      url.concat(i);
      //      url.concat(",");
      //      url.concat(j);
      //      url.concat(",");
      //      url.concat("FFFFFF");
      //      url.concat("\"> led");
      //      url.concat(i);
      //      url.concat("</a>");
      //
      //    client.println(url);
      //
      //
      //    client.print("<form action=\"http://");
      //    client.print(WiFi.localIP());
      //    client.print("/led1,");
      //    client.print(i);client.print(",");
      //    client.print(j);client.print(",");
      //    String valeurColor;
      //    if(ledsColors[i] == ""){
      //       valeurColor = "000000";
      //    }
      //    else{
      //      valeurColor=ledsColors[i];
      //    }
      //    client.print(valeurColor);
      //    client.print("\">");
      //    client.print(" <input type=\"submit\" value=\"led");
      //    client.print(i);
      //    client.print("\" /> ");
      //    client.print(" </form>");
      //    }
      //    client.println("<br>");

      client.print("<input type=button onclick=window.location.href='http://");
      client.print(WiFi.localIP());
      client.print("/led,");
      client.print(i); client.print(",");
      client.print(j); client.print(",");
      String valeurColor;
      if (ledsColors[i-1] == "") {
        valeurColor = "000000";
      }
      else {
        valeurColor = ledsColors[i-1];
      }
      client.print(valeurColor);
      client.print("'; style=\"background-color:#");
      client.print(valeurColor);
      client.print("\";>");
    }
    client.print("<br>");
  }

  //client.print("<form action=\"http://");
  //client.print(WiFi.localIP());
  //client.print("/led1,");
  //client.print(i);client.print(",");
  //client.print(j);client.print(",");
  //client.print(ledsColors[i]);
  //client.print("\">");
  //client.print(" <input type=\"submit\" value=\"led\" /> ");
  //client.print(" </form>");

  client.println("<a href=\"/reset\">RESET</a>");

  client.println("</br>");

  client.println("</br>");
  client.println("</body>");
  client.println("</html>");


  Serial.println("");
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
