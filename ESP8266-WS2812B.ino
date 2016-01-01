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

void handleLeds() {
  Serial.println("-------------------------------");
  Serial.println("Enter handleLeds");
  String header;
  if (is_authentified() < 2) {
    loginError = "U heeft niet de juiste rechten om dit te doen";
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
    
  }
  String content = "<html><head><title>Led control</title></head><body><H2>hello, leds things!</H2><div><table><tr><td>";
  content += knopmaken(1, "Uit"); 
  content += "</td></tr><tr>";
  content += knopmaken(2, "Rood");
  content += knopmaken(3, "Groen");
  content += knopmaken(4, "Blauw");
  content += "</tr><tr>";
  content += knopmaken(5, "Oranje");
  content += knopmaken(6, "Petrol");
  content += knopmaken(7, "Roze");
  content += "</tr><tr>";
  content += knopmaken(8, "Geel");
  content += knopmaken(9, "Licht blauw");
  content += knopmaken(10, "Paars");
  content += "</tr><tr>";
  content += knopmaken(11, "Wit");
  content += "</tr><tr>";
  content += knopmaken(12, "Regenboog");
  content += knopmaken(13, "Zoeklicht");
  content += knopmaken(14, "Regenboog zoeklicht");
  content += "</tr></table>";
  content += "</div><br>";
  content += "Made by Thijs &reg;<br><br> ";
  content += "<form action=\"/login?DISCONNECT=YES\">You can access this page until you <input type=\"submit\" value=\"Disconnect\"></form><br>";
  content += "<form action=\"/\">Terug naar<input type=\"submit\" value=\"Home\"></form></body></html>";
  server.send(200, "text/html", content);
  Serial.println("-------------------------------\n\n");

}

String knopmaken(int patternNum, String patternName)
{
  String output = "<td><a href=\"showLed?pattern=";
  output += patternNum;
  output += "\">";
  output += patternName;
  output += "</a></td>";
  return output;
}


//login page, also called for disconnect
void handleLogin() {
  Serial.println("-------------------------------");
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (has_access(server.arg("USERNAME"), server.arg("PASSWORD")) ) {
      Serial.println("Log in Successful");
      return;
    }
    msg = "<b><u>Fout wachtwoord of gebruikersnaam, probeer opnieuw!</b></u><br>";

    Serial.println("Log in Failed");
  }

  if (loginError != "")
  {
    msg = "<b><u>" + loginError + "</b></u><br>";
    loginError = "";
  }

  String content = "<html><head><title>LED systeem login</title></head><body><table><tr><h2>LED systeem login</h2></tr><form action='/login' method='POST'><tr><td>";
  content += "User:</td><td><input type='text' name='USERNAME' placeholder='user name'></td><br>";
  content += "</tr><tr><td>Password:</td><td><input type='password' name='PASSWORD' placeholder='password'></td></tr></table><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form><br>" + msg + "<br>";
  content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
  Serial.println("-------------------------------\n\n");
}

//no need authentification
void handleNotFound(){
  Serial.println("-------------------------------");
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println("-------------------------------\n\n");
}

//root page can be accessed only if authentification is ok
void handleRoot(){
  Serial.println("-------------------------------");
  Serial.println("Enter handleRoot");
  String header;
  int accesslevel = is_authentified();
  if (accesslevel == 0){
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  String content = "<html><head><title>Mainscreen Led control</title></head><body><H2>Mainscreen Led control</H2><br>";
  if (server.hasHeader("User-Agent")){
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  content += "<h3>You have got accesslevel: ";
  content += accesslevel;
  content += "</h3><br><a href=\"/leds\">Led control</a><Br><a href=\"/admin\">Admin control</a><br><a href=\"/login?DISCONNECT=YES\">Uitloggen</a></body></html>";
  server.send(200, "text/html", content);
  Serial.println("-------------------------------\n\n");
}

void fadeLed(int pin, int r, int g, int b){
  pixels.setPixelColor(pin, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 1, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 2, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 3, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 4, pixels.Color(r, g, b));
  r = r * .75;
  g = g * .75;
  b = b * .75;
  pixels.setPixelColor(pin - 5, pixels.Color(r, g, b));
  r = r * .75;
  g = g * .75;
  b = b * .75;
  pixels.setPixelColor(pin - 6, pixels.Color(r, g, b));
  pixels.setPixelColor(pin - 7, pixels.Color(0, 0, 0));
}

void fadeLedRed(int pin, int r, int g, int b){
  pixels.setPixelColor(pin - 7, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 6, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 5, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 4, pixels.Color(r, g, b));
  r = r * .5;
  g = g * .5;
  b = b * .5;
  pixels.setPixelColor(pin - 3, pixels.Color(r, g, b));
  r = r * .75;
  g = g * .75;
  b = b * .75;
  pixels.setPixelColor(pin - 2, pixels.Color(r, g, b));
  r = r * .75;
  g = g * .75;
  b = b * .75;
  pixels.setPixelColor(pin - 1, pixels.Color(r, g, b));
  pixels.setPixelColor(pin, pixels.Color(0, 0, 0));
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

int r_wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return 255 - WheelPos * 3;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return 0;
  } else {
    WheelPos -= 170;
    return WheelPos * 3;
  }
}

int g_wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return WheelPos * 3;
  } else {
    WheelPos -= 170;
    return 255 - WheelPos * 3;
  }
}

int b_wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return  WheelPos * 3;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return 255 - WheelPos * 3;
  } else {
    WheelPos -= 170;
    return 0;
  }
}

void fillLeds(int r, int g, int b){
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
}


//Check if header is present and correct
int is_authentified() {
  Serial.println("-------------------------------");
  Serial.println("Enter is_authentified");
  Serial.println(server.header(""));
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
     // Serial.println("Authentification Successful, Level 1");
      if (checkLevel(cookie.substring(24), 1))
          return 1;
      else {
        falseLogin();
          return 0;
        }
    }
    else if (cookie.indexOf("ESPSESSIONID=2") != -1) {
     // Serial.println("Authentification Successful, Level 2");
      cookie.substring(24);
      if (checkLevel(cookie.substring(24), 2))
          return 2;
      else {
        falseLogin();
          return 0;
        }

    }
    else if (cookie.indexOf("ESPSESSIONID=3") != -1) {
    //  Serial.println("Authentification Successful, Level 3");
      if (checkLevel(cookie.substring(24), 3))
          return 3;
      else {
        falseLogin();
        }
    }
  }
  Serial.println("Authentification Failed");
  return 0;
  Serial.println("-------------------------------\n\n");
}

bool has_access(String User, String Pass){
  if(User == "" || Pass == "")
  {
    return false;
  }
  for (int i = 0; i < accounts; i++) {
    if (User == user[i] && Pass == pass[i])
    {
      if (level[i] == 1)
      {
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nSet-Cookie: ESPNAME=";
        header += user[i] + "\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
      }
      if (level[i] == 2)
      {
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=2\r\nSet-Cookie: ESPNAME=";
        header += user[i] + "\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
      }
      if (level[i] == 3)
      {
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=3\r\nSet-Cookie: ESPNAME=";
        header += user[i] + "\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
      }
      return true;
    }
  }
  return false;
}

bool checkLevel(String username, int Level){
  for (int i = 0; i < accounts; i++) {
    if (username == user[i])
    {
      if (level[i] == Level)
      {
        return true;
      }
    }
  }
  loginError = "Uw gevegens kloppen niet, log opnieuw in!";
  return false;
}

void falseLogin(){
  String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nSet-Cookie: ESPNAME=Gast\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
  server.sendContent(header);
}

void handleAdmin()
{
  if (is_authentified() < 3) {
    loginError = "U heeft niet de juiste rechten om dit te doen";
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }

  String output = "<html><head><title>AdminPage</title></head><body><h1>AdminPage</h1><br>beheer hier uw accounts<br><table>";
  output += "<tr><td>user-ID</td><td>Gebruikersnaam</td><td>Wachtwoord</td><td>Level</td></tr>";
  
   for (int i = 0; i < accounts; i++) {
    output += "<tr><td><form action=\"/adminChange\"><input type='text' name='ID' value='";
    output += i;
    output += "'readonly></td><td>";
    output += "<input type='text' name='USERNAME' value='";
    output +=  user[i] ;
    output += "'></td><td><input type='text' name='PASSWORD' value='" ;
    output += pass[i]  ;
    output += "'></td><td><input type='text' name='LEVEL' value='" ;
    output +=  level[i];
    output += "'></td><td><input type='submit' name='SUBMIT' value='Submit'></form></tr>";
   }
   
   output += "</table><form action=\"/adminChange\"><input type='submit' name='add' value='Add account'><form><br><br>Made by Thijs &reg;<br><form action=\"/login?DISCONNECT=YES\">You can access this page until you <input type=\"submit\" value=\"Disconnect\"></form><br> <form action=\"/\">Terug naar <input type=\"submit\" value=\"Home\"></form>";
   output += "</body></html>";
   server.send(200, "text/html", output);
}

void handleAdminChange()
{
  if (is_authentified() < 3) {
    loginError = true;
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  
  if (server.hasArg("SUBMIT")){
    int i = server.arg("ID").toInt();
    if (user[i] != "admin") {
      
      if (server.arg("USERNAME") == "" || server.arg("PASSWORD") == "" || server.arg("LEVEL") == "") {
        String header = "HTTP/1.1 301 OK\r\nLocation: /admin\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
        return;
      }
      
      user[i] = server.arg("USERNAME");
      pass[i] = server.arg("PASSWORD");
      level[i] = server.arg("LEVEL").toInt();

      Serial.println("-------------------------");
      Serial.println("- Binnengekomen account -");
      Serial.println("-------------------------");
      Serial.println(server.arg("USERNAME"));
      Serial.println(server.arg("PASSWORD"));
      Serial.println(server.arg("LEVEL").toInt());
      Serial.println("-------------------------");
    }
  } 
  else if (server.hasArg("add")) {
    accounts++;
    if (accounts == 14)
      accounts = 13;
  }
  
  String header = "HTTP/1.1 301 OK\r\nLocation: /admin\r\nCache-Control: no-cache\r\n\r\n";
  server.sendContent(header);
}

void handleApp() {
  int accesslevel = is_authentified();
  if (accesslevel == 0) {
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    Serial.println("App Failed");
    return;
  }
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    if(cookie.indexOf("app") > -1){}
    else {Serial.println("Dit is niet de app");return;}
    Serial.println(cookie);
  }
  Serial.println("App Succes");
  fillLeds(server.arg("r").toInt(), server.arg("g").toInt(), server.arg("b").toInt());
  server.send(200, "text/html", "<html><body>Copy That</body></html>");
}

void handleSwitchLedState()
{
  Serial.println("-------------------------------");
  Serial.println("Entering switchLedState");
  Serial.println("checking...");
  
  if (is_authentified() < 2) {
    Serial.println("No access");
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  
  Serial.println("Access, change led state in a moment...");
  
  switch (server.arg("pattern").toInt()) {
    case 1:
      showLeds = 1;
      break;
    case 2:
      showLeds = 2;
      break;
    case 3:
      showLeds = 3;
      break;
    case 4:
      showLeds = 4;
      break;
    case 5:
      showLeds = 5;
      break;
    case 6:
      showLeds = 6;
      break;
    case 7:
      showLeds = 7;
      break;
    case 8:
      showLeds = 8;
      break;
    case 9:
      showLeds = 9;
      break;
    case 10:
      showLeds = 10;
      break;
    case 11:
      showLeds = 11;
      break;
    case 12:
      showLeds = 12;
      break;
    case 13:
      showLeds = 13;
      break;
    case 14:
      showLeds = 14;
      break;
    case 15:
      showLeds = 15;
      break;
    case 16:
      showLeds = 16;
      break;
    case 17:
      showLeds = 17;
      break;
    case 18:
      showLeds = 18;
      break;
    case 19:
      showLeds = 19;
      break;
    case 20:
      showLeds = 20;
      break;

  };

  String header = "HTTP/1.1 301 OK\r\nLocation: /leds\r\nCache-Control: no-cache\r\n\r\n";
  server.sendContent(header);
 Serial.println("Changed leds");
  pixels.clear();
  pixels.show();
  handleLeds();
}

