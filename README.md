# Bonjour

## Jeantu1 (quand je m'énerve lol)

Je suis en retraite (_j'ai donc du temps de libre_ lol) J'ai toujours aimé bricoler et... j'ai un poulailler avec deux poules !! (le coq s'est fait manger par un renard… snif)

!["alt text"](https://github.com/jeantu1/porte-des-poules/blob/main/photos/IMGP1252reduit.jpg)

### Cela n'a pas duré longtemps de devoir me lever le matin de bonne heure pour ouvrir la porte aux poules. J'ai donc équipé ce poulailler d'une porte commandée électriquement et automatiquement en fonction d'une heure bien définie.

##### Le principe est simple:
Quand l'__horloge "A__"  (une simple prise 220v programmée, située dans la maison) s'enclenche à l'heure dite (à 5 h du matin je dors encore lol), elle alimente un __transformateur "B"__ 12v qui à son tour alimente (sur le poulailler situé dans le jardin) une __électro-vanne "C"__ *(12v Mini Électrovanne Micro Normalement Fermé. 5€ environ)*
Cette électro-vanne, est alimentée pendant environ 5mn, pendant lequel s'écoule le 1/2 litre d'eau (qui est un peu supérieur au poids de ma porte) du __réservoir "D"__ (qui est situé en hauteur) vers le __bidon "E"__. En augmentant de poids, le __bidon "E"__, descend et entraîne, par un système de __poulie "F"__ et __câble "G"__, l’ouverture de la __porte "H"__.
Ce montage n'a pas besoin de ni: __fin de course__ (le réglage de la longueur du câble fait que le bidon touche le sol au moment où la porte est ouverte) ni __de moteur__ (c'est le poids de l'eau qui ouvre la porte)

!["alt text"](https://github.com/jeantu1/porte-des-poules/blob/main/photos/schema_porte.jpg)

Et le soir, quand je vais voir si nos poules vont bien, je relève les œufs du jour, je retire les graines du poulailler (que les souris ne viennent pas se servir) et je re-transvase l'eau du __bidon "E"__ dans le __réservoir "D"__ et le cycle est prêt pour le lendemain matin.

Il faut par contre un câble électrique (2x12v) qui part de la maison jusqu'au poulailler.

Detail du systeme

!["alt text"](https://github.com/jeantu1/porte-des-poules/blob/main/photos/mecanisme_ensemble.jpg)


Ouverture porte !["alt text"](https://github.com/jeantu1/porte-des-poules/photos/blob/main/photos/porte.jpg)

horloge de commande et transformateur 12v !["alt text"](https://github.com/jeantu1/porte-des-poules/blob/main/photos/commande1.jpg)


__Tout marchait bien__, mais j'ai découverts il y a peu de temps l'__univers Arduino…__

Étant obligé de changer régulièrement l'heure d'ouverture de la porte en fonction de la lever du jour et ne voulant pas que les poules fassent le bazar en été quand la porte est encore fermée ou l'hiver quand la porte s'ouvre trop tôt et que maître renard ne vienne pas faire son marché, j'en ai profité pour y mettre un arduino à la place de l'horloge sur la prise 220v (j'aurais pu mettre une cellule pour capter la lumière du jour, mais comme ma commande est à la cave… pas de lumière du jour (j'ai les endives qui y poussent !!)

Donc, le but du programme est qu'à partir d'un « Arduino wemos D1 mini » :

* 1) je récupère une fois par jour l'heure internet (à partir d'un site NTP) pour avoir une horloge interne

* 2) je récupère aussi une fois par jour, l'heure de lever du soleil du lendemain

* 3) je compare l'heure de l'horloge et celle de lever du soleil; si elle est égale, j'ouvre la porte du poulailler; sinon j'attends l'heure lol.

Développement
: 8 novembre 2020, confinement oblige, je suis en train d'écrire le programme.
: 06 janvier 2021, les collègues du club informatique linux m'ont dit que ce serait bien de développer le sujet (ce que je viens de faire)
: je vais dans les prochains jours, améliorer le programme __"porte_poules.ino"__

Je développe sous l'IDE Arduino et je découvre en même temps Git (trop bien ce système et ce principe merci __Linus Torvalds__)

• N'hésitez pas à me faire des remarques si vous en avez, sachant que je ne suis pas programmeur, mais mécanicien tourneur de formation (soyer indulgent! lol)

 L’installation se fait au travers de l’ __IDE d'Arduino__ et depuis peu d'__Atom__.
 
 Mise à jour le 7/1/2021
