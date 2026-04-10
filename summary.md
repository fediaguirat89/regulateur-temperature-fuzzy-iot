# RÉSUMÉ DU PROJET / PROJECT SUMMARY

---

## 🇫🇷 RÉSUMÉ (Français)

### Titre
**Implémentation de logique floue au régulateur de température industriel**

### Contexte
Ce travail de fin d'études a été réalisé au sein de la société **RL Technologie** (Tunisie),
spécialisée dans la distribution de matériel de réfrigération et de chauffage industriel.
Il s'inscrit dans le cadre du diplôme d'**Ingénieure en Informatique Embarquée et Mobile**
délivré par l'ITBS de Nabeul (2017–2018).

### Objectif
Concevoir et réaliser une plateforme intelligente pour l'automatisation et le contrôle à
distance de la température industrielle, en intégrant la **logique floue** comme algorithme
de régulation avancée.

### Ce que fait le système
- Mesure la température et la pression en temps réel via des capteurs
- Applique un algorithme de régulation par **logique floue** (Fuzzy Logic)
- Permet de choisir entre régulation automatique (floue ou TOR) et manuelle
- Supervise les paramètres à distance via une **interface web** (WiFi / IoT)
- Enregistre les données dans une base de données

### Technologies utilisées
- **Microcontrôleurs :** Arduino UNO (ATmega328), NodeMCU ESP8266
- **Capteurs :** LM35, BMP180, Thermocouple + MAX6675
- **Logique floue :** Bibliothèque eFLL (Embedded Fuzzy Logic Library) en C
- **Communication :** WiFi, Bus I2C, SPI
- **Outils :** Proteus ISIS/ARES, IDE Arduino, UML
- **Méthode :** SCRUM / Agile

### Résultats
Le système a été développé en 3 sprints sur 4 mois (juin–août 2018) :
- **Sprint 1 :** Régulateur TOR fonctionnel
- **Sprint 2 :** Logique floue implémentée + carte de commande réalisée
- **Sprint 3 :** Connectivité IoT via NodeMCU + supervision à distance

**Coût de réalisation :** 10 225 DA (moins cher que le produit commercial existant à 15 DA)

### Auteure
**Fedia GUIRAT** — Ingénieure en Informatique Embarquée  
ITBS Nabeul, Tunisie | Promotion 2018  
📧 fediaguirat89@gmail.com | 📍 Liège, Belgique

**Mots-clés :** IoT · Logique floue · Systèmes embarqués · Arduino · NodeMCU ESP8266 · Régulation de température · SCRUM · Proteus

---

## 🇬🇧 ABSTRACT (English)

### Title
**Implementation of Fuzzy Logic in an Industrial Temperature Controller**

### Context
This final-year engineering project was carried out at **RL Technologie** (Tunisia),
a company specialized in the distribution of industrial refrigeration and heating equipment.
It was completed as part of the **Embedded and Mobile Computing Engineering** degree
at ITBS Nabeul (2017–2018).

### Objective
To design and implement an intelligent platform for the automation and remote control of
industrial temperature, integrating **fuzzy logic** as an advanced control algorithm.

### What the system does
- Measures temperature and pressure in real time using sensors
- Applies a **fuzzy logic** control algorithm for intelligent regulation
- Allows switching between automatic (fuzzy or on/off) and manual control modes
- Remotely monitors parameters through a **web interface** (WiFi / IoT)
- Stores sensor data in a database

### Technologies used
- **Microcontrollers:** Arduino UNO (ATmega328), NodeMCU ESP8266
- **Sensors:** LM35, BMP180, Thermocouple + MAX6675
- **Fuzzy Logic:** eFLL (Embedded Fuzzy Logic Library) in C
- **Communication:** WiFi, I2C Bus, SPI
- **Tools:** Proteus ISIS/ARES, Arduino IDE, UML modeling
- **Methodology:** SCRUM / Agile

### Results
The system was developed in 3 sprints over 4 months (June–August 2018):
- **Sprint 1:** Functional on/off (TOR) temperature controller
- **Sprint 2:** Fuzzy logic implemented + custom ATmega328 control board
- **Sprint 3:** IoT connectivity via NodeMCU + remote web supervision

**Total cost:** 10,225 DA (lower than the existing commercial product at 15 DA)

### Author
**Fedia GUIRAT** — Embedded Systems Engineer  
ITBS Nabeul, Tunisia | Class of 2018  
📧 fediaguirat89@gmail.com | 📍 Liège, Belgium

**Keywords:** IoT · Fuzzy Logic · Embedded Systems · Arduino · NodeMCU ESP8266 · Temperature Control · SCRUM · Proteus
