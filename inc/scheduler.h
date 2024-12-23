/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
last date updated: 28/3/24             
File type: header file                   
//////////////////////////////////////*/ 

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h> /* size_t */
#include "uid.h" /* ilrd_uid_t */

/* Define scheduler structure */
typedef struct scheduler scheduler_t;
/* Function pointer type for the action function of a task */
typedef int (*action_func_t)(void* param);
/* Function pointer type for the cleanup function of a task */
typedef void (*cleanup_func_t)(void* param);

/*enum of the status*/
enum
{
    ERROR = -1,
    SUCCESS,
    STOP,
    REPEAT,
    RUNNING,
    NOT_RUNNING
};

/* Function: SchedCreate
 * ----------------------
 * Creates a new scheduler.
 * 
 * Returns: A pointer to the newly created scheduler, or NULL on failure.
 * 
 * Complexity: O(1)
 */
scheduler_t *SchedCreate(void);

/* Function: SchedDestroy
 * -----------------------
 * Destroys a scheduler, freeing all allocated memory.
 * 
 * sched: Pointer to the scheduler to be destroyed.
 * 
 * Complexity: O(n)
 * 
 * Warning: sched must not be NULL. 
 */
void SchedDestroy(scheduler_t *sched);

/* Function: SchedAddTask
 * -----------------------
 * Adds a task to the scheduler.
 * 
 * sched: Pointer to the scheduler.
 * interval: Time interval for the task's execution.
 * action_func: Pointer to the function to be executed by the task.
 * action_params: Parameters to be passed to the action function.
 * cleanup_func: Pointer to the cleanup function to be called upon task destruction.
 * cleanup_params: Parameters to be passed to the cleanup function.
 * 
 * Returns: The unique identifier of the added task, or bad_uid on failure.
 * 
 * Complexity: O(n)
 * 
 * Warning: sched and action_func must not be NULL. 
 */
ilrd_uid_t SchedAddTask(scheduler_t *sched, size_t interval,
 action_func_t action_func, void *action_params,
  cleanup_func_t cleanup_func, void *cleanup_params);


/* Function: SchedRemoveTask
 * --------------------------
 * Removes a task from the scheduler.
 * 
 * sched: Pointer to the scheduler.Warning: sched must not be NULL.
 * task_id: The unique identifier of the task to be removed.
 * 
 * Returns: SUCCESS if the task was successfully removed, ERROR otherwise.
 * 
 * Complexity: O(n)
 * 
 * Warning: sched must not be NULL. 
 */
int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t task_id);

/* Function: SchedRun
 * -------------------
 * Runs the scheduler, executing tasks as scheduled.
 * 
 * sched: Pointer to the scheduler.
 * 
 * Returns: The status of the scheduler after execution (ERROR, SUCCESS, STOP).
 * 
 * Complexity: O(n)
 * 
 * Warning: sched must not be NULL.
 */
int SchedRun(scheduler_t *sched);

/* Function: SchedStop
 * --------------------
 * Stops the execution of the scheduler.
 * 
 * sched: Pointer to the scheduler.
 * 
 * Returns: STOP to indicate that the scheduler has been stopped.
 * 
 * Complexity: O(1)
 * 
 * Warning: sched must not be NULL.
 */
int SchedStop(scheduler_t *sched);

/* Function: SchedClear
 * ---------------------
 * Clears all tasks from the scheduler.
 * 
 * sched: Pointer to the scheduler.
 * 
 * Complexity: O(n)
 * 
 * Warning: sched must not be NULL.
 */
void SchedClear(scheduler_t *sched);

/* Function: SchedSize
 * --------------------
 * Retrieves the number of tasks in the scheduler.
 * 
 * sched: Pointer to the scheduler.
 * 
 * Returns: The number of tasks in the scheduler.
 * 
 * Complexity: O(1)
 * 
 * Warning: sched must not be NULL.
 */
size_t SchedSize(const scheduler_t *sched);

/* Function: SchedIsEmpty
 * -----------------------
 * Checks if the scheduler is empty.
 * 
 * sched: Pointer to the scheduler.
 * 
 * Returns: 1 if the scheduler is empty, 0 otherwise.
 * 
 * Complexity: O(1)
 * 
 * Warning: sched must not be NULL.
 */
int SchedIsEmpty(const scheduler_t *sched);

#endif /*SCHEDULER_H*/
