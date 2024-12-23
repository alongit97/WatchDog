/*//////////////////////////////////////
Name: Alon Weinberg                    
Reviewer:                              
Last Date Updated: 04/4/24             
File Type: Test File                   
//////////////////////////////////////*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <math.h> /* pow */
#include <stdio.h> /* sprintf */

#include "dhcp.h" /* dhcp_t, status_t */

#define SIZE_IP (32)
#define SIZE_BYTE (8)
#define SIZE_HOST(dhcp) (SIZE_IP - dhcp->bits_in_subnet)

typedef enum children
{
    LEFT = 0,
    RIGHT,
    NUM_OF_CHILDREN
} children_t;

typedef enum is_full
{
    NOT_FULL = 0,
    FULL
} is_full_t;

typedef struct trie_node trie_node_t;

struct trie_node
{
    trie_node_t *children[NUM_OF_CHILDREN];
    int is_full;
};

struct dhcp
{
    unsigned char subnet_addr[BYTES_IN_IP];
    size_t bits_in_subnet;
    trie_node_t *tree;
};

static trie_node_t *TrieCreate();
static void TrieDestroy(trie_node_t *root);
static status_t TrieInsertMostLeft(trie_node_t *root, unsigned int *result_ip, unsigned int mask);
static status_t TrieInsertRequest(trie_node_t *root, unsigned int *result_ip,
                                  const unsigned int requested_ip, int mask);
static size_t TrieSize(trie_node_t *trie, int size);

static unsigned int CharToInt(const unsigned char *requested_ip, size_t bits_in_subnet);
static void ExtractNum(unsigned char *ip, unsigned int result_ip, size_t bits_in_subnet);
static int IsFull(trie_node_t *root);
static status_t ChangeToMostLeft(trie_node_t *root, status_t status,
                                  unsigned int *result_ip, int mask, unsigned int curr_host_msb);
static unsigned int ExtractBitsFromArray(const unsigned char requested[BYTES_IN_IP]);

/******************************************************************************/

/*******************************API Functions*********************************/

/******************************************************************************/

dhcp_t *DHCPCreate(const unsigned char subnet_addr[BYTES_IN_IP], 
                   size_t bits_in_subnet)
{
    status_t status = DHCP_STATUS_SUCCESS;
    unsigned char network_address[BYTES_IN_IP] = {0, 0, 0, 0};
    unsigned char server_address[BYTES_IN_IP] = {255, 255, 255, 254};
    unsigned char broadcast_address[BYTES_IN_IP] = {255, 255, 255, 255};

    dhcp_t *dhcp = malloc(sizeof(dhcp_t));
    if (!dhcp)
    {
        return (NULL);
    }

    dhcp->tree = TrieCreate();
    dhcp->bits_in_subnet = bits_in_subnet;
    memcpy(dhcp->subnet_addr, subnet_addr, BYTES_IN_IP);
    
    status = DHCPAllocateIP(dhcp, network_address, network_address);
    if (status != DHCP_STATUS_SUCCESS)
    {
        DHCPDestroy(dhcp);
        return (NULL);
    }

    status = DHCPAllocateIP(dhcp, server_address, server_address);
    if (status != DHCP_STATUS_SUCCESS)
    {
        DHCPDestroy(dhcp);
        return (NULL);
    }

    status = DHCPAllocateIP(dhcp, broadcast_address, broadcast_address);
    if (status != DHCP_STATUS_SUCCESS)
    {
        DHCPDestroy(dhcp);
        return (NULL);
    }

    return (dhcp);
}

void DHCPDestroy(dhcp_t *dhcp)
{
    TrieDestroy(dhcp->tree);
    dhcp->tree = NULL;
    free(dhcp);
}

status_t DHCPAllocateIP(dhcp_t *dhcp, unsigned char result_ip[BYTES_IN_IP],
                        const unsigned char requested_ip[BYTES_IN_IP])
{
    status_t status = DHCP_STATUS_SUCCESS;
    unsigned int host_requested = 0;
    unsigned int mask = (1 << (SIZE_HOST(dhcp) - 1));
    unsigned int result_ip_in_bits = 0;

    if (NULL == requested_ip)
    {
        status = TrieInsertMostLeft(dhcp->tree, &result_ip_in_bits, mask);
    }
    else
    {
        host_requested = CharToInt(requested_ip, dhcp->bits_in_subnet);
        status = TrieInsertRequest(dhcp->tree, &result_ip_in_bits, host_requested, mask);
        if (DHCP_STATUS_FULL_ERR == status)
        {
            status = TrieInsertMostLeft(dhcp->tree, &result_ip_in_bits, mask);
        }
    }

    memcpy(result_ip, dhcp->subnet_addr, sizeof(int));

    ExtractNum(result_ip, result_ip_in_bits, dhcp->bits_in_subnet);

    return (status);
}

status_t DHCPFreeIP(dhcp_t *dhcp, const unsigned char ip_addr[BYTES_IN_IP])
{
    unsigned int height = 32 - (dhcp->bits_in_subnet);
    unsigned int host_to_remove = ExtractBitsFromArray(ip_addr);
    unsigned int curr_host_msb = 0;
    trie_node_t *current_node = dhcp->tree;
    trie_node_t *parent = NULL;
    while (height > 0)
    {
        curr_host_msb = ((1 << (height -1)) & host_to_remove) == 0 ? LEFT : RIGHT; 
        if (current_node->children[curr_host_msb] == NULL)
        {
            return DHCP_STATUS_DOUBLE_FREE_ERR;
        }
        parent = current_node;
        current_node = current_node->children[curr_host_msb];

        parent->is_full = NOT_FULL;
        height--;
    }

    parent->children[curr_host_msb] = NULL;
    free(current_node);
    return DHCP_STATUS_SUCCESS;
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    size_t occupied = 0;
    assert(dhcp);
    occupied = TrieSize(dhcp->tree, SIZE_HOST(dhcp));
    return (pow(2, SIZE_HOST(dhcp)) - occupied);
}

/******************************************************************************/

/****************************Utilities Functions*******************************/

/******************************************************************************/

static trie_node_t *TrieCreate()
{
    trie_node_t *trie = (trie_node_t *)malloc(sizeof(trie_node_t));
    if (NULL == trie)
    {
        return (NULL);
    }

    trie->children[LEFT] = NULL;
    trie->children[RIGHT] = NULL;
    trie->is_full = NOT_FULL;

    return (trie);
}

static void TrieDestroy(trie_node_t *root)
{
    if (!root)
    {
        return;
    }

    TrieDestroy(root->children[LEFT]);
    TrieDestroy(root->children[RIGHT]);
    free(root);
}

static status_t TrieInsertMostLeft(trie_node_t *root,
                                    unsigned int *result_ip, unsigned int mask)
{
    int side = LEFT;
    status_t status = DHCP_STATUS_SUCCESS;

    if (FULL == root->is_full)
    {
        return (DHCP_STATUS_FULL_ERR);
    }

    if (mask == 0) 
    {
        root->is_full = FULL;
        return (DHCP_STATUS_SUCCESS);
    }

    if (root->children[side] && FULL == root->children[side]->is_full)
    {
        side = RIGHT;
    }

    if (!root->children[side])
    {
        root->children[side] = TrieCreate();
        if (!root->children[side])
        {
            return (DHCP_STATUS_FAIL);
        }
    }

    *result_ip = ((*result_ip << 1) | side);
    status = TrieInsertMostLeft(root->children[side], result_ip, mask >> 1);

    if (IsFull(root))
    {
        root->is_full = FULL;
    }

    return (status);
}

static status_t TrieInsertRequest(trie_node_t *root, unsigned int *result_ip,
                                  const unsigned int requested_ip, int mask)
{
    status_t status = DHCP_STATUS_SUCCESS;
    unsigned int curr_host_msb = (mask & requested_ip) == 0 ? 0 : 1;

    if (FULL == root->is_full)
    {
        return (DHCP_STATUS_FULL_ERR);
    }

    if (mask == 0)
    {
        root->is_full = FULL;
        return (DHCP_STATUS_SUCCESS);
    }

    if (NULL == root->children[curr_host_msb])
    {
        root->children[curr_host_msb] = TrieCreate();
        if (!root->children[curr_host_msb])
        {
            return (DHCP_STATUS_FAIL);
        }
    }
    *result_ip = ((*result_ip << 1) | curr_host_msb);
    status = TrieInsertRequest(root->children[curr_host_msb], result_ip, requested_ip, mask >> 1);

    if (DHCP_STATUS_FULL_ERR == status)
    {
        *result_ip = *result_ip >> 1;
        status = ChangeToMostLeft(root, status, result_ip, mask, curr_host_msb);
    }
    return (status);
}

static status_t ChangeToMostLeft(trie_node_t *root, status_t status,
                                  unsigned int *result_ip, int mask, unsigned int curr_host_msb)
{
    if (LEFT == curr_host_msb)
    {
        curr_host_msb = RIGHT;
        *result_ip = ((*result_ip << 1) | 1);
        if (!root->children[RIGHT])
        {
            root->children[RIGHT] = TrieCreate();
            if (!root->children[RIGHT])
            {
                return (DHCP_STATUS_FAIL);
            }
        }

        status = TrieInsertMostLeft(root->children[RIGHT], result_ip, mask >> 1);
        if (DHCP_STATUS_FULL_ERR == status)
        {
            *result_ip = *result_ip >> 1;
        }
    }

    return (status);
}

static size_t TrieSize(trie_node_t *root, int height)
{
    if (NULL == root)
    {
        return (0);
    }

    if (FULL == root->is_full)
    {
        return (pow(2, height));
    }

    return (TrieSize(root->children[LEFT], height - 1) +
            TrieSize(root->children[RIGHT], height - 1));
}

static void ExtractNum(unsigned char *ip, unsigned int result_ip, size_t bits_in_subnet)
{
    unsigned mask = 0xFF000000;
    size_t bits = 0;
    int i = 0;

    i = bits_in_subnet / SIZE_BYTE;
    bits = i * SIZE_BYTE;
    mask = mask >> (SIZE_BYTE * i);

    if (bits < bits_in_subnet) {
    ip[i] = (ip[i] >> (SIZE_BYTE - (bits_in_subnet % SIZE_BYTE))) << 
                      (SIZE_BYTE - (bits_in_subnet % SIZE_BYTE));
    ip[i] |= ((result_ip & mask) >> ((BYTES_IN_IP - i - 1) * SIZE_BYTE));
    mask >>= SIZE_BYTE;
    ++i;
}
    while (i < BYTES_IN_IP)
    {
        ip[i] = (result_ip & mask) >> ((BYTES_IN_IP - i - 1) * SIZE_BYTE);
        mask = mask >> SIZE_BYTE;
        i++;
    }
}

static unsigned int CharToInt(const unsigned char *requested_ip, size_t bits_in_subnet)
{
    unsigned int host = 0;
    size_t bits = 0;
    int i = 0;

    i = bits_in_subnet / SIZE_BYTE;
    bits = i * SIZE_BYTE;

    if (bits < bits_in_subnet)
    {
        host = requested_ip[i] << (bits_in_subnet % SIZE_BYTE);
        host = host >> (bits_in_subnet % SIZE_BYTE);
        i++;
    }

    while (i < BYTES_IN_IP)
    {
        host = host << SIZE_BYTE;
        host = host | requested_ip[i];
        i++;
    }

    return host;
}

static int IsFull(trie_node_t *root)
{
    return (NULL != root->children[LEFT] && FULL == root->children[LEFT]->is_full &&
            NULL != root->children[RIGHT] && FULL == root->children[RIGHT]->is_full);
}

static unsigned int ExtractBitsFromArray(const unsigned char requested[BYTES_IN_IP])
{
    unsigned int result = 0;
    size_t i = 0;
    for (i = 0; i < 4; i++)
    {
        result |= (unsigned int)(requested[i]) << ((BYTES_IN_IP - 1 - i) * 8);
    }
    return result;
}
