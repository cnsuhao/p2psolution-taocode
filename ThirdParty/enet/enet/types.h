/** 
 @file  types.h
 @brief type definitions for ENet
*/
#ifndef __ENET_TYPES_H__
#define __ENET_TYPES_H__

typedef unsigned char enet_uint8;       /**< unsigned 8-bit type  */
typedef unsigned short enet_uint16;     /**< unsigned 16-bit type */
typedef unsigned int enet_uint32;      /**< unsigned 32-bit type */
//oil ����64λ�޷�������
typedef unsigned long long int enet_uint64;

//IP�ַ������ֽڸ�����������������
#define SoENet_MaxCount_IPString 16

//��¼SoENetUserData�ṹ�����;
typedef enum _SoENetUserDataType
{
	UserData_None = 0, //��Чֵ�����ݶ�����Чֵ
	UserData_ClientConnectServer = 1, //�����Ӳ����У��ͻ�������P2P������
	UserData_ClientConnectClient = 2, //�����Ӳ����У��ͻ���������һ���ͻ���
	UserData_DisconnectBetweenCS = 3, //�ڶϿ����Ӳ����У��ͻ����������֮��Ͽ�������
	UserData_DisconnectBetweenCC = 4, //�ڶϿ����Ӳ����У��ͻ�����ͻ���֮��Ͽ�������
	UserData_DisconnectTimeout = 5, //ENet�ײ���⵽���ӳ�ʱ
} SoENetUserDataType;

//��ǶϿ����ӵ�ԭ��
typedef enum _SoDisconnectReason
{
	DisconnectReason_None = 0, //��Чֵ��
	DisconnectReason_PeerListIsFull = 1, //P2PServer������ͻ��˶Ͽ����ӣ�ԭ����������Ŀ�Ѵﵽ���ֵ��
	DisconnectReason_ServerShutdown = 2, //P2PServer������ͻ��˶Ͽ����ӣ�ԭ����P2PServer�����رա�
	DisconnectReason_ClientRequest = 3, //�ͻ���������P2PServer�Ͽ����ӡ�
	DisconnectReason_Timeout = 4, //���ӳ�ʱ�����������ӶϿ���
} SoDisconnectReason;

//�ڽ������ӺͶϿ����ӹ����У��û����ݵ��Զ������ݡ�
typedef struct _SoENetUserData
{
	//����ԱΪ�������ӷ���
	//ÿ���ͻ��˶���һ�������Լ���ClientID�����ID����GameServer����ģ�������P2PServer���䡣
	enet_uint64 ClientID;
	//����ԱΪ�������ӷ���
	//�洢�ͻ��˵�IP�ַ���������û��ʹ��uint32���͵���������ʾIP������Ϊ�����ֱ�ʾ�Ļ���
	//��һ�������ֽ���ͱ����ֽ�������⣬��ENetAddress�ṹ���ע���������ġ�
	//ENetAddress.host�������������ֽ��򣬶�һ�����ǵ���ʶ�ǣ������������ϴ���ʱʹ�������ֽ���
	//һ���洢�����أ���ʹ�ñ����ֽ�������ط������塣���Ըɴ�ʹ���ַ������IP��ַ��
	enet_uint8 ClientIP[SoENet_MaxCount_IPString];
	//����ԱΪ�������ӷ���
	//�洢�ͻ��˵Ķ˿ڡ�
	enet_uint16 ClientPort;
	//����ԱΪ�Ͽ����ӷ���
	//��¼�Ͽ����ӵ�ԭ��ֵΪSoDisconnectReasonö��ֵ��
	enet_uint8 DisconnectReason;
	//��¼���ṹ�����;��ֵΪSoENetUserDataTypeö��ֵ��
	enet_uint8 UserDataType;
} SoENetUserData;

#endif /* __ENET_TYPES_H__ */

