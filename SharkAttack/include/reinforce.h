#ifndef REINFORCE_H
#define REINFORCE_H

#include "config.h"
#include "input_output.h"
#include "shark.h"
#include "shark_controller.h"
#include "utils.h"
#include "utils_reinforce.h"
#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief initialise une trajectoire vide
 * @return pointeur vers la trajectoire allouée dynamiquement
 */
Trajectory *Trajectory_init();

/**
 * @brief Augmente la taille mémoire de la liste steps de trajectory si la
 * mémoire précédement alloué est pleinne.
 *
 * @param trajectory La trajectoire que l'on doit modifier (ajouter de la place
 * mémoire à steps)
 * @return int 1: Si il y a eu modification de la mémoire (realloc) 0 : sinon
 */
int Need_trajectory_growing(Trajectory *trajectory);

/**
 * @brief Ajout un état supplémentaire à notre trajectoire.
 * @param trajectory pointeur vers la trajectoire
 * @param phi valeurs des règles à cet l'état
 * @param action action choisie
 * @param reward récompense reçue
 */
void Add_step(Trajectory *trajectory, SharkPhi phi, Vector action,
              float reward);

/**
 * @brief libère la mémoire d'une trajectoire
 * @param trajectory pointeur vers la trajectoire à libérer
 */
void Trajectory_destroy(Trajectory *trajectory);

/**
 * @brief affiche les pas d'une trajectoire dont la récompense est supérieur à 0
 * @param trajectory pointeur vers la trajectoire à afficher
 */
void Trajectory_print(Trajectory *trajectory);

/**
 * @brief affiche un pas de trajectoire
 * @param step le pas à afficher
 */
void StepTrajectory_print(StepTrajectory step);

/**
 * @brief met à jour un pas de trajectoire
 * @param step pointeur vers le pas à modifier
 * @param phi valeurs des règles à cet l'état
 * @param action nouvelle action
 * @param reward nouvelle récompense
 */
void Step_update(StepTrajectory *step, SharkPhi phi, Vector action,
                 float reward);

/**
 * @brief retourne un gradient initialisé à zéro
 * @return structure Gradient avec tous les champs à zéro
 */
Gradient Gradient_zero();

/**
 * @brief fonction de travail pour les threads, exécute Compute_trajectory
 * @param args pointeur vers WorkerArgs contenant les paramètres
 * @return NULL
 */
void *Trajectory_worker(void *args);

/**
 * @brief exécute l'algorithme REINFORCE avec parallélisation
 * @param theta pointeur vers les paramètres de la politique à mettre à jour
 * @param hyperparameters hyperparamètres de l'apprentissage
 * @param thread_count nombre de threads à utiliser pour le calcul des
 * trajectoires
 */
void Reinforce_learning(VectorRule *theta, Hyperparameters hyperparameters,
                        int thread_count);
#endif
