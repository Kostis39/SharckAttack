#ifndef MJ_H
#define MJ_H
#include "config.h"
#include "fish.h"
#include "fish_controller.h"
#include "shark.h"
#include "shark_controller.h"
#include "vector.h"
#include "world.h"
/* #include "shark.h" */

void FishPerception_destroy(FishPerception *perception);
void Game_step(World *world);

void UpdateWorld(World *world, World *tmp_world);

void fish_perception_init(Fish *fish, World *world, FishPerception *perception);
void get_fish_perception(Fish *fish, World *world, FishPerception *perception);
void Get_shark_perception(Shark *shark, World *world,
                          SharkPerception *shark_perception);
#endif
