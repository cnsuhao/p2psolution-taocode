//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include "SoP2PServerManager.h"
#include "SoP2PProtocol.h"
#include "SoPeer.h"
#include "GGUILogAsyn.h"
//--------------------------------------------------------------------
//最大连接数目
//ENet源码中规定了最大连接数目是(ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF)
const soint32 gMaxPeerCount = 64;
//最大通道数目
const int gMaxChannelCount = 12;
//最大下载带宽，单位bytes/second。值为0表示不限制。
const int gIncomingBandwidth = 0;
//最大上传带宽，单位bytes/second。值为0表示不限制。
const int gOutgoingBandwidth = 0;
//
SoP2PServerManager* SoP2PServerManager::ms_pInstance = 0;
//--------------------------------------------------------------------
SoP2PServerManager::SoP2PServerManager()
:m_pHost(0)
,m_eLifeStep(LifeStep_None)
,m_pPeerList(0)
,m_nPeerListMaxSize(0)
{
	ms_pInstance = this;
}
//--------------------------------------------------------------------
SoP2PServerManager::~SoP2PServerManager()
{
	ReleaseP2PServerManager();
	ms_pInstance = 0;
}
//--------------------------------------------------------------------
bool SoP2PServerManager::InitP2PServerManager()
{
	m_eLifeStep = LifeStep_None;
	//初始化ENet库
	soint32 nResult = enet_initialize();
	if (nResult != 0)
	{
		LOG_ERROR("enet_initialize fail! nResult[%d]", nResult);
		return false;
	}
	//配置IP和端口
	m_Address.host = ENET_HOST_ANY;
	m_Address.port = 1234;
	//创建服务器host
	m_pHost = enet_host_create(&m_Address, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_pHost == NULL)
	{
		//创建主机对象失败。很可能是端口已经被占用，换个端口再试试。
		enet_deinitialize();
		LOG_ERROR("enet_host_create fail! Maybe Address:port[%d] have be taken, try to change the port", m_Address.port);
		return false;
	}
	//为PeerList分配内存空间。
	m_nPeerListMaxSize = gMaxPeerCount;
	m_pPeerList = new SoPeer[m_nPeerListMaxSize];
	//初始化成功。
	m_eLifeStep = LifeStep_Prepared;
	LOG_DEBUG("SoP2PServerManager::InitP2PServerManager() success!");
	return true;
}
//--------------------------------------------------------------------
void SoP2PServerManager::ReleaseP2PServerManager()
{
	if (m_pPeerList)
	{
		delete [] m_pPeerList;
		m_pPeerList = 0;
	}
	m_nPeerListMaxSize = 0;
	if (m_pHost)
	{
		//释放enet库。
		enet_deinitialize();
		m_pHost = 0;
	}
	m_eLifeStep = LifeStep_None;
	LOG_DEBUG("SoP2PServerManager::ReleaseP2PServerManager() finished");
}
//--------------------------------------------------------------------
void SoP2PServerManager::P2PServerRun()
{
	if (m_eLifeStep == LifeStep_Prepared)
	{
		//开始运转。
		DoLifeStep_Running();
	}
	else
	{
		LOG_WARING("SoP2PServerManager::P2PServerRun : not prepared!");
	}
}
//--------------------------------------------------------------------
void SoP2PServerManager::DoLifeStep_Running()
{
	m_eLifeStep = LifeStep_Running;
	//开始侦听。
	ENetEvent theEvent;
	const souint32 uiENetServiceTime = 5;
	while (m_eLifeStep != LifeStep_Finishing)
	{
		//nResult>0 : if an event occurred within the specified time limit
		//nResult==0 : if no event occurred
		//nResult<0 : on failure
		soint32 nResult = enet_host_service(m_pHost, &theEvent, uiENetServiceTime);
		if (nResult <= 0)
		{
			continue;
		}
		//处理消息。
		if (theEvent.type == ENET_EVENT_TYPE_CONNECT) //有客户端连接成功
		{
			//该客户端的ClientID
			SoClientID theClientID = (SoClientID)theEvent.UserData.ClientID;
			//为该客户端分配一个Peer对象
			SoPeerIndex theIndex = FindEmptyPeer(theClientID);
			if (theIndex == Invalid_SoPeerIndex)
			{
				//PeerList容器已满，达到连接上限。
				LOG_WARING("PeerList is full!");
				//服务器主动与这个客户端断开连接，并说明原因。
				SoENetUserData kUserData;
				kUserData.UserDataType = UserData_DisconnectBetweenCS;
				kUserData.DisconnectReason = DisconnectReason_PeerListIsFull;
				enet_peer_disconnect(theEvent.peer, &kUserData);
				//通知MonitorServer达到连接上限。
				//未完待续。
			}
			else
			{
				//初始化这个Peer对象。
				m_pPeerList[theIndex].InitPeer(theClientID, theIndex, theEvent.peer);
			}
		}
		else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE) //收到数据
		{
			const soint8 theProtoID = (soint8)(*(theEvent.packet->data));
			switch (theProtoID)
			{
			case SoP2PProtoID_Test_Req:
				{
					stP2PProtTestReq* pPackage = (stP2PProtTestReq*)theEvent.packet->data;
					//字符串必须含有结束符。
					pPackage->m_String[TestStringBuffSize-1] = 0;
					LOG_DEBUG("Receive stP2PProtTestReq : [%s]", pPackage->m_String);
					//给出回复。
					stP2PProtTestAck kAck;
					strcpy_s(kAck.m_String, sizeof(kAck.m_String), pPackage->m_String);
					ENetPacket* packet = enet_packet_create(&kAck, sizeof(kAck), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(theEvent.peer, 1, packet);
				}
				break;
			}
			//释放Packet内存。
			enet_packet_destroy(theEvent.packet);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //失去连接
		{
			//与客户端之间的连接断开了。
			switch (theEvent.UserData.DisconnectReason)
			{
			case DisconnectReason_PeerListIsFull:
				{
					//服务器的连接数目已满。
					//什么都不做。
				}
				break;
			case DisconnectReason_ServerShutdown:
				{
					//由于服务器关闭，主动与客户端断开连接。
					//这个事件只会在DoLifeStep_Finishing()中触发，并在DoLifeStep_Finishing()中处理。
					//什么都不做。
				}
				break;
			case DisconnectReason_ClientRequest:
				{
					//客户端主动请求断开连接。
					//此时，虽然theEvent.peer被执行了enet_peer_reset()函数，
					//但是theEvent.peer->data没有被清零，依然存储了该Peer对应的SoPeerIndex。
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			case DisconnectReason_Timeout:
				{
					//ENet底层侦测到连接超时。
					//此时，虽然theEvent.peer被执行了enet_peer_reset()函数，
					//但是theEvent.peer->data没有被清零，依然存储了该Peer对应的SoPeerIndex。
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			default:
				{
					//未知的DisconnectReason类型，报错。
					LOG_WARING("LifeStep_Running : Unknown DisconnectReason type!");
				}
				break;
			}
		}
	} //while
}
//--------------------------------------------------------------------
void SoP2PServerManager::DoLifeStep_Finishing()
{
	//开始进入服务器关闭流程。
	m_eLifeStep = LifeStep_Finishing;
	ENetEvent theEvent;
	//记录踢出客户端操作是否完成。
	bool bKickFinished = false;
	SoPeerIndex theKickIndex = 0;
	//踢出客户端操作完毕后，再等待若干秒钟。
	bool bWaitFinished = false;
	const souint32 uiENetServiceTime = 10;
	const souint32 uiMaxWaitTime = 3000;
	souint32 uiAlreadyWaitTime = 0;
	while (m_eLifeStep != LifeStep_Finished)
	{
		//尝试做踢出客户端的操作。
		if (IsValidIndex(theKickIndex))
		{
			if (!m_pPeerList[theKickIndex].IsEmpty())
			{
				//服务器主动与这个客户端断开连接，并说明原因。
				SoENetUserData kUserData;
				kUserData.UserDataType = UserData_DisconnectBetweenCS;
				kUserData.DisconnectReason = DisconnectReason_ServerShutdown;
				enet_peer_disconnect(m_pPeerList[theKickIndex].GetENetPeer(), &kUserData);
			}
			++theKickIndex;
		}
		else
		{
			bKickFinished = true;
		}
		//如果已经踢出完毕，则尝试做倒计时的操作。
		if (bKickFinished)
		{
			uiAlreadyWaitTime += uiENetServiceTime;
			if (uiAlreadyWaitTime >= uiMaxWaitTime)
			{
				bWaitFinished = true;
			}
		}
		//判断是否应该结束LifeStep_Finishing阶段。
		if (bKickFinished && bWaitFinished)
		{
			m_eLifeStep = LifeStep_Finished;
		}
		//执行ENet底层逻辑。
		soint32 nResult = enet_host_service(m_pHost, &theEvent, uiENetServiceTime);
		if (nResult <= 0)
		{
			continue;
		}
		//处理消息。
		if (theEvent.type == ENET_EVENT_TYPE_CONNECT) //有客户端连接成功
		{
			//服务器正在关闭过程中。
			//服务器主动与这个客户端断开连接，并说明原因。
			SoENetUserData kUserData;
			kUserData.UserDataType = UserData_DisconnectBetweenCS;
			kUserData.DisconnectReason = DisconnectReason_ServerShutdown;
			enet_peer_disconnect(theEvent.peer, &kUserData);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE) //收到数据
		{
			//对收到的数据不做任何处理。
			//释放Packet内存。
			enet_packet_destroy(theEvent.packet);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //失去连接
		{
			//与客户端之间的连接断开了。
			switch (theEvent.UserData.DisconnectReason)
			{
			case DisconnectReason_PeerListIsFull:
				{
					//服务器的连接数目已满。
					//在本函数内不会出现这个情况。
					//什么都不做。
				}
				break;
			case DisconnectReason_ServerShutdown:
				{
					//由于服务器关闭，主动与客户端断开连接。
					//此时，虽然theEvent.peer被执行了enet_peer_reset()函数，
					//但是theEvent.peer->data没有被清零，依然存储了该Peer对应的SoPeerIndex。
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			case DisconnectReason_ClientRequest:
				{
					//客户端主动请求断开连接。
					//此时，虽然theEvent.peer被执行了enet_peer_reset()函数，
					//但是theEvent.peer->data没有被清零，依然存储了该Peer对应的SoPeerIndex。
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			case DisconnectReason_Timeout:
				{
					//ENet底层侦测到连接超时。
					//此时，虽然theEvent.peer被执行了enet_peer_reset()函数，
					//但是theEvent.peer->data没有被清零，依然存储了该Peer对应的SoPeerIndex。
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			default:
				{
					//未知的DisconnectReason类型，报错。
					LOG_WARING("LifeStep_Finishing : Unknown DisconnectReason type!");
				}
				break;
			} //switch
		}
	} //while
	//LifeStep_Finishing阶段结束，m_eLifeStep的值为LifeStep_Finished。
	ReleaseP2PServerManager();
}
//--------------------------------------------------------------------
SoPeerIndex SoP2PServerManager::FindEmptyPeer(const SoClientID _ClientID) const
{
	SoPeerIndex nResult = Invalid_SoPeerIndex;
	if (_ClientID == Invalid_SoClientID)
	{
		return nResult;
	}
	//
	const soint32 nMaxCount = m_nPeerListMaxSize;
	soint32 nIndex = (soint32)(_ClientID % nMaxCount);
	for (soint32 i=0; i<nMaxCount; ++i)
	{
		nIndex += i;
		if (nIndex >= nMaxCount)
		{
			nIndex -= nMaxCount;
		}
		if (m_pPeerList[nIndex].IsEmpty())
		{
			nResult = nIndex;
			break;
		}
	}
	return nResult;
}
//--------------------------------------------------------------------
SoPeerIndex SoP2PServerManager::FindPeerByClientID(const SoClientID _ClientID) const
{
	SoPeerIndex nResult = Invalid_SoPeerIndex;
	if (_ClientID == Invalid_SoClientID)
	{
		return nResult;
	}
	//
	const soint32 nMaxCount = m_nPeerListMaxSize;
	soint32 nIndex = (soint32)(_ClientID % nMaxCount);
	for (soint32 i=0; i<nMaxCount; ++i)
	{
		nIndex += i;
		if (nIndex >= nMaxCount)
		{
			nIndex -= nMaxCount;
		}
		if (m_pPeerList[nIndex].GetClientID() == _ClientID)
		{
			nResult = nIndex;
			break;
		}
	}
	return nResult;
}
//--------------------------------------------------------------------
