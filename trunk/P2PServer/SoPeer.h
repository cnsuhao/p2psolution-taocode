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
	//��ʼ��Peer����
	void InitPeer(SoClientID theClientID, SoPeerIndex theIndex, ENetPeer* pENetPeer);
	//��Peer��������
	void ReleasePeer();
	//��ȡ�ͻ���ID
	SoClientID GetClientID() const;
	//��ȡSoPeerIndex
	SoPeerIndex GetPeerIndex() const;
	//��ȡENetPeer����ָ��
	ENetPeer* GetENetPeer() const;
	//�жϱ������Ƿ�Ϊ�ն��󣬼���δ�����Ч���ݡ�
	//�����δ�����Ч���ݣ��򷵻�true��
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
