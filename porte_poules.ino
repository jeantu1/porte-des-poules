//************ données pour le temps NTP le 24/11/2020
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <time.h>
const char* ssid = "mon_reseau";
const char* password = "mon_mot_de_passe_reseau";

const uint16_t port = 80;
unsigned long decalage_millis = 0;
const int decalage = 1;  // la valeur dépend de votre fuseau horaire. Essayez 2 pour la France.
const int delaiDemande = 5 * 60; // nombre de secondes entre deux demandes consécutives au serveur NTP
const int EteHivers = 3600;  // variable de changement heure été hivers (été = 0 ; hivers =3600)
//unsigned long MaintenantCorrige = 0;
unsigned long derniereDemande = millis(); // moment de la plus récente demande au serveur NTP
unsigned long derniereMaJ = millis(); // moment de la plus récente mise à jour de l'affichage de l'heure
time_t maintenant;
struct tm * timeinfo;
float NTP_partie_decimal = 0;

//************* données pour le client "heure de levé soleil" (https)
#include <WiFiClientSecure.h>
const char* httpshost = "api.sunrise-sunset.org";
const int httpsPort = 443;
String h = "";
String h_leve_soleil = "";
int h_leve_soleil_modifiee = 0;
int mn_leve_soleil_modifiee = 0;
String mn_leve_soleil = "";
float leve_soleil_hmn_decimale = 0;
const char fingerprint[] PROGMEM = "mon_adresse_page_meteo"; //créé un compte et voir https://www.tutiempo.net/  et  https://api.tutiempo.net/

//************* données autre
//int ledPin = 5;// led branchée sur la pin D1
int relais = 4;// relais branché sur la pin D2
int ledErreur = 14;// branchée sur la pin D5
int drapeau = 0;
int temps_monte_relais = 420;// temps en mn de la fermeture du relais (temps de coulure eau ouverture porte)
//float temps_temporaire = 0;
//*************Paramètres WiFi courriel IFTTT:
char MakerIFTTT_Key[] = "identifiant perso ifttt"; //créé un compte voir https://ifttt.com/maker_webhooks pour obtenir le numéro personnel de IFTTT
char MakerIFTTT_Event[] = "wemos-trigger"; // nom de l'event que j'ai créé
WiFiClient client;
int code1 = 0;
int code2 = 0;
char * message = " ";//long max message..........................................................
int drapeau_box = 0;//presence box 0=inactive 1=active 2=pb avec serveur temps
int comptage_drapeau = 0;
//*************Paramètres test connection internet:
const char* host_test_internet = "djxmmx.net";
const uint16_t port_test_internet = 17;

//******************************************************************************************************************
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  //  pinMode(ledPin, OUTPUT);  // put your setup code here, to run once:
  pinMode(relais, OUTPUT);
  pinMode(ledErreur, OUTPUT);
  digitalWrite(relais, LOW);
  //  digitalWrite(ledPin, HIGH);
  analogWrite(ledErreur, 0);


  // si pas de connection re_essai dans le 1/4 d'heure suivant
  do {
    if ((drapeau_box == 0) && (millis() >= (comptage_drapeau)))
    {
      test_acces_internet();
      comptage_drapeau = millis() + (15 * 60 * 1000);
      //comptage_drapeau = millis() + (2 * 60 * 1000);
      Serial.println("");
      Serial.println("relance attente 15 mn pour recommencer wifi");
      Serial.print("drapeau_box = "); Serial.println(drapeau_box);

    }
    //Serial.print("millis() "); Serial.println(millis());
    delay(1);//attention si pas de delay le chien de garde reboot arduino****!!!!!!!!!!!!!
  }  while (drapeau_box == 0);

  envoye_message("Demarrage de l'arduino porte poulaillet");//envoye courriel

  appel_NTP();//Fonction d'appel temps serveur internet NTP pour synchroniser l'horloge
  traitement_heure(maintenant + 3600);// Sépare en partie entiére et en décimale le temps NTP
  afficheHeureDate();//affiche la date et l'heure actuelle
  InterogationHTTPSLeveSoleil();//récupére l'heure de levé du soleil sur le net
  transforme_HeureEnDecimale(h_leve_soleil_modifiee, mn_leve_soleil_modifiee);// transforme les h:mn soleil en décimale ex: 0,12345
  Serial.println("fin du setup============================================");


}

//******************************************************************************************************************
void loop() {

  // récupére l'heure du jour
  time(&maintenant);// cette fonction permet de mettre la variable "maintenant" à l'heure (été hivers prés)
  traitement_heure(maintenant + 3600);// Sépare en partie entiére et en décimale le temps NTP

  if ((h_leve_soleil == 0) || (time(&maintenant) < 100000000)) //controle led Erreur
    //100000000 représente environ 3ans ce qui indique que la mise à l'heure n'a pas fontionnée
  {
    analogWrite(ledErreur, 0);//allumage de la led
    if (time(&maintenant) < 100000000)//
    {
      for (int y = 0; y <= 5; y ++)//clignottement lent pb mise à l'heure
      {
        analogWrite(ledErreur, 1023);
        delay(1000);
        analogWrite(ledErreur, 0);
        delay(1000);
      }
    }
    if (h_leve_soleil == 0)
    {
      for (int x = 0; x <= 51; x ++)//clignottement rapide erreur h soleil
      {
        analogWrite(ledErreur, 1023);
        delay(100);
        analogWrite(ledErreur, 0);
        delay(100);
      }
    }
    analogWrite(ledErreur, 0);
  }
  if ((time(&maintenant) > 100000000) && (h_leve_soleil != 0)) analogWrite(ledErreur, 1023);

  // test modification déboguage
  //leve_soleil_hmn_decimale = 0.3958333;
  //NTP_partie_decimal = 0.6875;


  Serial.print("heure leve du soleil en decimale ");  Serial.println(leve_soleil_hmn_decimale, 5);
  Serial.print("NTP_partie_decimal ");  Serial.println(NTP_partie_decimal, 5);
  Serial.print("maxi leve_soleil_hmn_decimale ");  Serial.println(leve_soleil_hmn_decimale + (1 / 24. / 60.) * temps_monte_relais, 5); //1/24/60*5 pour donner un temps de 5mn

  if (((NTP_partie_decimal >  leve_soleil_hmn_decimale )) && (NTP_partie_decimal < ((leve_soleil_hmn_decimale) + ((1. / 24. / 60.) * temps_monte_relais)))) //1/24/60*5 pour donner un temps de 5mn

  {
    Serial.println("fermeture du contact relais (c'est à dire mise en route)");
    digitalWrite(LED_BUILTIN, LOW);
    //    digitalWrite(ledPin, LOW);//led
    digitalWrite(relais, HIGH);//relais
  }
  else
  {
    Serial.println("ouverture du contact relais (c'est à dire arrêt)");
    digitalWrite(LED_BUILTIN, HIGH);
    //    digitalWrite(ledPin, HIGH);//led
    digitalWrite(relais, LOW);//relais
  }

  // test s'il est temps de mettre à l'heure par le net et récupérer l'heure levé soleil
  //Serial.println("**********avant mon test");
  if (NTP_partie_decimal < 0.04444444) (drapeau = 0); // mis à 1 empéche de passer plusieurs fois dans la condition if suivante. elle le met à 0 à 1h du matin
  //if (NTP_partie_decimal >  temps_temporaire )//test
  if ((NTP_partie_decimal >  0.41666666 ) && ( drapeau == 0))//met à l'heure chaque jour à 10h00 le matin

  {
    //temps_temporaire = (NTP_partie_decimal + 0.001388888 );
    envoye_message("mise à l'heure de arduino + levé soleil");//envoye courriel
    drapeau = 1;
    Serial.println("mise à jour par internet de l'heure et du levé soleil");
    appel_NTP();//Fonction d'appel temps serveur internet NTP pour synchroniser l'horloge
    traitement_heure(maintenant + 3600);// Sépare en partie entiére et en décimale le temps NTP
    afficheHeureDate();//affiche la date et l'heure actuelle
    InterogationHTTPSLeveSoleil();//récupére l'heure de levé du soleil sur le net
    transforme_HeureEnDecimale(h_leve_soleil_modifiee, mn_leve_soleil_modifiee);// transforme les h:mn soleil en décimale ex: 0,12345
  }
  else
  {
    Serial.println("pas encore de mise à jour du net");
  }
  //Serial.println("**********aprés mon test");

  delay (30000);
}

//******************************************************************************************************************
void afficheHeureDate()//affiche l'heure compréhensible par humain
{
  delay (30000);
  time(&maintenant);// cette fonction permet de mettre la variable "maintenant" à l'heure (été hivers prés)

  timeinfo = localtime(&maintenant) ;

  Serial.print("temps en seconde depuis le 01/01/1970 au chgt horaire prés "); Serial.println(maintenant + EteHivers);

  Serial.print("Heure:   ");
  if ((timeinfo->tm_hour ) < 10) {
    Serial.print("0");
  }
  Serial.print(timeinfo->tm_hour );  // heure entre 0 et 23
  Serial.print(":");

  if (timeinfo->tm_min < 10) {
    Serial.print("0");
  }
  Serial.print(timeinfo->tm_min);   // timeinfo->tm_min: minutes (0 - 59)
  Serial.print(":");   float seconde = 0;
  seconde = (float)(maintenant + EteHivers) / 60. / 60. / 24;
  // reste à traiter et tester la partie_entier et NTP_partie_decimal;

  if (timeinfo->tm_sec < 10) {
    Serial.print("0");
  }//1/24/620*5 pour donner un temps de 5mn
  Serial.print(timeinfo->tm_sec);   // timeinfo->tm_sec: secondes (0 - 60)

  Serial.print("        Date:    ");

  if (timeinfo->tm_mday < 10) {
    Serial.print("0");
  }
  Serial.print(timeinfo->tm_mday);  // timeinfo->tm_mday: jour du mois (1 - 31)
  Serial.print("-");

  if ((timeinfo->tm_mon + 1) < 10) {
    Serial.print("0");
  }

  Serial.print(timeinfo->tm_mon + 1);    // timeinfo->tm_mon: mois (0 - 11, 0 correspond à janvier)
  Serial.print("-");
  Serial.println(timeinfo->tm_year + 1900);  // timeinfo->tm_year: tm_year nombre d'années écoulées depuis 1900

}

//******************************************************************************************************************
void appel_NTP()//Fonction d'appel temps serveur internet NTP pour synchroniser l'horloge
{
  //fixation de l'adresse ip
  IPAddress ip(192, 168, 0, 20);
  IPAddress dns(192, 168, 0, 254);
  IPAddress gateway(192, 168, 0, 254);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, dns, gateway, subnet);
  Serial.println();
  WiFi.mode(WIFI_STA);//se met en réseau local
  WiFi.begin(ssid, password);//se connect a la box

  Serial.print("Connexion au reseau WiFi pour aller chercher l'heure du net");// attente de la connection wifi
  int echec_connect_wifi = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print("attente connection NTP prise heure sur le net= ");  Serial.println(WiFi.status());
    /*    //traitement de l'echec connection wifi
        echec_connect_wifi = echec_connect_wifi + 1;
        if (echec_connect_wifi > 5)
        {
          Serial.print("sortir");
        }
    */
  }

  Serial.println("");  // affichage de l'ip par jmt le 1/11/2020
  Serial.print("WiFi connecté à l'addresse IP:");
  Serial.println(WiFi.localIP());

  //pour test perte adresse horloge


  //récupération de l'heure sur le net
  configTime(decalage * 3600, 0, "ntp.unice.fr");//serveurs ntp  https://services.renater.fr/ntp/serveurs_francais
     Serial.print("time(nullptr) "); Serial.println(time(nullptr));

  while (time(nullptr) <= 6000)//si problème de mise a jour de l'heure regarder ici si les 6000 sont suffisant
  {
     if (time(nullptr) == 5900)
    {
      drapeau_box = 2;
      envoye_message("Pas de mise à l'heure arduino");//envoye courriel
      Serial.println("envoyé message pb h");
      Serial.print("maintenant ="); Serial.println(maintenant);
      while (maintenant == 0)
      {
        analogWrite(ledErreur, 0);//allumage de la led
        Serial.println("programme bloqué dans boucle fonction appel_NTP car pas acces au site");
        delay(1000);
      }
    }
    //Serial.println(time(nullptr));
    Serial.print(".");
    delay(1000);
  }
  Serial.print("drapeau_box ="); Serial.println(drapeau_box);


  time(&maintenant);// cette fonction permet de mettre la variable "maintenant" à l'heure (été hivers prés)
  Serial.print(" maintenant "); Serial.println(maintenant + EteHivers);


  //configTime(decalage * 3600, 0, "ntp.unice.fr");//serveurs ntp  https://services.renater.fr/ntp/serveurs_francais
  //maintenant est le temps écoulé en seconde depuis le 1/1/1970 attention au changement été hiver

  /*  // Attente date et heure
    while (time(nullptr) <= 100000)
    {
      Serial.print(".");
      delay(1000);
    }
  */

  float seconde = 0;
  seconde = (float)(maintenant + EteHivers) / 60. / 60. / 24;
  Serial.print("Date en décimale :");  Serial.println(seconde, 5);

  WiFi.forceSleepBegin();  //pour arreter le wifi
}

//******************************************************************************************************************
void traitement_heure(unsigned long heure )// Sépare en partie entiére et en décimale le temps NTP
{
  Serial.print("heure "); Serial.println(heure);
  long a = heure ;
  long b = 60. * 60. * 24.;
  int partie_entier;
  int reste;
  //float NTP_partie_decimal;
  partie_entier = a / b;
  NTP_partie_decimal = (a - (b * partie_entier)) / (float)b;
}

//******************************************************************************************************************
void InterogationHTTPSLeveSoleil()//récupére l'heure de levé du soleil sur le net
{
  Serial.println();
  // https://translate.google.com/translate?hl=fr&sl=auto&u=https%3A%2F%2Fgithub.com%2Fesp8266%2FArduino%2Fissues%2F3417
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  WiFiClientSecure client;
  client.setFingerprint(fingerprint);

  if (!client.connect(httpshost, httpsPort)) {
    Serial.println("connection echec pour récupére l'heure de levé du soleil");
    //Serial.println("modifier prog pour définir l'heure par défaut");
    envoye_message("Pas d'heure levé soleil");//envoye courriel
    while (1)// boucle infinie
    {
      analogWrite(ledErreur, 0);//allumage de la led
      Serial.println("programme bloqué dans boucle InterogationHTTPSLeveSoleil car pas acces au site");
      delay(1000);
    }
    return;
  }

  String url = "/json?lat=48.289249&lng=2.944234";//lieu gps du poulaillé lol

  // controle du certificat
  if (client.verify(fingerprint, httpshost)) {
    // Serial.println("certificat OK");
  } else {
    Serial.println("certificat non correct");
  }
  //  Serial.println("envoi requette");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + httpshost + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");


  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      // Serial.println(line);
      //  Serial.println("en-têtes reçus");

      while (client.available())
      {
        int n = n + 1;
        char c = client.read();
        //  Serial.print(c);

        h = h + c;

      }
      break;
    }
  }

  String line = client.readStringUntil('\n');
  // Serial.println(line);

  //Serial.print("jmt.");
  //Serial.println(h);

  //  if (line.startsWith("{\"state\":\"success\"")) {
  //    Serial.println("esp8266/Arduino CI successfull!");
  //  } else {
  //    Serial.println("esp8266/Arduino CI a échoué");
  //  }
  //Serial.println("la réponse était:");
  //Serial.println("==========");
  //Serial.println(h);//line);
  //Serial.println("==========");
  //Serial.println("fermeture connection");

  /*
    //levé du soleil
    h_leve_soleil = (h.substring(28, 29));
    mn_leve_soleil = (h.substring(30, 32));
    Serial.print("Levé du soleil (ajouter 1h )"); Serial.println(h.substring(28, 32));
    //String myString = "125";
    h_leve_soleil_modifiee = h_leve_soleil.toInt() + 1;
    mn_leve_soleil_modifiee = mn_leve_soleil.toInt();
    Serial.print("heure corrigée "); Serial.print(h_leve_soleil_modifiee);  Serial.print(":"); Serial.println(mn_leve_soleil);
  */

  //couché du soleil
  Serial.println("Attention c'est le couché soleil qui est choisi dans ce script");
  h_leve_soleil = (h.substring(50, 51));
  mn_leve_soleil = (h.substring(52, 54));
  Serial.print("couché du soleil (ajouter 1h )"); Serial.println(h.substring(50, 54));
  //String myString = "125";
  h_leve_soleil_modifiee = h_leve_soleil.toInt() + 13;
  mn_leve_soleil_modifiee = mn_leve_soleil.toInt();
  Serial.print("heure corrigée "); Serial.print(h_leve_soleil_modifiee);  Serial.print(":"); Serial.println(mn_leve_soleil);

  WiFi.forceSleepBegin();  //pour arreter le wifi (asser long en connection https; au moins 3mn)
}

//******************************************************************************************************************
void transforme_HeureEnDecimale(int HeureHumain, int minute_humain)// transforme les h:mn en décimale ex: 0,12345
{
  leve_soleil_hmn_decimale = ((1 / 24.) * HeureHumain) + ((1 / 24. / 60.) * minute_humain);
  Serial.print("L'heure levé soleil "); Serial.print(HeureHumain);  Serial.print("h");  Serial.print(minute_humain); Serial.print(" donne en décimale ");
  Serial.println( leve_soleil_hmn_decimale , 5);
  delay (1000);
}

//******************************************************************************************************************
void setupWiFi() {//envoye message
  Serial.println();
  Serial.println();
  Serial.print("Connecte a ");
  Serial.print(ssid);
  Serial.println("...");
  WiFi.begin(ssid, password);
  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 15)) {
    retries++;
    delay(500);
    Serial.print(".");
  } if (retries > 14) {
    drapeau_box = 0;
    comptage_drapeau = millis();
    Serial.println(F("Connection WiFi HS"));
  }
  if (WiFi.status() == WL_CONNECTED) {
    drapeau_box = 1;
    acces_iftt_envoye_courriel();
    Serial.println(F("WiFi connecté!"));
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  Serial.println(F("Setup ready"));
}


// ***  fonctions et constructing du POST data message***
// append a string or int to a buffer, return the resulting end of string
char *append_str(char *here, char *s) {
  while (*here++ = *s++)
    ;
  return here - 1;
}
char *append_ul(char *here, unsigned long u) {
  char buf[20];       // we "just know" this is big enough
  return append_str(here, ultoa(u, buf, 10));
}

// Send a POST to trigger the IFTTT/Maker event
void acces_iftt_envoye_courriel() {
  // connect to the Maker event server
  client.connect("maker.ifttt.com", 80);
  // construct the POST request
  char post_rqst[256];    // hand-calculated to be big enough
  char *p = post_rqst;
  p = append_str(p, "POST /trigger/");
  p = append_str(p, MakerIFTTT_Event);
  p = append_str(p, "/with/key/");
  p = append_str(p, MakerIFTTT_Key);
  p = append_str(p, " HTTP/1.1\r\n");
  p = append_str(p, "Host: maker.ifttt.com\r\n");
  p = append_str(p, "Content-Type: application/json\r\n");
  p = append_str(p, "Content-Length: ");

  char *content_length_here = p; // we need to remember where the content length will go, which is:

  p = append_str(p, "NN\r\n"); // it's always two digits, so reserve space for them (the NN)
  p = append_str(p, "\r\n"); // end of headers
  char *json_start = p; // construct the JSON; remember where we started so we will know len

  // As described - this example reports a pin, uptime, and "hello world"
  Serial.print("envoye courriel millis = "); Serial.println( millis() );
  p = append_str(p, "{\"value1\":\"");
  p = append_ul(p, code1);
  p = append_str(p, "\",\"value2\":\"");
  p = append_ul(p,  millis() / 1000 / 60);
  p = append_str(p, "\",\"value3\":\"");
  p = append_str(p, message);//*********** message envoyé
  p = append_str(p, "\"}");
  //  Serial.print("value2  :"); Serial.println(env_message);
  //  Serial.print("analogRead(pot)"); Serial.println(analogRead(pot));
  // go back and fill in the JSON length
  // we just know this is at most 2 digits (and need to fill in both)
  int i = strlen(json_start);
  content_length_here[0] = '0' + (i / 10);
  content_length_here[1] = '0' + (i % 10);

  // finally we are ready to send the POST to the server!
  client.print(post_rqst);
  client.stop();
}
//******routine d'envoye message ***********
void envoye_message(char * message_essai)
{
  message = message_essai; //long max message..........................................................
  //connection ifttt et envoy message
  setupWiFi(); //call our Wrelance arduinoiFi method to connect to the internet

  WiFi.disconnect();
}

//******test d'acces à internet ***********
void test_acces_internet ()
{
  //fonction test
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("connecting to ");
  Serial.print(host_test_internet);
  Serial.print(':');
  Serial.println(port_test_internet);
  //Serial.println("on à bien internet");
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host_test_internet, port_test_internet)) {
    Serial.println("connection failed");
    //ici drapeau hs
    drapeau_box = 0;
    delay(5000);
    return;
  }
  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }

  // wait for data to be available
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }
  Serial.println("connection établie");
  //ici drapeau ok
  drapeau_box = 1;
  // Read all the lines of the reply from server and print them to Serial
  //Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    //Serial.print(ch);
  }
  // Close the connection
  //Serial.println();
  Serial.println("closing connection");
  client.stop();

  // delay(300000); // execute once every 5 minutes, don't flood remote service
}
