#ifndef __GRAPH_STR__
#define __GRAPH_STR__

/* Vertex structure */
struct Vertex
{
    unsigned long index;
    long *coordinates;
    unsigned long n_dims;
} typedef Vertex;

/**
 * @brief Define a vertex, as a set of coordinates.
 *
 * @param v The vertex to be defined.
 * @param n_dims The number of dimensions of the coordinates.
 */
void define_vertex(Vertex *v, unsigned long index, unsigned long n_dims);

/**
 * @brief Free the vertex structure.
 *
 * @param v A vertex pointer to be freed.
 */
void free_vertex(Vertex **v);

/**
 * @brief Clone a vertex.
 *
 * @param v Original copy.
 * @param u New copy.
 */
void clone_vertex(Vertex *v, Vertex *u);

/* PartialGraph structure */
struct PartialGraph
{
    int n_vertex;
    Vertex **vertices;
} typedef PartialGraph;

/**
 * @brief Initialise a PartialGraph structure.
 * Define the number of vertex and the adjacency matrix.
 *
 * @param g The PartialGraph structure to be initialised.
 * @param n_vertex The number of vertex of the PartialGraph.
 * @param n_dims The number of dimensions in which the PartialGraph
 * will be defined.
 */
void define_graph(PartialGraph *g, unsigned long n_vertex, unsigned long n_dims);

/**
 * @brief Free a PartialGraph structure;
 *
 * @param g A pointer to the PartialGraph structure to be freed.
 */
void free_graph(PartialGraph **g);

/**
 * @brief Print the index and coordinates of a vertex.
 *
 *  FORMAT: %index% [ coord., sep. by spaces ]
 *
 * @param g The PartialGraph to take the coordinates from.
 * @param u_index The index of the vertex in the PartialGraph.
 */
void print_vertex(PartialGraph *g, unsigned long u_index);

#endif