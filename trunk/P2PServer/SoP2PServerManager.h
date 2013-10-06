//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#ifndef _SoP2PServerManager_h_
#define _SoP2PServerManager_h_
//--------------------------------------------------------------------
#include <enet/enet.h>
//--------------------------------------------------------------------
class SoP2PServerManager
{
public:
	SoP2PServerManager();
	~SoP2PServerManager();

	bool InitP2PServerManager();
	void ReleaseP2PServerManager();

protected:
	//<<<<<<<<<<<<<<<<<<< ���߳�ʹ�õĺ��� <<<<<<<<<<<<<<<<<<<<
	static unsigned int WINAPI ThreadProcess(void* pData);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

protected:
	//���߳���ת��״̬��
	enum eSubThreadState
	{
		SubThread_Running, //���߳���ת�С�
		SubThread_Finishing, //���߳�Ҫ��������̵߳����У����߳����ڹر��С�
		SubThread_Finished, //���߳��Ѿ�������
	};

private:
	//��¼�����̵�IP�Ͷ˿ڡ�
	ENetAddress m_Address;
	ENetHost* m_Host;
	//���̡߳�
	HANDLE m_hSubThread;
	unsigned int m_uiSubThreadID;

private:
	//�߳�����
	static CRITICAL_SECTION m_Lock;
	static eSubThreadState m_theSubThreadState;
};
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
