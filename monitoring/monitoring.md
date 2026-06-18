# Carnet de bord

[Lien vers le Google Sheet](https://docs.google.com/spreadsheets/d/1okuXoZ2LoURtgW37XICQSCUkHs4YVd-0aNOmB7Mmh0k/edit?usp=sharing)

## Lundi
Le lundi a commencé par la mise en place du *repository* gitlab, de l'arborescence de fichier et de la création d'un modèle de `Makefile` assez généraliste.

Durant l'après-midi nous avons réalisé l'activité proposé autour du **X fenêtré**, avec des variantes selon la personnes, chaque projet utilise de la gestion interactive de fenêtres ainsi que leur déplacement à l'écran.

## Mardi

### Mattéo
Gros *refactoring* de code pour nettoyer le code correspondant au XFenêtré. Cela permet une meilleure division des mini-projets, le coût est temporel.

### Sacha

### Romain

#### Objectif du jour :

Faire le *snake* le matin avec possible débordement sur l'après-midi

Finir les *sprites* l'après-midi

Et rajouter la fonctionnalité d'ajout de fenêtre dans X fenêtré en fin de journée

#### Résultat

## Mercredi 

Réalisation entière du jeu de la vie avec rendu SDL et rendu terminal.
 
Divisions du code en multiples composants, pour une meilleure modularité et lisibilité.

Création d'une documentation avec `Doxygen`.

## Jeudi

Conception du projet SMA, éventuelle création d'un diagramme style UML simplifié.

Avancement au plus possible sur le projet en lui même.

Puis réalisation de la présentation du vendredi.

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


