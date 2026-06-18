
#ifndef FISH_H
#define FISH_H

#include "config.h"
#include "perception.h"
#include "vector.h"
#include <stdbool.h>

typedef struct {
    Vector position;
    Vector velocity;
    bool alive; // true = vivant, false = mort
} Fish;

Fish *FishCreate(Vector pos); // Créer un poisson avec une position en parmètre
                              // et une vitesse aléatoire

/* Règles réactives individuelles : chacune transforme une FishPerception
   en une force désirée, sans aucun état interne ni mémoire. */
Vector RulesSeparation(Fish *self, FishPerception *p);
Vector RulesAlignment(Fish *self, FishPerception *p);
Vector RulesCohesion(Fish *self, FishPerception *p);

/* Combine les 3 règles ci-dessus (poids définis dans config.h)
   en une unique accélération désirée, transmise au Maître du Jeu */
Vector AgentCompute(Fish *self, FishPerception *p);

#endif