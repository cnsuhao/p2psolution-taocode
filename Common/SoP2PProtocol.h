//-----------------------------------------------------------------------------
// (C) oil
// 2013-10-07
//-----------------------------------------------------------------------------
#ifndef _SoP2PProtocol_h_
#define _SoP2PProtocol_h_
//-----------------------------------------------------------------------------
#include "SoP2PDefine.h"
//-----------------------------------------------------------------------------
enum eP2PProtoID
{
	SoP2PProtoID_ClientReq_ConnectInLogic = 1, //�ͻ������߼�����ʽ��������P2PServer
	SoP2PProtoID_ClientAck_ConnectInLogic = 2, //P2PServer����ʽ���ӵĽ���ظ����ͻ���
	SoP2PProtoID_Test_Req = 3, //���ڲ���
	SoP2PProtoID_Test_Ack = 4, //���ڲ���
};

#define TestStringBuffSize 128

struct stClientReqConnectInLogic
{
	soint8 m_ProtoID;
	SoClientID m_ClientID;
	//�ͻ��˰��Լ�������IP�Ͷ˿ڻ㱨����������
	//δ�������

	stClientReqConnectInLogic()
	{
		m_ProtoID = SoP2PProtoID_ClientReq_ConnectInLogic;
		m_ClientID = Invalid_SoClientID;
	}
};

struct stRespondConnectInLogicToClinet
{
	soint8 m_ProtoID;
	//ֵΪ1��ʾ�ɹ���ֵΪ0��ʾʧ�ܡ�
	soint8 m_ConnectResult;

	stRespondConnectInLogicToClinet()
	{
		m_ProtoID = SoP2PProtoID_ClientAck_ConnectInLogic;
		m_ConnectResult = 0;
	}
};

struct stP2PProtTestReq
{
	soint8 m_ProtoID;
	soint8 m_String[TestStringBuffSize];

	stP2PProtTestReq()
	{
		m_ProtoID = SoP2PProtoID_Test_Req;
		m_String[0] = 0;
	}
};

struct stP2PProtTestAck
{
	soint8 m_ProtoID;
	soint8 m_String[TestStringBuffSize];

	stP2PProtTestAck()
	{
		m_ProtoID = SoP2PProtoID_Test_Ack;
		m_String[0] = 0;
	}
};
//-----------------------------------------------------------------------------
#endif //_SoP2PProtocol_h_
//-----------------------------------------------------------------------------
