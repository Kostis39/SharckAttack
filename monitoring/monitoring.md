# Carnet de bord

## Lundi
Le lundi a commencé par la mise en place du *repository* gitlab, de l'arborescence de fichier et de la création d'un modèle de `Makefile` assez généraliste.

Durant l'après-midi nous avons réalisé l'activité proposé autour du **X fenêtré**, avec des variantes selon la personnes, chaque projet utilise de la gestion interactive de fenêtres ainsi que leur déplacement à l'écran.

## Mardi

### Mattéo

### Sacha

### Romain

#### Objectif du jour :

Faire le *snake* le matin avec possible débordement sur l'après-midi

Finir les *sprites* l'après-midi

Et rajouter la fonctionnalité d'ajout de fenêtre dans X fenêtré en fin de journée

#### Résultat


# Projet SMA

## Définition

Le thème de notre jeu: boids.
Le but réaliser un comportement de boids allant à un certaine position
Puis ajout d'action sur le monde de la part de l'utilisateur tq lancé d'obj ou autre.

Oiseau: - Position
        - Vecteur vitesse

Monde: - Vide avec des oiseaux ce déplacant

Perception: - Distance par rapport au autres si proche : ce détermine avec 3 paramètres
    paramètres: - zone de répulstion (bord de fentre compris)
                - zone d'orientation
                - zone d'attraction

Comportement:   - S'éloigner si voisins est dans zonne de répulsion
                - S'aligner si voisins dans zone d'orientation
                - Se rapprocher si dans zone d'attraction


Amélioration possible:
- Ajout système de vie et comportement changeant en fct de ça.

## Organisation

1. Affichage:
    - Affichage des boids représenté par une flèche, qui est la direction en SDL2. 
        - Besoin de tableau de Boids
    - Affichage de chaque zone d'effets
2. Programmation du SMA:
    - MJ


