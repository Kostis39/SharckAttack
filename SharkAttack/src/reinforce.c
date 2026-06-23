#include "reinforce.h"

/**
 * @brief Ajout un état supplémentaire à notre trajectoire.
 */
void New_step(StepTrajectory *step, Vector state, Vector action, float reward) {
    step->state = state;
    step->action = action;
    step->reward = reward;
}