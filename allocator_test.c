#include <stdio.h>
#include "allocator.h"

int main() {
    int** matrix = malloc(3 * sizeof(int*));
    for (int i=0; i<3; i++) {
        matrix[i] = malloc(3 * sizeof(int));

        for (int j=0; j<3; j++) {
            matrix[i][j] = i + j;
        }
    }

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    for (int i=0; i<3; i++) {
        free(matrix[i]);
    }
    free(matrix);

    printf("OK\n");

    return 0;
}