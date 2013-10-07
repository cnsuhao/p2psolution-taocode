//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#ifndef _SoP2PClientManager_h_
#define _SoP2PClientManager_h_
//--------------------------------------------------------------------
#include "SoP2PDefine.h"
#include "enet/enet.h"
//--------------------------------------------------------------------
class SoP2PClientManager
{
public:
	SoP2PClientManager();
	~SoP2PClientManager();
	static SoP2PClientManager* GetInstance();

	bool InitP2PClientManager();
	void ReleaseP2PClientManager();
	void UpdateP2PClientManager();

	void SetP2PServerInfo(const char* pszIP, souint16 nPort);
	bool ConnectP2PServer();
	void DisconnectP2PServer();

private:
	//本客户端与P2PServer之间的连接状态（步骤）。
	enum eConnectStepWithP2PServer
	{
		ConnectStep_P2PServer_None, //尚未开始连接
		ConnectStep_P2PServer_ConnectStart, //开始连接
		ConnectStep_P2PServer_ConnectedInENet, //在底层已经连接成功
		ConnectStep_P2PServer_ConnectedInLogic, //在逻辑层已经连接成功
		ConnectStep_P2PServer_DisconnectStart, //开始断开连接
		ConnectStep_P2PServer_DisconnectedInLogic, //在逻辑层已经断开连接
		ConnectStep_P2PServer_DisconnectedInENet, //在底层已经断开连接
	};

private:
	static SoP2PClientManager* ms_pInstance;
private:
	//客户端Host对象。
	ENetHost* m_pHost;
	ENetPeer* m_pP2PServerPeer;
	//记录P2PServer的IP和端口。
	ENetAddress m_P2PServerAddress;
	//记录与P2PServer之间的连接状态。
	eConnectStepWithP2PServer m_eCurrentStepWithP2PServer;
};
//--------------------------------------------------------------------
inline SoP2PClientManager* SoP2PClientManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
#endif //_SoP2PClientManager_h_
//--------------------------------------------------------------------
