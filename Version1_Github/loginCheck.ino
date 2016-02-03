//Check if header is present and correct
int is_authentified() {
  Serial.println("-------------------------------"); //Print lijntje
  Serial.println("Enter is_authentified");    //melden wat er gaat gebeuren
  if (server.hasHeader("Cookie")) {           //Kijken of cookie er is
    Serial.print("Found cookie: ");           //Zo ja, melden
    String cookie = server.header("Cookie");  //Zie eerder
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) { //Als de cookie dit bevat (=1) dan 
      Serial.println("Authentification Successful, Level 1");   //Melden dat level 1 is
      return 1;                               //Getal 1 retouneren
    }
    else if (cookie.indexOf("ESPSESSIONID=2") != -1) {  //Zie hierboven
      Serial.println("Authentification Successful, Level 2");
      return 2;
    }
    else if (cookie.indexOf("ESPSESSIONID=3") != -1) {  //Zie hierboven
      Serial.println("Authentification Successful, Level 3");
      return 3;
    }
  }
  Serial.println("Authentification Failed");  //Als anders
  return 0;                                 //Return
  Serial.println("-------------------------------\n\n"); //Print lijntje
}


bool has_access(String User, String Pass)
{
  String checkPass = hashIt(Pass, salt);
  for (int i = 0; i < accounts; i++) {      //for loop dat je alle accounts afloopt
    if (User == user[i] && checkPass == pass[i]) //Kijken of het overeenkomt
    {
      if (level[i] == 1)                    //Kijken welk level deze persoon is
      {
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);         //Verzenden van de hierboven beschreven header
      }
      if (level[i] == 2)
      {
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=2\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
      }
      if (level[i] == 3)
      {
        String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=3\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
        server.sendContent(header);
      }
      return true;                         //Melden dat het een geregisteerd account is
    }
  }
  return false;                            //Melden dat de combinatie niet bekend is
} 
