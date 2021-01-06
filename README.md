Bonjour

Jeantu1 (quand je m'énerve lol)

Je suis en retraite (_donc du temps de libre_ lol) J'ai toujours aimé bricoler et... j'ai un poulailler

![alt text](https://github.com/jeantu1/porte-des-poules/blob/main/IMGP1252original.JPG.jpg)

Cela n'a pas duré longtemps de devoir me levé le matin de bonne heure pour ouvrir la porte aux poules. J'ai donc équipé ce poulailler d'une porte commandée électriquement et automatiquement en fonction d'une heure bien définie.

Le principe est simple, quand une horloge "A" ![alt text](/home/jmt/Telechargements/horloge.jpg) (située dans la maison) s'enclenche, elle alimente un transformateur "B" 12v qui à son tour alimente (sur le poulailler) une électro-vanne"C". Cette électro-vanne alimentée environ 5mn le temps que le 1/2 d'eau égale au poids de la porte s'écoule  du réservoir "D" situé en hauteur dans un bidon "E" qui en augmentant de poids descendant et entraîne par un système de poulie "F" et câble "G" l’ouverture de la porte "H". Il n'y à __pas de fin de course__, la bonne longueur de câble fait que le bidon touche le sol en même temps que la porte est ouverte et __pas de moteur__.

Il faut par contre un câble électrique (2x12v) qui part de la maison jusqu'au poulailler._




J'ai découverts l'univers Arduino il y a peu de temps

Mais je suis obligé de changer régulièrement l'heure, si je ne veux par que les poules fassent le bazar en été quand la porte est encore fermée ou l'hiver quand la porte s'ouvre trop tôt et que maître renard ne vienne pas faire son marché !

Donc, j'ai mis un arduino à la place de l'horloge sur la prise 220v.

Le but du programme est qu'à partir d'un « Arduino wemos D1 mini » :

• 1) je récupère une fois par jour l'heure internet ( à partir d'un site NTP) pour avoir une horloge interne

• 2) je récupère une fois par jour l'heure de lever du soleil du lendemain

• 3) je compare l'heure de l'horloge et celle de lever du soleil, si elle est égale, j'ouvre la porte du poulailler, sinon j'attends l'heure lol.

• 18 novembre 2020, confinement oblige, je suis en train d'écrire le programme

Je développe sous l'IDE Arduino et je découvre Git (trop bien ce système)

• N'hésitez pas à me faire des remarques si vous en avez, sachant que je ne suis pas programmeur, mais mécanicien tourneur de formation (soyer indulgent!)






Je développe sous l’IDE Arduino et je découvre Git ( trop bien ce système)
    • N’hésitez pas à me faire des remarques si vous en avez, sachant que je ne suis pas programmeur, mais mécanicien tourneur de formation (soyer indulgent!)

    • L’installation se fait au travers de l’IDE Arduino. (comme ~~this~~)
