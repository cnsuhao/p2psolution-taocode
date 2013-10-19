//--------------------------------------------------------------------
// (C) oil
// 2013-10-06
//--------------------------------------------------------------------
#ifndef _SoPeer_h_
#define _SoPeer_h_
//--------------------------------------------------------------------
#include "enet/enet.h"
#include "SoP2PDefine.h"
//--------------------------------------------------------------------
class SoPeer
{
public:
	SoPeer();
	~SoPeer();

	void InitPeer(SoClientID theClientID, SoPeerIndex theIndex, ENetPeer* pENetPeer);
	//
	SoClientID GetClientID() const;
	SoPeerIndex GetPeerIndex() const;
	//如果本对象尚未填充有效数据，则返回true。
	bool IsEmpty() const;


private:
	SoClientID m_ClientID;
	SoPeerIndex m_PeerIndex;
	ENetPeer* m_pENetPeer;
};
//--------------------------------------------------------------------
inline SoClientID SoPeer::GetClientID() const
{
	return m_ClientID;
}
//--------------------------------------------------------------------
inline SoPeerIndex SoPeer::GetPeerIndex() const
{
	return m_PeerIndex;
}
//--------------------------------------------------------------------
inline bool SoPeer::IsEmpty() const
{
	return (m_ClientID == Invalid_SoClientID);
}
//--------------------------------------------------------------------
#endif //_SoPeer_h_
//--------------------------------------------------------------------
