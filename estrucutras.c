#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef uint32_t u32;

typedef struct _Grafo_t *Grafo;
typedef struct _Vertice_t *Vertice;
typedef struct _Lado_t *lado;

struct _Grafo_t {
	u32	n_vertices;
	u32 m_aristas;
	u32 coloreo;
	Vertice vertice_array[]; // ver si podemos usar aca el valor de n_vertices
	Vertice orden_array[];
};

struct _Vertice_t {
	u32 nombre_real;
	u32 etiqueta;
	u32 color;
	u32 valencia;
	Vertice vecinos_array[];
};

struct _Lado_t {
   u32 vertice_u;
   u32 vertice_v;
}


Grafo ConstruccionDelGrafo() {
	/* pedido de memoria para el grado */
	Grafo mi_grafo = calloc(1, sizeof(struct _Grafo_t));
	mi_grafo->nro_vertices = 0;
	mi_grafo->nro_aristas = 0;
	mi_grafo->nro_coloreo = 0;

	/* empezamos a leer el grafo por stdin */

	char linea[BUFFER_SIZE];
	u32 n_vertices, m_lados = 0;
    u32 vertice_u, vertice_v = 0;
    while(fgets(linea, sizeof(linea), stdin)) {
         if(strncmp(linea, "c", 1)) {
            if(!strncmp(linea, "p edge ", 7)) {
               sscanf(linea, "%*c %*s %u %u", &n_vertices, &m_lados);
               break;
            }
         }
      }
    if (n_vertices && m_lados) {

	printf("Esto es una prueba y he leido correctamente el numero de vertices y el numero de lados \n");
	mi_grafo->nro_vertices = n_vertices;
	mi_grafo->nro_aristas = m_lados;

	printf("Este grafo tiene %u vertices \n", mi_grafo->nro_vertices);
	printf("Este grafo tiene %u aristas \n", mi_grafo->nro_aristas);
	printf("Este grafo tiene %u colores \n", mi_grafo->nro_coloreo);

}