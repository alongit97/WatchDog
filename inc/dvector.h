/*//////////////////////////////////////
Name: Alon Weinberg                    /
Reviewer:                              /
last date updated: 06/2/24             /
File type: header file                 /
//////////////////////////////////////*/ 

#ifndef DVECTOR_H
#define DVECTOR_H

#include <stddef.h> /* size_t */


typedef struct dvector_t dvector_t;


/**
 * Creates a new dynamic vector with the specified capacity and element size.
 * 
 * @param: capacity The initial capacity of the dynamic vector.
 * @param: element_size The size of each element in the dynamic vector.
 * @return:A pointer to the newly created dynamic vector.
 *         NULL if memory allocation fails.
 * 
 * Time Complexity: O(1)
 */
dvector_t *DvectorCreate(size_t capacity, size_t element_size);



/**
 * Destroys a dynamic vector, freeing all allocated memory.
 * 
 * @param: vector The dynamic vector to be destroyed.
 * 
 * Time Complexity: O(1)
 */
void DvectorDestroy(dvector_t *vector);



/**
 * Gets access to the element at the specified index in the dynamic vector.
 * 
 * @param: dvector The dynamic vector.
 * @param: idx The index of the element to access.
 * @return: A pointer to the element at the specified index.
 *         Undefined if the index is out of bounds.
 * 
 * Time Complexity: O(1)
 */
void *DvectorGetAccessToElement(const dvector_t *dvector, size_t idx);



/**
 * Appends an element to the end of the dynamic vector.
 * 
 * @param: dvector The dynamic vector.
 * @param: data A pointer to the data to be appended.
 * @return: 0 if successful, -1 if memory allocation fails.
 * 
 * Time Complexity: O(1) amortized
 */
int DvectorPushBack(dvector_t *dvector, const void *data);



/**
 * Removes the last element from the dynamic vector.
 * 
 * @param: dvector The dynamic vector.
 * 
 * Time Complexity: O(1)
 */
void DvectorPopBack(dvector_t *dvector);


/**
 * Returns the number of elements currently stored in the dynamic vector.
 * 
 * @param: dvector The dynamic vector.
 * @return: The number of elements in the dynamic vector.
 * 
 * Time Complexity: O(1)
 */
size_t DvectorSize(const dvector_t *dvector);



/**
 * Returns the current capacity of the dynamic vector.
 * 
 * @param: dvector The dynamic vector.
 * @return: The current capacity of the dynamic vector.
 * 
 * Time Complexity: O(1)
 */
size_t DvectorCapacity(const dvector_t *dvector);



/**
 * Reserves space for at least the specified capacity in the dynamic vector.
 * 
 * @param: dvector The dynamic vector.
 * @param: capacity The new capacity to reserve.
 * @return: 0 if successful, -1 if memory allocation fails.
 * 
 * Time Complexity: O(n), where n is the new capacity
 */
int DvectorReserve(dvector_t *dvector, size_t capacity);



/**
 * Change the capacity of the dynamic vector to half the size.
 * 
 * @param: dvector The dynamic vector.
 * @return: 0 if successful, -1 if memory allocation fails.
 * 
 * Time Complexity: O(n), where n is the current size of the vector
 */
int DvectorShrink(dvector_t *dvector);



#endif /* DVECTOR_H */


