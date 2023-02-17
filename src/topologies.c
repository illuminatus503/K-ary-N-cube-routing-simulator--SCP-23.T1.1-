#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

#include "../include/topologies.h"

/*! K-ARY N-CUBE STRUCTURE -- INIT !*/

/**
 * @brief Define a k-ary n-cube graph.
 *  - Number of nodes: n^k (k nodes per dimension --n dims.--)
 *  - Digraph (Directed graph).
 *
 * @param cube K-ary n-cube graph.
 */
void define_kary_ncube(k_ary_n_cube *cube)
{
    long n_dims, k, n_vertex;
    bool has_rings;

    unsigned char scanf_buffer; // Output of scanf, to be saved.

    // Define number of dimensions
    printf("n = ");
    scanf_buffer = scanf("%ld", &n_dims);
    if (n_dims <= 0)
    {
        fprintf(stderr, "Error dims: %ld is not a valid dimension size.\n", n_dims);
        exit(errno);
    }

    // Define the number k
    printf("k = ");
    scanf_buffer = scanf("%ld", &k);
    if (k < 2)
    {
        fprintf(stderr, "Error dims: %ld is not a valid dimension size.\n", k);
        exit(errno);
    }

    // Number of vertex:
    n_vertex = powl(k, n_dims);

    // Has rings? (is it a torus?)
    printf("Has rings? = ");
    scanf_buffer = scanf("%hhd", &has_rings);
    if (has_rings > 1 || has_rings < 0)
    {
        fprintf(stderr, "Boolean error. Invalid true value.\n");
        exit(errno);
    }

    /* Allocate memory for the structure */
    cube->n = n_dims;
    cube->k = k;
    cube->g = (PartialGraph *)malloc(sizeof(PartialGraph));
    define_graph(cube->g, n_vertex, n_dims);

    /* Allocate memory for the register */
    cube->last_reg = (RoutingReg *)malloc(sizeof(RoutingReg));
    define_routing_reg(cube->last_reg, n_dims);

    // Encode the coordinates of the k-ary n-cube.
    encode_coordinates(cube);

    // Decide whether it's a hypercube (k == 2 and no rings)
    // a torus (k >= 2 and has rings) or a mesh (k >= 2 and no rings).
    // Define the edges to be set and the *routing function*.
    printf("%ld-ary %ld-", k, n_dims);
    if ((k == 2) && !has_rings)
    {
        printf("hypercube");
        cube->routing_function = &hypercube_routing_func;
    }
    else if (k >= 2)
    {
        if (has_rings)
        {
            printf("torus");
            cube->routing_function = &torus_routing_func;
        }
        else
        {
            printf("mesh");
            cube->routing_function = &mesh_routing_func;
        }
    }
    printf(": %ld nodes in total\n", n_vertex);
}

/**
 * @brief Routing from one vertex, to another.
 *
 * @param cube A K-ary N-cube.
 * @param u_index The index of the source node.
 * @param v_index The index of the destination node.
 */
void routing_from(k_ary_n_cube *cube, uint u_index, uint v_index)
{
    long coord_value, coord_index;
    unsigned long distance, reg_length = cube->last_reg->length;
    unsigned int n_steps_taken;

    Vertex *u_clone;

    // Represent which vertices are going to be source and destination.
    printf("The package goes from ");
    print_vertex(cube->g, u_index);
    printf(" to ");
    print_vertex(cube->g, v_index);
    printf(".\n\n");

    // Work out the routing register to go from one point to the other.
    cube->routing_function(cube, u_index, v_index);

    // Print the routing register and the distance between nodes.
    printf(" ** Routing Register ** \n");
    printf(" \t --> [ ");
    distance = 0;
    for (coord_index = 0; coord_index < reg_length; coord_index++)
    {
        coord_value = cube->last_reg->register_[coord_index];
        distance += abs(coord_value);
        printf("%ld ", coord_value);
    }
    printf("] <-- \n");
    printf("Graph distance between nodes: %ld\n\n", distance);

    // Clone origin vertex: later, we will modify the coordinates of the vertex.
    u_clone = (Vertex *)malloc(sizeof(Vertex));
    define_vertex(u_clone, 0, reg_length);
    clone_vertex(cube->g->vertices[u_index], u_clone);

    // Visualise the origin.
    n_steps_taken = 0;
    printf(" ** PATH TAKEN (from the begining to the end) ** \n\n");
    printf("( INDEX = %ld ) Step %d taken = [ ", u_clone->index, n_steps_taken++);
    for (int i = 0; i < reg_length; i++)
        printf("%ld ", u_clone->coordinates[i]);
    printf("]\n");

    // Translate: from origin to target, sequentially.
    // For visualization purposes.
    for (coord_index = reg_length - 1; coord_index >= 0; coord_index--)
    {
        // Firstly, move from one coordinate from origin, to the
        // other sequentially.
        while (abs(cube->last_reg->register_[coord_index]) > 0)
        {
            if (cube->last_reg->register_[coord_index] > 0)
            {
                u_clone->coordinates[coord_index] = (u_clone->coordinates[coord_index] + 1) % cube->k;
                cube->last_reg->register_[coord_index]--;
            }
            else if (cube->last_reg->register_[coord_index] < 0)
            {
                u_clone->coordinates[coord_index] = (u_clone->coordinates[coord_index] - 1) % cube->k;
                if (u_clone->coordinates[coord_index] < 0) // For torus, equivalent coordinate in arithmetic
                                                           // Modulo K by summing the period k to the neg. num.
                {
                    u_clone->coordinates[coord_index] += cube->k;
                }

                cube->last_reg->register_[coord_index]++;
            }

            // Print on screen the next step taken
            printf("( INDEX = %ld ) Step %d taken = [ ", decode_coordinates(u_clone, cube->k), n_steps_taken++);
            for (int i = 0; i < reg_length; i++)
                printf("%ld ", u_clone->coordinates[i]);
            printf("]\n");
        }
    }

    // Lastly, free the auxiliary vertex u_clone.
    free_vertex(&u_clone);
}

/**
 * @brief Free the k-ary n-cube structure.
 *
 * @param cube The cube to be freed.
 */
void free_kary_ncube(k_ary_n_cube **cube)
{
    free_graph(&((*cube)->g));              // Firstly, free the subjacent graph.
    free_routing_reg(&((*cube)->last_reg)); // Free last routing register structure.
    free(*cube);                            // Then, free the k-ary n-cube.
}

/*! ROUTING REGISTER -- INIT !*/

/**
 * @brief Define a routing register, from parameters.
 *
 * @param reg The register to be initialised.
 * @param length The length of the register.
 */
void define_routing_reg(RoutingReg *reg, unsigned long length)
{
    if (length <= 0)
    {
        fprintf(stderr, "Invalid register size.\n");
        exit(errno);
    }

    reg->length = length;
    reg->register_ = calloc(length, sizeof(long));
}

/**
 * @brief Free routing register.
 *
 * @param reg A pointer to a Routing register structure.
 */
void free_routing_reg(RoutingReg **reg)
{
    free((*reg)->register_);
    free(*reg);
}

/**
 * @brief Routing function for n-dimensional mesh, with k-nodes per dim.
 *
 * @param cube A k-ary n-cube
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void mesh_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index)
{
    if (u_index * v_index <= 0)
    {
        fprintf(stderr, "Invalid index on routing.\n");
        exit(errno);
    }
    
    // Take the vertices from the graph.
    Vertex *u, *v;
    u = cube->g->vertices[u_index];
    v = cube->g->vertices[v_index];

    // Routing register
    RoutingReg *reg;
    reg = cube->last_reg; // Se supone inicializado

    // Work out the steps to take in all dims of the mesh.
    for (int coordinate_index = 0; coordinate_index < u->n_dims; coordinate_index++)
    {
        reg->register_[coordinate_index] = v->coordinates[coordinate_index] - u->coordinates[coordinate_index];
    }
}

/**
 * @brief Routing function for n-dimensional torus, with k-nodes per dim.
 *
 * @param cube A k-ary n-cube
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void torus_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index)
{
    long reg_val;

    if (u_index * v_index <= 0)
    {
        fprintf(stderr, "Invalid index on routing.\n");
        exit(errno);
    }

    // Take the vertices from the graph.
    Vertex *u, *v;
    u = cube->g->vertices[u_index];
    v = cube->g->vertices[v_index];

    // Routing register
    RoutingReg *reg;
    reg = cube->last_reg; // Se supone inicializado

    // Work out the steps to take in all dims of the mesh.
    for (int coordinate_index = 0; coordinate_index < u->n_dims; coordinate_index++)
    {
        reg_val = v->coordinates[coordinate_index] - u->coordinates[coordinate_index];

        // Correct the path if it is very long.
        if (abs(reg_val) > (cube->k / 2))
        {
            if (reg_val > 0)
            {
                reg_val -= cube->k;
            }
            else
            {
                reg_val += cube->k;
            }
        }

        reg->register_[coordinate_index] = reg_val;
    }
}

/**
 * @brief Routing function for n-dimensional hypercube.
 *
 * @param cube A n-hypercube.
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void hypercube_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index)
{
    if (u_index * v_index <= 0)
    {
        fprintf(stderr, "Invalid index on routing.\n");
        exit(errno);
    }

    // Take the vertices from the graph.
    Vertex *u, *v;
    u = cube->g->vertices[u_index];
    v = cube->g->vertices[v_index];

    // Routing register
    RoutingReg *reg;
    reg = cube->last_reg; // Se supone inicializado

    // Work out the steps to take in all dims of the mesh.
    for (int coordinate_index = 0; coordinate_index < u->n_dims; coordinate_index++)
    {
        // In hex, 0 = 0b00000000 and 1 = 0b00000001. So, use bitwise XOR.
        reg->register_[coordinate_index] = (v->coordinates[coordinate_index]) ^ (u->coordinates[coordinate_index]);
    }
}

/**
 * @brief Define a k-ary n-cube vertex coordinates.
 * Encode the coordinates of each vertex using the
 * features o the cube.
 *
 * @param cube The cube to be defined.
 */
void encode_coordinates(k_ary_n_cube *cube)
{
    int index, vertex_index;
    unsigned long rest;

    Vertex *v;
    Vertex **vertices = cube->g->vertices;
    unsigned long n_dims = vertices[0]->n_dims;
    unsigned long n_vertex = cube->g->n_vertex;

    unsigned long k = cube->k;

    for (vertex_index = 0; vertex_index < n_vertex; vertex_index++)
    {
        // We have to convert an integer to a binary string
        v = vertices[vertex_index];

        // So, we will use binary decomposition.
        index = n_dims - 1;
        rest = vertex_index;
        while ((index >= 0) && (rest >= 0))
        {
            // Division and modulus: 17 // 2 = 8; 17 % 2 = 1
            v->coordinates[index] = rest % k;
            rest /= k;
            index--;
        }
    }

    // Coordinate bug: due to unsigned char rest overflow.
    // Sol.: allocate more space --> unsigned long rest.
    // v = vertices[751];

    // So, we will use binary decomposition.
    // index = n_dims-1;
    // rest = 751;
    // printf("vertex=%d\n", rest);
    // while ((index >= 0) && (rest >= 0))
    // {
    // Division and modulus: 17 // 2 = 8; 17 % 2 = 1
    //     printf("k = %ld; (rest//k = (%ld, %ld) (index = %d)\n", k, rest / k, rest % k, index);
    //     rest = rest / k;
    //     index--;
    // }
    // printf("\n");

    // printf("Saved = [ ");
    // for (int j = 0; j < n_dims; j++)
    // {
    //     printf("%ld ", v->coordinates[j]);
    // }
    // printf("]\n");
}

/**
 * @brief Decode the coordinates of a vertex into an index.
 *
 * @param v The vertex to be decoded.
 * @param basis
 * @return unsigned long
 */
unsigned long decode_coordinates(Vertex *v, unsigned long basis)
{
    unsigned long index_from_coord = 0;
    unsigned long pow_k = 1;
    for (int index = v->n_dims - 1; index >= 0; index--)
    {
        index_from_coord += (pow_k * (v->coordinates[index]));
        pow_k *= basis;
    }
    v->index = index_from_coord; // Save the new index into the vertex.
    return index_from_coord;
}