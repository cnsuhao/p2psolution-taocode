//-----------------------------------------------------------------------------
// (C) oil
// 2013-10-06
//-----------------------------------------------------------------------------
#ifndef _SoP2PServerDefine_h_
#define _SoP2PServerDefine_h_
//-----------------------------------------------------------------------------
#include "SoP2PDefine.h"
//-----------------------------------------------------------------------------
//当客户端登录到P2PServer后，P2PServer会为其分配一个Peer对象，
//该Peer对象在Peer数组中的下标即为SoPeerIndex。
typedef soint32 SoPeerIndex;
const SoPeerIndex Invalid_SoPeerIndex = -1;

//P2PServer的生命期状态。
enum eServerLifeStep
{
	LifeStep_None, //无效值。
	LifeStep_Prepared, //已经完成初始化，可以进入running状态。
	LifeStep_Running, //运转中。
	LifeStep_Finishing, //外界要求结束Server的运行，正在关闭中。
	LifeStep_Finished, //已经结束。
};

//-----------------------------------------------------------------------------
#endif //_SoP2PServerDefine_h_
//-----------------------------------------------------------------------------
