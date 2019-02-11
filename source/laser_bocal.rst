Remise à neuf de la K40 à Chemillé
##################################

Depuis Juin 2018, une collaboration épisodique s'est mis en place entre l'Infini Fab et le Boc@l de Chemillé.
Ce dernier possède une découpeuse laser de type K40 non utilisée et avec des travaux d'améliorations non-terminés.

L'objectif de l'Infini Fab est de terminer ces travaux d'amméliorations, vous trouverez ci-dessous toutes les étapes effectuées jusqu'à présent ainsi que celles prévues.

La K40 est une découpeuse laser populaire chez les bidouilleurs car elle n'est pas chère et peut s’améliorer sans trop de frais.

La situation avant le début des travaux:
""""""""""""""""""""""""""""""""""""""""

Le boc@l a déjà changé la carte de commande de base de la K40 pour la remplacer par une smoothieboard
Au départ le laser de la K40 fonctionne mais la smoothieboard ne semble pas commander les moteurs
Le boc@l a entrepris ses travaux d'amélioration en même temps qu'un autre fablabs
Pronterface est le logiciel utilisé pour communiquer avec la smoothieboard.

Reste à faire
"""""""""""""

* Installer un nouveau lit pour exploiter toute la surface de la laser `Le nouveau lit`_
* Changer la tête du laser pour ajuster le focus du laser `La tête du laser`_
* Callibrer les pas des moteurs pour avoir un déplacement précis
* Ajuster les mirroirs
* Usiner le capot de la partie commande de la laser pour y integrer le thermometre
* Fixer la smoothieboard, le level shifter, l'arduino et le raspberry pi dans la partie commande de la k40
* mettre un guide pour l'arrivée d'air
* installer laser visible
* reparer la vitre du capot
* Ajouter un PiTFT au raspberry Pi pour demarer un job en étant devant la Laser
* Ecrire un manuel d'utilisation pour la Laser



Les réparations
"""""""""""""""

+--------------------------------------+------------------------------------------------+-------------------------------+
|Les problemes                         | Cause possible                                 |  Action Corrective            |
+======================================+================================================+===============================+
|Moteurs OFF                           |branchement de l'alimentation                   | mise au bon endroit du + et - |
|                                      |moteur inversée sur la smoothieboard            |                               |
+--------------------------------------+------------------------------------------------+-------------------------------+
|Commandement des moteurs X&Y ne       |mauvais branchement de la commande de moteurs   |Moteur X branché sur M1        |
|correspond pas sur Pronterface        |sur la smoothieboard                            |Moteur Y branché sur M2        |
+--------------------------------------+------------------------------------------------+-------------------------------+
|Direction du moteur Y inversée        |moteur inversée sur la smoothieboard            |Changement du parametre de     |
|                                      |                                                |configuration de la smoothie   |
|                                      |                                                |board                          |
+--------------------------------------+------------------------------------------------+-------------------------------+
|Mauvais "homing" sur l'axe X          |l'interupteur fonctionne (testé avec la commande|changement des parametres      |
|                                      |M119 sur Pronterface)                           |moteurs :                      |
|                                      |message d'erreur lors du homing en "X" :        |alpha_steps_per_mm        80   |
|                                      |ERROR: Homing cycle failed - check the          |beta_steps_per_mm         80   |
|                                      |max_travel settings                             |par                            |
|                                      |                                                |200 pour les 2 valeurs         |
+--------------------------------------+------------------------------------------------+-------------------------------+
|Masse du boitier non connecté à la    |Machine suivant les normes chinoises            |raccord de la masse au boitier |
|masse du réseau electrique            |                                                |de la K40                      |
+--------------------------------------+------------------------------------------------+-------------------------------+
|Non commande du laser par la          |La smoothie envoi un signal logique entre 0 et  |production & intégration d'un  |
|smoothieboard                         |3.3V pour commander le laser, l'alimentation de |level-shifter permettant de    |
|                                      |la laser attends des ordres entre 0 et 5V.      |commander l'entrée en 0-5V avec|
|                                      |                                                |une sortie 0-3.3V              |
+--------------------------------------+------------------------------------------------+-------------------------------+


Les ameliorations
"""""""""""""""""

Le capteur de temperature
-------------------------

Le tube laser est sensible au hausse de température, pour éviter une surchauffe nous allons intaller une capteur de température au niveau du lquide de refroidissement.

**Materiel**:
* Sonde temperature DS18B20

.. image :: img/ds18b20.jpg
  :height: 100px

* Arduino nano
* Afficheur 7 segments
* 7 Resistance 220 Ohm
* 1 Resistance 10 kOhm

:download:`Mesurer la température <https://www.carnetdumaker.net/articles/mesurer-une-temperature-avec-un-capteur-1-wire-ds18b20-et-une-carte-arduino-genuino/>`

:download:`page où le code pour afficher la température à été récuperer <https://randomnerdtutorials.com/arduino-temperature-displayed-on-4-digit-7-segment/>`

:download:`le code final <code/k40_temp_measurement.ino>`

.. Note::
  Dans le code, on appelle la fonction température 1 fois toute les 1000 itérations de la loop pour avoir un affichage qui ne clignote pas

Le nouveau lit
""""""""""""""

La surface de travail de base de la K40 est assez réduite, alors qu'elle peut etre facilement doublée.
Il faudrait changer le type de lit, une surface métalique pleine telle que celle de base induit une réflexion du laser et donc sur le dessous du materiel à couper.
L'idéal serait un lit en nid d'abeille

.. image :: img/honeycombe.jpg


Mais un grillage, une plaque trouée devrait suffire

La tête du laser
""""""""""""""""

Le focus du laser est très important pour exploiter au maximum la puissance du laser. Ce focus se reggle en modifiant la hauteur entre le materiaux et la tête du laser.
Plutôt que d'opter pour une solution où c'est le lit du laser qui doit changer de hauteur nous allons chosir de changer la tête de base de la k40 pour une tête qui peut bouger en hauteur.

Comme celle-ci :

.. image :: https://ae01.alicdn.com/kf/HTB1FNLDc3fH8KJjy1zcq6ATzpXal/Cloudray-CO2-Laser-Head-Set-pour-2030-4060-K40-De-Gravure-Laser-Machine-De-D-coupe.jpg_640x640.jpg
  :target: https://fr.aliexpress.com/item/Cloudray-CO2-Laser-Head-Set-pour-2030-4060-K40-De-Gravure-Laser-Machine-De-D-coupe/32836741034.html
