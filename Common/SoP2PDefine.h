//-----------------------------------------------------------------------------
// (C) oil
// 2013-10-06
//-----------------------------------------------------------------------------
#ifndef _SoP2PDefine_h_
#define _SoP2PDefine_h_
//-----------------------------------------------------------------------------
#include "SoBaseTypeDefine.h"
using namespace GGUI;
//-----------------------------------------------------------------------------
//���ͻ��˵�¼��GameServerʱ��GameServer����Ϊ�ͻ��˷���һ��ClientID��
//ÿ���ͻ��˱���Ψһ��Ӧһ��ClientID��ClientID�����ظ���
//���ͻ�����P2PServer����ʱ����Я�����Լ���ClientID��
//P2PServer����ClinetID�ҵ�����ͻ��˵�SoPeerIndex��
typedef soint64 SoClientID;
const SoClientID Invalid_SoClientID = -1;

//���ͻ��˵�¼��P2PServer��P2PServer��Ϊ�����һ��Peer����
//��Peer������Peer�����е��±꼴ΪSoPeerIndex��
typedef soint64 SoPeerIndex;
const SoPeerIndex Invalid_SoPeerIndex = -1;
//-----------------------------------------------------------------------------
#endif //_SoP2PDefine_h_
//-----------------------------------------------------------------------------
