#include "base.h"

#include <stdio.h>

int main() {
    int N = 1000;
    int* mass = crossplatform_alloc(N * sizeof(int));
    
    for (int i=0; i<N; i++) {
        mass[i] = i;
        printf("%d ", mass[i]);
    }
    printf("\n");
    crossplatform_free(mass);
    printf("OK\n");
    return 0;
}