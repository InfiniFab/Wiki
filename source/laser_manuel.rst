.. _manuel-k40:

Manuel d'utilisation de la K40
##################################


Consignes de sécurité
""""""""""""""""""""""

Les choses à faire avant de démarrer le laser
---------------------------------------------

* brancher le circuit d'eau du laser sur la pompe à eau
* Allumer la pompe à eau (il suffit de la brancher)

.. important::

	Regarder au niveau du tube laser, toutes les bulles d'air doivent être évacuées. S'il le faut soulever un peut la k40 sur le côté pour évacuer les bulles qui restent.


* Installer l'évacuation d'air de la k40
* brancher l'evacuation d'air
* S'assurer que le bouton "laser" est relevé
* S'assurer que le Raspberry Pi est allumé

Demarage de la K40
------------------

* Appuyer sur le bouton power de la K40
* Allumer le compresseur
* Appuyer sur le bouton "laser"

.. important::

	Lorsque l'on appui sur le bouton laser, il peut arriver que la commande du laser dans la smoothie soit bloquée à "tirer", mettre un materiel de test sous la tête du laser lors de l'appui du bouton laser et s'assurer qu'il n'y a pas de lase de "tirer"

Préparation du fichier
""""""""""""""""""""""

Installation et configuration de Visicut
----------------------------------------

* Installer `Visicut <https://visicut.org/>`_
* Importer les parametres de la K40
	* Option > Importer la configuration

:download:`Parametre de la K40 <_static/k40_param.vcsettings>`

* Importer le fichier à usiner, doit etre un fichier image (Jpeg, Svg, Bmp, Png, ...) Fichier > Importer
* Selectionner les parametres de coupe/gravure
	* Pour graver une image
		* importer une image pixelisée (JPEG, BMP, PNG, ...)
		* Dans la partie de droite il y a 3 onglet "configuration", "position" et "configuration du laser"
		* Dans l'onglet "Configuration", vous pouvez choiser "Tout engrave 3d"
		* Dans l'onglet "Coonfiguration du laser", choisi les parametres qui conviennent le mieux, se referer au document ci dessous pour choisir les bons parametres
	* Pour découper selon une image
		* Importer une image de type vectoriel (SVG, IA, ...)
		* dans l'onglet "Configuration", vous avez plusieurs choix, s'il n'y a qu'une seule couleur, vous pouvez choisir "tout engrave", sinon choisissez "configuration par simple propriété"
		* Pour chaques couleurs du dessin vous pouvez choisir soit de couper ("cut"), ou de graver. Si la couleur est une forme pleine, et si vous choisissez de couper, visicut detectera automatiquement les contours de la couleur
* Ajuster si besoin, pour s'inspirer vous avez le docuent suivant

:download:`Idée de parametre pour la gravure et la découpe en fonction du materiau <_static/speedsfeeds_RL.pdf>`

.. attention::

	Les paramêtres donnés ici ne sont pas une réference absolus, ceux ci peuvent dépendre de l'état du tube laser , il est vivement conseiller de **tester les parametres choisis sur un échantillon** avant de lancer le job définitif.

* Exporter le job en G-code
	* Fichier > Exporter le G-Code
* Se connecter à Octoprint


Lancement du job
""""""""""""""""""

* Sur Octoprint, importer le G-code générer sur Visicut
* Verifier les limites du job
* Lancer le job via "print"

Après le job
""""""""""""

* Eteindre le laser
* Eteindre la K40
* Eteindre la pompe à eau
* Vider l'eau du tube
* Eteindre l'evacuation d'air
* eteindre le compresseur
* eteindre la multiprise
