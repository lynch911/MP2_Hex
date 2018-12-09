#pragma once
#include <vector>
#include <map>

namespace lynch {
	struct PosInfo
	{
		int Key;//自己的键值
		int PosX;//点的X坐标
		int PosY;//点的Y坐标
		int Occupation;//记录该点的落子情况 0:未落子 1:己方 2:对方
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
	HexAgent(int, int);//第一个值是GameSize,第二个是先后手,第三个是搜索深度
	~HexAgent();

	int EncodePos(int x, int y);
	lynch::Position DecodePos(int code);	
	void Receive(std::string, int);//接收对手的落子
	std::string Output();//输出自己的落子 //...
	
	void MyDoubleDistance(int Edge);//Edge为计算双距离的方向（0为正方向，1为反方向）
	void OpDoubleDistance(int Edge);
	void ComputePotentials();
	//void SetMove(int Key);//假设下一步(直接对chessboard操作)


	//检查用：
	void ShowNeighbors(char letter, int y);
	void ShowOpLastMove();
	std::map<int, bool>* ShowCheckboard();
	lynch::PosInfo* Find(char letter, int y);

private:
	std::map<int, lynch::PosInfo> *Chessboard;//记录棋盘点位信息
	//std::map<int, lynch::PosInfo> *BackupChessboard;//棋盘点位信息备份(仅在搜素树的时候使用)
	std::map<int, bool> *Checkboard;//记录棋盘点位是否检测过
	std::map<int, bool> *Checkboard2;//用于记录遍历
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
	void Receive(int Key, int i);//仅作内部模拟用
	//第一个参数是那种棋为队友, 第二个为计算双距离的方向（0为正方向）
	int MyDetectNeighbor(int Partner, int Edge, int NeighborKeyValue, bool & isBlocked, int & Last1, int & Last2);
	int OpDetectNeighbor(int Partner, int Edge, int NeighborKeyValue, bool & isBlocked, int & Last1, int & Last2);
	int ValueFunction(lynch::PosInfo*);//...
	void SortPotential(std::map<int, lynch::PosInfo> *Board, std::vector<int> *KeyContainer, std::vector<int> *PotentialContainer);
	int MinMaxTreeSearch();
	int SearchTree(int LastKey, int LastColor,int &Deepth);//返回下一层的值
	std::map<int, lynch::PosInfo>* SafeCopy(std::map<int, lynch::PosInfo> *);
};
