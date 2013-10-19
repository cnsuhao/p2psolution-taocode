//--------------------------------------------------------------------
// (C) oil
// 2013-10-06
//--------------------------------------------------------------------
#include "SoPeer.h"
//--------------------------------------------------------------------
SoPeer::SoPeer()
:m_ClientID(Invalid_SoClientID)
,m_PeerIndex(Invalid_SoPeerIndex)
,m_pENetPeer(0)
{

}
//--------------------------------------------------------------------
SoPeer::~SoPeer()
{
	m_ClientID = Invalid_SoClientID;
	m_PeerIndex = Invalid_SoPeerIndex;
	m_pENetPeer = 0;
}
//--------------------------------------------------------------------
void SoPeer::InitPeer(SoClientID theClientID, SoPeerIndex theIndex, ENetPeer* pENetPeer)
{
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
