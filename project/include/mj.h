#ifndef MJ_H
#define MJ_H
#include "fish.h"
#include "fish_controller.h"
#include "world.h"
/* #include "shark.h" */
FishPerception Get_fish_perception(Fish *fish, World *world);
// SharkPerception get_shark_perception(World *world);
void Update_fishes(World *world);
#endif
