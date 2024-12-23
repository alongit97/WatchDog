/*//////////////////////////////////////
Name: Alon Weinebrg                    
Reviewer: Auri                              
last date updated: 28/3/24             
File type: source file                   
//////////////////////////////////////*/ 

#include <assert.h>
#include <stdlib.h>

#include "heap.h"
#include "dvector.h"


#define LEFT_CHILD(i) (2*(i) + 1)
#define RIGHT_CHILD(i) (2*(i) + 2)
#define PARENT(i) (((i) - 1) / 2)

struct heap 
{
    heap_cmp_func_t cmp_func;
    dvector_t *heap_container; 
};

typedef enum status_heapify 
{
    CONTINUE = 0,
    DONE
} status_heapify_t;

static void HeapifyUp(heap_t *heap, size_t idx);
static void HeapifyDown(heap_t *heap, int idx);
static void Swap(void **pointer1, void **pointer2);


heap_t *HeapCreate(heap_cmp_func_t cmp_func) 
{
    dvector_t *elements = NULL;

    heap_t *heap = malloc(sizeof(heap_t));
    if (heap == NULL) 
    {
        return NULL;
    }
    
    elements = DvectorCreate(100, sizeof(void*));
    if(elements == NULL) 
    {
        free(heap);
        return NULL;
    }

    heap->cmp_func = cmp_func;
    heap->heap_container = elements;

    return heap;
}

void HeapDestroy(heap_t *heap) 
{
    assert(heap);

    DvectorDestroy(heap->heap_container);
    free(heap);
}

int HeapIsEmpty(const heap_t *heap) 
{
    assert(heap);

    return (HeapSize(heap) == 0);
}

size_t HeapSize(const heap_t *heap) 
{
    assert(heap);

    return DvectorSize(heap->heap_container);
}

status_t HeapPush(heap_t *heap, void *data)
{
    status_t status = SUCCESS;
    
    assert(heap);

    status = (DvectorPushBack(heap->heap_container, &data) == 0) ? SUCCESS : FAILURE;
    if (status == FAILURE) 
    {
        return (FAILURE);
    }

    HeapifyUp(heap, DvectorSize(heap->heap_container) - 1);

    return (SUCCESS);
}

void *HeapPeek(const heap_t *heap) 
{
    void **element_pointer = NULL;

    assert(heap);

    if (HeapIsEmpty(heap)) 
    {
        return NULL;
    }

    element_pointer = DvectorGetAccessToElement(heap->heap_container, 0);

    return *(element_pointer);
}

void HeapPop(heap_t *heap) 
{
    void **first = DvectorGetAccessToElement(heap->heap_container, 0);
    void **last_leaf = DvectorGetAccessToElement(heap->heap_container, DvectorSize(heap->heap_container)-1);

    assert(heap);

    Swap(first, last_leaf);

    DvectorPopBack(heap->heap_container);

    HeapifyDown(heap, 0);
}

void *HeapRemove(heap_t *heap, heap_match_func_t match_func, const void *params) 
{
    size_t i = 0;
    size_t size = 0;
    int to_remove_index = -1;
    void **candidate = NULL;
    void **remove_element = NULL;
    void **last_element = NULL;
    void *data_remove = NULL;

    assert(heap);
    assert(match_func);

    size = DvectorSize(heap->heap_container);
    last_element = DvectorGetAccessToElement(heap->heap_container, size - 1); 
    remove_element = DvectorGetAccessToElement(heap->heap_container, i);

    for (i = 0; i < size; i++) 
    {
        candidate = DvectorGetAccessToElement(heap->heap_container, i);

        if (match_func(*candidate, params) == 1) 
        {
            to_remove_index = i;
            remove_element = candidate;
            break;
        }
    }

    if (to_remove_index != -1)
    {
        data_remove = *remove_element;
        Swap(last_element, remove_element);
        DvectorPopBack(heap->heap_container);

        HeapifyDown(heap, to_remove_index);
        HeapifyUp(heap, to_remove_index);
    }

    return (data_remove);
}

/******************************************************************************/

static void HeapifyUp(heap_t *heap, size_t idx)
{
    size_t parent_index = 0;
    heap_cmp_func_t compare = heap->cmp_func;
    void **child = NULL; 
    void **parent = NULL;

    assert(heap);

    while (idx > 0)
    {
        parent_index = PARENT(idx);

        child = DvectorGetAccessToElement(heap->heap_container, idx);
        parent = DvectorGetAccessToElement(heap->heap_container, parent_index);

        if (compare(*child, *parent) < 0)
        {
            Swap(child, parent);
            idx = PARENT(idx);
        }
        else
        {
            break;
        }
    }
}


static void HeapifyDown(heap_t *heap, int idx)
{
    int max_idx = DvectorSize(heap->heap_container);
    status_heapify_t heapify_status = CONTINUE;
    void **data = NULL;
    void **left_child_data = NULL;
    void **right_child_data = NULL;
    void **min_child = NULL;
    int side = 0;

    assert(heap);

    while (LEFT_CHILD(idx) < max_idx && CONTINUE == heapify_status)
    {
        side = LEFT_CHILD(idx);
        left_child_data = DvectorGetAccessToElement(heap->heap_container, LEFT_CHILD(idx));
        heapify_status = DONE;

        data = DvectorGetAccessToElement(heap->heap_container, idx);
        if (RIGHT_CHILD(idx) < max_idx)
        {
            right_child_data = DvectorGetAccessToElement(heap->heap_container, RIGHT_CHILD(idx));
            side = heap->cmp_func(*left_child_data, *right_child_data) > 0 ? RIGHT_CHILD(idx) : LEFT_CHILD(idx);
        }

        min_child = DvectorGetAccessToElement(heap->heap_container, side);

        if (heap->cmp_func(*data, *min_child) > 0)
        {
            Swap(data, min_child);
            idx = side;
            heapify_status = CONTINUE;
        }
    }
}


static void Swap(void **pointer1, void **pointer2) 
{
    void *temp = *pointer1;
    *pointer1 = *pointer2;
    *pointer2 = temp;
}
