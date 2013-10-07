//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#include "SoP2PClientManager.h"
//--------------------------------------------------------------------
//���������Ŀ
//ENetԴ���й涨�����������Ŀ��(ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF)
const int gMaxPeerCount = 9;
//���ͨ����Ŀ
const int gMaxChannelCount = 12;
//������ش�����λbytes/second��ֵΪ0��ʾ�����ơ�
const int gIncomingBandwidth = 0;
//����ϴ�������λbytes/second��ֵΪ0��ʾ�����ơ�
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
	//��ʼ��ENet��
	soint32 nResult = enet_initialize();
	if (nResult != 0)
	{
		return false;
	}
	//��������host����
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
		//�ͷ�enet�⡣
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
			//cout <<"�յ����" <<event.peer->data <<"������,��" <<event.channelID <<"ͨ������" <<endl;
			//cout <<"���ݴ�С:" <<event.packet->dataLength <<endl;
			//cout <<"����:" <<(char*)event.packet->data <<endl;
			enet_packet_destroy(theEvent.packet); //ע���ͷſռ�
			//cout <<endl;
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //ʧȥ����
		{
			//cout <<"���" <<event.peer->data <<"Զ���Ѿ��ر�����" <<endl;
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
