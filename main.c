#include <stdio.h>
#include <stdlib.h>

#include "include/topologies.h"


int main()
{
    k_ary_n_cube *cube;
    unsigned long u_index, v_index;
    unsigned char scanf_buffer;

    // Initialise the k-ary n-cube
    cube = (k_ary_n_cube *)malloc(sizeof(k_ary_n_cube));
    define_kary_ncube(cube);

    // Perform routing from one point to the other
    printf("\nOrigin: ");
    scanf_buffer = scanf("%ld", &u_index);
    printf("Target: ");
    scanf_buffer = scanf("%ld", &v_index);
    printf("\n");
    routing_from(cube, u_index, v_index);

    // End of routing process.
    printf("\n\n ** <<END of ROUTING>> ** BYE! **\n\n");

    // Free the k-ary n-cube
    free_kary_ncube(&cube);

    return 0;
}