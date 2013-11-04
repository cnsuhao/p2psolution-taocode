//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include "SoP2PServerManager.h"
#include "SoP2PProtocol.h"
#include "SoPeer.h"
#include "GGUILogAsyn.h"
//--------------------------------------------------------------------
//���������Ŀ
//ENetԴ���й涨�����������Ŀ��(ENET_PROTOCOL_MAXIMUM_PEER_ID = 0xFFF)
const soint32 gMaxPeerCount = 64;
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
	//��ʼ��ENet��
	soint32 nResult = enet_initialize();
	if (nResult != 0)
	{
		LOG_ERROR("enet_initialize fail! nResult[%d]", nResult);
		return false;
	}
	//����IP�Ͷ˿�
	m_Address.host = ENET_HOST_ANY;
	m_Address.port = 1234;
	//����������host
	m_pHost = enet_host_create(&m_Address, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_pHost == NULL)
	{
		//������������ʧ�ܡ��ܿ����Ƕ˿��Ѿ���ռ�ã������˿������ԡ�
		enet_deinitialize();
		LOG_ERROR("enet_host_create fail! Maybe Address:port[%d] have be taken, try to change the port", m_Address.port);
		return false;
	}
	//ΪPeerList�����ڴ�ռ䡣
	m_nPeerListMaxSize = gMaxPeerCount;
	m_pPeerList = new SoPeer[m_nPeerListMaxSize];
	//��ʼ���ɹ���
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
		//�ͷ�enet�⡣
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
		//��ʼ��ת��
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
	//��ʼ������
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
		//������Ϣ��
		if (theEvent.type == ENET_EVENT_TYPE_CONNECT) //�пͻ������ӳɹ�
		{
			//�ÿͻ��˵�ClientID
			SoClientID theClientID = (SoClientID)theEvent.UserData.ClientID;
			//Ϊ�ÿͻ��˷���һ��Peer����
			SoPeerIndex theIndex = FindEmptyPeer(theClientID);
			if (theIndex == Invalid_SoPeerIndex)
			{
				//PeerList�����������ﵽ�������ޡ�
				LOG_WARING("PeerList is full!");
				//����������������ͻ��˶Ͽ����ӣ���˵��ԭ��
				SoENetUserData kUserData;
				kUserData.UserDataType = UserData_DisconnectBetweenCS;
				kUserData.DisconnectReason = DisconnectReason_PeerListIsFull;
				enet_peer_disconnect(theEvent.peer, &kUserData);
				//֪ͨMonitorServer�ﵽ�������ޡ�
				//δ�������
			}
			else
			{
				//��ʼ�����Peer����
				m_pPeerList[theIndex].InitPeer(theClientID, theIndex, theEvent.peer);
			}
		}
		else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE) //�յ�����
		{
			const soint8 theProtoID = (soint8)(*(theEvent.packet->data));
			switch (theProtoID)
			{
			case SoP2PProtoID_Test_Req:
				{
					stP2PProtTestReq* pPackage = (stP2PProtTestReq*)theEvent.packet->data;
					//�ַ������뺬�н�������
					pPackage->m_String[TestStringBuffSize-1] = 0;
					LOG_DEBUG("Receive stP2PProtTestReq : [%s]", pPackage->m_String);
					//�����ظ���
					stP2PProtTestAck kAck;
					strcpy_s(kAck.m_String, sizeof(kAck.m_String), pPackage->m_String);
					ENetPacket* packet = enet_packet_create(&kAck, sizeof(kAck), ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(theEvent.peer, 1, packet);
				}
				break;
			}
			//�ͷ�Packet�ڴ档
			enet_packet_destroy(theEvent.packet);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //ʧȥ����
		{
			//��ͻ���֮������ӶϿ��ˡ�
			switch (theEvent.UserData.DisconnectReason)
			{
			case DisconnectReason_PeerListIsFull:
				{
					//��������������Ŀ������
					//ʲô��������
				}
				break;
			case DisconnectReason_ServerShutdown:
				{
					//���ڷ������رգ�������ͻ��˶Ͽ����ӡ�
					//����¼�ֻ����DoLifeStep_Finishing()�д���������DoLifeStep_Finishing()�д���
					//ʲô��������
				}
				break;
			case DisconnectReason_ClientRequest:
				{
					//�ͻ�����������Ͽ����ӡ�
					//��ʱ����ȻtheEvent.peer��ִ����enet_peer_reset()������
					//����theEvent.peer->dataû�б����㣬��Ȼ�洢�˸�Peer��Ӧ��SoPeerIndex��
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			case DisconnectReason_Timeout:
				{
					//ENet�ײ���⵽���ӳ�ʱ��
					//��ʱ����ȻtheEvent.peer��ִ����enet_peer_reset()������
					//����theEvent.peer->dataû�б����㣬��Ȼ�洢�˸�Peer��Ӧ��SoPeerIndex��
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			default:
				{
					//δ֪��DisconnectReason���ͣ�����
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
	//��ʼ����������ر����̡�
	m_eLifeStep = LifeStep_Finishing;
	ENetEvent theEvent;
	//��¼�߳��ͻ��˲����Ƿ���ɡ�
	bool bKickFinished = false;
	SoPeerIndex theKickIndex = 0;
	//�߳��ͻ��˲�����Ϻ��ٵȴ��������ӡ�
	bool bWaitFinished = false;
	const souint32 uiENetServiceTime = 10;
	const souint32 uiMaxWaitTime = 3000;
	souint32 uiAlreadyWaitTime = 0;
	while (m_eLifeStep != LifeStep_Finished)
	{
		//�������߳��ͻ��˵Ĳ�����
		if (IsValidIndex(theKickIndex))
		{
			if (!m_pPeerList[theKickIndex].IsEmpty())
			{
				//����������������ͻ��˶Ͽ����ӣ���˵��ԭ��
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
		//����Ѿ��߳���ϣ�����������ʱ�Ĳ�����
		if (bKickFinished)
		{
			uiAlreadyWaitTime += uiENetServiceTime;
			if (uiAlreadyWaitTime >= uiMaxWaitTime)
			{
				bWaitFinished = true;
			}
		}
		//�ж��Ƿ�Ӧ�ý���LifeStep_Finishing�׶Ρ�
		if (bKickFinished && bWaitFinished)
		{
			m_eLifeStep = LifeStep_Finished;
		}
		//ִ��ENet�ײ��߼���
		soint32 nResult = enet_host_service(m_pHost, &theEvent, uiENetServiceTime);
		if (nResult <= 0)
		{
			continue;
		}
		//������Ϣ��
		if (theEvent.type == ENET_EVENT_TYPE_CONNECT) //�пͻ������ӳɹ�
		{
			//���������ڹرչ����С�
			//����������������ͻ��˶Ͽ����ӣ���˵��ԭ��
			SoENetUserData kUserData;
			kUserData.UserDataType = UserData_DisconnectBetweenCS;
			kUserData.DisconnectReason = DisconnectReason_ServerShutdown;
			enet_peer_disconnect(theEvent.peer, &kUserData);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_RECEIVE) //�յ�����
		{
			//���յ������ݲ����κδ���
			//�ͷ�Packet�ڴ档
			enet_packet_destroy(theEvent.packet);
		}
		else if (theEvent.type == ENET_EVENT_TYPE_DISCONNECT) //ʧȥ����
		{
			//��ͻ���֮������ӶϿ��ˡ�
			switch (theEvent.UserData.DisconnectReason)
			{
			case DisconnectReason_PeerListIsFull:
				{
					//��������������Ŀ������
					//�ڱ������ڲ��������������
					//ʲô��������
				}
				break;
			case DisconnectReason_ServerShutdown:
				{
					//���ڷ������رգ�������ͻ��˶Ͽ����ӡ�
					//��ʱ����ȻtheEvent.peer��ִ����enet_peer_reset()������
					//����theEvent.peer->dataû�б����㣬��Ȼ�洢�˸�Peer��Ӧ��SoPeerIndex��
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			case DisconnectReason_ClientRequest:
				{
					//�ͻ�����������Ͽ����ӡ�
					//��ʱ����ȻtheEvent.peer��ִ����enet_peer_reset()������
					//����theEvent.peer->dataû�б����㣬��Ȼ�洢�˸�Peer��Ӧ��SoPeerIndex��
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			case DisconnectReason_Timeout:
				{
					//ENet�ײ���⵽���ӳ�ʱ��
					//��ʱ����ȻtheEvent.peer��ִ����enet_peer_reset()������
					//����theEvent.peer->dataû�б����㣬��Ȼ�洢�˸�Peer��Ӧ��SoPeerIndex��
					SoPeerIndex thePeerIndex = (SoPeerIndex)theEvent.peer->data;
					if (IsValidIndex(thePeerIndex))
					{
						m_pPeerList[thePeerIndex].ReleasePeer();
					}
				}
				break;
			default:
				{
					//δ֪��DisconnectReason���ͣ�����
					LOG_WARING("LifeStep_Finishing : Unknown DisconnectReason type!");
				}
				break;
			} //switch
		}
	} //while
	//LifeStep_Finishing�׶ν�����m_eLifeStep��ֵΪLifeStep_Finished��
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
