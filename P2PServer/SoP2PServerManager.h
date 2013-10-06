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
	//<<<<<<<<<<<<<<<<<<< 子线程使用的函数 <<<<<<<<<<<<<<<<<<<<
	static unsigned int WINAPI ThreadProcess(void* pData);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

protected:
	//子线程运转的状态。
	enum eSubThreadState
	{
		SubThread_Running, //子线程运转中。
		SubThread_Finishing, //主线程要求结束子线程的运行；子线程正在关闭中。
		SubThread_Finished, //子线程已经结束。
	};

private:
	//记录本进程的IP和端口。
	ENetAddress m_Address;
	ENetHost* m_Host;
	//子线程。
	HANDLE m_hSubThread;
	unsigned int m_uiSubThreadID;

private:
	//线程锁。
	static CRITICAL_SECTION m_Lock;
	static eSubThreadState m_theSubThreadState;
};
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
