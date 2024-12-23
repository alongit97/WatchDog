/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
last date updated: 3/3/24              
File type: source file                 
//////////////////////////////////////*/ 

#include "task.h" /*task_t*/

#include <stdio.h>/*NULL*/
#include <stddef.h>/*size_t*/
#include <stdlib.h>/*free*/


struct task
{
	ilrd_uid_t id;
	task_action_func_t action;
	task_clean_func_t clean_func;
	void *action_param;
	void *clean_up_param;
	time_t exec_time;
	size_t interval;
};

task_t* TaskCreate(size_t interval, task_action_func_t action, 
        task_clean_func_t clean_func , void *action_param, void *clean_up_param)
{
	task_t* new_task = (task_t*)malloc(sizeof(task_t));	
	if (NULL == new_task)
	{
		return NULL;
	}
	
	new_task->id = UIDGenerate();
	if (UIDIsEqual(new_task->id, bad_uid))
	{
		free(new_task);
		return NULL;
	}
	
	new_task->action = action;
	
	new_task->clean_func = clean_func;
	
	new_task->action_param = action_param;
	
	new_task->clean_up_param = clean_up_param;
	
	new_task->interval = interval;
	
	new_task->exec_time = time(NULL) + interval;
	
	return (new_task);	
}

void TaskDestroy(task_t *task)
{
	if(NULL != task->clean_func)
	{
	   task->clean_func(task->clean_up_param);
	}
	
	free(task);
}

ilrd_uid_t TaskGetUID(const task_t *task)
{
	return (task->id);
}

int TaskRun(task_t *task)
{
	return (task->action(task->action_param));
}

int TaskIsSame(const task_t *task1, const task_t *task2)
{
	return (UIDIsEqual(TaskGetUID(task1), TaskGetUID(task2)));
}

time_t TaskGetTimeToRun(const task_t *task)
{
	return (task->exec_time);
}

void TaskUpdateTimeToRun(task_t *task)
{
	task->exec_time = time(NULL) + task->interval;
}
