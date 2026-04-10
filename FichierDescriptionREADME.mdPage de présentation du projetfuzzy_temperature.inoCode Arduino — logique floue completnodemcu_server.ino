/*
 * ============================================================
 * SERVEUR WEB NODEMCU ESP8266 — RÉGULATEUR IoT
 * ============================================================
 * Auteure  : Fedia GUIRAT
 * Projet   : PFE Ingénieure en Informatique Embarquée
 * École    : ITBS Nabeul, Tunisie
 * Année    : 2017-2018
 *
 * Description :
 * Ce programme transforme le NodeMCU ESP8266 en serveur web.
 * Il permet la supervision et la configuration à distance du
 * régulateur de température via une interface web accessible
 * depuis un PC ou un smartphone connecté au même réseau WiFi.
 * ============================================================
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ============================================================
// CONFIGURATION WIFI
// ============================================================
const char* ssid     = "VOTRE_WIFI";       // Nom de votre réseau WiFi
const char* password = "VOTRE_MOT_DE_PASSE"; // Mot de passe WiFi

// ============================================================
// SERVEUR WEB SUR PORT 80
// ============================================================
ESP8266WebServer server(80);

// ============================================================
// VARIABLES PARTAGÉES AVEC LA CARTE DE COMMANDE
// ============================================================
float temperature     = 0.0;
float pression        = 0.0;
int   consigne        = 25;
bool  regulationAuto  = true;
bool  chauffageActif  = false;
bool  ventilActif     = false;
String modeRegulation = "fuzzy"; // "fuzzy" ou "TOR"

// ============================================================
// PAGE HTML DE L'INTERFACE WEB
// ============================================================
const char MAIN_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Régulateur Connecté</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f0f4f8;
      margin: 0;
      padding: 20px;
      color: #333;
    }
    h1 {
      color: #2c3e50;
      text-align: center;
      border-bottom: 2px solid #3498db;
      padding-bottom: 10px;
    }
    .card {
      background: white;
      border-radius: 10px;
      padding: 20px;
      margin: 15px auto;
      max-width: 500px;
      box-shadow: 0 2px 8px rgba(0,0,0,0.1);
    }
    .value {
      font-size: 2em;
      font-weight: bold;
      color: #e74c3c;
      text-align: center;
    }
    .label {
      text-align: center;
      color: #7f8c8d;
      margin-bottom: 5px;
    }
    .btn {
      display: block;
      width: 100%;
      padding: 12px;
      margin: 8px 0;
      border: none;
      border-radius: 8px;
      font-size: 1em;
      cursor: pointer;
      text-decoration: none;
      text-align: center;
    }
    .btn-on  { background: #27ae60; color: white; }
    .btn-off { background: #e74c3c; color: white; }
    .btn-blue { background: #3498db; color: white; }
    .status-on  { color: #27ae60; font-weight: bold; }
    .status-off { color: #e74c3c; font-weight: bold; }
  </style>
  <script>
    // Rafraîchissement automatique toutes les 5 secondes
    setTimeout(function(){ location.reload(); }, 5000);
  </script>
</head>
<body>
  <h1>🌡️ Régulateur Connecté</h1>

  <div class="card">
    <div class="label">Température actuelle</div>
    <div class="value" id="temp">-- °C</div>
    <div class="label">Pression actuelle</div>
    <div class="value" id="pres" style="color:#3498db">-- hPa</div>
  </div>

  <div class="card">
    <p><strong>Mode de régulation :</strong> Logique Floue (Fuzzy)</p>
    <p><strong>Chauffage :</strong>
      <span class="status-on">ACTIF</span>
    </p>
    <p><strong>Ventilateur :</strong>
      <span class="status-off">INACTIF</span>
    </p>
  </div>

  <div class="card">
    <a href="/ledon"  class="btn btn-on">🔥 Activer Chauffage</a>
    <a href="/ledoff" class="btn btn-off">❄️ Arrêter Chauffage</a>
    <a href="/"       class="btn btn-blue">🔄 Actualiser</a>
  </div>

  <div class="card" style="text-align:center; color:#7f8c8d; font-size:0.9em">
    Projet PFE — Fedia GUIRAT — ITBS 2018
  </div>
</body>
</html>
)=====";

// ============================================================
// GESTIONNAIRES DES ROUTES WEB
// ============================================================

// Page principale
void handleRoot() {
  server.send(200, "text/html", MAIN_PAGE);
}

// Activer le chauffage manuellement
void handleLedOn() {
  chauffageActif = true;
  Serial.println(">>> Chauffage activé via interface web");
  server.send(200, "text/html",
    "<html><body><h2>Chauffage ACTIVÉ</h2>"
    "<a href='/'>Retour</a></body></html>");
}

// Désactiver le chauffage manuellement
void handleLedOff() {
  chauffageActif = false;
  Serial.println(">>> Chauffage désactivé via interface web");
  server.send(200, "text/html",
    "<html><body><h2>Chauffage DÉSACTIVÉ</h2>"
    "<a href='/'>Retour</a></body></html>");
}

// API JSON pour récupérer les données en temps réel
void handleData() {
  String json = "{";
  json += "\"temperature\":" + String(temperature, 1) + ",";
  json += "\"pression\":"    + String(pression, 1)    + ",";
  json += "\"consigne\":"    + String(consigne)        + ",";
  json += "\"chauffage\":"   + String(chauffageActif ? "true" : "false") + ",";
  json += "\"ventilateur\":" + String(ventilActif     ? "true" : "false");
  json += "}";
  server.send(200, "application/json", json);
}

// Page non trouvée
void handleNotFound() {
  server.send(404, "text/plain", "Page non trouvée");
}

// ============================================================
// SETUP
// ============================================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== DÉMARRAGE SERVEUR WEB NODEMCU ===");

  // Connexion WiFi
  Serial.print("Connexion au réseau WiFi : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ WiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  Serial.println("Ouvrez votre navigateur et accédez à l'adresse ci-dessus.");

  // Configuration des routes du serveur
  server.on("/",       handleRoot);
  server.on("/ledon",  handleLedOn);
  server.on("/ledoff", handleLedOff);
  server.on("/data",   handleData);
  server.onNotFound(handleNotFound);

  // Démarrage du serveur
  server.begin();
  Serial.println("Serveur web démarré sur le port 80.");
}

// ============================================================
// BOUCLE PRINCIPALE
// ============================================================
void loop() {
  // Traitement des requêtes web
  server.handleClient();

  // Simulation lecture capteurs (à remplacer par vraie lecture I2C/SPI)
  // temperature = lireTemperature();
  // pression    = lirePression();

  delay(10);
}
