#pragma once
#include <vector>
#include <map>

namespace lynch {
	struct PosInfo
	{
		int Key;//�Լ��ļ�ֵ
		int PosX;//���X����
		int PosY;//���Y����
		int Occupation;//��¼�õ��������� 0:δ���� 1:���� 2:�Է�
		int MyDistance[2];
		int OpDistance[2];
		int MyPotential;
		int OpPotential;
		int TotalPotential;
		std::vector<PosInfo *>* Neighbors;
	};

	struct Position
	{
		int X;
		int Y;
	};
}

class HexAgent
{
public:
	HexAgent(int, int);//��һ��ֵ��GameSize,�ڶ������Ⱥ���,���������������
	~HexAgent();

	int EncodePos(int x, int y);
	lynch::Position DecodePos(int code);	
	void Receive(std::string, int);//���ն��ֵ�����
	std::string Output();//����Լ������� //...
	
	void MyDoubleDistance(int Edge);//EdgeΪ����˫����ķ���0Ϊ������1Ϊ������
	void OpDoubleDistance(int Edge);
	void ComputePotentials();
	//void SetMove(int Key);//������һ��(ֱ�Ӷ�chessboard����)


	//����ã�
	void ShowNeighbors(char letter, int y);
	void ShowOpLastMove();
	std::map<int, bool>* ShowCheckboard();
	lynch::PosInfo* Find(char letter, int y);

private:
	std::map<int, lynch::PosInfo> *Chessboard;//��¼���̵�λ��Ϣ
	//std::map<int, lynch::PosInfo> *BackupChessboard;//���̵�λ��Ϣ����(������������ʱ��ʹ��)
	std::map<int, bool> *Checkboard;//��¼���̵�λ�Ƿ����
	std::map<int, bool> *Checkboard2;//���ڼ�¼����
	bool TouchDown1;
	bool TouchDown2;
	int GameSize;
	int SetDeepth;
	bool Offensive;

	lynch::Position *MyLastMove;
	lynch::Position *OpLastMove;

	int ChartoInt(char);
	char InttoChar(int);
	void BoardReset(std::map<int, bool> *);
	void MyDistanceSetZero(lynch::PosInfo *, int Edge);
	void OpDistanceSetZero(lynch::PosInfo *, int Edge);
	void Receive(int Key, int i);//�����ڲ�ģ����
	//��һ��������������Ϊ����, �ڶ���Ϊ����˫����ķ���0Ϊ������
	int MyDetectNeighbor(int Partner, int Edge, int NeighborKeyValue, bool & isBlocked, int & Last1, int & Last2);
	int OpDetectNeighbor(int Partner, int Edge, int NeighborKeyValue, bool & isBlocked, int & Last1, int & Last2);
	int ValueFunction(lynch::PosInfo*);//...
	void SortPotential(std::map<int, lynch::PosInfo> *Board, std::vector<int> *KeyContainer, std::vector<int> *PotentialContainer);
	int MinMaxTreeSearch();
	int SearchTree(int LastKey, int LastColor,int &Deepth);//������һ���ֵ
	std::map<int, lynch::PosInfo>* SafeCopy(std::map<int, lynch::PosInfo> *);
};
