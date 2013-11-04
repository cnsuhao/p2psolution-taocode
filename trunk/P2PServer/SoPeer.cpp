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
	//��SoPeerIndex������pENetPeer���Զ��������ڣ�
	//��pENetPeer֪���Լ���PeerList�е�������ַ��
	pENetPeer->data = (void*)theIndex;
	//Ϊ��Ա������ֵ��
	m_ClientID = theClientID;
	m_PeerIndex = theIndex;
	m_pENetPeer = pENetPeer;

	/*
	ENetAddress remote=theEvent.peer->address; //Զ�̵�ַ
	char ip[256];
	enet_address_get_host_ip(&remote,ip,256);
	//cout <<"ip:" <<ip <<" �Ѿ�����,���:" <<num <<endl;
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
