/** 
 @file  unix.h
 @brief ENet Unix header
*/
#ifndef __ENET_UNIX_H__
#define __ENET_UNIX_H__

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

typedef int ENetSocket;

enum
{
    ENET_SOCKET_NULL = -1
};

#define ENET_HOST_TO_NET_16(value) (htons (value)) /**< macro that converts host to net byte-order of a 16-bit value */
#define ENET_HOST_TO_NET_32(value) (htonl (value)) /**< macro that converts host to net byte-order of a 32-bit value */
#define ENET_HOST_TO_NET_64(value) (hton64 (value))

#define ENET_NET_TO_HOST_16(value) (ntohs (value)) /**< macro that converts net to host byte-order of a 16-bit value */
#define ENET_NET_TO_HOST_32(value) (ntohl (value)) /**< macro that converts net to host byte-order of a 32-bit value */
#define ENET_NET_TO_HOST_64(value) (ntoh64 (value))

typedef struct
{
    void * data;
    size_t dataLength;
} ENetBuffer;

#define ENET_CALLBACK

#define ENET_API extern

typedef fd_set ENetSocketSet;

#define ENET_SOCKETSET_EMPTY(sockset)          FD_ZERO (& (sockset))
#define ENET_SOCKETSET_ADD(sockset, socket)    FD_SET (socket, & (sockset))
#define ENET_SOCKETSET_REMOVE(sockset, socket) FD_CLEAR (socket, & (sockset))
#define ENET_SOCKETSET_CHECK(sockset, socket)  FD_ISSET (socket, & (sockset))

//oil 新增
//对64位整数做本机字节序和网络字节序的相互转换。
unsigned long long int hton64(unsigned long long int theValue);
unsigned long long int ntoh64(unsigned long long int theValue);

#endif /* __ENET_UNIX_H__ */

