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
	SoP2PProtoID_ClientReq_ConnectInLogic = 1, //客户端在逻辑上正式请求连接P2PServer
	SoP2PProtoID_ClientAck_ConnectInLogic = 2, //P2PServer把正式连接的结果回复给客户端
	SoP2PProtoID_Test_Req = 3, //用于测试
	SoP2PProtoID_Test_Ack = 4, //用于测试
};

#define TestStringBuffSize 128

struct stClientReqConnectInLogic
{
	soint8 m_ProtoID;
	SoClientID m_ClientID;
	//客户端把自己的内网IP和端口汇报给服务器。
	//未完待续。

	stClientReqConnectInLogic()
	{
		m_ProtoID = SoP2PProtoID_ClientReq_ConnectInLogic;
		m_ClientID = Invalid_SoClientID;
	}
};

struct stRespondConnectInLogicToClinet
{
	soint8 m_ProtoID;
	//值为1表示成功；值为0表示失败。
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
