//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include <stdio.h>
#include "SoP2PServerManager.h"
#include "SoConsoleHelp.h"
//--------------------------------------------------------------------
void main()
{
	SoConsoleHelp::SetMyConsoleCtrlHandler();
	SoConsoleHelp::DisableCloseButton();
	printf("P2PServer\n");

	SoP2PServerManager* pServer = new SoP2PServerManager;
	pServer->InitP2PServerManager();
	pServer->P2PServerRun();
	delete pServer;
	pServer = 0;
}
//--------------------------------------------------------------------
