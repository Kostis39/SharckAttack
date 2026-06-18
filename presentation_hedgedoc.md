# Présentation Projet ZZ1 — Soutenance du 1er vendredi

**Équipe :** Mattéo, Romain, Sacha, Mohamed, Matt
**Durée :** 10 min max

---

## Plan

1. Introduction & reformulation des attendus
2. Phase 1 — Démos SDL2 individuelles
3. Phase 2 — Jeu de la Vie (SMA)
4. Phase 3 — Shark Attack (architecture & avancement)
5. Gestion de projet & Collaboration
6. Bilan, limites & perspectives
7. Questions

---

## 1. Introduction & reformulation des attendus

Le projet ZZ1 se décompose en deux phases :

**Phase 1 (lundi → mercredi midi) :** Apprentissage guidé de la SDL2, puis mise en œuvre d'un premier travail de groupe — le Jeu de la Vie — pour appréhender le paradigme des **systèmes multi-agents réactifs (SMA)**.

**Phase 2 (mercredi → vendredi 2e semaine) :** Développement d'un jeu de simulation/arcade intégrant un **SMA** et une forme d'**apprentissage par renforcement (REINFORCE)**.

**Ce qu'on présente aujourd'hui :** Le bilan de la première phase — démos individuelles, Jeu de la Vie, et l'état d'avancement de Shark Attack.

---

## 2. Phase 1 — Démos SDL2 individuelles

> **Compétence C3 — AC1 :** Programmer dans un langage spécifique (C + SDL2)

Chaque membre a réalisé 2-3 mini-projets en autonomie pour se familiariser avec la SDL2 :

| Membre | Démos |
|--------|-------|
| **Matteo** | X-fenêtré (bouncer), Sprites (animation spritesheets) |
| **Romain** | X-fenêtré (DVD bounce), Sprites (scrolling), Pavé de serpents |
| **Sacha** | X-fenêtré, Snake (déplacement vectoriel) |
| **Mohamed** | X-fenêtré, Sprites (jeu avec ennemis, scènes) |

**Compétences acquises :**
- Création de fenêtres et renderers SDL2
- Gestion des événements clavier / souris
- Affichage de textures et animations (spritesheets)
- Boucle de jeu temps réel

> En deux jours, chaque membre est passé de "je ne connais pas SDL2" à "je sais faire une application graphique interactive". Cette montée en compétence rapide nous a permis d'attaquer le projet d'équipe dès mercredi.

---

## 3. Phase 2 — Jeu de la Vie (SMA)

> **Compétence C2 — AC1 :** Comprendre le modèle SMA (agents réactifs, émergence, Maître du Jeu)

### Modélisation SMA

Le Jeu de la Vie est un **système multi-agents réactif** où chaque cellule est un agent :

- **Perception :** voisinage 3×3 (Moore) — l'agent observe ses 8 voisins
- **Règle :** fonction de transition déterministe (règles de Conway)
  - Une cellule vivante survit avec 2 ou 3 voisins, sinon elle meurt
  - Une cellule morte naît avec exactement 3 voisins
- **Action :** changer d'état (vivant ↔ mort)
- **Pas de mémoire :** l'état suivant ne dépend que de l'état courant et des voisins

Le **Maître du Jeu** (`mj.h`) orchestre la simulation :
1. Chaque agent perçoit son voisinage
2. Chaque agent calcule son nouvel état
3. Le MJ applique toutes les mises à jour **simultanément** (double-buffering)

> Ce pattern — perception → décision → action synchronisée — est la base de tout SMA réactif. On le retrouve tel quel dans Shark Attack.

### Architecture (6 modules)

| Module | Rôle |
|--------|------|
| `cell.h/c` | Cellule (vivante/morte), allocation/libération 2D |
| `world.h/c` | Monde (grille 2D), zoom, offset, perception 3×3 |
| `agent.h/c` | Règles de transition (comptage voisins, nouvel état) |
| `mj.h/c` | Moteur de simulation (double-buffering) |
| `terminalDisplay.h/c` | Affichage texte (codes ANSI) |
| `SDLDisplay.h/c` | Affichage graphique SDL2 |

### Fonctionnalités livrées

- ✅ Double mode : **terminal** (codes ANSI) et **SDL2** (graphique)
- ✅ Grille **torique** (wrapping aux bords)
- ✅ Zoom / déplacement de la vue (Z/Q/S/D ou molette souris)
- ✅ Clic souris pour basculer une cellule
- ✅ Itération pas à pas (Espace)
- ✅ Documentation **Doxygen** complète

### Démonstration

> *[Démonstration en direct du Jeu de la Vie en mode terminal puis SDL2]*

---

## 4. Phase 3 — Shark Attack (architecture & avancement)

> **Compétence C2 — AC2 :** Lire, comprendre et adapter les algorithmes présentés (SMA réactifs)
> **Compétence C3 — AC3 :** Développer une application en lien avec les bibliothèques standards et la SDL2

### Concept

Simulation d'un écosystème marin : 80 poissons évoluent selon des règles de **flocking** (Boids de Craig Reynolds) en présence d'un requin prédateur. Rendu temps réel via SDL2.

### Architecture logicielle (9 modules)

```
config.h            → Constantes (vitesses, rayons, poids)
vector.h/c          → Maths 2D (vecteurs, opérations)
fish.h/c            → Agent poisson
fish_controller.h/c → Perception + règles de flocking
shark.h/c           → Agent requin
world.h/c           → Conteneur du monde
mj.h/c              → Moteur de simulation (perception)
game.h/c            → Boucle de jeu + SDL
render_sdl.h/c      → Rendu graphique
```

### Choix techniques

**Séparation interface / implémentation :**
- Headers dans `include/`, sources dans `src/`
- Chaque module compile indépendamment : `make TARGET=fish`

**Gestion des dépendances circulaires :**
- `fish.h` avait besoin de `FishPerception`, et `fish_controller.h` avait besoin de `Fish`
- Résolu avec des **forward declarations** (`struct FishPerception;`) dans les headers, et les `#include` réels dans les `.c`

**Gestion mémoire propre :**
- Allocation : `calloc` systématique
- Libération : chaque module a son destructeur (`Fish_destroy`, `Shark_destroy`, `World_destroy`)

### Règles de flocking (Boids)

| Règle | Principe | Pondération |
|-------|----------|-------------|
| **Séparation** | S'éloigner des voisins trop proches | 1.5 |
| **Alignement** | S'aligner sur la direction moyenne | 1.0 |
| **Cohésion** | Se diriger vers le centre du groupe | 1.0 |

Chaque règle produit un **vecteur force** → combinaison pondérée → nouvelle vitesse → nouveau déplacement.

### État d'avancement

| Statut | Éléments |
|--------|----------|
| ✅ Fait | Architecture complète, vector, fish, shark, world, perception, rendu SDL2, Makefile ciblé |
| 🔄 En cours | Règles de flocking (séparation implémentée, alignement/cohésion en cours) |
| 📋 À faire | Comportement du requin, collisions, événements SDL, intégration REINFORCE |

---

## 5. Gestion de projet & Collaboration

> **Compétence C4 :** S'intégrer dans un projet et mettre en place des outils de gestion

### Déroulement

1. **Mise en place** — Setup GitLab, arborescence, Makefile de référence
2. **Démos individuelles** — X-fenêtré, Snake, Sprites : chacun apprend la SDL2 en autonomie
3. **Jeu de la Vie** — Premier projet d'équipe, implémentation SMA complète (terminal + SDL2)
4. **Shark Attack** — Développement du projet principal, architecture modulaire, intégration

### Outils & Méthodes

**Git collaboratif :**
- Branche `dev` pour le développement, `master` pour les versions stables
- Commits réguliers avec messages explicites
- Chaque membre a contribué sur des modules distincts

**Makefile évolué :**
- Compilation séparée par module (`make TARGET=fish`)
- Flags de warning stricts (`-Wall -Wextra -g`)
- Nettoyage propre (`make clean`)

**Documentation :**
- Doxygen pour le Jeu de la Vie
- Commentaires dans les headers (rôle de chaque fonction, structures documentées)
- Carnet de bord de suivi

### Répartition du travail

| Membre | Modules principaux |
|--------|-------------------|
| **Matteo** | Vector, Fish, Makefile, refactoring |
| **Sacha** | Shark, FishController (perception), World |
| **Romain** | Game, MJ (moteur de simulation) |
| **Mohamed** | Render SDL2 (affichage) |
| **Matt** | Intégration, résolution dépendances, tests |

---

## 6. Bilan, limites & perspectives

### Compétences acquises — auto-évaluation

| Compétence | Niveau | Démontré par |
|------------|--------|--------------|
| **C1 — Support de communication** | Bien | Support structuré, reformulation des attendus, démo préparée |
| **C2 — Modélisation SMA** | Bien | Jeu de la Vie : agents, perception, règles, émergence compris et implémentés |
| **C3 — Implémentation C/SDL2** | Bien | Architecture modulaire, code compilable, SDL2 maîtrisée |
| **C4 — Gestion de projet** | Bien | Git collaboratif, répartition claire, Makefile maintenu |

### Limites & bogues connus

- Les règles de flocking ne sont pas encore toutes implémentées (seule la séparation est codée)
- Le comportement du requin n'est pas encore implémenté
- Pas de gestion des collisions poisson/requin
- REINFORCE n'a pas encore été intégré (prévu pour la 2e semaine)
- Pas de tests unitaires formels (teZZt) ni d'utilisation systématique de Valgrind/sanitizer

### Perspectives (2e semaine)

1. Finaliser les règles de flocking (alignement, cohésion)
2. Implémenter le comportement de chasse du requin
3. Gérer les collisions poisson / requin
4. Intégrer REINFORCE pour l'apprentissage du requin
5. Ajouter les tests unitaires et la validation mémoire (Valgrind)
6. Finaliser la boucle de jeu complète avec gestion des événements

---
