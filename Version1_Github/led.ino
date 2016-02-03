void handleLed() {

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
  content += "<body><div class=\"view\">\n     <div class=\"header-wrapper\">\n      <h1>Relay control</h1>\n</div>";

  content += "<table border=\"0\" align=\"center\"><tr><td><h3>Led control</h3></td></tr>";

  content += "<tr><td><form action=\"changeLed\"><input type='submit' name='quickOn' value ='Allemaal Aan'></form></td>";
  content += "<td><form action=\"changeLed\"><input type='submit' name='quickOff' value ='Allemaal Uit'></form></td>";
  content += "<td><form action=\"changeLed\"><input type='submit' name='Reverse' value ='Reverse'></form></tr>";

  int x = 0;
  while (x != aantalKnoppen) {
    content += "<tr><td>";
    content += knopNaam[x];
    content += "</td><td>";
    if (!reverseOutput) {
      if (led_status[x])
        content += "<div class='green-circle'><div class='glare'></div></div>";
      else
        content += "<div class='black-circle'><div class='glare'></div></div>";
    } else {
      if (!led_status[x])
        content += "<div class='green-circle'><div class='glare'></div></div>";
      else
        content += "<div class='black-circle'><div class='glare'></div></div>";
    }
    content += "</td><td>";
    content += "<form action=\"changeLed\"><input type='submit' name='led' value='";
    content += x + 1;
    content += "'></form></td></tr>";
    x++;
  }
  content += "<tr><td></td><td><a href=\"/\">Home</a></td></tr>";
  content += "</center></table></body></html>";

  server.send(200, "text/html", content);
}

void handleLedSwitch()
{
  if (server.hasArg("led")) {
    int i = server.arg("led").toInt();
    Serial.print("Led Switch id: ");
    Serial.println(i);
    i = i - 1;
    if (led_status[i])
      led_status[i] = false;
    else
      led_status[i] = true;
    digitalWrite(knopPin[i], led_status[i]);
    EEPROM.write(i, led_status[i]);
    EEPROM.commit();
  }
  if (server.hasArg("Reverse")) {
    int i = 0;
    while (i != aantalKnoppen) {
      if (led_status[i] == 1) {
        EEPROM.write(i , 0);
        led_status[i] = 0;
        digitalWrite(knopPin[i], 0);
      }
      else {
        EEPROM.write(i , 1);
        led_status[i] = 1;
        digitalWrite(knopPin[i], 1);
      }
      i++;
    }
    EEPROM.commit();
  }
  if (reverseOutput) {
    if (server.hasArg("quickOn")) {
      int i = 0;
      while (i != aantalKnoppen) {
        EEPROM.write(i , 0);
        led_status[i] = 0;
        digitalWrite(knopPin[i], 0);
        i++;
      }
      EEPROM.commit();
    }
    else if (server.hasArg("quickOff")) {
      int i = 0;
      while (i != aantalKnoppen) {
        EEPROM.write(i , 1);
        led_status[i] = 1;
        digitalWrite(knopPin[i], 1);
        i++;
      }
      EEPROM.commit();
    }
  } else {
    if (server.hasArg("quickOn")) {
      int i = 0;
      while (i != aantalKnoppen) {
        EEPROM.write(i , 1);
        led_status[i] = 1;
        digitalWrite(knopPin[i], 1);
        i++;
      }
      EEPROM.commit();
    }
    else if (server.hasArg("quickOff")) {
      int i = 0;
      while (i != aantalKnoppen) {
        EEPROM.write(i , 0);
        led_status[i] = 0;
        digitalWrite(knopPin[i], 0);
        i++;
      }
    }
    EEPROM.commit();
  }
  String header = "HTTP/1.1 301 OK\r\nLocation: /led\r\nCache-Control: no-cache\r\n\r\n"; //Melden dat de pagina /login moet zijn
  server.sendContent(header);             //Zenden van de hierboven beschreven header
  return;
}

