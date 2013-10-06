//--------------------------------------------------------------------
// (C) oil
// 2013-10-06
//--------------------------------------------------------------------
#include "SoPeerForServer.h"
//--------------------------------------------------------------------
SoPeerForServer::SoPeerForServer()
:m_myPeerID(SoPeerID_Invalid)
,m_pPeer(0)
{

}
//--------------------------------------------------------------------
SoPeerForServer::~SoPeerForServer()
{

}
//--------------------------------------------------------------------
void SoPeerForServer::InitPeer(SoPeerID theID, ENetPeer* pPeer)
{
	m_myPeerID = theID;
	m_pPeer = pPeer;

	/*
	ENetAddress remote=theEvent.peer->address; //远程地址
	char ip[256];
	enet_address_get_host_ip(&remote,ip,256);
	//cout <<"ip:" <<ip <<" 已经连接,序号:" <<num <<endl;
	//theEvent.peer->data=(void*)num++;
	*/
}
//--------------------------------------------------------------------
