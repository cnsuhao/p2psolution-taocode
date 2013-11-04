//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#include <stdio.h>
#include <process.h>
#include "SoP2PClientManager.h"
#include "SoConsoleHelp.h"
//--------------------------------------------------------------------
SoP2PClientManager* g_pClientManager = 0;
CRITICAL_SECTION g_ThreadLock;
bool g_bMainRunning = true;
bool g_bThreadRunning = true;
//--------------------------------------------------------------------
unsigned int WINAPI myThreadProcess(void* p)
{
	EnterCriticalSection(&g_ThreadLock);
	g_pClientManager = new SoP2PClientManager;
	g_pClientManager->InitP2PClientManager(654321);
	g_pClientManager->SetP2PServerInfo("127.0.0.1", 1234);
	g_pClientManager->ConnectP2PServer();
	LeaveCriticalSection(&g_ThreadLock);
	//
	while (g_bThreadRunning)
	{
		EnterCriticalSection(&g_ThreadLock);
		g_pClientManager->UpdateP2PClientManager();
		LeaveCriticalSection(&g_ThreadLock);
		//
		Sleep(10);
	}
	//
	EnterCriticalSection(&g_ThreadLock);
	delete g_pClientManager;
	g_pClientManager = 0;
	LeaveCriticalSection(&g_ThreadLock);
	return 1;
}
//--------------------------------------------------------------------
void ParseCommand(char* pszCommand)
{
	const char* pCmdType = "";
	const char* pCmdParam1 = "";
	const char* pCmdParam2 = "";
	bool bCmdType = false;
	bool bCmdParam1 = false;
	bool bCmdParam2 = false;
	//����������͡�
	pCmdType = pszCommand;
	while (*pszCommand != 0)
	{
		if (*pszCommand == ' ')
		{
			*pszCommand = 0;
			++pszCommand;
			bCmdType = true;
			break;
		}
		else
		{
			++pszCommand;
		}
	}
	if (bCmdType)
	{
		//�ҵ��˿ո��ַ����õ����������ͣ������Եõ���һ��������
		//��õ�һ��������
		pCmdParam1 = pszCommand;
		while (*pszCommand != 0)
		{
			if (*pszCommand == ' ')
			{
				*pszCommand = 0;
				++pszCommand;
				bCmdParam1 = true;
				break;
			}
			else
			{
				++pszCommand;
			}
		}
		if (bCmdParam1)
		{
			//�õ��˵�һ�������������Եõ��ڶ���������
			pCmdParam2 = pszCommand;
			//�������������
			if (strlen(pCmdParam2) > 0)
			{
				bCmdParam2 = true;
			}
		}
		else
		{
			if (strlen(pCmdParam1) > 0)
			{
				bCmdParam1 = true;
			}
		}
	}
	else
	{
		if (strlen(pCmdType) > 0)
		{
			bCmdType = true;
		}
	}
	//�ж��������͡�
	if (strcmp(pCmdType, "talkserver") == 0)
	{
		//��P2PServer����һ��������Ϣ��
		//����1���������ݡ�
		if (bCmdParam1)
		{
			EnterCriticalSection(&g_ThreadLock);
			if (g_pClientManager)
			{
				g_pClientManager->TalkToP2PServer(pCmdParam1);
			}
			LeaveCriticalSection(&g_ThreadLock);
		}
	}
	else if (strcmp(pCmdType, "connectserver") == 0)
	{
		//��P2PServer�������ӡ�
		EnterCriticalSection(&g_ThreadLock);
		if (g_pClientManager)
		{
			g_pClientManager->ConnectP2PServer();
		}
		LeaveCriticalSection(&g_ThreadLock);
	}
	else if (strcmp(pCmdType, "disconnectserver") == 0)
	{
		//��P2PServer�Ͽ����ӡ�
		EnterCriticalSection(&g_ThreadLock);
		if (g_pClientManager)
		{
			g_pClientManager->DisconnectP2PServer();
		}
		LeaveCriticalSection(&g_ThreadLock);
	}
}
//--------------------------------------------------------------------
void main()
{
	SoConsoleHelp::SetMyConsoleCtrlHandler();
	SoConsoleHelp::DisableCloseButton();
	printf("P2PClient\n");
	//�����̡߳�
	InitializeCriticalSection(&g_ThreadLock);
	HANDLE m_hSubThread = (HANDLE)_beginthreadex(NULL, 0, myThreadProcess, NULL, CREATE_SUSPENDED, NULL);
	if (m_hSubThread)
	{
		ResumeThread(m_hSubThread);
	}
	else
	{
		printf("Create thread fail!\n");
	}
	//���û��������
	while (g_bMainRunning)
	{
		//printf("Wait for your command:");
		char* pszCmd = SoConsoleHelp::GetInputString(false);
		ParseCommand(pszCmd);
	}
	//
	DeleteCriticalSection(&g_ThreadLock);
}
//--------------------------------------------------------------------
