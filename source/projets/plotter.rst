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

 ::download:`les parametres <../_static/anet a6 plot.curaprofile>`

 - exporter le g-Code
 - faire un homing sur l'imprimante
 - regler la position du crayon quand le Z=0
 - lancer l'impression via l'imprimante
