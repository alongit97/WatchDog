/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
last date updated: 28/3/24             
File type: source file                   
//////////////////////////////////////*/ 
#include <stdlib.h> /*malloc*/
#include <assert.h>

#include "heap.h" /*heap_t*/

typedef struct pq
{
    heap_t *heap;
} pq_t;   

pq_t *PQCreate(heap_cmp_func_t cmp_func)
{
    pq_t *pq = {NULL};

    assert(cmp_func);

    pq = (pq_t*)malloc(sizeof(pq_t));
    if (NULL == pq)
    {
        return NULL;
    }

    pq->heap = HeapCreate(cmp_func);
    if (NULL == pq->heap)
    {
        free(pq);
        
        return NULL;
    }

    return (pq);
}

void PQDestroy(pq_t *pq)
{
    assert(pq);

    HeapDestroy(pq->heap);
    pq->heap = NULL;

    free(pq);
}

int PQEnqueue(pq_t *pq, void *data)
{
    assert(pq);

    return (HeapPush(pq->heap, data));
}

void *PQDequeue(pq_t *pq)
{
    void *data = NULL;

    assert(pq);

    data = HeapPeek(pq->heap);
    HeapPop(pq->heap);

    return (data);
}

void *PQPeek(const pq_t *pq)
{
    assert(pq);

    return (HeapPeek(pq->heap));
}

int PQIsEmpty(const pq_t *pq)
{
    assert(pq);

    return (HeapIsEmpty(pq->heap));
}

size_t PQCount(const pq_t *pq)
{
    assert(pq);

    return (HeapSize(pq->heap));
}

void *PQErase(pq_t *pq, heap_match_func_t match_func, void *param)
{
    assert(pq);
    assert(match_func);

    return (HeapRemove(pq->heap, match_func, param));
}

void PQClear(pq_t *pq)
{
    assert(pq);

    while (!PQIsEmpty(pq))
    {
        PQDequeue(pq);
    }
}
