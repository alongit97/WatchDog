#include "dlist.h" /* dlist_t */

#include <stdlib.h> /*malloc*/
#include <assert.h> /* assert */

typedef struct node node_t;

struct node
{
    void *data;
    node_t *next;
    node_t *prev;
};

struct dlist
{
    node_t head;
    node_t tail;
};

enum
{
	SUCCESS = 0,
	FAILURE,
	NOT_FOUND 
};

static node_t* IterToNode(dlist_iter_t iter);
static dlist_iter_t NodeToIter(node_t* node);
static node_t* NodeCreate(void* data, void* next, void* prev);
static int ActionFunc(void *data, void *add_factor);
static int ISWhereInRange(dlist_iter_t from, dlist_iter_t to , dlist_iter_t where);


dlist_t *DListCreate()
{
    
    dlist_t* list = (dlist_t*)malloc(sizeof(dlist_t));
    if (NULL == list)
    {
        return NULL;
    }
    
    list->head.next = &(list->tail);
    list->head.prev = NULL;
    
    list->tail.prev = &(list->head);
    list->tail.next = NULL;

    return (list);
}

void DListDestroy(dlist_t *list)
{
    dlist_iter_t current = NodeToIter(list->head.next);
    dlist_iter_t next = current;
    
    assert(NULL != list);
    
    while (current != DListEnd(list))
    {
         next = current->next;
         free(current);
         current = next;
    }

    free(list);
}

dlist_iter_t DListBegin(const dlist_t *list)
{
    assert(NULL != list);
    
    return NodeToIter(list->head.next);
}


dlist_iter_t DListEnd(const dlist_t *list)
{
    assert(NULL != list);
    
    return NodeToIter((node_t*)&(list->tail));
}


dlist_iter_t DListNext(dlist_iter_t iter)
{
    assert(NULL != iter);
    
    return NodeToIter(IterToNode(iter)->next);
}

dlist_iter_t DListPrev(dlist_iter_t iter)
{
    assert(NULL != iter);
    
    return NodeToIter(IterToNode(iter)->prev);
}

int DListIsIterSame(dlist_iter_t element1, dlist_iter_t element2)
{
    assert(NULL != IterToNode(element1));
    assert(NULL != IterToNode(element2));
    
    return (IterToNode(element1) == IterToNode(element2));
}

dlist_iter_t DListInsert(dlist_iter_t where, void* data)
{
     node_t* new_node = NULL;
     
     assert(NULL != where);
     assert(NULL != data);

     new_node = NodeCreate(data, where, where->prev);
     if (NULL == new_node)
     {
         while (NULL != where->next)
         {
              where = where->next;
         }
         return where;
     }
     
     where->prev->next = new_node;
     where->prev = new_node;
     
     return new_node;
}

dlist_iter_t DListRemove(dlist_iter_t to_remove)
{
     dlist_iter_t tmp = to_remove->next;
     
     assert(NULL != to_remove);
     
     to_remove->prev->next = tmp;
     tmp->prev = to_remove->prev;
     free(to_remove);
     
     return tmp;
}

void *DListGetData(const dlist_iter_t iter)
{
     assert(NULL != iter);
     
     return (IterToNode(iter)->data);
}

void DListSetData(dlist_iter_t iter, void *data)
{
     assert(NULL != iter);
     assert(NULL != data);
     
     IterToNode(iter)->data = data;
}

int DListIsEmpty(const dlist_t *list)
{
     assert(NULL != list);
     return (DListIsIterSame(DListBegin(list), DListEnd(list)));
}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, 
                       match_func_t is_match, void *param)
{
     dlist_iter_t runner = from;
     
     assert(NULL != IterToNode(from));
     assert(NULL != IterToNode(to));     
     assert(is_match);
     
     while (!DListIsIterSame(runner, to) && 
     	    !is_match(IterToNode(runner)->data, param))
     {
          runner = DListNext(runner);
     }
     return (runner);
}

int DListForEach(dlist_iter_t from, dlist_iter_t to,
                  action_func_t action, void* param)
{
    dlist_iter_t runner = from;
    int result = SUCCESS;
    
    assert(NULL != IterToNode(from));
    assert(NULL != IterToNode(to));
    assert(action);
    
    while (!DListIsIterSame(runner, to) && SUCCESS == result)
    {
        result = action(runner->data, param);
        runner = DListNext(runner);
    }
    
    return (result);
}

size_t DListCount(const dlist_t *list)
{
    size_t counter = 0;
    dlist_iter_t start = DListBegin(list);
    dlist_iter_t end = DListEnd(list);
    
    assert(NULL != list);
    
    DListForEach(start, end, &ActionFunc, &counter);
    return (counter);
}

void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
     node_t* tmp = where->prev;
     
     assert(ISWhereInRange(from, to, where));

     to->prev->next = where;
     where->prev = to->prev;
     
     from->prev->next = to;
     to->prev = from->prev;
     
     tmp->next = from;
     from->prev = tmp;
}

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, match_func_t is_match, 
	                                            void *param, dlist_t *found_elements)
{
    dlist_iter_t runner = from;
    int result = NOT_FOUND;
    dlist_iter_t status;

    while (!DListIsIterSame(runner ,to))
    {
    	if (is_match(DListGetData(runner), param))
    	{
    	    status = DListPushBack(found_elements, DListGetData(runner));    	    
    	    if (DListIsIterSame(status, DListEnd(found_elements)))
    	    {
    	         return FAILURE;
    	    }
    	    
    	    result = SUCCESS;
    	}
    	
    	runner = DListNext(runner);
    }
    
    return result;
}

dlist_iter_t DListPushBack(dlist_t *list, void *data)
{
    assert(list);
    assert(data);

    return DListInsert(DListEnd(list), data);
}

void *DListPopBack(dlist_t *list)
{
    void *data = DListGetData(DListEnd(list)->prev);
    
    assert(list);
    
    DListRemove(DListEnd(list)->prev);
    
    return data;
}


dlist_iter_t DListPushFront(dlist_t *list, void *data)
{
    assert(list);
    assert(data);

    return DListInsert(DListBegin(list), data);

}

void *DListPopFront(dlist_t *list)
{
    void *data = DListGetData(DListBegin(list));
    
    assert(list);
    
    DListRemove(DListBegin(list));
    
    return data;
}


/*////////////////////////////////////////////////*/

dlist_iter_t NodeToIter(node_t* node)
{
     return ((dlist_iter_t)node);
}

node_t* IterToNode(dlist_iter_t iter)
{
     return ((node_t*)iter);
}

node_t* NodeCreate(void* data, void* next, void* prev)
{
     node_t* node = NULL;
     
     node = (node_t*)malloc(sizeof(dlist_t));
     if (NULL == node)
     {
          return NULL;
     }
     
     node->data = data;
     node->next = next;
     node->prev = prev;
     
     return (node);
}

int ActionFunc(void *data, void *add_factor)
{
    (void)data;
    *(int*)add_factor += 1;
    return (0);
}

int ISWhereInRange(dlist_iter_t from, dlist_iter_t to , dlist_iter_t where)
{
     dlist_iter_t current = from;
     while (current != to)
     {
         if (current == where)
         {
             return 0;
         }
     current = current->next;    
     }
     
     return 1;
}

dlist_iter_t GetToEnd(dlist_iter_t where)
{
	while (NULL != where->next)
         {
              where = where->next;
         }
    return where;
}
