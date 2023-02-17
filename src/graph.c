#include <stdlib.h>
#include <stdio.h>

#include "../include/graph.h"

/*! VERTEX STRUCTURE -- INIT !*/

/**
 * @brief Define a vertex, as a set of coordinates.
 *
 * @param v The vertex to be defined.
 * @param n_dims The number of dimensions of the coordinates.
 */
void define_vertex(Vertex *v, unsigned long index, unsigned long n_dims)
{
    v->index = index;                                      // Set the index of the vertex.
    v->n_dims = n_dims;                                    // Set the number of dimensions of the coordinates.
    v->coordinates = (long *)calloc(n_dims, sizeof(long)); // Allocate space for coordinates.
}

/**
 * @brief Free the vertex structure.
 *
 * @param v A vertex pointer to be freed.
 */
void free_vertex(Vertex **v)
{
    free((*v)->coordinates);
    free(*v);
}

/**
 * @brief Clone a vertex.
 *
 * @param v Original copy.
 * @param u New copy.
 */
void clone_vertex(Vertex *v, Vertex *u)
{
    u->index = v->index;
    u->n_dims = v->n_dims;
    for (int index = 0; index < v->n_dims; index++)
    {
        u->coordinates[index] = v->coordinates[index];
    }
}

void print_vertex(Graph *g, unsigned long u_index)
{
    int coord_index;
    Vertex *v = g->vertices[u_index];

    printf("%ld [ ", u_index);
    for (coord_index = 0; coord_index < v->n_dims; coord_index++)
    {
        printf("%ld ", v->coordinates[coord_index]);
    }
    printf("]");
}

/*! GRAPH STRUCTURE -- INIT !*/

/**
 * @brief Initialise a Graph structure.
 * Define the number of vertex and the adjacency matrix.
 *
 * @param g The graph structure to be initialised.
 * @param n_vertex The number of vertex of the graph.
 * @param n_dims The number of dimensions in which the graph
 * will be defined.
 */
void define_graph(Graph *g, unsigned long n_vertex, unsigned long n_dims)
{
    unsigned long vertex_index;

    // Define the number of vertex.
    g->n_vertex = n_vertex;

    // Allocate mem. for a vertex array.
    // Allocate mem. and init. to 0 the adjacency matrix.
    g->vertices = (Vertex **)malloc(n_vertex * sizeof(Vertex *));
    g->edges = (unsigned long **)malloc(n_vertex * sizeof(unsigned long *));
    for (vertex_index = 0; vertex_index < n_vertex; vertex_index++)
    {
        // Declare and init. each vertex.
        g->vertices[vertex_index] = (Vertex *)malloc(sizeof(Vertex));
        define_vertex(g->vertices[vertex_index], vertex_index, n_dims);

        // Allocate mem. for the adjacency matrix.
        g->edges[vertex_index] = (unsigned long *)calloc(n_vertex, sizeof(unsigned long));
    }
}

/**
 * @brief Free a graph structure;
 *
 * @param g A pointer to the graph structure to be freed.
 */
void free_graph(Graph **g)
{
    for (int vertex = 0; vertex < (*g)->n_vertex; vertex++)
    {
        free((*g)->edges[vertex]);
        free_vertex(&((*g)->vertices[vertex]));
    }
    free((*g)->vertices);
    free((*g)->edges);
    free(*g);
}