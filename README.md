# Arduino : 
# porte de poulailler d'après le lever du soleil
****
## Présentation

_Bonjour_  

_Jeantu1 (quand je m'énerve lol)_  

_Je suis en retraite (j'ai donc du temps de libre lol) J'ai toujours aimé bricoler et... j'ai un poulailler avec deux poules !! (le coq s'est fait mangé par un renard… snif)_

<p align="center">
 <strong> Le poulailler</strong> <br>
<img src="https://github.com/jeantu1/porte-des-poules/blob/main/photos/IMGP1252reduit.jpg" width="480">
 </p>
 
_Cela n'a pas duré longtemps de devoir me lever le matin de bonne heure pour ouvrir la porte aux poules. J'ai donc équipé ce poulailler d'une porte commandée électriquement et automatiquement en fonction d'une heure bien définie._

## Principe de fonctionnement
Quand l'__horloge A__  (une simple prise 220v programmée, située dans la maison) s'enclenche à l'heure dite (_à 5 h du matin je dors encore lol_), elle alimente un __transformateur B__ (_12volts_) qui à son tour alimente (sur le poulailler situé dans le jardin) une __électro-vanne C__ (_12volts Mini Électrovanne Micro Normalement Fermé. 4€ environ_).

Cette électro-vanne, est alimentée pendant environ 5mn, temps pendant lequel s'écoule au travers d'un tuyau souple et par gravité, le litre et demi d'eau (qui est un peu supérieur au poids de ma porte) du __réservoir D__ (_qui est situé en hauteur_) vers le __bidon E__. En augmentant de poids, le __bidon E__, descend et entraîne, par un système de __poulie F__ et __câble G__, l’ouverture de la __porte H__.

Ce montage n'a besoin ni de __fin de course__ (_le réglage de la longueur du câble est tel que le bidon touche le sol au moment où la porte est ouverte_) ni __de moteur__ (_c'est le poids de l'eau qui ouvre la porte, donc la force est réglable_)

<p align="center">
  <strong> Schéma de principe </strong> <br>
<img src="https://github.com/jeantu1/porte-des-poules/blob/main/photos/schema_porte.jpg" width="480">
 </p>

Le soir, quand je vais voir si nos poules vont bien, je relève les œufs du jour, je retire les graines du poulailler (que les souris ne viennent pas se servir) et je re-transvase l'eau du __bidon E__ dans le __réservoir D__ et le cycle est prêt pour le lendemain matin.

Il faut par contre un câble électrique (_2x12volts_) qui part de la maison jusqu'au poulailler.

<p align="center">
 <strong> Détail du systeme </strong> <br>
<img src="https://github.com/jeantu1/porte-des-poules/blob/main/photos/mecanisme_ensemble.jpg" width="480">
 </p>
 
 <p align="center">
<strong> Ouverture porte </strong><br>
<img src="https://github.com/jeantu1/porte-des-poules/blob/main/photos/porte.jpg" width="480">
 </p>
 
 <p align="center">
 <strong> horloge de commande et transformateur 12v </strong><br>
<img src="https://github.com/jeantu1/porte-des-poules/blob/main/photos/commande1.jpg" width="480">
 </p>

## La mise en place d'un arduino
Tout marchait bien, mais j'ai découvert il y a peu de temps l'univers de l'__Arduino !__

Étant obligé de changer régulièrement l'heure d'ouverture de la porte en fonction de l'heure de lever du jour et ne voulant pas que les poules fassent le bazar en été quand la porte est encore fermée ou l'hiver quand la porte s'ouvre trop tôt et que maître Renard ne vienne pas faire son marché, j'en ai eu envie de mettre un _arduino_ à la place de l'horloge sur la prise 220v. 

J'aurais pu mettre une cellule pour capter la lumière du jour, mais comme ma commande est à la cave… pas de lumière du jour (_j'ai les endives qui y poussent_ !!)

Donc, le but du programme est qu'à partir d'un _Arduino wemos D1 mini_ :

1. je récupère une fois par jour l'heure internet (_à partir d'un site NTP_) pour avoir une horloge interne

2. je récupère aussi une fois par jour, l'heure de lever du soleil du lendemain

3. je compare l'heure de l'horloge et celle de lever du soleil ; si elle est égale, j'ouvre la porte du poulailler; sinon j'attends l'heure _lol_.


 <p align="center">
 <strong> L'arduino et son relais qui pilote le transfo </strong><br>
<img src="https://github.com/jeantu1/porte-des-poules/blob/main/photos/arduino_porte.jpg" width="480">
 </p>

## Développement
* 8 novembre 2020, confinement oblige, je suis en train d'écrire le programme.
* 06 janvier 2021, les collègues du club informatique linux m'ont dit que ce serait bien de développer le sujet (ce que je viens de faire)

## Conclusion (provisoire)
Je vais dans les prochains jours, améliorer le programme __"porte_poules.ino"__

Je développe sous l'_IDE Arduino_ et je découvre en même temps _Git_ (trop bien ce système et ce principe merci __Linus Torvalds__)

N'hésitez pas à me faire des remarques si vous en avez, sachant que je ne suis pas programmeur, mais mécanicien tourneur de formation (_soyez indulgent! lol_)

 L’installation se fait au travers de l’ __IDE d'Arduino__ et depuis peu a partir d'__Atom__.
 
 Mise à jour le 7/1/2021
