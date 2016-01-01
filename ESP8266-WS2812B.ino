//-----------------------------
//
//  Includes Webclient
//
//-----------------------------
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#define ssid "your_ssid"
#define password "your_password"


//-----------------------------
//
//  Includes Leds
//
//-----------------------------

#include <Adafruit_NeoPixel.h>
#define PIN           D1
#define NUMPIXELS     60

//-----------------------------
//
//  Variables Webclient
//
//-----------------------------
String user[] = {"admin" , "", "", "", "", "", "", "", "", "" , "" , "" , ""};
String pass[] = {"admin", "", "", "", "", "", "", "", "", "" , "" , "" , ""};
int level[] =    { 3, , 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0};
int accounts = 1;
String loginError = "admin/admin";


//-----------------------------
//
//  Variables Leds
//
//-----------------------------

int NUMTHIJS = (NUMPIXELS / 2);
int fps = 30;                  //Frames per second
int brightness = 50;          //max 255
int delayval = (1000 / fps);
int showLeds = 0;
int j = 0;

//-----------------------------
//
//  Others Webclient
//
//-----------------------------

ESP8266WebServer server(80);

void handleRoot();
void handleLogin();
void handleLeds();
void handleAdmin();
void handleAdminChange();
void handleNotFound();
void handleSwitchLedState();
void handleApp();
bool has_access(String, String);
int is_authentified();

//-----------------------------
//
//  Others Leds
//
//-----------------------------

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.on("/", handleRoot);
  server.on("/login", handleLogin);

  server.on("/leds", handleLeds);
  server.on("/showLed", handleSwitchLedState);

  server.on("/admin", handleAdmin);
  server.on("/adminChange", handleAdminChange);

  server.on("/app", handleApp);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("HTTP server started");
  pixels.begin();
  pixels.show();
  pixels.setBrightness(brightness);

  Serial.println("Leds ready");
}

void loop(void) {
  server.handleClient();
  if (showLeds == 1) { //Uit
    pixels.clear();
    pixels.show();
  }
  else if (showLeds == 2) { //Rood
    fillLeds(150, 0, 0);
    showLeds = 0;
  }
  else if (showLeds == 3) { //Groen
    fillLeds(0, 150, 0);
    showLeds = 0;
  }
  else if (showLeds == 4) { //Blauw
    fillLeds(0, 0, 150);
    showLeds = 0;
  }
  else if (showLeds == 5) { //Oranje
      fillLeds(255, 145, 0);
      showLeds = 0;
  }
  else if (showLeds == 6) { //Petrol
      fillLeds(0, 255, 145);
      showLeds = 0;
      }
  else if (showLeds == 7) { //Roze
      fillLeds(145, 0, 255);
    showLeds = 0;
  }
  else if (showLeds == 8) { //Geel
    fillLeds(255, 255, 0);
    showLeds = 0;
  }
  else if (showLeds == 9) { //Licht blauw
    fillLeds(0, 255, 255);
    showLeds = 0;
  }
  else if (showLeds == 10) { //Paars
    fillLeds(150, 0, 150);
    showLeds = 0;
  }
  else if (showLeds == 11) { //Wit
    fillLeds(150, 150, 150);
    showLeds = 0;
  }
  else if (showLeds == 12) { //Rainbow
    rainbowCycle(delayval);
  }
  else if (showLeds == 13) { //Zoeklicht gewoon
    for (int i = NUMTHIJS; i > 0; i--) {
      fadeLedRed(i - 4, 0, 0, 150);
      fadeLed(NUMPIXELS - i, 150, 0, 0);
      pixels.show();
      delay(delayval);
      j++;
    }
    pixels.clear();
    for (int i = 0; i < NUMTHIJS; i++) {
      fadeLed(i, 0, 0, 150);
      fadeLedRed(NUMPIXELS - (i - 4), 150, 0, 0);
      pixels.show();
      delay(delayval);
      j++;
    }
    pixels.clear();
  }
  else if (showLeds == 14) { //Zoeklicht regenboog
    for (int i = NUMTHIJS; i > 0; i--) {
      fadeLedRed(i - 4, r_wheel((((j / 2) * 256 / pixels.numPixels()) + 127) & 255), g_wheel((((j / 2) * 256 / pixels.numPixels()) + 127) & 255), b_wheel((((j / 2) * 256 / pixels.numPixels()) + 127) & 255));
      fadeLed(NUMPIXELS - i, r_wheel((((j / 2) * 256 / pixels.numPixels())) & 255) , g_wheel((((j / 2) * 256 / pixels.numPixels())) & 255), b_wheel((((j / 2) * 256 / pixels.numPixels())) & 255));
      pixels.show();
      delay(delayval);
      j++;
    }
    pixels.clear();
    for (int i = 0; i < NUMTHIJS; i++) {
      fadeLedRed(NUMPIXELS - (i - 4), r_wheel((((j / 2) * 256 / pixels.numPixels()) ) & 255), g_wheel((((j / 2) * 256 / pixels.numPixels())) & 255), b_wheel((((j / 2) * 256 / pixels.numPixels())) & 255));
      fadeLed(i, r_wheel((((j / 2) * 256 / pixels.numPixels()) + 127) & 255) , g_wheel((((j / 2) * 256 / pixels.numPixels()) + 127) & 255), b_wheel((((j / 2) * 256 / pixels.numPixels()) + 127) & 255));
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(delayval); // Delay for a period of time (in milliseconds).
      j++;
    }
    pixels.clear();
  }
}
