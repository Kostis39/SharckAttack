#ifndef MJ_H
#define MJ_H
#include "config.h"
#include "fish.h"
#include "fish_controller.h"
#include "vector.h"
#include "world.h"
/* #include "shark.h" */

void FishPerception_destroy(FishPerception *perception);

void UpdateWorld(World *world, World *tmp_world);
#endif
