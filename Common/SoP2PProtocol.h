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
	SoP2PProtoID_ClientReqConnectInLogic = 0, //�ͻ������߼�����ʽ��������P2PServer
	SoP2PProtoID_RespondConnectInLogicToClinet = 1, //P2PServer����ʽ���ӵĽ���ظ����ͻ���
};

struct stClientReqConnectInLogic
{
	SoClientID m_ClientID;
	//�ͻ��˰��Լ�������IP�Ͷ˿ڻ㱨����������
	//δ�������
};

struct stRespondConnectInLogicToClinet
{
	//ֵΪ1��ʾ�ɹ���ֵΪ0��ʾʧ�ܡ�
	soint8 m_ConnectResult;
};
//-----------------------------------------------------------------------------
#endif //_SoP2PProtocol_h_
//-----------------------------------------------------------------------------
