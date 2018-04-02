#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

//include "proyecto.h"

#define BUFFER_SIZE 1024
typedef uint32_t u32;
typedef uint32_t size32;

/* Definiendo estructuras */
typedef struct lado_t {
   u32 vertice_u;
   u32 vertice_v;
} lado;

struct _Vecindad_t {
	u32 vertice;
	u32 grado;
	u32 color;
	u32 *puntero;
};

typedef struct _Vecindad_t Vecindad;

struct _Vertice_t {
	u32 nombre_real;
	u32 etiqueta;
	u32 color;
	u32 grado;
	Vecindad *vecinos;
};

typedef struct _Vertice_t *Vertice;

struct _Grafo_t
{
	u32 nro_vertices;
	u32 nro_aristas;
	u32 nro_colores;
	//Vertice *vertice_grafo;
	//Vertice *vertice_array[];
	Vertice vertice_array[];
};

typedef struct _Grafo_t *Grafo; /* *Grafo será un puntero a la estructura struct _Grafo_t */

/* Funcion generadora de número pseudo-random */

/*
u16 lfsr = 0xACE1u;
u16 bit;

 unsigned rand()
 {
   bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
   return lfsr =  (lfsr >> 1) | (bit << 15);
 }
*/
/* Funciones de comparación */

int comparar_vertices_u(const void* a, const void* b) {
      /* Casteo argumentos a punteros a la estructura 'lado'. */
      lado* lado_a = (lado*) a;
      lado* lado_b = (lado*) b;
      /* resultado = 0 -> iguales. */
      /* resultado < 0 -> lado_a es mas chico. */
      /* resultado > 0 -> lado_a es mas grande. */
      int resultado = (lado_a -> vertice_u) - (lado_b -> vertice_u);

      return(resultado);
}

int comparar_vertices_v(const void* a, const void* b) {
      /* Casteo argumentos a punteros a la estructura 'lado'. */
      lado* lado_a = (lado*) a;
      lado* lado_b = (lado*) b;
      /* resultado = 0 -> iguales. */
      /* resultado < 0 -> lado_a es mas chico. */
      /* resultado > 0 -> lado_a es mas grande. */
      int resultado = (lado_a -> vertice_v) - (lado_b -> vertice_v);

      return(resultado);
}

/* Fin funciones de comparación */

/* ------------------------------------------------------------------------------------------------------------- */

/* Carga del grafo */

/* Alloca memoria para un Nuevo Grado leido por entrada estándar */

Grafo ConstruccionDelGrafo() {

	/* pedido de memoria para el grado */
	Grafo mi_grafo = calloc(1, sizeof(struct _Grafo_t));
	mi_grafo->nro_vertices = 0;
	mi_grafo->nro_aristas = 0;
	mi_grafo->nro_colores = 0;

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
	printf("Este grafo tiene %u colores \n", mi_grafo->nro_colores);

	/* --------------------------------------------------------------------------------------------------- */

         u32 e_lineas = 0;

         /*
            Creamos un array de 2 * M lados donde se van a guardar las lecturas de cada lado
            de manera espejada. Es decir, tanto el lado u-v como v-u.
            La idea es tener una estructura intermedia donde se alojen los lados de manera
            parcial que nos permita ordenarlos y 'manipularlos' un poco para facilitar la
            carga de los mismos a la estructura final.///////////////////////###########
         */

         size32 size_espejada = 2 * m_lados; // Longitud del arreglo de lados espejados.
         lado* lados_espejados =malloc(size_espejada*sizeof(struct lado_t)); // Estructura auxiliar.
         lado* lado_uv = NULL; // 'Lado forward'  u-v.
         lado* espejo_vu = NULL; // 'Lado backward' v-u.
         u32 indice_espejado = 0; // Indice que recorre el arreglo de lados espejados.

         Vertice vertice = malloc(n_vertices*sizeof(struct _Vertice_t)); // arreglo de vertices
         u32 indice_vertice = 0;

         while(fgets(linea, sizeof(linea), stdin)) {
            assert(e_lineas < m_lados);
            assert(indice_espejado < size_espejada);

            if(!strncmp(linea, "e", 1)) {
               sscanf(linea, "%*c %u %u", &vertice_u, &vertice_v);

               // Lado forward.
               lados_espejados[indice_espejado].vertice_u = vertice_u;
               lados_espejados[indice_espejado].vertice_v =vertice_v;
               indice_espejado += 1;

               // Lado espejado.
               lados_espejados[indice_espejado].vertice_v =vertice_u;
               lados_espejados[indice_espejado].vertice_u =vertice_v;
               indice_espejado += 1;

               e_lineas += 1;
            }
         }
         assert(e_lineas == m_lados);
         assert(indice_espejado == size_espejada);
         qsort(lados_espejados, size_espejada, sizeof(struct lado_t), comparar_vertices_u);
         size32 size_vecindad = n_vertices; //en el peor de los casos
         Vecindad* vecindad = calloc(size_vecindad, sizeof(struct _Vecindad_t));
         vecindad[0].vertice = lados_espejados[0].vertice_u;
         int j = 0;
         int etiqueta = 0;
         for(int i = 0; i < size_espejada; i++) {
            if(lados_espejados[i].vertice_u == vecindad[j].vertice) {
               vecindad[j].grado += 1;
            } else {
               j += 1;
               etiqueta += 1;
               vecindad[j].vertice = lados_espejados[i].vertice_u;
               vecindad[j].grado += 1;
            }
            vecindad[j].color = 0;
         }
         for (int i=0; i < n_vertices; i++) {
         	/* Armado de vertices */
         	vertice[i].nombre_real = vecindad[i].vertice;
         	vertice[i].etiqueta = i;
         	vertice[i].color = 0;
         	//vertice[i].grado = vecindad[i].grado;
         	vertice[i].vecinos = NULL;
         	mi_grafo->vertice_array[i] = &vertice[i]; //estoy llenando el arreglo de punteros
         	/* chequeo */
         	//printf("Hola soy el vertice con nombre real %u, etiqueta %u, color %u y grado %u\n", vertice[i].nombre_real, vertice[i].etiqueta, vertice[i].color, vertice[i].grado);
            printf("Hola soy el vertice con nombre real %u, etiqueta %u y color %u\n", vertice[i].nombre_real, vertice[i].etiqueta, vertice[i].color);//, vertice[i].grado);

         }
         printf("DEFNITIVAMENTE SON LOS VERTICES\n");
         printf("hasta aca estan las valencias\n");
         qsort(lados_espejados, size_espejada, sizeof(struct lado_t), comparar_vertices_v);
         u32 v = lados_espejados[0].vertice_v;
         int tag = 0;
         for(int i = 0; i < size_espejada; i++) {
            if(lados_espejados[i].vertice_v != v) {

               tag += 1;
               v = lados_espejados[i].vertice_v;
            }
            lados_espejados[i].vertice_v = tag;

         }
         int aux = 0;
         for(int i = 0; i < size_vecindad; i++) {
            vecindad[i].puntero = malloc(vecindad[i].grado*(sizeof(uint32_t)));
            for (int j = 0; j < vecindad[i].grado; j++) {
               vecindad[i].puntero[j] = lados_espejados[aux].vertice_u;
               aux++;
            }
         }
         for (int i = 0 ; i < size_vecindad; i++) {
            for(int j = 0 ; j < vecindad[i].grado; j++){
               printf("Vertice %u es vecino de %u\n", vecindad[i].vertice, vecindad[i].puntero[j]);
            }
         }
         free(lados_espejados); /* Destruyo los lados espejados */
         for (int i = 0; i < size_vecindad; ++i) {
               free(vecindad[i].puntero);
         }

         free(vecindad);
		 return mi_grafo;
	}
}

/* Liberación de memoria del grafo */

u32 NumeroDeVertices(Grafo G) {
	return G->nro_vertices;
}
u32 NumeroDeLados(Grafo G) {
	return G->nro_aristas;
}
u32 NumeroDeColores(Grafo G) {
	return G->nro_colores;
}

/* Funciones para extraer información de los vértices */


u32 NombreDelVertice(Grafo G, u32 i){
	return (G->vertice_array[i]->nombre_real);
}
/*
u32 ColorDelVertice(Grafo G, u32 i) {
	return (G->vertice_array[i]->color);
}
u32 GradoDelVertice(Grafo G, u32 i) {
	return (G->vertice_array[i]->grado);
}
u32 ColorJotaesimoVecino(Grafo G, u32 i, u32 j) {
	return (G->vertice_array[i]->vecinos[j].color);
}
u32 NombreJotaesimoVecino(Grafo G, u32 i,u32 j) {
	return (G->vertice_array[i]->vecinos[j].vertice);
}c
*/

/* -------------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------------------------------------------- */

int main (void) {
	Grafo mi_grafo = NULL;
	mi_grafo = ConstruccionDelGrafo();
	printf("ESTOY PROBANDO DENTRO DEL MAIN\n");
	printf("Numero de vertices de G = %u\n", NumeroDeVertices(mi_grafo));
	printf("Numero de lados de G = %u\n", NumeroDeLados(mi_grafo));
	printf("Numero de colores de G = %u\n", NumeroDeColores(mi_grafo));

	// printf("Qué paso?");

	printf("estoy probando la función NombreDelVertice con etiqueta 1 = %u\n", NombreDelVertice(mi_grafo, 1));
   // nota: se rompe cuando quiero agregar el grado para la funcion GradoDelVertice linea 180
	// printf("estoy probando la función ColorDelVertice con etiqueta 1 = %u\n", ColorDelVertice(mi_grafo, 1));
	// printf("estoy probando la función GradoDelVertice con etiqueta 1 = %u\n", GradoDelVertice(mi_grafo, 1));
	// printf("estoy probando la función ColorJotaesimoVecino con etiqueta 1 = %u\n", ColorJotaesimoVecino(mi_grafo, 1, 1));
	// printf("estoy probando la función NombreJotaesimoVecino con etiqueta 1 = %u\n", NombreJotaesimoVecino(mi_grafo, 1, 1));
	return 0;
}
