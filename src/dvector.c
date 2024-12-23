/*//////////////////////////////////////
Name: Alon Weinberg                    /
Reviewer:                              /
last date updated: 06/2/24             /
File type: header file                 /
//////////////////////////////////////*/ 

#include "../inc/dvector.h"
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#define SUCCESS (0)
#define FAILURE (-1)
#define GROWTH_FACTOR (2)

struct dvector_t
{
    size_t capacity;
    size_t element_size;
    size_t size;
    void *elements;
};

dvector_t *DvectorCreate(size_t capacity , size_t element_size)
{
     dvector_t* dvector = (dvector_t*)malloc(sizeof(dvector_t));
     
     assert(0 != element_size);
     assert(0 != capacity);
     if (NULL != dvector)
     {
          dvector->capacity = capacity;
          dvector->element_size = element_size;
          dvector->size = 0;
          dvector->elements = malloc(dvector->capacity * dvector->element_size);
          if(NULL == dvector->elements)
          {
               free(dvector);
               return NULL;
          }
          return dvector;
     }
     else
     {
         return NULL;
     }
}


void DvectorDestroy(dvector_t* dvector)
{
     assert(NULL != dvector); 

     free(dvector->elements);
     free(dvector);
     dvector = NULL;
}


void *DvectorGetAccessToElement(const dvector_t *dvector, size_t idx)
{
     assert(NULL != dvector);
     assert(idx < dvector->size);
     if (idx < dvector->size)
     {
         return (char *)dvector->elements + (idx * dvector->element_size);
     }
     return NULL;
}


int DvectorPushBack(dvector_t *dvector, const void *data)
{
     dvector_t* tmp = NULL;
     void* push_adress = NULL;
     dvector->size++;
     assert(NULL != dvector);
     if (dvector->size >= dvector->capacity)
     {
          dvector->capacity *= GROWTH_FACTOR;
          tmp = realloc(dvector->elements, dvector->capacity * dvector->element_size);
          if (NULL == tmp)
          {
               dvector->capacity /= GROWTH_FACTOR;
               dvector->size--;
               return FAILURE;
          }
          else
          {
               dvector->elements = tmp;
          }
     }
     push_adress = (char *)dvector->elements + ((dvector->size - 1) * dvector->element_size);
     memcpy(push_adress, data, dvector->element_size);
     return SUCCESS;
}


void DvectorPopBack(dvector_t *dvector)
{
     dvector_t* tmp = NULL;
     dvector->size--;
     assert(NULL != dvector);
     if (dvector->size <= 0.25 * dvector->capacity)
     {
          dvector->capacity /= GROWTH_FACTOR;
          tmp = realloc(dvector->elements, dvector->capacity * dvector->element_size);
          if (NULL == tmp)
          {
               dvector->capacity *= GROWTH_FACTOR;
               dvector->size++;
          }
          else
          {
               dvector->elements = tmp;
          }
     }     
}


size_t DvectorSize(const dvector_t *dvector)
{
      assert(NULL != dvector);
     
      return dvector->size;
}

 
size_t DvectorCapacity(const dvector_t *dvector)
{
      assert(NULL != dvector);
      return dvector->capacity;
}


int DvectorReserve(dvector_t *dvector, size_t capacity)
{
      dvector_t* tmp = NULL;
      assert(NULL != dvector);
      if (capacity < dvector->size)
      {
           capacity = dvector->size;
      }
      tmp = realloc(dvector->elements, capacity * dvector->element_size);
      if (NULL == tmp)
      {
           return FAILURE;
      }
      else
      {
           dvector->elements = tmp;
           dvector->capacity = capacity;
           return SUCCESS;
      }
}

int DvectorShrink(dvector_t *dvector)
{
      dvector_t* tmp = NULL;
      assert(NULL != dvector);
      tmp = realloc(dvector->elements, dvector->size * GROWTH_FACTOR * dvector->element_size);
      if (NULL == tmp)
      {
           return FAILURE;
      }
      else
      {
           dvector->capacity = dvector->size * GROWTH_FACTOR;
           return SUCCESS;
      }
}
