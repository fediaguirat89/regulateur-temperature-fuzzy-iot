/*
 * ============================================================
 * RÉGULATEUR DE TEMPÉRATURE PAR LOGIQUE FLOUE
 * ============================================================
 * Auteure  : Fedia GUIRAT
 * Projet   : PFE Ingénieure en Informatique Embarquée
 * École    : ITBS Nabeul, Tunisie
 * Année    : 2017-2018
 * 
 * Description :
 * Ce programme implémente un régulateur de température industriel
 * utilisant la logique floue (Fuzzy Logic) via la bibliothèque eFLL.
 * Le système mesure la température via un thermocouple (MAX6675)
 * et commande des actionneurs (chauffage / ventilation) via des relais.
 * ============================================================
 */

#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <SPI.h>
#include <max6675.h>

// ============================================================
// DÉCLARATION DES BROCHES
// ============================================================
const int relayHeaterPin = 7;   // Relais chauffage
const int relayFanPin    = 8;   // Relais ventilateur
const int thermoCLK      = 6;   // MAX6675 CLK
const int thermoCS       = 5;   // MAX6675 CS
const int thermoDO       = 4;   // MAX6675 DO

// ============================================================
// VARIABLES GLOBALES
// ============================================================
float temperature = 0.0;
float pression    = 0.0;
int   consigne    = 25;         // Consigne de température (°C)

// Sorties du régulateur flou
float chaudier    = 0.0;
float ventilateur = 0.0;

// Capteur thermocouple
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Objet régulateur flou
Fuzzy* fuzzy = new Fuzzy();

// ============================================================
// FONCTION : INITIALISATION DU RÉGULATEUR FLOU
// ============================================================
void setup_fuzzy() {

  // --- Univers de discours : Température externe ---
  FuzzyInput* temperature_extern = new FuzzyInput(1);

  FuzzySet* moin   = new FuzzySet(0,  0,  0,  25);   // Froid
  FuzzySet* meme   = new FuzzySet(20, 25, 25, 30);   // Tiède
  FuzzySet* supieur = new FuzzySet(25, 50, 50, 50);  // Chaud

  temperature_extern->addFuzzySet(moin);
  temperature_extern->addFuzzySet(meme);
  temperature_extern->addFuzzySet(supieur);
  fuzzy->addFuzzyInput(temperature_extern);

  // --- Univers de discours : Pression ---
  FuzzyInput* pression_input = new FuzzyInput(3);

  FuzzySet* faible3  = new FuzzySet(0,   0,   0,   700);
  FuzzySet* normal3  = new FuzzySet(350, 700, 700, 1023);
  FuzzySet* eleve3   = new FuzzySet(700, 1023, 1500, 1500);

  pression_input->addFuzzySet(faible3);
  pression_input->addFuzzySet(normal3);
  pression_input->addFuzzySet(eleve3);
  fuzzy->addFuzzyInput(pression_input);

  // --- Sortie : Chaudière (chauffage) ---
  FuzzyOutput* chaudier_out = new FuzzyOutput(1);

  FuzzySet* Stre_faible1 = new FuzzySet(0,  0,  0,  6);
  FuzzySet* S_faible1    = new FuzzySet(0,  23, 23, 46);
  FuzzySet* S_moyen1     = new FuzzySet(23, 46, 46, 69);
  FuzzySet* S_eleve1     = new FuzzySet(46, 77, 77, 115);
  FuzzySet* Stre_eleve1  = new FuzzySet(96, 115, 115, 115);

  chaudier_out->addFuzzySet(Stre_faible1);
  chaudier_out->addFuzzySet(S_faible1);
  chaudier_out->addFuzzySet(S_moyen1);
  chaudier_out->addFuzzySet(S_eleve1);
  chaudier_out->addFuzzySet(Stre_eleve1);
  fuzzy->addFuzzyOutput(chaudier_out);

  // --- Sortie : Ventilateur ---
  FuzzyOutput* ventilateur_out = new FuzzyOutput(2);

  FuzzySet* Stre_faible2 = new FuzzySet(0,  10, 10, 10);
  FuzzySet* S_faible2    = new FuzzySet(0,  25, 25, 50);
  FuzzySet* S_moyen2     = new FuzzySet(25, 50, 50, 75);
  FuzzySet* S_eleve2     = new FuzzySet(50, 75, 75, 100);

  ventilateur_out->addFuzzySet(Stre_faible2);
  ventilateur_out->addFuzzySet(S_faible2);
  ventilateur_out->addFuzzySet(S_moyen2);
  ventilateur_out->addFuzzySet(S_eleve2);
  fuzzy->addFuzzyOutput(ventilateur_out);

  // --- Règle 1 : Si température FROIDE → Chauffage ÉLEVÉ ---
  FuzzyRuleAntecedent* ifTempFroide = new FuzzyRuleAntecedent();
  ifTempFroide->joinSingle(moin);
  FuzzyRuleConsequent* thenChauffageEleve = new FuzzyRuleConsequent();
  thenChauffageEleve->addOutput(S_eleve1);
  thenChauffageEleve->addOutput(Stre_faible2);
  FuzzyRule* rule1 = new FuzzyRule(1, ifTempFroide, thenChauffageEleve);
  fuzzy->addFuzzyRule(rule1);

  // --- Règle 2 : Si température TIÈDE → Maintien ---
  FuzzyRuleAntecedent* ifTempTiede = new FuzzyRuleAntecedent();
  ifTempTiede->joinSingle(meme);
  FuzzyRuleConsequent* thenMaintien = new FuzzyRuleConsequent();
  thenMaintien->addOutput(S_moyen1);
  thenMaintien->addOutput(S_faible2);
  FuzzyRule* rule2 = new FuzzyRule(2, ifTempTiede, thenMaintien);
  fuzzy->addFuzzyRule(rule2);

  // --- Règle 3 : Si température CHAUDE → Ventilation ÉLEVÉE ---
  FuzzyRuleAntecedent* ifTempChaude = new FuzzyRuleAntecedent();
  ifTempChaude->joinSingle(supieur);
  FuzzyRuleConsequent* thenVentilationElevee = new FuzzyRuleConsequent();
  thenVentilationElevee->addOutput(Stre_faible1);
  thenVentilationElevee->addOutput(S_eleve2);
  FuzzyRule* rule3 = new FuzzyRule(3, ifTempChaude, thenVentilationElevee);
  fuzzy->addFuzzyRule(rule3);
}

// ============================================================
// FONCTION : FUZZIFICATION ET DÉFUZZIFICATION
// ============================================================
void fuzzifier() {
  fuzzy->setInput(1, temperature);
  fuzzy->setInput(3, pression);

  fuzzy->fuzzify();

  ventilateur = fuzzy->defuzzify(1);
  chaudier    = fuzzy->defuzzify(2);
}

// ============================================================
// FONCTION : COMMANDER LES ACTIONNEURS
// ============================================================
void commanderActionneurs() {
  // Chauffage
  if (chaudier > 50) {
    digitalWrite(relayHeaterPin, HIGH);
    Serial.println(">>> CHAUFFAGE ACTIVÉ");
  } else {
    digitalWrite(relayHeaterPin, LOW);
  }

  // Ventilateur
  if (ventilateur > 50) {
    digitalWrite(relayFanPin, HIGH);
    Serial.println(">>> VENTILATEUR ACTIVÉ");
  } else {
    digitalWrite(relayFanPin, LOW);
  }
}

// ============================================================
// SETUP ARDUINO
// ============================================================
void setup() {
  Serial.begin(9600);
  Serial.println("=== RÉGULATEUR DE TEMPÉRATURE - LOGIQUE FLOUE ===");

  // Configuration des broches relais
  pinMode(relayHeaterPin, OUTPUT);
  pinMode(relayFanPin,    OUTPUT);
  digitalWrite(relayHeaterPin, LOW);
  digitalWrite(relayFanPin,    LOW);

  // Initialisation du régulateur flou
  setup_fuzzy();

  Serial.println("Système initialisé. Démarrage de la régulation...");
  delay(500);
}

// ============================================================
// BOUCLE PRINCIPALE
// ============================================================
void loop() {
  // 1. Lecture de la température via thermocouple
  temperature = thermocouple.readCelsius();

  Serial.print("Température mesurée : ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Consigne           : ");
  Serial.print(consigne);
  Serial.println(" °C");

  // 2. Fuzzification et calcul de la commande
  fuzzifier();

  Serial.print("Commande chauffage    : ");
  Serial.println(chaudier);
  Serial.print("Commande ventilateur  : ");
  Serial.println(ventilateur);

  // 3. Commander les actionneurs
  commanderActionneurs();

  Serial.println("------------------------------------------");
  delay(2000); // Attente 2 secondes entre chaque mesure
}
