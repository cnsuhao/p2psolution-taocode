//--------------------------------------------------------------------
// (C) oil
// 2013-10-07
//--------------------------------------------------------------------
#ifndef _SoP2PClientManager_h_
#define _SoP2PClientManager_h_
//--------------------------------------------------------------------
#include "SoP2PDefine.h"
#include "enet/enet.h"
//--------------------------------------------------------------------
class SoP2PClientManager
{
public:
	SoP2PClientManager();
	~SoP2PClientManager();
	static SoP2PClientManager* GetInstance();

	bool InitP2PClientManager();
	void ReleaseP2PClientManager();
	void UpdateP2PClientManager();

	void SetP2PServerInfo(const char* pszIP, souint16 nPort);
	bool ConnectP2PServer();
	void DisconnectP2PServer();

private:
	//���ͻ�����P2PServer֮�������״̬�����裩��
	enum eConnectStepWithP2PServer
	{
		ConnectStep_P2PServer_None, //��δ��ʼ����
		ConnectStep_P2PServer_ConnectStart, //��ʼ����
		ConnectStep_P2PServer_ConnectedInENet, //�ڵײ��Ѿ����ӳɹ�
		ConnectStep_P2PServer_ConnectedInLogic, //���߼����Ѿ����ӳɹ�
		ConnectStep_P2PServer_DisconnectStart, //��ʼ�Ͽ�����
		ConnectStep_P2PServer_DisconnectedInLogic, //���߼����Ѿ��Ͽ�����
		ConnectStep_P2PServer_DisconnectedInENet, //�ڵײ��Ѿ��Ͽ�����
	};

private:
	static SoP2PClientManager* ms_pInstance;
private:
	//�ͻ���Host����
	ENetHost* m_pHost;
	ENetPeer* m_pP2PServerPeer;
	//��¼P2PServer��IP�Ͷ˿ڡ�
	ENetAddress m_P2PServerAddress;
	//��¼��P2PServer֮�������״̬��
	eConnectStepWithP2PServer m_eCurrentStepWithP2PServer;
};
//--------------------------------------------------------------------
inline SoP2PClientManager* SoP2PClientManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
#endif //_SoP2PClientManager_h_
//--------------------------------------------------------------------
