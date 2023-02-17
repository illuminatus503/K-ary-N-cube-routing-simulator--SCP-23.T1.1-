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

/* Graph structure */
struct Graph
{
    int n_vertex;
    Vertex **vertices;
    unsigned long **edges;
} typedef Graph;

/**
 * @brief Initialise a Graph structure.
 * Define the number of vertex and the adjacency matrix.
 *
 * @param g The graph structure to be initialised.
 * @param n_vertex The number of vertex of the graph.
 * @param n_dims The number of dimensions in which the graph
 * will be defined.
 */
void define_graph(Graph *g, unsigned long n_vertex, unsigned long n_dims);

/**
 * @brief Free a graph structure;
 *
 * @param g A pointer to the graph structure to be freed.
 */
void free_graph(Graph **g);

/**
 * @brief Print the index and coordinates of a vertex.
 *
 *  FORMAT: %index% [ coord., sep. by spaces ]
 *
 * @param g The graph to take the coordinates from.
 * @param u_index The index of the vertex in the graph.
 */
void print_vertex(Graph *g, unsigned long u_index);

#endif