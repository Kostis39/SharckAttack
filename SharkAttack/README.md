# SharkAttack

## But du jeu

SharkAttack est un jeu où l'on doit manger tout les poissons avant qu'un autre requin controlé par un bot ne mange tout les poissons étant dans son monde.

## Exécution

Pour lancer le programme, utilisez la commande bash `make` ou `make run` pour compiler et éxécuter le programme.

Voici toutes les commandes et leurs description pour compiler différentes parties du project.

**Compilation :**
 
| Commande | Description |
|:---:|:---|
| `make` | Compile et lance tous les fichiers c |
| `make clean` | Supprime les dossiers `bin/` et `build/` |
 
**Lancement :**
 
| Commande | Description |
|:---:|:---|
| `make run` | Lance le programme en mode SDL |
| `make run-term` | Lance le programme en mode terminal |
| `make run-learn` | Lance le programme en mode apprentissage |
 
**Tests :**
 
| Commande | Description |
|:---:|:---|
| `make test` | Compile tous les tests Unity fais dans `test/` |
| `make run_test` | Compile et exécute tous les tests |
| `make run_test_nom` | Exécute uniquement le test `nom` |
 
**Documentation :**
 
| Commande | Description |
|:---:|:---|
| `make doc` | Génère la documentation Doxygen dans `docs/` |
| `make doc-clean` | Supprime le dossier `docs/` |

## Documentation

La documentation [`Doxygen`](./docs/html/index.html) peut être trouvé dans le dossier [`doc`], Si elle n'y est pas déjà faites `make doc` pour la créer
