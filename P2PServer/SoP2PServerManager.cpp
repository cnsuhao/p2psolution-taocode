//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include <stdio.h>
#include "SoP2PServerManager.h"
#include "SoPeer.h"
//--------------------------------------------------------------------
//最大连接数目
//ENet源码中规定了最大连接数目是(ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF)
const soint64 gMaxPeerCount = 64;
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
,m_eLifeStep(LifeStep_Starting)
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
	m_eLifeStep = LifeStep_Starting;
	//初始化ENet库
	soint32 nResult = enet_initialize();
	if (nResult != 0)
	{
		printf("[ERROR]enet_initialize fail; nResult[%d]\n", nResult);
		return false;
	}
	//配置IP和端口
	m_Address.host = ENET_HOST_ANY;
	m_Address.port = 1234;
	//
	m_pHost = enet_host_create(&m_Address, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_pHost == NULL)
	{
		//创建主机对象失败。很可能是端口已经被占用，换个端口再试试。
		enet_deinitialize();
		printf("[ERROR]Create server host fail\n");
		return false;
	}
	//为PeerList分配内存空间。
	m_nPeerListMaxSize = gMaxPeerCount;
	m_pPeerList = new SoPeer[m_nPeerListMaxSize];
	//
	printf("SoP2PServerManager::InitP2PServerManager() success!\n");
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
	m_eLifeStep = LifeStep_Finished;
	printf("SoP2PServerManager::ReleaseP2PServerManager() success!\n");
}
//--------------------------------------------------------------------
void SoP2PServerManager::P2PServerRun()
{
	if (m_pHost == NULL)
	{
		m_eLifeStep = LifeStep_Finished;
		printf("[ERROR]SoP2PServerManager::P2PServerRun() fail; m_pHost == NULL\n");
		return;
	}
	m_eLifeStep = LifeStep_Running;
	//开始侦听。
	ENetEvent theEvent;
	while (m_eLifeStep != LifeStep_Finishing)
	{
		//nResult>0 : if an event occurred within the specified time limit
		//nResult==0 : if no event occurred
		//nResult<0 : on failure
		soint32 nResult = enet_host_service(m_pHost, &theEvent, 10);
		if (nResult > 0)
		{
			if (theEvent.type == ENET_EVENT_TYPE_CONNECT) //有客户端连接成功
			{
				//什么都不做。等待客户端发送逻辑上的正式请求连接。
				//当收到正式请求连接后，再为其分配一个Peer对象。


				//SoPeerIndex newPeerID = FindEmptyPeer();
				//if (newPeerID == Invalid_SoPeerIndex)
				//{
				//	//PeerList容器已满，达到连接上限。
				//	//通知这个客户端达到连接上限。
				//	//通知MonitorServer达到连接上限。
				//}
				//else
				//{
				//	//把newPeerID保存在theEvent.peer的自定义数据内，
				//	//让theEvent.peer知道自己在PeerList中的索引地址。
				//	theEvent.peer->data = (void*)newPeerID;
				//	m_pPeerList[newPeerID].InitPeer(newPeerID, theEvent.peer);
				//}
			}
			else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE) //收到数据
			{
				//cout <<"收到序号" <<event.peer->data <<"的数据,从" <<event.channelID <<"通道发送" <<endl;
				//cout <<"数据大小:" <<event.packet->dataLength <<endl;
				//cout <<"数据:" <<(char*)event.packet->data <<endl;
				enet_packet_destroy(theEvent.packet);    //注意释放空间
				//cout <<endl;
			}
			else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //失去连接
			{
				//cout <<"序号" <<event.peer->data <<"远程已经关闭连接" <<endl;
			}
		}
		else
		{

		}
	} //while
	//
	if (m_eLifeStep == LifeStep_Finishing)
	{
		m_eLifeStep = LifeStep_Finished;
		printf("m_eLifeStep = LifeStep_Finished\n");
	}
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
	const soint64 nMaxCount = m_nPeerListMaxSize;
	soint64 nIndex = _ClientID % nMaxCount;
	for (soint64 i=0; i<nMaxCount; ++i)
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
	const soint64 nMaxCount = m_nPeerListMaxSize;
	soint64 nIndex = _ClientID % nMaxCount;
	for (soint64 i=0; i<nMaxCount; ++i)
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
