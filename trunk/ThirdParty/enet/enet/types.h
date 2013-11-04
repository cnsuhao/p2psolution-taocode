/** 
 @file  types.h
 @brief type definitions for ENet
*/
#ifndef __ENET_TYPES_H__
#define __ENET_TYPES_H__

typedef unsigned char enet_uint8;       /**< unsigned 8-bit type  */
typedef unsigned short enet_uint16;     /**< unsigned 16-bit type */
typedef unsigned int enet_uint32;      /**< unsigned 32-bit type */
//oil 新增64位无符号整数
typedef unsigned long long int enet_uint64;

//IP字符串的字节个数，包括结束符。
#define SoENet_MaxCount_IPString 16

//记录SoENetUserData结构体的用途
typedef enum _SoENetUserDataType
{
	UserData_None = 0, //无效值，数据都是无效值
	UserData_ClientConnectServer = 1, //在连接操作中，客户端连接P2P服务器
	UserData_ClientConnectClient = 2, //在连接操作中，客户端连接另一个客户端
	UserData_DisconnectBetweenCS = 3, //在断开连接操作中，客户端与服务器之间断开了连接
	UserData_DisconnectBetweenCC = 4, //在断开连接操作中，客户端与客户端之间断开了连接
	UserData_DisconnectTimeout = 5, //ENet底层侦测到连接超时
} SoENetUserDataType;

//标记断开连接的原因。
typedef enum _SoDisconnectReason
{
	DisconnectReason_None = 0, //无效值。
	DisconnectReason_PeerListIsFull = 1, //P2PServer主动与客户端断开连接，原因是连接数目已达到最大值。
	DisconnectReason_ServerShutdown = 2, //P2PServer主动与客户端断开连接，原因是P2PServer即将关闭。
	DisconnectReason_ClientRequest = 3, //客户端主动与P2PServer断开连接。
	DisconnectReason_Timeout = 4, //连接超时，导致了连接断开。
} SoDisconnectReason;

//在建立连接和断开连接过程中，用户传递的自定义数据。
typedef struct _SoENetUserData
{
	//本成员为建立连接服务。
	//每个客户端都有一个属于自己的ClientID。这个ID是由GameServer分配的，不是由P2PServer分配。
	enet_uint64 ClientID;
	//本成员为建立连接服务。
	//存储客户端的IP字符串。这里没有使用uint32类型的数字来表示IP，是因为用数字表示的话，
	//有一个网络字节序和本地字节序的问题，如ENetAddress结构体的注释中描述的。
	//ENetAddress.host必须是用网络字节序，而一般我们的认识是，数据在网络上传输时使用网络字节序，
	//一旦存储到本地，则使用本地字节序。这个地方有歧义。所以干脆使用字符串标记IP地址。
	enet_uint8 ClientIP[SoENet_MaxCount_IPString];
	//本成员为建立连接服务。
	//存储客户端的端口。
	enet_uint16 ClientPort;
	//本成员为断开连接服务。
	//记录断开连接的原因。值为SoDisconnectReason枚举值。
	enet_uint8 DisconnectReason;
	//记录本结构体的用途。值为SoENetUserDataType枚举值。
	enet_uint8 UserDataType;
} SoENetUserData;

#endif /* __ENET_TYPES_H__ */

