#include "reinforce.h"
#include "mj.h"
#include "world.h"

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
void Add_step(Trajectory *trajectory, SharkPhi phi, Vector action,
              float reward) {
    int i = trajectory->length;
    trajectory->steps[i].phi = phi;
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
    printf("taille : %d\n", trajectory->length);
    for (int i = 0; i < trajectory->length; ++i) {
        if (trajectory->steps[i].reward > 0) {
            printf("Step: %d phi: (%f, %f, %f, %f, %f, %f) "
                   "Action:(%f, %f) "
                   "Reward: %f\n",
                   i, trajectory->steps[i].phi.x.center,
                   trajectory->steps[i].phi.y.center,
                   trajectory->steps[i].phi.x.alignment,
                   trajectory->steps[i].phi.y.alignment,
                   trajectory->steps[i].phi.x.pursuit,
                   trajectory->steps[i].phi.y.pursuit,
                   trajectory->steps[i].action.x, trajectory->steps[i].action.y,
                   trajectory->steps[i].reward);
        }
    }
}

void StepTrajectory_print(StepTrajectory step) {
    printf("phi: (%f, %f, %f, %f, %f, %f) "
           "Action:(%f, %f) "
           "Reward: %f\n",
           step.phi.x.center, step.phi.y.center, step.phi.x.alignment,
           step.phi.y.alignment, step.phi.x.pursuit, step.phi.y.pursuit,
           step.action.x, step.action.y, step.reward);
}

/**
 * @brief Ajout un état supplémentaire à notre trajectoire.
 */
void New_step(StepTrajectory *step, SharkPhi phi, Vector action, float reward) {
    step->phi = phi;
    step->action = action;
    step->reward = reward;
}

void Step_update(StepTrajectory *step, SharkPhi phi, Vector action,
                 float reward) {
    if (!step)
        return;
    step->phi = phi;
    step->action = action;
    step->reward = reward;
}

Gradient Gradient_zero() {
    Gradient G;

    G.x.center = 0;
    G.y.center = 0;
    G.x.alignment = 0;
    G.y.alignment = 0;
    G.x.pursuit = 0;
    G.y.pursuit = 0;

    return G;
}

Gradient Generate_gradient(SharkTheta theta, Hyperparameters hyperparameters) {
    float G = 0;
    float GG;
    float mu_x, mu_y;
    VectorRule score_x, score_y;
    Gradient D = Gradient_zero();

    World *world = World_init(WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB, theta,
                              hyperparameters.sigma, false);

    for (int i = 0; (world->nb_fish - world->fish_eaten != 0) &&
                    i < hyperparameters.nb_occurrence;
         i++) {
        Game_step(world);
    }
    Trajectory trajectory = *world->trajectory;
    // pour afficher la trajectoir : Trajectory_print(&trajectory);

    for (int u = 0; u < trajectory.length; u++) {
        int t = trajectory.length - 1 - u;
        StepTrajectory step = trajectory.steps[t];
        G = step.reward + hyperparameters.gamma * G;
        GG = pow(hyperparameters.gamma, t) * G;

        mu_x = Dot_product(theta.x, step.phi.x);
        mu_y = Dot_product(theta.y, step.phi.y);

        score_x = Vector_rule_scaled(step.phi.x,
                                     (1.0f / pow(hyperparameters.sigma, 2)) *
                                         (step.action.x - mu_x));
        score_y = Vector_rule_scaled(step.phi.y,
                                     (1.0f / pow(hyperparameters.sigma, 2)) *
                                         (step.action.y - mu_y));

        D.x = Vector_rule_add(D.x, Vector_rule_scaled(score_x, GG));
        D.y = Vector_rule_add(D.y, Vector_rule_scaled(score_y, GG));
    }

    World_destroy(world);

    return D;
}

void Reinforce_learning(SharkTheta *theta, Hyperparameters hyperparameters) {

    for (int k = 0; k < hyperparameters.nb_gen; k++) {

        Gradient D_total = Gradient_zero();

        // générer N trajectoires et accumuler leur gradient
        for (int i = 0; i < hyperparameters.nb_game; i++) {
            Gradient D_i = Generate_gradient(
                *theta, hyperparameters); // joue 1 trajectoire et calcule sa
                                          // contribution
            D_total.x = Vector_rule_add(D_total.x, D_i.x);
            D_total.y = Vector_rule_add(D_total.y, D_i.y);
        }

        // estimateur du gradient
        Gradient grad;
        grad.x = Vector_rule_scaled(D_total.x, 1.0f / hyperparameters.nb_game);
        grad.y = Vector_rule_scaled(D_total.y, 1.0f / hyperparameters.nb_game);

        // mise à jour de theta
        theta->x = Vector_rule_add(
            theta->x, Vector_rule_scaled(grad.x, hyperparameters.alpha));
        theta->y = Vector_rule_add(
            theta->y, Vector_rule_scaled(grad.y, hyperparameters.alpha));

        printf("======================\n");
        printf("itération %d / %d\n", k + 1, hyperparameters.nb_gen);
        printf("theta_x = (%f, %f, %f)\n", theta->x.center, theta->x.alignment,
               theta->x.pursuit);
        printf("theta_y = (%f, %f, %f)\n", theta->y.center, theta->y.alignment,
               theta->y.pursuit);
    }
}