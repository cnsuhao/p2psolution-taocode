//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include <process.h>
#include "SoP2PServerManager.h"
//--------------------------------------------------------------------
//���������Ŀ
const int gMaxPeerCount = 64;
//���ͨ����Ŀ
const int gMaxChannelCount = 12;
//������ش�����λbytes/second��ֵΪ0��ʾ�����ơ�
const int gIncomingBandwidth = 0;
//����ϴ�������λbytes/second��ֵΪ0��ʾ�����ơ�
const int gOutgoingBandwidth = 0;
CRITICAL_SECTION SoP2PServerManager::m_Lock;
SoP2PServerManager::eSubThreadState SoP2PServerManager::m_theSubThreadState = SoP2PServerManager::SubThread_Finished;
//--------------------------------------------------------------------
SoP2PServerManager::SoP2PServerManager()
:m_Host(0)
,m_hSubThread(0)
,m_uiSubThreadID(0)
{
	InitializeCriticalSection(&m_Lock);
}
//--------------------------------------------------------------------
SoP2PServerManager::~SoP2PServerManager()
{
	ReleaseP2PServerManager();
	DeleteCriticalSection(&m_Lock);
}
//--------------------------------------------------------------------
bool SoP2PServerManager::InitP2PServerManager()
{
	//��ʼ��enet��
	if (enet_initialize())
	{
		return false;
	}
	//����IP�Ͷ˿�
	m_Address.host = ENET_HOST_ANY;
	m_Address.port = 1234;
	//
	m_Host = enet_host_create(&m_Address, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_Host == NULL)
	{
		//������������ʧ�ܡ��ܿ����Ƕ˿��Ѿ���ռ�ã������˿������ԡ�
		enet_deinitialize();
		return false;
	}
	//�����̡߳�
	m_hSubThread = (HANDLE)_beginthreadex(NULL, 0, SoP2PServerManager::ThreadProcess,
		m_Host, CREATE_SUSPENDED, &m_uiSubThreadID);
	if (m_hSubThread == NULL)
	{
		//::MessageBox(NULL, TEXT("_beginthreadex fail"), TEXT("GGUILogAsyn::InitLog"), MB_OK);
		return false;
	}
	m_theSubThreadState = SubThread_Running;
	ResumeThread(m_hSubThread);
	//
	return true;
}
//--------------------------------------------------------------------
void SoP2PServerManager::ReleaseP2PServerManager()
{
	if (m_Host)
	{
		//�������̡߳�
		if (m_theSubThreadState == SubThread_Running)
		{
			m_theSubThreadState = SubThread_Finishing;
			WaitForSingleObject(m_hSubThread, INFINITE);
		}
		if (m_hSubThread)
		{
			CloseHandle(m_hSubThread);
		}
		m_uiSubThreadID = 0;
		//�ͷ�enet�⡣
		enet_deinitialize();
		m_Host = 0;
	}
}
//--------------------------------------------------------------------
unsigned int WINAPI SoP2PServerManager::ThreadProcess(void* pData)
{
	ENetHost* theHost = (ENetHost*)pData;
	if (theHost == NULL)
	{
		return 0;
	}
	//��ʼ������
	ENetEvent theEvent;
	while (m_theSubThreadState != SubThread_Finished)
	{
		//nResult>0 : if an event occurred within the specified time limit
		//nResult==0 : if no event occurred
		//nResult<0 : on failure
		int nResult = enet_host_service(theHost, &theEvent, 10);
		if (nResult >= 0)
		{
			if(theEvent.type==ENET_EVENT_TYPE_CONNECT) //�пͻ������ӳɹ�
			{
				ENetAddress remote=theEvent.peer->address; //Զ�̵�ַ
				char ip[256];
				enet_address_get_host_ip(&remote,ip,256);
				//cout <<"ip:" <<ip <<" �Ѿ�����,���:" <<num <<endl;
				//theEvent.peer->data=(void*)num++;
			}
			else if(theEvent.type==ENET_EVENT_TYPE_RECEIVE) //�յ�����
			{
				//cout <<"�յ����" <<event.peer->data <<"������,��" <<event.channelID <<"ͨ������" <<endl;
				//cout <<"���ݴ�С:" <<event.packet->dataLength <<endl;
				//cout <<"����:" <<(char*)event.packet->data <<endl;
				enet_packet_destroy(theEvent.packet);    //ע���ͷſռ�
				//cout <<endl;
			}
			else if(theEvent.type==ENET_EVENT_TYPE_DISCONNECT) //ʧȥ����
			{
				//cout <<"���" <<event.peer->data <<"Զ���Ѿ��ر�����" <<endl;
			}
		}
		else
		{

		}
		//
		if (m_theSubThreadState == SubThread_Finishing)
		{
			m_theSubThreadState = SubThread_Finished;
		}
	}
	return 1;
}