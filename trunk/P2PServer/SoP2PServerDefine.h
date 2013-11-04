//-----------------------------------------------------------------------------
// (C) oil
// 2013-10-06
//-----------------------------------------------------------------------------
#ifndef _SoP2PServerDefine_h_
#define _SoP2PServerDefine_h_
//-----------------------------------------------------------------------------
#include "SoP2PDefine.h"
//-----------------------------------------------------------------------------
//���ͻ��˵�¼��P2PServer��P2PServer��Ϊ�����һ��Peer����
//��Peer������Peer�����е��±꼴ΪSoPeerIndex��
typedef soint32 SoPeerIndex;
const SoPeerIndex Invalid_SoPeerIndex = -1;

//P2PServer��������״̬��
enum eServerLifeStep
{
	LifeStep_None, //��Чֵ��
	LifeStep_Prepared, //�Ѿ���ɳ�ʼ�������Խ���running״̬��
	LifeStep_Running, //��ת�С�
	LifeStep_Finishing, //���Ҫ�����Server�����У����ڹر��С�
	LifeStep_Finished, //�Ѿ�������
};

//-----------------------------------------------------------------------------
#endif //_SoP2PServerDefine_h_
//-----------------------------------------------------------------------------
