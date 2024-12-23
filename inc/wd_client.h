#ifndef __ILRD_WD_1556__
#define __ILRD_WD_1556__

typedef enum wd_status
{
    SUCCESS_WD = 0,
    FAILURE_WD
} wd_status_t;

/*
Description:
    -Defends a critical section: if this section crashes, the program is revived
Params:
    -cmd: command line to reinitiate the process {"./a.out", "arguments"...}
Return:
    -status:
        -SUCCESS: section is protected
        -FAILURE: section isn't protected
Notes:
    -this utility uses SIGUSR1 SIGUSR2 signals
*/
wd_status_t WDStart(const char **name);

/*
Description:
    -Ends the critical section
Notes:
    -Function does not guarentee to free used resources immediately at function exit
*/
void WDStop(void);

#endif /*__ILRD_WD_1556__*/