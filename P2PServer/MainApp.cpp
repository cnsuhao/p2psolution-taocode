//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#include <stdio.h>
#include "SoP2PServerManager.h"
#include "GGUILogAsyn.h"
#include "SoConsoleHelp.h"
#include "GGUIStringHelp.h"
//--------------------------------------------------------------------
void main()
{
	SoConsoleHelp::SetMyConsoleCtrlHandler();
	SoConsoleHelp::DisableCloseButton();
	printf("P2PServer\n");

	//初始化log模块。
	//生成文件名。
	char szLogFileName[MaxLength_LogFileName] = {};
	SYSTEMTIME kTime;
	GetSystemTime(&kTime);
	StringCbPrintfA(szLogFileName, sizeof(szLogFileName), "%04d-%02d-%02d %02d-%02d-%02d.txt",
		kTime.wYear, kTime.wMonth, kTime.wDay, kTime.wHour+8, kTime.wMinute, kTime.wSecond);
	//创建log模块对象。
	GGUILogAsyn* pLog = new GGUILogAsyn;
	stLogParam theParam;
	theParam.pszLogFileName = szLogFileName;
	theParam.nSleepTime = 5;
	theParam.theLogLevel = LogLevel_Debug;
	GGUILogAsyn::GetInstance()->InitLog(theParam);

	SoP2PServerManager* pServer = new SoP2PServerManager;
	bool bInit = pServer->InitP2PServerManager();
	if (bInit)
	{
		printf("P2PServer init success! Start running!\n");
		pServer->P2PServerRun();
	}
	else
	{
		printf("SoP2PServerManager init failed!\n");
		while (true)
		{
			printf("Please enter \"quit\" to exit:\n");
			const char* pszString = SoConsoleHelp::GetInputString(false);
			if (strcmp(pszString, "quit") == 0)
			{
				break;
			}
		}
	}
	delete pServer;
	pServer = 0;
	//
	if (pLog)
	{
		delete pLog;
		pLog = 0;
	}
}
//--------------------------------------------------------------------
