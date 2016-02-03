#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <Hash.h>
ESP8266WebServer server(80);

const char *ssid = "vanikke";
const char *password = "thijstan2310";

bool reverseOutput = true;

int accounts = 2;
String user[] = {"admin", "mark"};
String pass[] = {"766a84f3c9e07e421a1f305c420957c7526347af", "98eb031dda9650f768855a4571600ecb19da0842"};
int level[] =    {3, 2};
String loginError = "Welkom terug!";
String salt = "-m#VFZ6'W%";

bool EnableEEPROM = true;

int aantalKnoppen = 8;
String knopNaam[] = {"Led 1", "Led 2", "Led 3", "Led 4", "Led 5", "Led 6", "Led 7", "Led 8"};
int knopPin[] = {D1, D2, D3, D4, D5, D6, D7, D8};
bool led_status[] = {1, 0, 0, 0, 0, 0, 0, 0};

String hashIt(String,String);
String hashIt(String);
void handleRoot();
void handleLogin();
void handleNotFound();
void handleCss();
void handleLed();
void handleLedFancy();
void handleLedSwitch();
void handleLedSwitchFancy();
bool has_access(String, String);
int is_authentified();
void handleCreatePass();
void handleAdmin();
void handleAdminChange();
String css(); //Bevat de css pagina van de site

char status;

void setup() {
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);

  Serial.begin(115200);
  int x = 0;
  Serial.println("\nRelay control over WiFi start op");
  if (EnableEEPROM) {
    EEPROM.begin(512);
    Serial.println("Fail restore on\nPinmode wijzigeen en laatste instellingen terug zetten");
  } else {
    Serial.println("Fail restore off\nPinmode wijzigen");
  }

  while (x != aantalKnoppen)
  {
    led_status[x] = EEPROM.read(x);
    pinMode(knopPin[x], OUTPUT);
    digitalWrite(knopPin[x], led_status[x]);
    Serial.print(knopNaam[x]);
    Serial.print("   : ");
    if (reverseOutput) {
      if (led_status[x])
        Serial.print("uit");
      else
        Serial.print("aan");
    } else {
      if (led_status[x])
        Serial.print("aan");
      else
        Serial.print("uit");
    }

    Serial.println("\tPinmode is output");
    x++;
  }

  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Verbonden met: ");
  Serial.println(ssid);
  Serial.print("IP adres: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/style.css", handleCss);
  server.on("/led", handleLed);
  server.on("/ledFancy", handleLedFancy);
  server.on("/changeLedFancy", handleLedSwitchFancy);
  server.on("/changeLed", handleLedSwitch);
  server.on("/createPass", handleCreatePass);
  server.on("/inline", []() {
    server.send(200, "text/html", "<html><head><link rel=\"stylesheet\" href=\"style.css\"></head><body><div class=\"header-wrapper\"><h1>Ethernet Remote Inline page</h1></div>this works without need of authentification</body></html>");
  });
  server.onNotFound(handleNotFound);
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("Relay control is opgestart");
  digitalWrite(D0, HIGH);

}

void loop() {
  server.handleClient();
}
