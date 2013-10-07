//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#include <stdio.h>
#include "SoP2PClientManager.h"
#include "SoConsoleHelp.h"
//--------------------------------------------------------------------
void main()
{
	SoConsoleHelp::SetMyConsoleCtrlHandler();
	SoConsoleHelp::DisableCloseButton();
	printf("P2PClient\n");

	SoP2PClientManager* pServer = new SoP2PClientManager;
	pServer->InitP2PClientManager();
	pServer->SetP2PServerInfo("127.0.0.1", 1234);
	pServer->ConnectP2PServer();

	//Ö÷Ñ­»·
	bool bRunning = true;
	while (bRunning)
	{
		if (pServer)
		{
			pServer->UpdateP2PClientManager();
		}
		//
		Sleep(10);
	}

	delete pServer;
	pServer = 0;
}
//--------------------------------------------------------------------
