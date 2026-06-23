#include "reinforce.h"

Trajectory *Trajectory_init() {
    Trajectory *new_trajectory = calloc(1, sizeof(Trajectory));
    if (new_trajectory == NULL)
        return NULL;
    new_trajectory->length = 0;
    new_trajectory->lenght_alloc = TRAJECTORY_LENGHT_ALLOC;
    new_trajectory->capacity = TRAJECTORY_CAPACITY;
    new_trajectory->steps =
        calloc(new_trajectory->lenght_alloc, sizeof(StepTrajectory));
    return new_trajectory;
}

/**
 * @brief Augmente la taille mémoire de la liste steps de trajectory si la
 * mémoire précédement alloué est pleinne.
 *
 * @param trajectory La trajectoire que l'on doit modifier (ajouter de la place
 * mémoire à steps)
 * @return int 1: Si il y a eu modification de la mémoire (realloc) 0 : sinon
 */
int Need_trajectory_growing(Trajectory *trajectory) {
    if (trajectory->length == trajectory->lenght_alloc) {
        int new_lenght_alloc =
            trajectory->lenght_alloc + TRAJECTORY_LENGHT_ALLOC;
        trajectory->lenght_alloc = (new_lenght_alloc < trajectory->capacity)
                                       ? new_lenght_alloc
                                       : trajectory->capacity;
        trajectory->steps = (StepTrajectory *)realloc(
            trajectory->steps,
            (trajectory->lenght_alloc) * sizeof(StepTrajectory));
        assert(trajectory->steps != NULL);
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief Ajout un état supplémentaire à notre trajectoire.
 */
void Add_step(Trajectory *trajectory, Vector state, Vector action,
              float reward) {
    int i = trajectory->length;
    trajectory->steps[i].state = state;
    trajectory->steps[i].action = action;
    trajectory->steps[i].reward = reward;
    ++trajectory->length;
}

void Trajectory_destroy(Trajectory *trajectory) {
    free(trajectory->steps);
    free(trajectory);
}

/**
 * @brief Permet d'afficher toutes les trajectoire où le reward est suppérieur à
 * 0
 *
 * @param trajectory La trajectoire à afficher
 */
void Trajectory_print(Trajectory *trajectory) {
    for (int i = 0; i < trajectory->length; ++i) {
        if (trajectory->steps[i].reward > 0) {
            printf("Step %d: State: (%f, %f) Action:(%f, %f) Reward: %f\n", i,
                   trajectory->steps[i].state.x, trajectory->steps[i].state.y,
                   trajectory->steps[i].action.x, trajectory->steps[i].action.y,
                   trajectory->steps[i].reward);
        }
    }
}