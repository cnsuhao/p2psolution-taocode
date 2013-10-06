//--------------------------------------------------------------------
// (C) oil
// 2013-10-06
//--------------------------------------------------------------------
#ifndef _SoPeerForServer_h_
#define _SoPeerForServer_h_
//--------------------------------------------------------------------
#include "SoP2PDefine.h"
#include "enet/enet.h"
//--------------------------------------------------------------------
class SoPeerForServer
{
public:
	SoPeerForServer();
	~SoPeerForServer();

	void InitPeer(SoPeerID theID, ENetPeer* pPeer);
	//如果本对象尚未填充有效数据，则返回true。
	bool IsEmpty();


private:
	SoPeerID m_myPeerID;
	ENetPeer* m_pPeer;
};
//--------------------------------------------------------------------
inline bool SoPeerForServer::IsEmpty()
{
	return (m_myPeerID == SoPeerID_Invalid);
}
//--------------------------------------------------------------------
#endif //_SoPeerForServer_h_
//--------------------------------------------------------------------
