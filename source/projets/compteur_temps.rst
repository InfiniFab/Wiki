
Faire un compteur de temps
--------------------------

L'idée de ce projet est de concevoir un petit compteur qui permettrait de mesurer le temps passé à chaques tâches.
Ce compteur pourrait avoir plusieurs version

La V1 : La base
===============

**status** : Code et schéma électrique validé, besoin de construire le 1er prototype

Spécifications
**************

* Capable de compter les minutes sur 9 tâches simultanées
* Le compteur affiche 4 chiffres, le 1er pour la tâche sélectionnée, les 3 suivant pour le temps compté en minute
* L'utilisateur dit être capable de choisir la tâche en cour, de lancer/arrêter le compteur sur plusieurs tâches et de remettre le compteur à zéro
* Le compteur doit permettre de conserver les info du timer sans alimentation

Materiel Choisi
***************

* Un afficheur 7 Segment , 4 Chiffres
* 2 boutons poussoir, 1 pour démarer/arreter le compteur, 1 pour remettre le compteur à 0
* 1 arduino nano pour controller l'ensemble
* 8 resistances 220 Ohm pour l'afficheur 7 segments (8 en réalité avec le point)
* 1 résistance de 10kOhm pour l'interupteur qui arrete/démare le compteur
* 1 condensateur de 100nFarad (pour éviter les rebonds de l'interupteur Arret/demare compteur)
* 2 Shift register (74hc595) pour permettre d'économiser les pins de sorties de l'arduino
* Un potentiometre pour choisir la tache

Le schéma electrique
********************

.. figure :: ../_static/Schematic_timer3_Sheet-2_20190606195032.png

  le schema de principe du 1er prototype

Sur le schema on rajoute un connecteur, si l'on monte ce compteur sur un circuit imprimé on gardera la possibilité d'évolution en laissant accessible d'autre pin inutilisées de l'arduino.

:download:`En suivant ce schéma, on a le code arduino correspondant <../_static/compteur_perso.ino>`

Ce schema et le code on été validé sur une plaque de prototypage, il s'agit de souder l'ensemble

La V2
=====

Integrer un adaptateur SD, pour stocker le temps mesurer sur un fichier (XCL) sur la carte SD
Possibilité de formater ce fichier pour exporter et importer facilement le temps passé dans un logiciel tier de suivit du temps

`Un exemple de fichier csv pour Toggl <https://support.toggl.com/import-and-export/csv-import/editing-and-uploading-a-csv-file>`_

La V3
=====

Integrer un module wifi ou bluetooth pour permettre au compteur de communiquer avec un logiciel tiers et mettre à jour automatique le suivi de temps

La V4
=====

Integrer un module RFID, pour définir
