/****************************
Alon Weinberg               *
29/02/2024                  *
                            *
source file                 *
exercise: pqueue            *
*****************************/


#include <stddef.h> /* size_t */

/* Function pointer type for comparing elements in the priority queue */
typedef int (*cmp_func_t)(const void *data, const void *param);

/* Function pointer type for matching elements in the priority queue */
typedef int (*match_func_t)(const void *data, void *param);

/* Struct for representing a priority queue */
typedef struct pq pq_t;

/**
 * Function: PQCreate
 * ------------------
 * Creates a new priority queue.
 * 
 * cmp_func: Pointer to a comparison function used for ordering elements in the priority queue.
 * 
 * Returns: A pointer to the newly created priority queue.
 * 
 * Complexity: O(1)
 * 
 * Notes: The comparison function must return an integer less than, equal to, or greater than zero if the first argument is considered to be respectively less than, equal to, or greater than the second.
 */
pq_t *PQCreate(cmp_func_t cmp_func);

/**
 * Function: PQDestroy
 * -------------------
 * Destroys a priority queue, freeing all allocated memory.
 * 
 * pq: Pointer to the priority queue to be destroyed.
 * 
 * Complexity: O(n)

 */
void PQDestroy(pq_t *pq);

/**
 * Function: PQEnqueue
 * --------------------
 * Enqueues an element into the priority queue.
 * 
 * pq: Pointer to the priority queue.
 * data: Pointer to the data to be enqueued.
 * 
 * Returns: 1 if successful, 0 otherwise.
 * 
 * Complexity: O(n)
 */
int PQEnqueue(pq_t *pq, void *data);

/**
 * Function: PQDequeue
 * --------------------
 * Dequeues the element with the highest priority from the priority queue.
 * 
 * pq: Pointer to the priority queue.
 * 
 * Returns: A pointer to the dequeued data.
 * 
 * Complexity: O(1)
 */
void *PQDequeue(pq_t *pq);

/**
 * Function: PQPeek
 * -----------------
 * Retrieves the data of the element with the highest priority from the priority queue without removing it.
 * 
 * pq: Pointer to the priority queue.
 * 
 * Returns: A pointer to the data of the element with the highest priority.
 * 
 * Complexity: O(1)
 */
void *PQPeek(const pq_t *pq);

/**
 * Function: PQIsEmpty
 * --------------------
 * Checks if the priority queue is empty.
 * 
 * pq: Pointer to the priority queue.
 * 
 * Returns: 1 if the priority queue is empty, 0 otherwise.
 * 
 * Complexity: O(1)
 */
int PQIsEmpty(const pq_t *pq);

/**
 * Function: PQCount
 * ------------------
 * Counts the number of elements in the priority queue.
 * 
 * pq: Pointer to the priority queue.
 * 
 * Returns: The number of elements in the priority queue.
 * 
 * Complexity: O(n)
 */
size_t PQCount(const pq_t *pq);

/**
 * Function: PQErase
 * ------------------
 * Removes an element from the priority queue that matches the given criteria.
 * 
 * pq: Pointer to the priority queue.
 * match_func: Pointer to a matching function used to identify the element to be erased.
 * param: Parameter to be passed to the matching function, can be NULL.
 * 
 * Returns: A pointer to the data of the erased element, or NULL if no matching element is found.
 * 
 * Complexity: O(n)
 */
void *PQErase(pq_t *pq, match_func_t match_func, void *param);

/**
 * Function: PQClear
 * ------------------
 * Removes all elements from the priority queue.
 * 
 * pq: Pointer to the priority queue.
 * 
 * Returns: A pointer to the cleared priority queue.
 * 
 * Complexity: O(n)
 */
void PQClear(pq_t *pq);

