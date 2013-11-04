//--------------------------------------------------------------------
// (C) oil
// 2013-10-06
//--------------------------------------------------------------------
#ifndef _SoPeer_h_
#define _SoPeer_h_
//--------------------------------------------------------------------
#include "SoP2PServerDefine.h"
//--------------------------------------------------------------------
class SoPeer
{
public:
	SoPeer();
	~SoPeer();
	//初始化Peer对象
	void InitPeer(SoClientID theClientID, SoPeerIndex theIndex, ENetPeer* pENetPeer);
	//把Peer对象清零
	void ReleasePeer();
	//获取客户端ID
	SoClientID GetClientID() const;
	//获取SoPeerIndex
	SoPeerIndex GetPeerIndex() const;
	//获取ENetPeer对象指针
	ENetPeer* GetENetPeer() const;
	//判断本对象是否为空对象，即尚未填充有效数据。
	//如果尚未填充有效数据，则返回true。
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
inline ENetPeer* SoPeer::GetENetPeer() const
{
	return m_pENetPeer;
}
//--------------------------------------------------------------------
inline bool SoPeer::IsEmpty() const
{
	return (m_ClientID == Invalid_SoClientID);
}
//--------------------------------------------------------------------
#endif //_SoPeer_h_
//--------------------------------------------------------------------
