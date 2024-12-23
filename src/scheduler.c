/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
last date updated: 28/3/24             
File type: source file                   
//////////////////////////////////////*/ 
#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/
#include <unistd.h> /*sleep()*/

#include "scheduler.h" /*scheduler_t*/
#include "pq_heap.h" /*pq_t*/
#include "task.h" /*task_t*/

static int PriorityRule(const void *data, const void *dest_data);
static int FindToRemove(const void *data, void *param );

struct scheduler
{
    pq_t *priority_queue;
    task_t* active;
    int is_running;
};

scheduler_t *SchedCreate(void)
{
    scheduler_t *sched = (scheduler_t *)malloc(sizeof(scheduler_t));
    if (NULL == sched)
    {
        return NULL;
    }

    sched->priority_queue = PQCreate(&PriorityRule);
    if (NULL == sched->priority_queue)
    {
        free(sched);
        return NULL;
    }

    sched->is_running = NOT_RUNNING;
    sched->active = NULL;

    return sched;
}

void SchedDestroy(scheduler_t *sched)
{
    assert(sched);
    
    SchedClear(sched);
    PQDestroy(sched->priority_queue);
    sched->priority_queue = NULL;

    free(sched);
}

ilrd_uid_t SchedAddTask(scheduler_t *sched, size_t interval,
 action_func_t action_func, void *action_params,
  cleanup_func_t cleanup_func, void *cleanup_params)
{
    task_t *new_task = NULL;

    assert(sched);
    assert(action_func);

    new_task = TaskCreate(interval, action_func, cleanup_func, action_params
                                                            ,cleanup_params);
    if(!new_task || PQEnqueue(sched->priority_queue, new_task))
    {
        TaskDestroy(new_task);
        return bad_uid;
    }

    return TaskGetUID(new_task);
}

int SchedRemoveTask(scheduler_t *sched, ilrd_uid_t task_id)
{
    task_t *task;
    assert(sched);

    task = PQErase(sched->priority_queue, &FindToRemove, &task_id);
    if(NULL == task)
    {
        return ERROR;
    }

    TaskDestroy(task);

    return SUCCESS;
}

int SchedRun(scheduler_t *sched)
{
    task_t *run_task = NULL;
    int status = SUCCESS;
    assert(sched);
    sched->is_running = RUNNING;

    while (!SchedIsEmpty(sched) && !status && sched->is_running == RUNNING)
    {
        while (TaskGetTimeToRun(PQPeek(sched->priority_queue)) > time(NULL))
        {
            sleep(1);
        }
        run_task = PQDequeue(sched->priority_queue);
        sched->active = run_task;

        status = TaskRun(run_task);
        if(status == REPEAT)
        {
            TaskUpdateTimeToRun(run_task);
            status = PQEnqueue(sched->priority_queue, run_task);
            if (status)
            {
                TaskDestroy(run_task);
            }
        }
        else
        {
            TaskDestroy(run_task);
        }
        sched->active = NULL;
    }
    sched->is_running = NOT_RUNNING;

    return (status);
}

int SchedStop(scheduler_t *sched)
{
    assert(sched);

    sched->is_running = NOT_RUNNING;

    return (STOP);
}

void SchedClear(scheduler_t *sched)
{
    assert(sched);
    
    while (!SchedIsEmpty(sched))
    {
        TaskDestroy(PQDequeue(sched->priority_queue));
    }

    sched->active = NULL;
}

size_t SchedSize(const scheduler_t *sched)
{
    size_t size = 0;

    assert(sched);

    size = PQCount(sched->priority_queue);

    return (sched->active ? size + 1 : size);
}

int SchedIsEmpty(const scheduler_t *sched)
{
    assert(sched);
    
    if (sched->is_running == RUNNING && sched->active != NULL)
    {
        return 0;
    }
    
    return (PQIsEmpty(sched->priority_queue));
}


static int PriorityRule(const void *data, const void *dest_data)
{
    time_t time1, time2;

    assert(data);
    assert(dest_data);

    time1 = TaskGetTimeToRun((task_t*)data);
    time2 = TaskGetTimeToRun((task_t*)dest_data);

    return (time1 - time2);
}

static int FindToRemove(const void *data, void *param )
{
    return (UIDIsEqual(TaskGetUID((task_t*)data), *(ilrd_uid_t*)param));
}  

