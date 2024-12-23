#ifndef OL_155_6_HEAP
#define OL_155_6_HEAP

#include <stddef.h> /* size_t */

typedef struct heap heap_t;

typedef int(*heap_cmp_func_t)(const void *data, const void *params);
typedef int(*heap_match_func_t)(const void *data, const void *params);

typedef enum status
{
    SUCCESS = 0,
    FAILURE
} status_t;

heap_t *HeapCreate(heap_cmp_func_t cmp_func); /* O(1) */ 
void HeapDestroy(heap_t *heap);  /* O(n) */ 
status_t HeapPush(heap_t *heap, void *data);  /* O(logn)   */ 
void HeapPop(heap_t *heap); /*O(1) */
void *HeapPeek(const heap_t *heap);  /* O(1) */
void *HeapRemove(heap_t *heap, heap_match_func_t match_func, const void *params); /* O(logn)  */ 
int HeapIsEmpty(const heap_t *heap); /* O(n) */
size_t HeapSize(const heap_t *heap); /* O(n) */


#endif /* OL_155_6_HEAP */