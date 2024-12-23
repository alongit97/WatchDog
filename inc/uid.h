/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
last date updated: 28/2/24             
File type: header file                 
//////////////////////////////////////*/ 

#ifndef ILRD_UID_H
#define ILRD_UID_H

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*pid_t*/
#include <time.h> /*time_t*/

typedef struct ilrd_uid
{
    size_t counter;
    pid_t pid;
    time_t time;
} ilrd_uid_t;

extern const ilrd_uid_t bad_uid;

ilrd_uid_t UIDGenerate(void);
int UIDIsEqual(ilrd_uid_t one, ilrd_uid_t other);

#endif /*ILRD_UID_H*/
