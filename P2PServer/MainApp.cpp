//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include "SoP2PServerManager.h"
#include <stdio.h>
#include "../Common/SoConsoleHelp.h"
#include "../Common/GGUIStringHelp.h"
//--------------------------------------------------------------------
void main()
{
	SoConsoleHelp::SetMyConsoleCtrlHandler();
	SoConsoleHelp::DisableCloseButton();
	printf("P2PServer\n");

	SoP2PServerManager* pServer = 0;
	bool bMainLoop = true;
	while (bMainLoop)
	{
		printf("====Wait for your command:\n");
		char* pString = SoConsoleHelp::GetInputString(false);
		GGUI::ConvertToLowCharacter(pString);
		if (strcmp(pString, "exit") == 0)
		{
			//½áÊøÖ÷Ñ­»·¡£
			bMainLoop = false;
		}
		else if (strcmp(pString, "create") == 0)
		{
			if (pServer == 0)
			{
				pServer = new SoP2PServerManager;
				pServer->InitP2PServerManager();
			}
		}
		else if (strcmp(pString, "release") == 0)
		{
			if (pServer)
			{
				delete pServer;
				pServer = 0;
			}
		}
		else
		{
			printf("?? Unknown command\n");
		}
	}
	//
	if (pServer)
	{
		delete pServer;
		pServer = 0;
	}
}
//--------------------------------------------------------------------
