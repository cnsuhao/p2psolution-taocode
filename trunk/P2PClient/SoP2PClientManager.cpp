//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#include "SoP2PClientManager.h"
#include "SoP2PProtocol.h"
#include <stdio.h>
//--------------------------------------------------------------------
//最大连接数目
//ENet源码中规定了最大连接数目是(ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF)
const int gMaxPeerCount = 9;
//最大通道数目
const int gMaxChannelCount = 12;
//最大下载带宽，单位bytes/second。值为0表示不限制。
const int gIncomingBandwidth = 0;
//最大上传带宽，单位bytes/second。值为0表示不限制。
const int gOutgoingBandwidth = 0;
//
SoP2PClientManager* SoP2PClientManager::ms_pInstance = 0;
//--------------------------------------------------------------------
SoP2PClientManager::SoP2PClientManager()
:m_theClientID(Invalid_SoClientID)
,m_pHost(0)
,m_pP2PServerPeer(0)
,m_eLifeStep(ClientLifeStep_None)
{
	ms_pInstance = this;
}
//--------------------------------------------------------------------
SoP2PClientManager::~SoP2PClientManager()
{
	ReleaseP2PClientManager();
	ms_pInstance = 0;
}
//--------------------------------------------------------------------
bool SoP2PClientManager::InitP2PClientManager(SoClientID _ClientID)
{
	m_theClientID = _ClientID;
	//初始化ENet库
	soint32 nResult = enet_initialize();
	if (nResult != 0)
	{
		return false;
	}
	//创建本地host对象
	m_pHost = enet_host_create(NULL, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_pHost == NULL)
	{
		return false;
	}
	return true;
}
//--------------------------------------------------------------------
void SoP2PClientManager::ReleaseP2PClientManager()
{
	if (m_pHost)
	{
		//释放enet库。
		enet_deinitialize();
		m_pHost = 0;
	}
}
//--------------------------------------------------------------------
void SoP2PClientManager::UpdateP2PClientManager()
{
	//nResult>0 : if an event occurred within the specified time limit
	//nResult==0 : if no event occurred
	//nResult<0 : on failure
	ENetEvent theEvent;
	const souint32 uiENetServiceTime = 5;
	soint32 nResult = enet_host_service(m_pHost, &theEvent, uiENetServiceTime);
	if (nResult > 0)
	{
		if (theEvent.type == ENET_EVENT_TYPE_CONNECT)
		{
			//分析连接情况。
			if (theEvent.UserData.UserDataType == UserData_ClientConnectServer)
			{
				//本客户端主动连接了P2PServer。
				m_eLifeStep = ClientLifeStep_ConnectedInENet;
			}
		}
		else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE)
		{
			soint8 theProtoID = (soint8)(*(theEvent.packet->data));
			switch (theProtoID)
			{
			case SoP2PProtoID_Test_Req:
				{
					stP2PProtTestReq* pPackage = (stP2PProtTestReq*)theEvent.packet->data;
					//字符串必须含有结束符。
					pPackage->m_String[TestStringBuffSize-1] = 0;
					printf("Receive stP2PProtTestReq : [%s]\n", pPackage->m_String);
					//给出回复。
					stP2PProtTestAck kAck;
					strcpy_s(kAck.m_String, sizeof(kAck.m_String), pPackage->m_String);
					ENetPacket* packet = enet_packet_create(&kAck, sizeof(kAck), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(theEvent.peer, 1, packet);
				}
				break;
			case SoP2PProtoID_Test_Ack:
				{
					stP2PProtTestAck* pPackage = (stP2PProtTestAck*)theEvent.packet->data;
					//字符串必须含有结束符。
					pPackage->m_String[TestStringBuffSize-1] = 0;
					printf("Receive stP2PProtTestAck : [%s]\n", pPackage->m_String);
				}
				break;
			}
			//释放Packet内存。
			enet_packet_destroy(theEvent.packet);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //失去连接
		{
			//分析断开连接的原因。
			switch (theEvent.UserData.DisconnectReason)
			{
			case DisconnectReason_PeerListIsFull:
				{
					//服务器连接数目已达到最大值。
					printf("DisconnectReason_PeerListIsFull\n");
				}
				break;
			case DisconnectReason_ServerShutdown:
				{
					//P2PServer即将关闭。
					printf("DisconnectReason_ServerShutdown\n");
				}
				break;
			case DisconnectReason_ClientRequest:
				{
					if (theEvent.UserData.UserDataType == UserData_DisconnectBetweenCS)
					{
						//主动与P2PServer断开连接。
						printf("DisconnectReason_ClientRequest : DisconnectBetweenCS\n");
					}
					else if (theEvent.UserData.UserDataType == UserData_DisconnectBetweenCC)
					{
						//主动与其他客户端断开连接。
						printf("DisconnectReason_ClientRequest : DisconnectBetweenCC\n");
					}
				}
				break;
			case DisconnectReason_Timeout:
				{
					//ENet底层侦测到连接超时。
					//这里要根据Peer对象来判断是与P2PServer断开了连接，还是与其他客户端断开了连接。
					if (m_pP2PServerPeer == theEvent.peer)
					{
						//与P2PServer断开了连接。
						printf("DisconnectReason_Timeout : P2PServer\n");
					}
					else
					{
						printf("DisconnectReason_Timeout : other client\n");
					}
				}
				break;
			}
		}
	}
}
//--------------------------------------------------------------------
void SoP2PClientManager::SetP2PServerInfo(const char* pszIP, souint16 nPort)
{
	enet_address_set_host(&m_P2PServerAddress, pszIP);
	m_P2PServerAddress.port = nPort;
}
//--------------------------------------------------------------------
bool SoP2PClientManager::ConnectP2PServer()
{
	souint32 channelCount = 3;
	SoENetUserData kUserData;
	kUserData.UserDataType = UserData_ClientConnectServer;
	kUserData.ClientID = (enet_uint64)m_theClientID;
	kUserData.ClientIP[0] = 5;
	kUserData.ClientIP[1] = 6;
	kUserData.ClientIP[2] = 7;
	kUserData.ClientIP[3] = 0;
	kUserData.ClientPort = 99;
	//
	m_pP2PServerPeer = enet_host_connect(m_pHost, &m_P2PServerAddress, channelCount, &kUserData);
	if (m_pP2PServerPeer == 0)
	{
		return false;
	}
	//
	m_eLifeStep = ClientLifeStep_ConnectStart;
	return true;
}
//--------------------------------------------------------------------
void SoP2PClientManager::DisconnectP2PServer()
{
	SoENetUserData kUserData;
	kUserData.UserDataType = UserData_DisconnectBetweenCS;
	kUserData.DisconnectReason = DisconnectReason_ClientRequest;
	//使用这个函数来断开连接，保证先前排在发包序列中的协议包都发送完毕后再断开连接。
	enet_peer_disconnect_later(m_pP2PServerPeer, &kUserData);
}
//--------------------------------------------------------------------
void SoP2PClientManager::TalkToP2PServer(const char* pszWords)
{
	//发出一个测试包。
	stP2PProtTestReq kReq;
	strcpy_s(kReq.m_String, sizeof(kReq.m_String), pszWords);
	ENetPacket* packet = enet_packet_create(&kReq, sizeof(kReq), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(m_pP2PServerPeer, 1, packet);
}
//--------------------------------------------------------------------
