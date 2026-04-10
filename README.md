# regulateur-temperature-fuzzy-iot
Implémentation de logique floue au régulateur de température industriel - Arduino, NodeMCU, IoT
#  Régulateur de Température Intelligent — Logique Floue & IoT

> Projet de Fin d'Études — Ingénieure en Informatique Embarquée  
> ITBS Nabeul, Tunisie | 2017–2018  
> **Auteure : Fedia GUIRAT**

---

##  Description du projet

Ce projet consiste en la conception et la réalisation d'une **plateforme intelligente de contrôle et d'automatisation de la température** à distance, intégrant la **logique floue** (Fuzzy Logic) pour une régulation avancée.

Le système permet de :
- Contrôler la température en temps réel via des capteurs
- Choisir entre régulation automatique (logique floue ou TOR) et manuelle
- Superviser et configurer les paramètres à distance via une **interface web**
- Connecter le régulateur à Internet grâce à la technologie **IoT (NodeMCU ESP8266)**

---

## 🛠️ Technologies utilisées

| Domaine | Technologies |
|--------|-------------|
| Microcontrôleur | Arduino UNO (ATmega328), NodeMCU ESP8266 |
| Capteurs | LM35 (température), BMP180 (pression), Thermocouple + MAX6675 |
| Logique floue | Bibliothèque eFLL (Embedded Fuzzy Logic Library) en C |
| Communication | WiFi (ESP8266), Bus I2C, SPI |
| Conception circuit | Proteus ISIS / ARES |
| Interface web | HTML, Application web embarquée |
| Méthodologie | SCRUM / Agile |
| Modélisation | UML |

---

##  Architecture du système

```
Capteurs (LM35 / BMP180)
        ↓
Microcontrôleur (ATmega328)
  ├── Régulation TOR
  └── Régulation par Logique Floue (eFLL)
        ↓
Actionneurs (Relais → Chauffage / Ventilation)
        ↕ WiFi
NodeMCU ESP8266
        ↕
Interface Web (PC / Smartphone)
```

---

## 🔬 Fonctionnement de la logique floue

Le régulateur flou utilise **3 niveaux de température** :
- ❄️ **Froid** (< 20°C) → Déclenchement du chauffage
- 🌤️ **Tiède** (20–32°C) → Maintien de l'état
- 🔥 **Chaud** (> 32°C) → Déclenchement de la ventilation

**Les 3 étapes du traitement flou :**
1. **Fuzzification** — conversion des valeurs réelles en degrés d'appartenance
2. **Inférence** — application des règles linguistiques
3. **Défuzzification** — conversion en signal de commande

---

##  Composants matériels

| Composant | Rôle |
|-----------|------|
| ATmega328 (Arduino UNO) | Contrôleur principal |
| NodeMCU ESP8266 | Connexion WiFi / IoT |
| Capteur LM35 | Mesure de température (-55°C à +150°C) |
| Capteur BMP180 | Mesure de pression atmosphérique |
| MAX6675 + Thermocouple | Mesure haute température |
| Relais 5V | Commande des actionneurs |
| Module I2C | Communication entre composants |

---

##  Structure du projet

```
regulateur-temperature-fuzzy-iot/
├── README.md
├── code/
│   ├── arduino_TOR/          # Code régulation TOR (Prototype 1)
│   ├── fuzzy_logic/          # Code logique floue en C (Prototype 2)
│   └── nodemcu_wifi/         # Code serveur web NodeMCU (Prototype 3)
├── schemas/
│   ├── circuit_proteus.png   # Schéma électronique Proteus ISIS
│   └── routage_ARES.png      # Routage carte de commande
├── rapport/
│   └── rapport_PFE.pdf       # Rapport complet du projet
└── docs/
    └── architecture.png      # Schéma d'architecture du système
```

---

##  Résultats obtenus

✅ **Sprint 1** — Régulateur TOR fonctionnel (lecture température + régulation)  
✅ **Sprint 2** — Logique floue implémentée + carte de commande ATmega328 réalisée  
✅ **Sprint 3** — Connectivité IoT via NodeMCU ESP8266 + supervision à distance  

**Coût de réalisation :** 10 225 DA (inférieur au régulateur commercial à 15 DA)

---

##  Perspectives d'amélioration

- Contrôle de plusieurs paramètres simultanés
- Simulation avec MATLAB/Simulink Fuzzy Logic Toolbox
- Développement d'une application mobile
- Intégration de capteurs industriels avancés

---

##  Auteure

**Fedia GUIRAT**  
Ingénieure en Informatique Embarquée & Data Analyst  
📍 Liège, Belgique  
📧 fediaguirat89@gmail.com  

*Encadrant académique : M. Rokbani Nizar*  
*Encadrant professionnel : M. Ridha Chikha (RL Technologie)*

---

## 📄 Licence

Ce projet est réalisé dans un cadre académique. Tous droits réservés © Fedia GUIRAT 2018.
