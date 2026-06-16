# Mon Projet Génial

Projet réalisé en langage C avec SDL2.

## Exercice 1 — X fenêtré

Objectif : démontrer la capacité à ouvrir, fermer, déplacer et redimensionner des fenêtres SDL2.

Pour cet exercice, deux versions ont été réalisées afin de tester progressivement les fonctionnalités de SDL2.

--Version 1 — X dessiné dans une fenêtre

Cette première version utilise une seule fenêtre SDL2.
Elle permet de tester la création d'une fenêtre, la création d'un renderer, le dessin dans la fenêtre et l'animation simple.

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

--Version 2 — Fenêtres placées en forme de X

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
    SDL_RenderClear, SDL_RenderFillRect et SDL_RenderPresent pour afficher un contenu simple dans chaque fenêtre ;
    SDL_DestroyRenderer, SDL_DestroyWindow et SDL_Quit pour fermer proprement.

## Compilation de la version finale :

gcc -Wall -Wextra -g src/X_fenetre_main2.c -o x_fenetre2 $(sdl2-config --cflags --libs)

## Exécution :

./x_fenetre2

# Remarque : la version 1 a servi de première étape pour tester le dessin et l'animation dans une fenêtre. La version 2 est la version principale à présenter, car elle correspond mieux à l'idée du X fenêtré avec plusieurs fenêtres. 

## Exercice 2 — Pavé de serpents

Objectif : démontrer la capacité à dessiner des formes géométriques simples et à les animer avec SDL2.

Fonctionnalités réalisées :

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

Fonctions SDL2 utilisées :

    SDL_Init pour initialiser SDL2 ;
    SDL_CreateWindow pour créer la fenêtre ;
    SDL_CreateRenderer pour créer le renderer ;
    SDL_SetRenderDrawColor pour choisir les couleurs ;
    SDL_RenderClear pour effacer l'ancien affichage ;
    SDL_RenderFillRect pour dessiner les rectangles des serpents ;
    SDL_RenderDrawLine pour dessiner la grille et certains liens ;
    SDL_RenderPresent pour afficher le rendu ;
    SDL_Delay pour contrôler la vitesse de l'animation ;
    SDL_PollEvent pour gérer les événements clavier et la fermeture ;
    SDL_GetWindowSize pour récupérer la taille de la fenêtre.

Organisation du code :

    src/serpent_main.c : boucle principale SDL2 et gestion des événements ;
    src/snake.c : logique de déplacement des serpents ;
    src/draw.c : fonctions d'affichage ;
    src/window_tools.c : fonctions utiles pour la fenêtre ;
    include/config.h : constantes du programme ;
    include/snake.h : structure des serpents ;
    include/draw.h : prototypes d'affichage ;
    include/window_tools.h : prototypes des fonctions liées à la fenêtre.

## Compilation :

gcc -Wall -Wextra -g src/serpent_main.c src/snake.c src/draw.c src/window_tools.c -Iinclude -o serpent $(sdl2-config --cflags --libs) -lm

## Exécution :

./serpent