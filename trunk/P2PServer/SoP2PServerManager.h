//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#ifndef _SoP2PServerManager_h_
#define _SoP2PServerManager_h_
//--------------------------------------------------------------------
#include "SoP2PDefine.h"
#include "enet/enet.h"
class SoPeerForServer;
//--------------------------------------------------------------------
class SoP2PServerManager
{
public:
	SoP2PServerManager();
	~SoP2PServerManager();
	static SoP2PServerManager* GetInstance();

	bool InitP2PServerManager();
	void ReleaseP2PServerManager();
	//开启消息处理循环。
	void P2PServerRun();

private:
	//从m_pPeerList中找一个空元素，返回该元素的索引位置。
	//如果没有空元素，说明列表已满，则返回无效值。
	SoPeerID GenerateNewPeerID();

private:
	//本Server的生命期状态。
	enum eServerLifeStep
	{
		LifeStep_Starting, //正在启动中。
		LifeStep_Running, //运转中。
		LifeStep_Finishing, //外界要求结束Server的运行，正在关闭中。
		LifeStep_Finished, //已经结束。
	};

private:
	static SoP2PServerManager* ms_pInstance;
private:
	//服务器Host对象。
	ENetHost* m_Host;
	//记录本进程的IP和端口。
	ENetAddress m_Address;
	//记录生命期阶段。
	eServerLifeStep m_eLifeStep;
	//维护一个Peer列表，存放所有与本服务器相连接的客户端信息。
	SoPeerForServer* m_pPeerList;
	//PeerList的最大容量。
	soint32 m_nPeerListMaxSize;
};
//--------------------------------------------------------------------
inline SoP2PServerManager* SoP2PServerManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
