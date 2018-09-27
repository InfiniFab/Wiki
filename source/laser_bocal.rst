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
