
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

String codeVersion = "Version 1.0  Aug 2016 by TonesB";

// WiFi Router Login - change these to your router settings
const char* SSID = "Xana";
const char* password = "lyoko2468";



// Create the ESP Web Server on port 80
WiFiServer WebServer(80);
// Web Client
WiFiClient client;
int cpt = 0;

void setup() {

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( 40 );

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

  // Process the request:

  if (request.indexOf("/led1") != -1 ) {
    if (led1 == false) {
      led1 = true;
      leds[0] = CRGB::Blue;
    }
    else {
      leds[0] = CRGB::Black;
      led1 = false;
    }
  }
  if (request.indexOf("/led2") != -1 ) {
    if (led2 == false) {
      led2 = true;
      leds[1] = CRGB::Blue;
    }
    else {
      leds[1] = CRGB::Black;
      led2 = false;
    }
  }
  if (request.indexOf("/led3") != -1 ) {
    if (led3 == false) {
      led3 = true;
      leds[2] = CRGB::Blue;
    }
    else {
      leds[2] = CRGB::Black;
      led3 = false;
    }

  }
  if (request.indexOf("/led4") != -1 ) {
    if (led4 == false) {
      led4 = true;
      leds[3] = CRGB::Blue;
    }
    else {
      leds[3] = CRGB::Black;
      led4 = false;
    }
  }
  if (request.indexOf("/led5") != -1 ) {
    if (led5 == false) {
      led5 = true;
      leds[4] = CRGB::Blue;
    }
    else {
      leds[4] = CRGB::Black;
      led5 = false;
    }

  }
  if (request.indexOf("/led6") != -1 ) {
    if (led6 == false) {
      led6 = true;
      leds[5] = CRGB::Blue;
    }
    else {
      leds[5] = CRGB::Black;
      led6 = false;
    }

  }
  if (request.indexOf("/led7") != -1 ) {
    if (led7 == false) {
      led7 = true;
      leds[6] = CRGB::Blue;
    }
    else {
      leds[6] = CRGB::Black;
      led7 = false;
    }

  }
  if (request.indexOf("/led8") != -1 ) {
    if (led8 == false) {
      led8 = true;
      leds[7] = CRGB::Blue;
    }
    else {
      leds[7] = CRGB::Black;
      led8 = false;
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


  if (true) {
    client.print("le compteur est à " );
    client.print(cpt);
    client.print("<br>");
    client.println("allumer la led 1 <a href=\"/led1\">led 1</a></br>");
    client.println("allumer la led 2 <a href=\"/led2\">led 2</a></br>");
    client.println("allumer la led 3 <a href=\"/led3\">led 3</a></br>");
    client.println("allumer la led 4 <a href=\"/led4\">led 4</a></br>");
    client.println("allumer la led 5 <a href=\"/led5\">led 5</a></br>");
    client.println("allumer la led 6 <a href=\"/led6\">led 6</a></br>");
    client.println("allumer la led 7 <a href=\"/led7\">led 7</a></br>");
    client.println("allumer la led 8 <a href=\"/led8\">led 8</a></br>");

    for(int i = 1;i<30;i++){
    
    String url = "allumer la led";
    url.concat(i);
    url.concat("<a href=\"/led");
    url.concat(i);
    url.concat("\">led");
    url.concat(i);
    url.concat("</a></br>");

    client.println(url);
    }
  }

  Serial.println("led 1 ");
  Serial.println(led1);
  Serial.println("<br>");

  Serial.println("led 2 ");
  Serial.println(led2);
  Serial.println("<br>");

  Serial.println("led 3 ");
  Serial.println(led3);
  Serial.println("<br>");

  Serial.println("led 4 ");
  Serial.println(led4);
  Serial.println("<br>");

  Serial.println("led 5 ");
  Serial.println(led5);
  Serial.println("<br>");

  Serial.println("led 6 ");
  Serial.println(led6);
  Serial.println("<br>");

  Serial.println("led 7 ");
  Serial.println(led7);
  Serial.println("<br>");

  Serial.println("led 8 ");
  Serial.println(led8);
  Serial.println("<br>");

  client.println("</br>");

  client.println("</br>");
  client.println("</body>");
  client.println("</html>");


  Serial.println("");
  FastLED.show();
}
