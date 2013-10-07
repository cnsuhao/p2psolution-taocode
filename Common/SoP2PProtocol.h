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
	SoP2PProtoID_ClientReqConnectInLogic = 0, //客户端在逻辑上正式请求连接P2PServer
	SoP2PProtoID_RespondConnectInLogicToClinet = 1, //P2PServer把正式连接的结果回复给客户端
};

struct stClientReqConnectInLogic
{
	SoClientID m_ClientID;
	//客户端把自己的内网IP和端口汇报给服务器。
	//未完待续。
};

struct stRespondConnectInLogicToClinet
{
	//值为1表示成功；值为0表示失败。
	soint8 m_ConnectResult;
};
//-----------------------------------------------------------------------------
#endif //_SoP2PProtocol_h_
//-----------------------------------------------------------------------------
