//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#ifndef _SoP2PClientManager_h_
#define _SoP2PClientManager_h_
//--------------------------------------------------------------------
#include "SoP2PClientDefine.h"
//--------------------------------------------------------------------
class SoP2PClientManager
{
public:
	SoP2PClientManager();
	~SoP2PClientManager();
	static SoP2PClientManager* GetInstance();

	bool InitP2PClientManager(SoClientID _ClientID);
	void ReleaseP2PClientManager();
	void UpdateP2PClientManager();

	void SetP2PServerInfo(const char* pszIP, souint16 nPort);
	bool ConnectP2PServer();
	void DisconnectP2PServer();

	void TalkToP2PServer(const char* pszWords);

private:
	static SoP2PClientManager* ms_pInstance;
private:
	//客户端自己的ID。
	SoClientID m_theClientID;
	//客户端Host对象。
	ENetHost* m_pHost;
	ENetPeer* m_pP2PServerPeer;
	//记录P2PServer的IP和端口。
	ENetAddress m_P2PServerAddress;
	//记录与P2PServer之间的连接状态。
	eClientLifeStep m_eLifeStep;
};
//--------------------------------------------------------------------
inline SoP2PClientManager* SoP2PClientManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
#endif //_SoP2PClientManager_h_
//--------------------------------------------------------------------
