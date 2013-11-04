//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#ifndef _SoP2PServerManager_h_
#define _SoP2PServerManager_h_
//--------------------------------------------------------------------
#include "SoP2PServerDefine.h"
class SoPeer;
//--------------------------------------------------------------------
class SoP2PServerManager
{
public:
	SoP2PServerManager();
	~SoP2PServerManager();
	static SoP2PServerManager* GetInstance();

	//初始化ENet库，并预先创建SoPeer对象列表。
	bool InitP2PServerManager();
	//删除所有的SoPeer对象，并结束ENet库。
	void ReleaseP2PServerManager();
	//开启消息处理循环。
	void P2PServerRun();

private:
	//处理在LifeStep_Running阶段的业务。
	void DoLifeStep_Running();
	//处理在LifeStep_Finishing阶段的业务。
	void DoLifeStep_Finishing();
	//从m_pPeerList中找一个空元素，返回该元素的索引位置。
	//如果没有空元素，说明列表已满，则返回无效值。
	SoPeerIndex FindEmptyPeer(const SoClientID _ClientID) const;
	//根据SoClientID找到这个客户端的Peer对象。
	SoPeerIndex FindPeerByClientID(const SoClientID _ClientID) const;
	//判断SoPeerIndex是否为合法的有效值。
	bool IsValidIndex(SoPeerIndex _PeerIndex) const;

private:
	static SoP2PServerManager* ms_pInstance;
private:
	//服务器Host对象。
	ENetHost* m_pHost;
	//记录本进程的IP和端口。
	ENetAddress m_Address;
	//记录生命期阶段。
	eServerLifeStep m_eLifeStep;
	//维护一个Peer列表，存放所有与本服务器相连接的客户端信息。
	SoPeer* m_pPeerList;
	//PeerList的最大容量。
	soint32 m_nPeerListMaxSize;
};
//--------------------------------------------------------------------
inline SoP2PServerManager* SoP2PServerManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
inline bool SoP2PServerManager::IsValidIndex(SoPeerIndex _PeerIndex) const
{
	return (_PeerIndex >= 0 && _PeerIndex < m_nPeerListMaxSize);
}
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
