#include "reinforce.h"
#include "mj.h"
#include "world.h"

extern volatile sig_atomic_t stop_requested;

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
            printf("Step: %d ", i);
            SharkPhi_print(trajectory->steps[i].phi);
            printf(" Action:(%f, %f) Reward: %f\n",
                   trajectory->steps[i].action.x, trajectory->steps[i].action.y,
                   trajectory->steps[i].reward);
        }
    }
}

void StepTrajectory_print(StepTrajectory step) {
    SharkPhi_print(step.phi);
    printf(" Action:(%f, %f) Reward: %f\n", step.action.x, step.action.y,
           step.reward);
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
    G.x = VectorRule_init();
    G.y = VectorRule_init();
    return G;
}

TrajectoryCalculation Compute_trajectory(VectorRule theta,
                                         Hyperparameters hyperparameters) {
    float G = 0;
    float GG;
    float mu_x, mu_y;
    VectorRule score_x, score_y;
    Gradient D = Gradient_zero();

    World *world = World_init(WIDTH, HEIGHT, FISH_NB, COLLIDERS_NB, theta,
                              hyperparameters.sigma,
                              hyperparameters.nb_occurrence, false, true);

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

        mu_x = VectorRule_dot_product(theta, step.phi.x);
        mu_y = VectorRule_dot_product(theta, step.phi.y);

        score_x = VectorRule_scaled(step.phi.x,
                                    (1.0f / pow(hyperparameters.sigma, 2)) *
                                        (step.action.x - mu_x));
        score_y = VectorRule_scaled(step.phi.y,
                                    (1.0f / pow(hyperparameters.sigma, 2)) *
                                        (step.action.y - mu_y));

        D.x = VectorRule_add(D.x, VectorRule_scaled(score_x, GG));
        D.y = VectorRule_add(D.y, VectorRule_scaled(score_y, GG));
    }

    World_destroy(world);

    TrajectoryCalculation result_iteration = {D, world->fish_eaten, G,
                                              trajectory.length};
    return result_iteration;
}

void *Trajectory_worker(void *args) {
    init_seed((unsigned int)pthread_self());
    WorkerArgs *wargs = (WorkerArgs *)args;
    *(wargs->result) = Compute_trajectory(wargs->theta, wargs->hyperparameters);
    return NULL;
}

void Reinforce_learning(VectorRule *theta, Hyperparameters hyperparameters,
                        int thread_count) {
    int i;

    pthread_t *t = calloc(thread_count, sizeof(*t));
    WorkerArgs *args = calloc(thread_count, sizeof(*args));
    TrajectoryCalculation *result_trajectories =
        calloc(thread_count, sizeof(*result_trajectories));

    for (int k = 0; k < hyperparameters.nb_gen; k++) {
        if (stop_requested) {
            printf(
                "=== Arrêt prématuré de l'entraînement (Génération %d) ===\n",
                k);
            break;
        }
        Gradient D_total = Gradient_zero();
        float average_reward = 0.0f;
        float average_gain = 0.0f;
        float average_iteration = 0.0f;

        /*******THREAD CREATOR*******/
        for (i = 0; i < thread_count; ++i) {
            args[i] = (WorkerArgs){
                .theta = *theta,
                .hyperparameters = hyperparameters,
                .result = &result_trajectories[i],
            };
            pthread_create(&t[i], NULL, Trajectory_worker, &args[i]);
        }
        /****************************/

        /*******THREAD JOINATOR*******/
        for (i = 0; i < thread_count; ++i) {
            pthread_join(t[i], NULL);
        }
        /****************************/
        for (int j = 0; j < thread_count; ++j) {

            D_total.x =
                VectorRule_add(D_total.x, result_trajectories[j].grad.x);
            D_total.y =
                VectorRule_add(D_total.y, result_trajectories[j].grad.y);

            average_reward += result_trajectories[j].total_reward;
            average_gain += result_trajectories[j].total_gain;
            average_iteration += result_trajectories[j].total_iteration;
        }

        average_reward /= thread_count;
        average_gain /= thread_count;
        average_iteration /= thread_count;

        // estimateur du gradient
        Gradient grad;
        grad.x = VectorRule_scaled(D_total.x, 1.0f / thread_count);
        grad.y = VectorRule_scaled(D_total.y, 1.0f / thread_count);

        // mise à jour de theta
        *theta = VectorRule_add(
            *theta, VectorRule_scaled(grad.x, hyperparameters.alpha));
        *theta = VectorRule_add(
            *theta, VectorRule_scaled(grad.y, hyperparameters.alpha));

        if ((k + 1) % STEP_LOG == 0) {
            logs_generation(theta, k + 1, average_reward, average_gain,
                            average_iteration, FILE_LOG);
        }

        printf("=== Génération %d / %d ===\n", k + 1, hyperparameters.nb_gen);
        printf("avg reward = %f, avg gain = %f, avg iteration = %f\n",
               average_reward, average_gain, average_iteration);
        printf("Theta = ");
        VectorRule_print(*theta);
        printf("\n\n");
    }

    free(result_trajectories);
    free(t);
    free(args);
}