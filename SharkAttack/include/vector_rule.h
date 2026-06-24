#ifndef VECTOR_RULE_H
#define VECTOR_RULE_H

#include "config.h"

typedef struct {
    float vect[Rules_Lenght];
} VectorRule;

VectorRule VectorRule_init();

void VectorRule_print(VectorRule v);

float VectorRule_dot_product(VectorRule a, VectorRule b);
VectorRule VectorRule_scaled(VectorRule v, float scalar);
VectorRule VectorRule_add(VectorRule a, VectorRule b);

#endif