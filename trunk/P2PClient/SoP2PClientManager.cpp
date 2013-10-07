//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#include "SoP2PClientManager.h"
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
:m_pHost(0)
,m_pP2PServerPeer(0)
,m_eCurrentStepWithP2PServer(ConnectStep_P2PServer_None)
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
bool SoP2PClientManager::InitP2PClientManager()
{
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
	soint32 nResult = enet_host_service(m_pHost, &theEvent, 0);
	if (nResult > 0)
	{
		if (theEvent.type == ENET_EVENT_TYPE_CONNECT)
		{

		}
		else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE)
		{
			//cout <<"收到序号" <<event.peer->data <<"的数据,从" <<event.channelID <<"通道发送" <<endl;
			//cout <<"数据大小:" <<event.packet->dataLength <<endl;
			//cout <<"数据:" <<(char*)event.packet->data <<endl;
			enet_packet_destroy(theEvent.packet); //注意释放空间
			//cout <<endl;
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //失去连接
		{
			//cout <<"序号" <<event.peer->data <<"远程已经关闭连接" <<endl;
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
	m_pP2PServerPeer = enet_host_connect(m_pHost, &m_P2PServerAddress, 1, 222);
	if (m_pP2PServerPeer == 0)
	{
		return false;
	}
	//
	return true;
}
//--------------------------------------------------------------------
void SoP2PClientManager::DisconnectP2PServer()
{
	enet_peer_disconnect(m_pP2PServerPeer, 0);
	/* We've arrived here, so the disconnect attempt didn't */
	/* succeed yet.  Force the connection down.             */
	//enet_peer_reset (peer);
}
//--------------------------------------------------------------------
