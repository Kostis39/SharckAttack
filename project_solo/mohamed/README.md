# Mon Projet Génial

Projet réalisé en langage C avec SDL2.

## Exercice 1 — X fenêtré
Objectif

Démontrer la capacité à ouvrir, fermer, déplacer et redimensionner des fenêtres SDL2.

Pour cet exercice, deux versions ont été réalisées afin de tester progressivement les fonctionnalités de SDL2.
Version 1 — X dessiné dans une fenêtre

Cette première version utilise une seule fenêtre SDL2.
Elle permet de tester la création d'une fenêtre, la création d'un renderer, le dessin dans la fenêtre et une animation simple.

Fonctionnalités réalisées :

    création d'une fenêtre SDL2 redimensionnable ;
    création d'un renderer ;
    dessin d'un X à l'intérieur de la fenêtre ;
    animation d'un carré rouge qui rebondit sur les bords ;
    déplacement de la fenêtre avec les flèches ;
    redimensionnement avec + et - ;
    affichage de la position et de la taille avec i ;
    recentrage avec c ;
    fermeture propre avec q, Échap ou la croix.

Version 2 — Fenêtres placées en forme de X

Cette deuxième version est la version finale de l'exercice.
Elle représente directement le X avec plusieurs fenêtres SDL2 placées sur l'écran.

Fonctionnalités réalisées :
    création de plusieurs fenêtres SDL2 redimensionnables ;
    placement des fenêtres sous forme de X ;
    création d'un renderer pour chaque fenêtre ;
    déplacement de tout le X avec les flèches ;
    redimensionnement de toutes les fenêtres avec + et - ;
    affichage de la position et de la taille de chaque fenêtre avec i ;
    recentrage du X avec c ;
    fermeture propre avec q, Échap ou la croix.

Fonctions SDL2 utilisées :

    SDL_Init pour initialiser SDL2 ;
    SDL_CreateWindow pour créer les fenêtres ;
    SDL_CreateRenderer pour créer les renderers ;
    SDL_SetWindowPosition pour positionner les fenêtres ;
    SDL_GetWindowPosition pour récupérer leur position ;
    SDL_GetWindowSize pour récupérer leur taille ;
    SDL_SetWindowSize pour les redimensionner ;
    SDL_GetCurrentDisplayMode pour récupérer la résolution de l'écran ;
    SDL_PollEvent pour gérer les événements clavier et la fermeture ;
    SDL_RenderClear, SDL_RenderFillRect et SDL_RenderPresent pour afficher un contenu simple ;
    SDL_DestroyRenderer, SDL_DestroyWindow et SDL_Quit pour fermer proprement.

Compilation et exécution

Depuis la racine du projet :

cd X-window
gcc -Wall -Wextra -g src/X_fenetre_main2.c -o x_fenetre2 $(sdl2-config --cflags --libs)
./x_fenetre2
cd ..

Remarque : la version 1 a servi de première étape pour tester le dessin et l'animation dans une fenêtre. La version 2 est la version principale à présenter, car elle correspond mieux à l'idée du X fenêtré avec plusieurs fenêtres.

## Exercice 2 — Pavé de serpents
Objectif

Démontrer la capacité à dessiner des formes géométriques simples et à les animer avec SDL2.
Fonctionnalités réalisées

    création d'une fenêtre SDL2 redimensionnable ;
    création d'un renderer ;
    dessin d'un fond sombre avec une grille ;
    affichage de plusieurs serpents animés ;
    chaque serpent est composé de petits rectangles ;
    les segments du corps suivent la tête du serpent ;
    déplacement automatique des serpents dans la fenêtre ;
    changement de direction automatique ;
    rebond / correction lorsque les serpents atteignent les bords ;
    pause et reprise avec Espace ou p ;
    accélération avec + ;
    ralentissement avec - ;
    réinitialisation des serpents avec r ;
    fermeture propre avec q, Échap ou la croix.

Fonctions SDL2 utilisées

    SDL_Init pour initialiser SDL2 ;
    SDL_CreateWindow pour créer la fenêtre ;
    SDL_CreateRenderer pour créer le renderer ;
    SDL_SetRenderDrawColor pour choisir les couleurs ;
    SDL_RenderClear pour effacer l'ancien affichage ;
    SDL_RenderFillRect pour dessiner les rectangles des serpents ;
    SDL_RenderDrawLine pour dessiner la grille ;
    SDL_RenderPresent pour afficher le rendu ;
    SDL_Delay pour contrôler la vitesse de l'animation ;
    SDL_PollEvent pour gérer les événements clavier et la fermeture ;
    SDL_GetWindowSize pour récupérer la taille de la fenêtre.

Rôle des fichiers :

    src/serpent_main.c : boucle principale SDL2 et gestion des événements ;
    src/snake.c : logique de déplacement des serpents ;
    src/draw.c : fonctions d'affichage ;
    src/window_tools.c : fonctions utiles liées à la fenêtre ;
    include/config.h : constantes du programme ;
    include/snake.h : structure des serpents ;
    include/draw.h : prototypes des fonctions d'affichage ;
    include/window_tools.h : prototypes des fonctions liées à la fenêtre.

Compilation et exécution

Depuis la racine du projet :

cd snake
gcc -Wall -Wextra -g src/serpent_main.c src/snake.c src/draw.c src/window_tools.c -Iinclude -o serpent $(sdl2-config --cflags --libs) -lm
./serpent
cd ..

## Exercice 3 — Animer des sprites
Objectif

Démontrer la capacité à utiliser des textures, une planche de sprites, un décor, des événements clavier/souris et une animation fluide.

Cet exercice a été réalisé progressivement :

    une première version simple avec des rectangles pour comprendre la logique du joueur, de l'ennemi, de l'attaque et de la collision ;

    une deuxième version avec le sprite proposé dans les fichiers du professeur ;

    une version plus complète avec fond, sol, parallaxe, saut, frappe, ennemi, barre de vie, texte et organisation en fichiers .c et .h.

Fonctionnalités réalisées

    chargement d'un fond avec SDL_image ;
    affichage d'un décor ;
    ajout d'un effet de parallaxe avec plusieurs couches ;
    affichage d'un sol ;
    chargement d'une planche de sprites PNG ;
    découpage de la planche avec un rectangle source ;
    affichage du sprite avec un rectangle destination ;
    animation du joueur selon son état : immobile, marche, saut, attaque ;
    déplacement avec les flèches gauche/droite ;
    saut avec Z ou flèche haut ;
    attaque avec F ou Espace ;
    téléportation avec le clic gauche ;
    affichage d'un ennemi ;
    affichage d'une barre de vie ;
    collision entre la zone d'attaque et l'ennemi ;
    affichage du texte d'aide avec SDL_ttf ;
    fermeture propre avec q, Échap ou la croix.

Principe technique

La planche de sprites contient plusieurs images du personnage dans un seul fichier.
Le programme utilise deux rectangles importants :
    source : zone de la planche de sprites à afficher ;
    destination : position et taille du sprite dans la fenêtre.

L'animation est obtenue en changeant régulièrement la frame affichée.
La vitesse de l'animation est contrôlée avec SDL_GetTicks.

Pour l'attaque, une hitbox rectangulaire est placée devant le joueur.
La collision avec l'ennemi est testée avec SDL_HasIntersection.
Effet de parallaxe

Le décor est composé de plusieurs couches qui se déplacent à des vitesses différentes :

    fond image ;
    étoiles lentes ;
    bâtiments lointains ;
    bâtiments moyens ;
    bâtiments proches ;
    sol au premier plan.

Cela donne une impression de profondeur : les éléments éloignés bougent plus lentement que les éléments proches.

Fonctions SDL importantes utilisées

    SDL_Init : initialise SDL2 ;
    IMG_Init : initialise SDL_image ;
    TTF_Init : initialise SDL_ttf ;
    SDL_CreateWindow : crée la fenêtre ;
    SDL_CreateRenderer : crée le renderer ;
    IMG_LoadTexture : charge une image en texture ;
    SDL_QueryTexture : récupère la taille d'une texture ;
    SDL_RenderCopy : affiche une texture ;
    SDL_RenderCopyEx : affiche une texture avec retournement gauche/droite ;
    SDL_PollEvent : lit les événements ;
    SDL_GetKeyboardState : détecte les touches maintenues ;
    SDL_GetMouseState : récupère la position de la souris ;
    SDL_GetTicks : gère le temps de l'animation ;
    SDL_HasIntersection : teste la collision entre deux rectangles ;
    TTF_RenderUTF8_Blended : crée du texte ;
    SDL_RenderPresent : affiche le rendu final.

Rôle des fichiers :

    src/sprites3_main.c : boucle principale, événements, mise à jour et affichage général ;
    src/sprites_utils.c : chargement des textures, affichage du texte et nettoyage SDL ;
    src/scene.c : fond, parallaxe et sol ;
    src/player.c : déplacement, saut, attaque et animation du joueur ;
    src/enemy.c : affichage de l'ennemi et de sa barre de vie ;
    include/sprites_config.h : constantes du programme ;
    include/sprites_utils.h : prototypes des fonctions utilitaires ;
    include/scene.h : prototypes du décor ;
    include/player.h : structure et prototypes du joueur ;
    include/enemy.h : prototypes de l'ennemi.

Compilation et exécution

Depuis la racine du projet :

cd sprites
gcc -Wall -Wextra -g src/sprites3_main.c src/sprites_utils.c src/scene.c src/player.c src/enemy.c -Iinclude -o sprites3 $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf
./sprites3
cd ..

Remarque : l'exécution doit se faire depuis le dossier sprites, car le programme charge les ressources depuis le dossier assets.
