#ifndef __TOPOLOGY__
#define __TOPOLOGY__

#include "graph.h"

/* Boolean primitive in C */
#define True 1
#define False 0
typedef unsigned char bool;

/* Routing Register structure */
struct RoutingReg
{
    long *register_; // Instead of only accepting positive coordinates,
                     // also accepts negative coordinates (step in another dir.).
    unsigned long length;
} typedef RoutingReg;

/**
 * @brief Define a routing register, from parameters.
 *
 * @param reg The register to be initialised.
 * @param length The length of the register.
 */
void define_routing_reg(RoutingReg *reg, unsigned long length);

/**
 * @brief Free routing register.
 *
 * @param reg A pointer to a Routing register structure.
 */
void free_routing_reg(RoutingReg **reg);

/* K-ary N-cube structure */
struct k_ary_n_cube
{
    PartialGraph *g;
    bool has_rings;
    long n, k;
    RoutingReg *last_reg;
    void (*routing_function)(struct k_ary_n_cube *, uint, uint); // The routing function
} typedef k_ary_n_cube;

/**
 * @brief Define a k-ary n-cube graph.
 *  - Number of nodes: n^k (k nodes per dimension --n dims.--)
 *  - Digraph (Directed graph).
 *
 * @param cube K-ary n-cube graph.
 */
void define_kary_ncube(k_ary_n_cube *cube);

/**
 * @brief Free the k-ary n-cube structure.
 *
 * @param cube The cube to be freed.
 */
void free_kary_ncube(k_ary_n_cube **cube);

/**
 * @brief Routing from one vertex, to another.
 *
 * @param cube A K-ary N-cube.
 * @param u_index The index of the source node.
 * @param v_index The index of the destination node.
 */
void routing_from(k_ary_n_cube *cube, uint u_index, uint v_index);

/**
 * @brief Routing function for n-dimensional mesh, with k-nodes per dim.
 *
 * @param cube A k-ary n-cube
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void mesh_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index);

/**
 * @brief Routing function for n-dimensional torus, with k-nodes per dim.
 *
 * @param cube A k-ary n-cube
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void torus_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index);

/**
 * @brief Routing function for n-dimensional hypercube.
 *
 * @param cube A n-hypercube.
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void hypercube_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index);

/**
 * @brief Define a k-ary n-cube vertex coordinates.
 * Encode the coordinates of each vertex using the
 * features o the cube.
 *
 * @param cube The cube to be defined.
 */
void encode_coordinates(k_ary_n_cube *cube);

/**
 * @brief Decode the coordinates of a vertex into an index.
 *
 * @param v The vertex to be decoded.
 * @param basis
 * @return unsigned long
 */
unsigned long decode_coordinates(Vertex *v, unsigned long basis);

#endif