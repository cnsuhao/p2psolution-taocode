//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include <stdio.h>
#include "SoP2PServerManager.h"
#include "SoPeer.h"
//--------------------------------------------------------------------
//���������Ŀ
//ENetԴ���й涨�����������Ŀ��(ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF)
const soint64 gMaxPeerCount = 64;
//���ͨ����Ŀ
const int gMaxChannelCount = 12;
//������ش�����λbytes/second��ֵΪ0��ʾ�����ơ�
const int gIncomingBandwidth = 0;
//����ϴ�������λbytes/second��ֵΪ0��ʾ�����ơ�
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
	//��ʼ��ENet��
	soint32 nResult = enet_initialize();
	if (nResult != 0)
	{
		printf("[ERROR]enet_initialize fail; nResult[%d]\n", nResult);
		return false;
	}
	//����IP�Ͷ˿�
	m_Address.host = ENET_HOST_ANY;
	m_Address.port = 1234;
	//
	m_pHost = enet_host_create(&m_Address, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_pHost == NULL)
	{
		//������������ʧ�ܡ��ܿ����Ƕ˿��Ѿ���ռ�ã������˿������ԡ�
		enet_deinitialize();
		printf("[ERROR]Create server host fail\n");
		return false;
	}
	//ΪPeerList�����ڴ�ռ䡣
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
		//�ͷ�enet�⡣
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
	//��ʼ������
	ENetEvent theEvent;
	while (m_eLifeStep != LifeStep_Finishing)
	{
		//nResult>0 : if an event occurred within the specified time limit
		//nResult==0 : if no event occurred
		//nResult<0 : on failure
		soint32 nResult = enet_host_service(m_pHost, &theEvent, 10);
		if (nResult > 0)
		{
			if (theEvent.type == ENET_EVENT_TYPE_CONNECT) //�пͻ������ӳɹ�
			{
				//ʲô���������ȴ��ͻ��˷����߼��ϵ���ʽ�������ӡ�
				//���յ���ʽ�������Ӻ���Ϊ�����һ��Peer����


				//SoPeerIndex newPeerID = FindEmptyPeer();
				//if (newPeerID == Invalid_SoPeerIndex)
				//{
				//	//PeerList�����������ﵽ�������ޡ�
				//	//֪ͨ����ͻ��˴ﵽ�������ޡ�
				//	//֪ͨMonitorServer�ﵽ�������ޡ�
				//}
				//else
				//{
				//	//��newPeerID������theEvent.peer���Զ��������ڣ�
				//	//��theEvent.peer֪���Լ���PeerList�е�������ַ��
				//	theEvent.peer->data = (void*)newPeerID;
				//	m_pPeerList[newPeerID].InitPeer(newPeerID, theEvent.peer);
				//}
			}
			else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE) //�յ�����
			{
				//cout <<"�յ����" <<event.peer->data <<"������,��" <<event.channelID <<"ͨ������" <<endl;
				//cout <<"���ݴ�С:" <<event.packet->dataLength <<endl;
				//cout <<"����:" <<(char*)event.packet->data <<endl;
				enet_packet_destroy(theEvent.packet);    //ע���ͷſռ�
				//cout <<endl;
			}
			else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //ʧȥ����
			{
				//cout <<"���" <<event.peer->data <<"Զ���Ѿ��ر�����" <<endl;
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
