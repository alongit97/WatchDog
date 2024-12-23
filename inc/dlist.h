/*//////////////////////////////////////
Name: Alon Weinberg                    /
Reviewer:                              /
last date updated: 18/2/24             /
File type: header file                 /
//////////////////////////////////////*/ 

#ifndef DLIST_H
#define DLIST_H

#include <stddef.h> /* size_t */

/**
 * @brief Structure representing a doubly linked list.
 */
typedef struct dlist dlist_t;

/**
 * @brief Iterator for traversing a doubly linked list.
 */
typedef struct node *dlist_iter_t;

/**
 * @brief Function pointer type for matching elements in the list.
 *
 * @param data The data to match.
 * @param param Additional parameter for matching.
 * @return An integer indicating whether the match is successful or not.
 */
typedef int (*match_func_t)(const void *data, void *param);

/**
 * @brief Function pointer type for performing an action on elements in the list.
 *
 * @param data The data to perform the action on.
 * @param param Additional parameter for the action.
 * @return An integer indicating the success of the action.
 */
typedef int (*action_func_t)(void *data, void *param);

/**
 * @brief Creates a new doubly linked list.
 *
 * @return A pointer to the newly created list.
 */
dlist_t *DListCreate();

/**
 * @brief Destroys the doubly linked list and frees allocated memory.
 *
 * @param list The list to be destroyed.
 */
void DListDestroy(dlist_t *list);

/**
 * @brief Gets an iterator to the first node in the list.
 *
 * @param list The relevant list.
 * @return An iterator to the first node in the list.
 */
dlist_iter_t DListBegin(const dlist_t *list);

/**
 * @brief Gets an iterator to the last node in the list.
 *
 * @param list The relevant list.
 * @return An iterator to the last node in the list.
 */
dlist_iter_t DListEnd(const dlist_t *list);

/**
 * @brief Gets the next node relative to the iterator.
 *
 * @param iter An iterator pointer.
 * @return An iterator to the next node in the list.
 */
dlist_iter_t DListNext(dlist_iter_t iter);

/**
 * @brief Gets the previous node relative to the iterator.
 *
 * @param iter An iterator pointer.
 * @return An iterator to the previous node in the list.
 */
dlist_iter_t DListPrev(dlist_iter_t iter);

/**
 * @brief Compares the addresses of two iterators.
 *
 * @param iter1 First iterator pointer.
 * @param iter2 Second iterator pointer.
 * @return 1 if the addresses are the same, 0 otherwise.
 */
int DListIsIterSame(dlist_iter_t iter1, dlist_iter_t iter2);

/**
 * @brief Inserts a new node before the iterator.
 *
 * @param where An iterator pointer.
 * @param data The data to be inserted into the new node.
 * @return An iterator pointed to the new node.
 */
dlist_iter_t DListInsert(dlist_iter_t where, void* data);

/**
 * @brief Removes the node pointed by the iterator.
 *
 * @param to_remove An iterator pointer to the node to be removed.
 * @return An iterator pointed to the next node after the removed one.
 */
dlist_iter_t DListRemove(dlist_iter_t to_remove);

/**
 * @brief Gets the data stored in the node pointed by the iterator.
 *
 * @param iter An iterator pointer.
 * @return The data from the iterator.
 */
void *DListGetData(dlist_iter_t iter);

/**
 * @brief Sets data to a specific node.
 *
 * @param iter An iterator pointer.
 * @param data The data to insert into the node.
 */
void DListSetData(dlist_iter_t iter, void *data);

/**
 * @brief Gets the number of nodes in a list.
 *
 * @param list The relevant list.
 * @return The number of nodes in the list.
 */
size_t DListCount(const dlist_t *list);

/**
 * @brief Checks if the list is empty.
 *
 * @param list The relevant list.
 * @return 1 if the list is empty, 0 otherwise.
 */
int DListIsEmpty(const dlist_t *list);

/**
 * @brief Runs a match function through nodes in a list.
 *
 * @param from First node to test.
 * @param to Last node to test.
 * @param is_match A function to run through the nodes.
 * @param param Parameter to pass to the function.
 * @return The node where the function passed.
 */
dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to,
    match_func_t is_match, void *param);

/**
 * @brief Runs a function through nodes in a list.
 *
 * @param from First node to test.
 * @param to Last node to test.
 * @param action A function to run through the nodes.
 * @param param Parameter to pass to the function.
 * @return 1 if the function succeeds, 0 otherwise.
 */
int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func_t action, 
    void *param);

/**
 * @brief Runs a function through nodes in a list.
 *
 * @param from First node to slice.
 * @param to Last node to slice ("to"'s node is excluded).
 * @param where Insert the sliced list before "where"'s node.
 * @note "where" iterator must not point to a node within the sliced list.
 */
void DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where);

/**
 * @brief Runs a match function through nodes in a list and inserts all matches into a different list.
 *
 * @param from First node to test.
 * @param to node to stop testing.
 * @param is_match A function to run through the nodes.
 * @param param Parameter to pass to the function.
 * @param found_elements The list where matching elements will be inserted.
 * @return 0 if at least one match is found and -1 otherwise.
 */
int DListMultiFind(dlist_iter_t from, dlist_iter_t to, match_func_t is_match, 
    void *param, dlist_t *found_elements);

/**
 * @brief Pushes a node to the end of a list.
 *
 * @param list The relevant list.
 * @param data The data to be inserted into the node.
 * @return An iterator pointed to the new node.
 */
dlist_iter_t DListPushBack(dlist_t *list, void *data);

/**
 * @brief Pops a node from the end of a list. Empty list results in undefined.
 *
 * @param list The relevant list.
 * @return The data stored in the popped node.
 */
void* DListPopBack(dlist_t *list);

/**
 * @brief Pushes a node to the beginning of a list.
 *
 * @param list The relevant list.
 * @param data The data to be inserted into the node.
 * @return An iterator pointed to the new node.
 */
dlist_iter_t DListPushFront(dlist_t *list, void *data);

/**
 * @brief Pops a node from the beginning of a list. Empty list results in undefined.
 *
 * @param list The relevant list.
 * @return The data stored in the popped node.
 */
void* DListPopFront(dlist_t *list);

#endif /*DLIST_H*/
