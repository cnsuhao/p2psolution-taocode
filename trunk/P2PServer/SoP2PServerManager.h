//--------------------------------------------------------------------
// (C) oil
// 2013-09-05
//--------------------------------------------------------------------
#ifndef _SoP2PServerManager_h_
#define _SoP2PServerManager_h_
//--------------------------------------------------------------------
#include "SoP2PServerDefine.h"
class SoPeer;
//--------------------------------------------------------------------
class SoP2PServerManager
{
public:
	SoP2PServerManager();
	~SoP2PServerManager();
	static SoP2PServerManager* GetInstance();

	//��ʼ��ENet�⣬��Ԥ�ȴ���SoPeer�����б�
	bool InitP2PServerManager();
	//ɾ�����е�SoPeer���󣬲�����ENet�⡣
	void ReleaseP2PServerManager();
	//������Ϣ����ѭ����
	void P2PServerRun();

private:
	//������LifeStep_Running�׶ε�ҵ��
	void DoLifeStep_Running();
	//������LifeStep_Finishing�׶ε�ҵ��
	void DoLifeStep_Finishing();
	//��m_pPeerList����һ����Ԫ�أ����ظ�Ԫ�ص�����λ�á�
	//���û�п�Ԫ�أ�˵���б��������򷵻���Чֵ��
	SoPeerIndex FindEmptyPeer(const SoClientID _ClientID) const;
	//����SoClientID�ҵ�����ͻ��˵�Peer����
	SoPeerIndex FindPeerByClientID(const SoClientID _ClientID) const;
	//�ж�SoPeerIndex�Ƿ�Ϊ�Ϸ�����Чֵ��
	bool IsValidIndex(SoPeerIndex _PeerIndex) const;

private:
	static SoP2PServerManager* ms_pInstance;
private:
	//������Host����
	ENetHost* m_pHost;
	//��¼�����̵�IP�Ͷ˿ڡ�
	ENetAddress m_Address;
	//��¼�����ڽ׶Ρ�
	eServerLifeStep m_eLifeStep;
	//ά��һ��Peer�б���������뱾�����������ӵĿͻ�����Ϣ��
	SoPeer* m_pPeerList;
	//PeerList�����������
	soint32 m_nPeerListMaxSize;
};
//--------------------------------------------------------------------
inline SoP2PServerManager* SoP2PServerManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
inline bool SoP2PServerManager::IsValidIndex(SoPeerIndex _PeerIndex) const
{
	return (_PeerIndex >= 0 && _PeerIndex < m_nPeerListMaxSize);
}
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
