#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Boolean primitive in C */
#define True 1
#define False 0
typedef unsigned char bool;

/* Vertex structure */
struct Vertex
{
    long *coordinates;
    unsigned long n_dims;
} typedef Vertex;

/**
 * @brief Define a vertex, as a set of coordinates.
 *
 * @param v The vertex to be defined.
 * @param n_dims The number of dimensions of the coordinates.
 */
void define_vertex(Vertex *v, unsigned long n_dims)
{
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

/* Graph structure */
struct Graph
{
    int n_vertex;
    Vertex **vertices;
    uint **edges;
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
void define_graph(Graph *g, uint n_vertex, uint n_dims)
{
    int vertex;

    // Define the number of vertex.
    g->n_vertex = n_vertex;

    // Allocate mem. for a vertex array.
    // Allocate mem. and init. to 0 the adjacency matrix.
    g->vertices = (Vertex **)malloc(n_vertex * sizeof(Vertex *));
    g->edges = (uint **)malloc(n_vertex * sizeof(uint *));
    for (vertex = 0; vertex < n_vertex; vertex++)
    {
        // Declare and init. each vertex.
        g->vertices[vertex] = (Vertex *)malloc(sizeof(Vertex));
        define_vertex(g->vertices[vertex], n_dims);

        // Allocate mem. for the adjacency matrix.
        g->edges[vertex] = (uint *)calloc(n_vertex, sizeof(uint));
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
void define_routing_reg(RoutingReg *reg, unsigned long length)
{
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

/* K-ary N-cube structure */
struct k_ary_n_cube
{
    Graph *g;
    bool has_rings;
    long n, k;
    RoutingReg *last_reg;
    void (*routing_function)(struct k_ary_n_cube *, uint, uint); // The routing function
} typedef k_ary_n_cube;

/**
 * @brief Routing function for n-dimensional mesh, with k-nodes per dim.
 *
 * @param cube A k-ary n-cube
 * @param u A vertex in the cube
 * @param v Another vertex in the cube
 */
void mesh_routing_func(k_ary_n_cube *cube, uint u_index, uint v_index)
{
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
 * @brief Define a k-ary n-cube graph.
 *  - Number of nodes: n^k (k nodes per dimension --n dims.--)
 *  - Digraph (Directed graph).
 *
 * @param cube K-ary n-cube graph.
 */
void define_kary_ncube(k_ary_n_cube *cube)
{
    uint n_dims, k, n_vertex;
    bool has_rings;

    uint scanf_buffer; // Output of scanf, to be saved.

    // Define number of dimensions
    printf("n = ");
    scanf_buffer = scanf("%d", &n_dims);

    // Define the number k
    printf("k = ");
    scanf_buffer = scanf("%d", &k);

    // Number of vertex:
    n_vertex = powl(k, n_dims);

    // Has rings? (is it a torus?)
    printf("Has rings? = ");
    scanf_buffer = scanf("%hhd", &has_rings);

    /* Allocate memory for the structure */
    cube->n = n_dims;
    cube->k = k;
    cube->g = (Graph *)malloc(sizeof(Graph));
    define_graph(cube->g, n_vertex, n_dims);

    /* Allocate memory for the register */
    cube->last_reg = (RoutingReg *)malloc(sizeof(RoutingReg));
    define_routing_reg(cube->last_reg, n_dims);

    // Decide whether it's a hypercube (k == 2 and no rings)
    // a torus (k >= 2 and has rings) or a mesh (k >= 2 and no rings).
    // Define the edges to be set and the *routing function*.
    printf("%d-ary %d-", k, n_dims);
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
    printf(": %d nodes in total\n", n_vertex);
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

int main()
{
    k_ary_n_cube *cube;

    // Initialise the k-ary n-cube
    cube = (k_ary_n_cube *)malloc(sizeof(k_ary_n_cube));
    define_kary_ncube(cube);

    // Remove the k-ary n-cube
    free_kary_ncube(&cube);
    return 0;
}