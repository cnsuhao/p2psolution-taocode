//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#ifndef _SoP2PServerManager_h_
#define _SoP2PServerManager_h_
//--------------------------------------------------------------------
#include "SoP2PDefine.h"
#include "enet/enet.h"
class SoPeerForServer;
//--------------------------------------------------------------------
class SoP2PServerManager
{
public:
	SoP2PServerManager();
	~SoP2PServerManager();
	static SoP2PServerManager* GetInstance();

	bool InitP2PServerManager();
	void ReleaseP2PServerManager();
	//������Ϣ����ѭ����
	void P2PServerRun();

private:
	//��m_pPeerList����һ����Ԫ�أ����ظ�Ԫ�ص�����λ�á�
	//���û�п�Ԫ�أ�˵���б��������򷵻���Чֵ��
	SoPeerID GenerateNewPeerID();

private:
	//��Server��������״̬��
	enum eServerLifeStep
	{
		LifeStep_Starting, //���������С�
		LifeStep_Running, //��ת�С�
		LifeStep_Finishing, //���Ҫ�����Server�����У����ڹر��С�
		LifeStep_Finished, //�Ѿ�������
	};

private:
	static SoP2PServerManager* ms_pInstance;
private:
	//������Host����
	ENetHost* m_Host;
	//��¼�����̵�IP�Ͷ˿ڡ�
	ENetAddress m_Address;
	//��¼�����ڽ׶Ρ�
	eServerLifeStep m_eLifeStep;
	//ά��һ��Peer�б���������뱾�����������ӵĿͻ�����Ϣ��
	SoPeerForServer* m_pPeerList;
	//PeerList�����������
	soint32 m_nPeerListMaxSize;
};
//--------------------------------------------------------------------
inline SoP2PServerManager* SoP2PServerManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
