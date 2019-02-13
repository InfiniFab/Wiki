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

* Installer Visicut
* Importer les parametres de la K40
* Importer le fichier à usiner
* Selectionner les parametres de coupe/gravure
* Ajuster si besoin
* Exporter le job en G-code
* Se connecter à Octoprint


Lancement du job
""""""""""""""""""

* Sur Octoprint, importer le G-code générer sur Visicut
* Verifier les limites du job
* Lancer le job

Après le job
""""""""""""

* Eteindre le laser
* Eteindre la K40
* Eteindre la pompe à eau
* Vider l'eau du tube
* Eteindre l'evacuation d'air
* eteindre le compresseur
* eteindre la multiprise
