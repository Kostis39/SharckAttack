#ifndef INPUT_OUTPUT_H
#define INPUT_OUTPUT_H

#include "config.h"
#include "utils_reinforce.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief sauvegarde les paramètres theta, les hyperparamètres et le meilleur
 * gain dans un fichier
 * @param theta pointeur vers le vecteur theta à sauvegarder
 * @param hyperparams pointeur vers la structure des hyperparamètres
 * @param best_gain meilleur gain obtenu avec cette politique
 * @param filename nom du fichier de sauvegarde
 * @return true si la sauvegarde a réussi, false sinon
 */
bool save_params(VectorRule *theta, Hyperparameters *hyperparams,
                 float best_gain, char *filename);

/**
 * @brief charge les paramètres theta et les hyperparamètres depuis un fichier
 * @param theta pointeur vers le vecteur theta à remplir
 * @param hyperparams pointeur vers la structure des hyperparamètres à remplir
 * @param filename nom du fichier source
 * @return true si le chargement a réussi, false sinon
 */
bool load_params(VectorRule *theta, Hyperparameters *hyperparams,
                 char *filename);

/**
 * @brief charge uniquement le vecteur theta depuis un fichier
 * @param theta pointeur vers le vecteur theta à remplir
 * @param filename nom du fichier source
 * @return true si le chargement a réussi, false sinon
 */
bool load_theta(VectorRule *theta, char *filename);

/**
 * @brief écrit un log pour une génération d'apprentissage
 * @param theta politique actuelle
 * @param gen_number numéro de la génération
 * @param avg_reward récompense moyenne sur la génération
 * @param avg_gain gain moyen sur la génération
 * @param avg_iteration itération moyenne sur la génération
 * @param filename nom du fichier de logs
 * @return true si l'écriture a réussi, false sinon
 */
bool logs_generation(VectorRule *theta, int gen_number, float avg_reward,
                     float avg_gain, float avg_iteration, char *filename);

/**
 * @brief Réinitialise le fichier de logs et y écrit la configuration initiale
 * provenant du fichier de paramètres.
 * @param filename_logs   Nom du fichier de logs à créer/réinitialiser.
 * @param filename_params Nom du fichier contenant les paramètres initiaux.
 * @return true si l'initialisation a réussi, false sinon.
 */
bool init_logs(char *filename_params, char *filename_logs);

/**
 * @brief termine l'apprentissage en comparant le meilleur gain trouvé avec
 * celui du fichier de paramètres, met à jour le fichier de paramètres si un
 * meilleur gain est trouvé
 * @param filename_params nom du fichier de paramètres
 * @param filename_logs nom du fichier de logs
 * @return true si l'opération a réussi, false sinon
 */
bool end_logs(char *filename_params, char *filename_logs);

#endif