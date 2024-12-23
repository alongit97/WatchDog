/*//////////////////////////////////////
Name: Alon Weinberg
Reviewer:
Last date updated: 3/3/24
File type: header file
//////////////////////////////////////*/

#ifndef TASK_H
#define TASK_H

#include "uid.h" /* ilrd_uid_t */
#include <time.h> /* time_t */

/*
 * Description:
 *    This module provides functionality for creating and managing tasks
 *    that can be scheduled to run at specified intervals.
 */

typedef struct task task_t;

typedef int (*task_action_func_t)(void* param);
typedef void (*task_clean_func_t)(void* param);

/*
 * Function: TaskCreate
 * ---------------------
 * Creates a new task.
 *
 * interval: The interval between executions of the task (in seconds).
 * action: A function pointer to the action function to be executed.
 * clean_func: A function pointer to a cleanup function to be executed when the task is destroyed.
 * action_param: A parameter to be passed to the action function.
 * clean_up_param: A parameter to be passed to the cleanup function.
 *
 * Returns:
 *    A pointer to the newly created task.
 *
 * Complexity: O(1)
 */
task_t* TaskCreate(size_t interval, task_action_func_t action,
        task_clean_func_t clean_func, void *action_param, void *clean_up_param);

/*
 * Function: TaskDestroy
 * ----------------------
 * Destroys a task and frees its allocated memory.
 *
 * task: A pointer to the task to be destroyed.
 *
 * Complexity: O(1)
 */
void TaskDestroy(task_t *task);

/*
 * Function: TaskGetUID
 * ---------------------
 * Gets the unique ID of the task.
 *
 * task: A pointer to the task.
 *
 * Returns:
 *    The unique ID of the task.
 *
 * Complexity: O(1)
 */
ilrd_uid_t TaskGetUID(const task_t *task);

/*
 * Function: TaskRun
 * ------------------
 * Executes the action function associated with the task.
 *
 * task: A pointer to the task.
 *
 * Returns:
 *    The return value of the action function.
 *
 * Complexity: O(1)
 */
int TaskRun(task_t *task);

/*
 * Function: TaskIsSame
 * ---------------------
 * Checks if two tasks are the same by comparing their unique IDs.
 *
 * task1: A pointer to the first task.
 * task2: A pointer to the second task.
 *
 * Returns:
 *    1 if the tasks are the same.
 *    0 if the tasks are different.
 *
 * Complexity: O(1)
 */
int TaskIsSame(const task_t *task1, const task_t *task2);

/*
 * Function: TaskGetTimeToRun
 * ---------------------------
 * Gets the time at which the task is scheduled to run next.
 *
 * task: A pointer to the task.
 *
 * Returns:
 *    The time at which the task is scheduled to run next.
 *
 * Complexity: O(1)
 */
time_t TaskGetTimeToRun(const task_t *task);

/*
 * Function: TaskUpdateTimeToRun
 * ------------------------------
 * Updates the time at which the task is scheduled to run next.
 *
 * task: A pointer to the task.
 *
 * Complexity: O(1)
 */
void TaskUpdateTimeToRun(task_t *task);

#endif /* TASK_H */

