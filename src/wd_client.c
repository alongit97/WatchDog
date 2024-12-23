/*//////////////////////////////////////////////////
File Name: wd_client.c
File Type: source
Made By: Alon Weinberg
Review By: Mia Vaserman
Last Modefy Date: 19/21/24
//////////////////////////////////////////////////*/
/*
compile with:
make TARGET=wd_client
gd -pthread wd.c wd_client.c scheduler.c dhcp.c dlist.c dvector.c heap.c pq_heap.c task.c uid.c -I../inc -lm -o wd.out
*/

#define _POSIX_C_SOURCE 200809L /*for sigaction related cpmmands*/

#include <stdlib.h> /*NULL*/
#include <unistd.h> /*fork*/
#include <signal.h> /*SIGUSR1*/
#include <string.h> /*strcmp*/
#include <pthread.h> /*pthread_t*/
#include <stdio.h> /*printf*/
#include <semaphore.h> /*sem_t*/
#include <stdatomic.h> /*atomic_int*/
#include <fcntl.h> 

#include "scheduler.h" /*scheduler*/
#include "uid.h" /*ilrd_uid_t*/
#include "wd_client.h" /*SUCCESS_WD*/

#define ERROR_LIMIT (5)
#define USER_SEM_NAME ("USER_SEMA")
#define WD_SEM_NAME ("WD_SEMA")

void HandleSigusr1(int signo, siginfo_t *info, void *context);
void HandleSigusr2(int signo, siginfo_t *info, void *context);
static void* RunSched(void* arg);
static int Task1(void *arg);
static int Task2(void *arg);
static int Task3(void *arg);
static int MakeSchedulerAndTasks(scheduler_t **scheduler ,ilrd_uid_t uid, const char**cmd);
static int Revive(const char** cmd);
static wd_status_t InitSemaphores(const char** cmd);

pid_t child_pid = 0;
pthread_t thread;
atomic_int counter = 0;
atomic_int end_flag = 0;
pid_t other_process_pid;
scheduler_t *scheduler = NULL;
typedef enum side_sem
{
    CURR_SEM = 0,
    OTHER_SEM
} side_sem_t;
sem_t *semaphores[2];

wd_status_t WDStart(const char** cmd)
{
    char pid_str[20];
    pid_t pid = 0;
    int status = 0;
    ilrd_uid_t uid;
    struct sigaction sig1 = {0};
    struct sigaction sig2 = {0};
    sig1.sa_sigaction = HandleSigusr1;
    sig2.sa_sigaction = HandleSigusr2;
    sig1.sa_flags = SA_SIGINFO;
    sig2.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sig1, NULL);
    sigaction(SIGUSR2, &sig2, NULL);

    if (SUCCESS_WD != InitSemaphores(cmd))
    {
        return (FAILURE_WD);
    }

    if (FAILURE_WD == MakeSchedulerAndTasks(&scheduler, uid, cmd))
    {
        return (FAILURE_WD);
    }

    /*watchdog*/
    if (0 == strcmp("./wd.out", cmd[0]))
    {   
        pid = getpid();
        other_process_pid = getppid();

        snprintf(pid_str, sizeof(pid_str), "%d", pid);
        if (setenv("WD_PID", pid_str, 1) != 0) 
        {
            perror("Error setting environment variable");
            return (FAILURE_WD);
        }
        RunSched(scheduler);
    }

    /*client*/
    else
    {
        /*no watchdog*/
        if (NULL == getenv("WD_PID"))
        {
            child_pid = fork();
            if (child_pid == -1)
            {
                printf("failed to create new process");
                return (FAILURE_WD);
            }

            /*child*/
            if (child_pid == 0)
            {   
                status = execvp("./wd.out", (char * const*)cmd[1]);
                if (status != 0)
                {
                    printf("error with execvp\n");
                    return (FAILURE_WD);
                }
            }

            other_process_pid = child_pid;
        }
        else
        {
            other_process_pid = atoi(getenv("WD_PID"));
        }
        
        /*parent*/
        status = pthread_create(&thread, NULL, &RunSched, scheduler);

        if (0 != status)
        {
            printf("failed to create thread\n");
            return (FAILURE_WD);
        }
                
    }

    return (SUCCESS_WD);
}

void WDStop()
{
    kill(other_process_pid, SIGUSR2);

    sem_wait(semaphores[CURR_SEM]);
    SchedStop(scheduler);

    sem_unlink("USER_SEMA");
    sem_unlink("WD_SEMA");
    sem_close(semaphores[CURR_SEM]);
    sem_close(semaphores[OTHER_SEM]);
    
    pthread_join(thread,NULL);
}
/******************************************************************************/

/***************************UTILITY FUNCTIONS**********************************/

/******************************************************************************/
static void* RunSched(void* arg)
{
    (void)arg;

    sem_post(semaphores[OTHER_SEM]);
    sem_wait(semaphores[CURR_SEM]);

    SchedRun(scheduler);
    SchedDestroy(scheduler);

    return (NULL);
}

static int Revive(const char** cmd)
{
    pid_t pid;

    pid = fork();

    if (0 == pid)
    {
        /*revive user*/
        if (0 == strcmp(cmd[0], "./wd.out"))
        {
            printf("revive usr\n\n");
            execvp(((char **)cmd[1])[0], (char **)cmd[1]);
            return(FAILURE_WD);
        }

        /*revive wd*/
        printf("revive wd\n\n");
        execvp("./wd.out", (char **)cmd[1]);
        return (FAILURE_WD); 
    }

    other_process_pid = pid;

    sem_post(semaphores[OTHER_SEM]);
    sem_wait(semaphores[CURR_SEM]);

    return(SUCCESS_WD);
}

static wd_status_t InitSemaphores(const char** cmd)
{
    side_sem_t user_sem = CURR_SEM;
    side_sem_t wd_sem = OTHER_SEM;

    if (0 == strcmp(cmd[0], "./wd.out"))
    {
        user_sem = OTHER_SEM;
        wd_sem = CURR_SEM;
    }

    semaphores[user_sem] = sem_open(USER_SEM_NAME, O_CREAT, 0666, 0);
    if (semaphores[user_sem] == SEM_FAILED)
    {
        printf("sem open failed\n");
        return (FAILURE_WD);
    }

    semaphores[wd_sem] = sem_open(WD_SEM_NAME, O_CREAT, 0666, 0);
    if (semaphores[wd_sem] == SEM_FAILED)
    {
        printf("sem open failed\n");
        return (FAILURE_WD);
    }

    return (SUCCESS_WD);
}
/******************************************************************************/

/************************************Tasks*************************************/

/******************************************************************************/
static int Task1(void *arg)
{
    (void)arg;

    printf("task1: counter %d, pid: %d\n\n", counter, getpid());

    counter++;

    kill(other_process_pid, SIGUSR1);
    return (REPEAT);
}

static int Task2(void *cmd)
{
    if (counter >= ERROR_LIMIT)
    {
        atomic_exchange(&counter, 0);
        printf("enetr revive\n\n");
        Revive(cmd);
    }
    return (REPEAT);
}

static int Task3(void *arg)
{
    (void)arg;

    if (end_flag == 1)
    {
        sem_post(semaphores[OTHER_SEM]);
        
        SchedStop(scheduler);

        if (unsetenv("WD_PID") != 0) 
        {
            printf("unsetenv\n");
            return (FAILURE_WD);
        }
        return (STOP);
    }

    return (REPEAT);
}

static int MakeSchedulerAndTasks(scheduler_t **scheduler ,ilrd_uid_t uid,
                                 const char**cmd)                                                  
{
    *scheduler = SchedCreate();
    if (*scheduler == NULL)
    {
        return (FAILURE_WD);
    }

    uid = SchedAddTask(*scheduler, 1, Task1, NULL, NULL, NULL);
    if (UIDIsEqual(uid, bad_uid))
    {
        printf("uid1 failed\n");
        return(FAILURE_WD);
    }

    uid = SchedAddTask(*scheduler, 1, Task2, cmd, NULL, NULL);
    if (UIDIsEqual(uid, bad_uid))
    {
        printf("uid2 failed\n");
        return(FAILURE_WD);
    }

    uid = SchedAddTask(*scheduler, 2, Task3, NULL, NULL, NULL);
    if (UIDIsEqual(uid, bad_uid))
    {
        printf("uid failed\n");
        return(FAILURE_WD);
    }

    return (SUCCESS_WD);
}
/******************************************************************************/

/***************************HANDLER FUNCTIONS**********************************/

/******************************************************************************/
void HandleSigusr1(int signo, siginfo_t *info, void *context)
{
    (void)signo;
    (void)context;
    if (other_process_pid == info->si_pid)
    {
        atomic_exchange(&counter, 0);
    }
}

void HandleSigusr2(int signo, siginfo_t *info, void *context)
{
    (void)signo;
    (void)context;
    if (other_process_pid == info->si_pid)
    {
        atomic_exchange(&end_flag, 1);
    }
}