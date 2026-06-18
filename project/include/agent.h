#ifndef AGENT_H
#define AGENT_H

#include "config.h"
#include "fish.h"
#include "perception.h"
#include "vector.h"

/* Règles réactives individuelles : chacune transforme une FishPerception
   en une force désirée, sans aucun état interne ni mémoire. */
Vector RulesSeparation(Fish *self, FishPerception *p);
Vector RulesAlignment(Fish *self, FishPerception *p);
Vector RulesCohesion(Fish *self, FishPerception *p);

/* Combine les quatre règles ci-dessus (poids définis dans config.h)
   en une unique accélération désirée, transmise au Maître du Jeu */
Vector AgentCompute(Fish *self, FishPerception *p);

#endif