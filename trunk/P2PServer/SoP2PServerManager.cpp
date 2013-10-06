//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include <process.h>
#include "SoP2PServerManager.h"
//--------------------------------------------------------------------
//最大连接数目
const int gMaxPeerCount = 64;
//最大通道数目
const int gMaxChannelCount = 12;
//最大下载带宽，单位bytes/second。值为0表示不限制。
const int gIncomingBandwidth = 0;
//最大上传带宽，单位bytes/second。值为0表示不限制。
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
	//初始化enet库
	if (enet_initialize())
	{
		return false;
	}
	//配置IP和端口
	m_Address.host = ENET_HOST_ANY;
	m_Address.port = 1234;
	//
	m_Host = enet_host_create(&m_Address, gMaxPeerCount, gMaxChannelCount, gIncomingBandwidth, gOutgoingBandwidth);
	if (m_Host == NULL)
	{
		//创建主机对象失败。很可能是端口已经被占用，换个端口再试试。
		enet_deinitialize();
		return false;
	}
	//创建线程。
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
		//结束子线程。
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
		//释放enet库。
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
	//开始侦听。
	ENetEvent theEvent;
	while (m_theSubThreadState != SubThread_Finished)
	{
		//nResult>0 : if an event occurred within the specified time limit
		//nResult==0 : if no event occurred
		//nResult<0 : on failure
		int nResult = enet_host_service(theHost, &theEvent, 10);
		if (nResult >= 0)
		{
			if(theEvent.type==ENET_EVENT_TYPE_CONNECT) //有客户机连接成功
			{
				ENetAddress remote=theEvent.peer->address; //远程地址
				char ip[256];
				enet_address_get_host_ip(&remote,ip,256);
				//cout <<"ip:" <<ip <<" 已经连接,序号:" <<num <<endl;
				//theEvent.peer->data=(void*)num++;
			}
			else if(theEvent.type==ENET_EVENT_TYPE_RECEIVE) //收到数据
			{
				//cout <<"收到序号" <<event.peer->data <<"的数据,从" <<event.channelID <<"通道发送" <<endl;
				//cout <<"数据大小:" <<event.packet->dataLength <<endl;
				//cout <<"数据:" <<(char*)event.packet->data <<endl;
				enet_packet_destroy(theEvent.packet);    //注意释放空间
				//cout <<endl;
			}
			else if(theEvent.type==ENET_EVENT_TYPE_DISCONNECT) //失去连接
			{
				//cout <<"序号" <<event.peer->data <<"远程已经关闭连接" <<endl;
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