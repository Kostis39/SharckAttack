#include "vector_rule.h"

VectorRule VectorRule_init() {
    VectorRule rules;
    for (int i = 0; i < Rules_Lenght; ++i) {
        rules.vect[i] = 0.0f;
    }
    return rules;
}

void VectorRule_print(VectorRule v) {
    printf("(");
    for (int i = 0; i < Rules_Lenght; ++i) {
        printf("%f", v.vect[i]);

        if (i < Rules_Lenght - 1) {
            printf(", ");
        }
    }
    printf(")");
}

void VectorRule_fprint(FILE *file, VectorRule v) {
    fprintf(file, "(");
    for (int i = 0; i < Rules_Lenght; ++i) {
        fprintf(file, "%f", v.vect[i]);

        if (i < Rules_Lenght - 1) {
            fprintf(file, ", ");
        }
    }
    fprintf(file, ")");
}

float VectorRule_dot_product(VectorRule a, VectorRule b) {
    float result = 0.0f;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result += a.vect[i] * b.vect[i];
    }

    return result;
}

VectorRule VectorRule_scaled(VectorRule v, float scalar) {
    for (int i = 0; i < Rules_Lenght; ++i) {
        v.vect[i] *= scalar;
    }

    return v;
}

VectorRule VectorRule_add(VectorRule a, VectorRule b) {
    VectorRule result;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result.vect[i] = a.vect[i] + b.vect[i];
    }
    return result;
}

VectorRule VectorRule_sub(VectorRule a, VectorRule b) {
    VectorRule result;
    for (int i = 0; i < Rules_Lenght; ++i) {
        result.vect[i] = a.vect[i] - b.vect[i];
    }
    return result;
}