void handleCreatePass() {
  Serial.println("-------------------------------");//Printen van lijntje
  Serial.println("Enter handleRoot");       //Melden wat er gebeurd
  String header;
  int accesslevel = is_authentified();      //Het opvragen van het level van de persoon (gast is 0)
  if (accesslevel != 3) {                   //Als het gaat om een gast of nog niet ingelogd persoon
    loginError = "U heeft niet de juiste rechten om deze pagina te bezoeken!";
    String header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n"; //Melden dat de pagina /login moet zijn
    server.sendContent(header);             //Zenden van de hierboven beschreven header
    return;
  }
  bool hasArg = false;
  String toHash;
  if (server.hasArg("toPass"))
  {
    hasArg = true;
    toHash = hashIt(server.arg("toPass"), salt);
  }
  String content = "<html>";
  content += "<head><title>Password Converter</title>\n<meta name=\"description\" content=\"Ethetnet Switchbox\"/>";
  content += "<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n";
  content += "<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">\n";
  content += "<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\">\n";
  content += "<link rel=\"stylesheet\" href=\"style.css\"></head>";
  content += "<body><div class=\"view\">\n     <div class=\"header-wrapper\">\n      <h1>Password Converter</h1>\n</div><br><br>";
  content += "<center><form action=\"createPass\"><input type='text' name='toPass' value='";

  content += "'><input type='submit' value='Convert'></form>";
  if (hasArg) {
    content += "<br>Output: " + toHash;
  }

  content += "<br><a href=\"/\">Home</a></body></html>";
  server.send(200, "text/html", content);
}

String hashIt(String input, String Salt)
{
  String output = "";
  output = input + sha1(Salt);
  output = sha1(sha1(output));
  return output;
}

String hashIt(String input)
{
  String output = "";
  output = sha1(sha1(output));
  return output;

}

