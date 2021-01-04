Bonjour
Jeantu1 (quand je m’énerve lol)
Je suis en retraite (donc du temps de libre lol), j’ai toujours aimé bricoler.
J’ai découverts l’univers Arduino il y a peut de temps.

J’ai un poulailler qui est munis d’une porte automatique commandé par une simple prise 220v, type horloge programmé sur la journée j’alimente un transfo 12v qui ouvre la porte. Mais je suis obligé de changer régulièrement l’heure, si je ne veux par que les poules fassent le bazar en été quand la porte est encore fermée ou l’hiver que la porte s’ouvre trop top et que maître renard ne vienne faire son marché !
Donc j'ai mis un arduino à la place de l'horloge sur la prise 220v.
      Le but du programme est qu’à partir d’un « Arduino wemos D1 mini » :
    • 1) je récupère une fois par jour l’heure internet ( à partir d’un site NTP) pour avoir une horloge interne
    • 2) je récupère une fois par jour l’heure de levé du soleil du lendemain
    • 3) je compare l’heure de l’horloge et celle du levé soleil, si elle est égale, j’ouvre la porte du poulailler, sinon j’attends l’heure lol.

    • 18 Novembre 2020, confinement oblige, je suis en train d’écrire le programme
Je développe sous l’IDE Arduino et je découvre Git ( trop bien ce système)
    • N’hésitez pas à me faire des remarques si vous en avez, sachant que je ne suis pas programmeur, mais mécanicien tourneur de formation (soyer indulgent!)

    • L’installation se fait au travers de l’IDE Arduino.
