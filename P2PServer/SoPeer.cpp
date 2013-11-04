//--------------------------------------------------------------------
// (C) oil
// 2013-10-06
//--------------------------------------------------------------------
#include "SoPeer.h"
//--------------------------------------------------------------------
SoPeer::SoPeer()
:m_pENetPeer(0)
{
	ReleasePeer();
}
//--------------------------------------------------------------------
SoPeer::~SoPeer()
{
	ReleasePeer();
}
//--------------------------------------------------------------------
void SoPeer::InitPeer(SoClientID theClientID, SoPeerIndex theIndex, ENetPeer* pENetPeer)
{
	//把SoPeerIndex保存在pENetPeer的自定义数据内，
	//让pENetPeer知道自己在PeerList中的索引地址。
	pENetPeer->data = (void*)theIndex;
	//为成员变量赋值。
	m_ClientID = theClientID;
	m_PeerIndex = theIndex;
	m_pENetPeer = pENetPeer;

	/*
	ENetAddress remote=theEvent.peer->address; //远程地址
	char ip[256];
	enet_address_get_host_ip(&remote,ip,256);
	//cout <<"ip:" <<ip <<" 已经连接,序号:" <<num <<endl;
	//theEvent.peer->data=(void*)num++;
	*/
}
//--------------------------------------------------------------------
void SoPeer::ReleasePeer()
{
	m_ClientID = Invalid_SoClientID;
	m_PeerIndex = Invalid_SoPeerIndex;
	if (m_pENetPeer)
	{
		m_pENetPeer->data = 0;
		m_pENetPeer = 0;
	}
}
//--------------------------------------------------------------------
