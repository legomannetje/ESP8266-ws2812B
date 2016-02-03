void handleRoot() {
  Serial.println("-------------------------------");//Printen van lijntje
  Serial.println("Enter handleRoot");       //Melden wat er gebeurd
  String header;
  int accesslevel = is_authentified();      //Het opvragen van het level van de persoon (gast is 0)
  if (accesslevel == 0) {                   //Als het gaat om een gast of nog niet ingelogd persoon
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n"; //Melden dat de pagina /login moet zijn
    server.sendContent(header);             //Zenden van de hierboven beschreven header
    return;
  }
  String content = "<html>";
  content += "<head><title>Ethernet Switchbox</title>\n<meta name=\"description\" content=\"Ethetnet Switchbox\"/>";
  content += "<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n";
  content += "<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">\n";
  content += "<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">\n";
  content += "<link rel=\"stylesheet\" href=\"style.css\">";
  content += "<body><div class=\"view\">\n     <div class=\"header-wrapper\">\n      <h1>Ethernet Remote</h1>\n</div>";
  content += "<H2>hello, you successfully connected to esp8266!</H2><br>"; //Maken van de pagina
  content += "<br><Br><a href=\"/led\">Led change</a>";
  content += "<br><Br><a href=\"/ledFancy\">Led change, but fancy</a>";
  if (accesslevel == 3) {
    content += "<br><Br><a href=\"/createPass\">Generate passwords</a>";
  }
  content += "<h3>You have got accesslevel: ";  //Melden welk level de persoon heeft
  content += accesslevel;
  content += "</h3><br><Br>You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>"; //uitlog link geven
  server.send(200, "text/html", content);   //Melden dat het goed gegaan is, het gaat om text/html en de code geven
  Serial.println("-------------------------------\n\n"); //Printen van lijntje
}

void handleNotFound() {
  Serial.println("-------------------------------");    //Printen van lijntje
  Serial.println("Pagina niet gevonden: 404");
  String message = "File Not Found\n\n";    //Begin van het bericht wat verzonden wordt naar client
  message += "URI: ";
  message += server.uri();                  //Vermelden het adres in de 404 melding
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST"; //Vermelden welke methode is gebruikt in het verzoek
  message += "\nArguments: ";
  message += server.args();                 //Hoeveel argumenten er gegeven zijn
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { //Alle argumenten geven
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);  //Melden dat het om een 404 gaat en de message versturen
  Serial.println("-------------------------------\n\n");  //Afsluiten
}

void handleLogin() {
  Serial.println("-------------------------------"); //Printen van lijntje
  if (server.hasHeader("Cookie")) {         //Kijken of er in de header cookie aanwezig is
    Serial.print("Found cookie: ");         //Zo ja melden
    String cookie = server.header("Cookie"); //En de header naar een string zetten
    Serial.println(cookie);                 //En deze printen
  }
  if (server.hasArg("DISCONNECT")) {        //Als er disconnect voorkomt in de header
    Serial.println("Disconnection");        //Printen dat dit zo is
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n"; //deze header zenden zodat de cookie naar 0 gezet word
    server.sendContent(header);             //Header verzenden
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {   //Als de header Username en Password bevat
    if (has_access(server.arg("USERNAME"), server.arg("PASSWORD")) ) { //Check of de gegevens overeenkomen met de gegevens boven in dit document
      Serial.println("Log in Successful");  //Melden dat het inloggen succesvol is mocht dat zo zijn
      return;
    }
    loginError = "Verkeerde gebruikersnaam of wachtwoord, probeer het opnieuw"; //Als dit niet is melden dat dit niet zo is
    Serial.println("Log in Failed");          //Melden dat inloggen mis gegaan is
  }
  if (loginError == "")
    loginError = "Welkom terug!";
    
  String content = "<html>";
  content += "<head><title>Ethernet Switchbox</title>\n<meta name=\"description\" content=\"Ethetnet Switchbox\"/>";
  content += "<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n";
  content += "<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">\n";
  content += "<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">\n";
  content += "<link rel=\"stylesheet\" href=\"style.css\">";
  content += "<body><div class=\"view\">\n     <div class=\"header-wrapper\">\n      <h1>Ethernet Remote</h1>\n</div>";
  content += "<center><form action='/login' method='POST'><br><table><tr><td>"; //Content maken om pagina te weergeven
  content += "User:</td><td><input type='text' name='USERNAME' placeholder='user name'></td><br>";
  content += "</tr><tr><td>Password:</td><td><input type='password' name='PASSWORD' placeholder='password'></td></tr><br>";
  content += "<tr><td></td><td><input type='submit' name='SUBMIT' value='Login'></form></td></tr></table><table><tr><td>" + loginError + "</td></tr></table>"; //Als inlog poging mislukt is dit er tussen zetten
  content += "You also can go <a href='/inline'>here</a></center></body></html>";
  server.send(200, "text/html", content);   //Zie eerder
  Serial.println("-------------------------------\n\n"); //Print lijntje
  loginError = "";
}


