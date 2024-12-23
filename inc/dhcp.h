/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
last date updated: 02/4/24             
File type: header file                   
//////////////////////////////////////*/ 


#ifndef __DHCP_ILRD_1556__
#define __DHCP_ILRD_1556__

#include <stddef.h> /* size_t */

#define BYTES_IN_IP (4)

typedef enum status
{
    DHCP_STATUS_SUCCESS = 0,
    DHCP_STATUS_DOUBLE_FREE_ERR,
    DHCP_STATUS_FULL_ERR,
    DHCP_STATUS_FAIL
} status_t;

typedef struct dhcp dhcp_t;

/* broadcast - host ip is all 1's
   DHCP ip   - host ip is all 1's but the last (...11110)
    - host ip is all 0's                                */
/*O(logn)*/
dhcp_t *DHCPCreate(const unsigned char subnet_addr[BYTES_IN_IP], 
                   size_t bits_in_subnet); 


/* if gets NULL - does nothing */
/*O(n)*/
void DHCPDestroy(dhcp_t *dhcp); 

/*the first free address, or the address he wanted and if it isnt free i will give him the first larger ip that is free*/
/* or the address he wanted. when taken, the next free addr after it */
/*O(Log n)*/
status_t DHCPAllocateIP(dhcp_t *dhcp, 
                        unsigned char result_ip[BYTES_IN_IP],
                        const unsigned char requested_ip[BYTES_IN_IP]); 

/* indicate double free error */
/*O(Log n)*/
status_t DHCPFreeIP(dhcp_t *dhcp, 
                    const unsigned char ip_addr[BYTES_IN_IP]);

/* without preserved addresses */
/*O(n)*/
size_t DHCPCountFree(const dhcp_t *dhcp); 


#endif /* __DHCP_ILRD_1556__ */

