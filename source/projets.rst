Les projets de l'Infini fab
###########################

Sur cette page vous trouverez l'ensemble des projets en cours de l'Infini Fab, que ce soit des produits destinés à être présent dans le catalogue de l'Infini Fab mais aussi les collaborations en cours entre l'Infini Fab et d'autre fablab/makers.



.. toctree::
   :maxdepth: 1
   :caption: Les Projets:

   laser_bocal
   laser_manuel
   Ateliers_template


Sur cette page vous trouverez une présentation des divers projets, plus ou moins important qui n'ont pas nécéssiter une page particulière


Dessiner sur un imprimante 3D
-----------------------------

Transformer une Anet A6 pour lui permettre de déssiner sur une feuille

 - Imprimer la _'pièce qui tiendra le crayon <https://www.thingiverse.com/thing:2847766>'
 - Monter la pièce
 - Choisir une image voulu
 - La convertire en dessin vectoriel, ne mettre que 2 niveau de gris
 - Ouvrir Cura
 - ouvrir l'image voulu
 - Cura demande alors l'épaisseur de l'image et le niveau de la base, pour l'épaisseur mettre 0.2 mm, pour le niveau de la base mettre 0
 - Au niveau des paramettres de l'imprimantes importer les suivant (globalement on a mis à 0.1mm la couche d'impression, imposer un 'hop' du z entre les traits, et mis à 0 les températures de la tête, et du plateau)

 ::download:`les parametre <_static/anet a6 plot.curaprofile>`

 - exporter le g-Code
 - faire un homing sur l'imprimante
 - regler la position du crayon quand le Z=0
 - lancer l'impression via l'imprimante

Faire un compteur de temps
--------------------------

L'idée de ce projet est de concevoir un petit compteur qui permettrait de mesurer le temps passé à chaques tâches.
Ce compteur pourrait avoir plusieurs version

 - La V1 : La base

Cette version devrait avoir un afficheur 7 segment et 4 chiffres, un potentiometre pour selectionner la tâche en cour, un bouton pour demarer/arreter le compteur, un bouton pour le remettre à 0
Sur les 4 chiffres, le 1er represente la tâche selectionnée, les 3 suivant le temps en minute accumulé sur la tâche.
Le tout est controllé par un arduino, et le temps cumulé pour chaques tâche est stocké sur l'EEprom de l'arduino

 - La V2

Integrer un adaptateur SD, pour stocker le temps mesurer sur un fichier (XCL) sur la carte SD
Possibilité de formater ce fichier pour exporter et importer facilement le temps passé dans un logiciel tier de suivit du temps

`Un exemple de fichier csv pour Toggl <https://support.toggl.com/import-and-export/csv-import/editing-and-uploading-a-csv-file>`_

 - La V3

Integrer un module wifi ou bluetooth pour permettre au compteur de communiquer avec un logiciel tiers et mettre à jour automatique le suivi de temps

 - La V4

Integrer un module RFID, pour définir
