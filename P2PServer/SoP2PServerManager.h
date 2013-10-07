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
	void AddClientIDPeerID(SoClientID _ClientID, SoPeerID _PeerID);
	void RemoveClientIDPeerID(SoClientID _ClientID, SoPeerID _PeerID);
	SoPeerID GetPeerIDByClientID(SoClientID _ClientID);

private:
	//��Server��������״̬��
	enum eServerLifeStep
	{
		LifeStep_Starting, //���������С�
		LifeStep_Running, //��ת�С�
		LifeStep_Finishing, //���Ҫ�����Server�����У����ڹر��С�
		LifeStep_Finished, //�Ѿ�������
	};
	//����ṹ�����ڸ���SoClientID��ȡSoPeerID��
	struct stClientID2PeerID
	{
		SoClientID theClientID;
		SoPeerID thePeerID;
		stClientID2PeerID() : theClientID(SoClientID_Invalid), thePeerID(SoPeerID_Invalid) {};
		bool IsEmpty() { return (theClientID==SoClientID_Invalid && thePeerID==SoPeerID_Invalid); };
		void Clear() { theClientID = SoClientID_Invalid; thePeerID = SoPeerID_Invalid; };
	};
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
	SoPeerForServer* m_pPeerList;
	//PeerList�����������
	soint32 m_nPeerListMaxSize;
	//ά����SoClinetID��SoPeerID��ӳ�䡣
	//������Ԫ�صĸ�����m_nPeerListMaxSize��
	//ͨ��һ���㷨�����SoClientID�任Ϊ�±�������
	stClientID2PeerID* m_pClientID2PeerID;
};
//--------------------------------------------------------------------
inline SoP2PServerManager* SoP2PServerManager::GetInstance()
{
	return ms_pInstance;
}
//--------------------------------------------------------------------
#endif //_SoP2PServerManager_h_
//--------------------------------------------------------------------
