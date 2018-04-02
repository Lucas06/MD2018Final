#include <stdlib.h>
#include <stdbool.h>


typedef uint32_t u32;
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
typedef struct _QueueSt_t {
    u32 capacity;           /* total available slots */
    u32 front;              /* index of first element */
    u32 rear;               /* Index of last element */
    Vertice **elements;   /* array of elements */
} Queue;


Queue* Queue_init(u32 max_elements) {

    Queue* Q;
    Q = (Queue *)malloc(sizeof(Queue));
    /* Initialise its properties */
    Q->elements = malloc(max_elements * sizeof(Vertice*));
    Q->capacity = max_elements;
    Q->front = 0;
    Q->rear = 0;

    return Q;
}

void Dequeue(Queue *Q) {

    /* Removing an element is equivalent to incrementing index of front by one */

    Q->front++;
    /* As we fill elements in circular fashion */
    if(Q->front == Q->capacity)
        Q->front = 0;
}
//saqueeeeee puntero Vertice*
Vertice Queue_front(Queue *Q) {
    /* Return the element which is at the front */
    return Q->elements[Q->front];
}

void Enqueue(Queue *Q, Vertice *element) {

    /* Insert the element in its rear side */ 
    Q->elements[Q->rear] = element;
    Q->rear = Q->rear + 1;
    /* As we fill the queue in circular fashion */
    if(Q->rear == Q->capacity)
        Q->rear = 0;
}

bool Queue_is_empty(Queue *Q) {
    return (Q->front == Q->rear ? true : false);
}

void Queue_free(Queue *Q) {
    free(Q->elements);
    free(Q);
    Q = NULL;
}

