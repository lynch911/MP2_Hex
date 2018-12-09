#include "stdafx.h"
#include "HexAgent.h"
#include <queue>
#include "iostream"
#include <string>
#include <vector>


HexAgent::HexAgent(int Size, int Deepth)
{
	GameSize = Size;
	SetDeepth = Deepth;
	MyLastMove = new lynch::Position; MyLastMove->X = -1; MyLastMove->Y = -1;
	OpLastMove = new lynch::Position; OpLastMove->X = -1; OpLastMove->Y = -1;
	TouchDown1 = false;
	TouchDown2 = false;
	Checkboard = new std::map<int, bool>;
	Checkboard2 = new std::map<int, bool>;
	Chessboard = new std::map<int, lynch::PosInfo>;
	//BackupChessboard = new std::map<int, lynch::PosInfo>;
	for (int j = 0; j < Size; j++) {
		for (int i = 0; i < Size; i++) {
			lynch::PosInfo PosI; PosI.Occupation = 0; PosI.PosX = i; PosI.PosY = j;
			PosI.MyDistance[0] = 999999; PosI.OpDistance[0] = 999999; PosI.MyDistance[1] = 999999; PosI.OpDistance[1] = 999999;
			PosI.Key = EncodePos(i, j);
			if (i == 0 || j == 0 || i == Size - 1 || j == Size - 1) {
				if ((i == 0 || i == Size - 1) && (j == 0 || j == Size - 1)) {
					if ((i == 0 && j == 0) || (i == Size - 1 && j == Size - 1)) {
						PosI.Neighbors = new std::vector<lynch::PosInfo *>(2);//角点有2个邻居
					}
					else {
						PosI.Neighbors = new std::vector<lynch::PosInfo *>(3);//角点有3个邻居
					}
				}
				else {
					PosI.Neighbors = new std::vector<lynch::PosInfo *>(4);//边缘点有4个邻居
				}
			}
			else {
				PosI.Neighbors = new std::vector<lynch::PosInfo *>(6);//在中部的点位有6个邻居
			}
			Chessboard->insert(std::make_pair(EncodePos(i, j), PosI));
			Checkboard->insert(std::make_pair(EncodePos(i, j), false));
			Checkboard2->insert(std::make_pair(EncodePos(i, j), false));
		}
	}
	std::map<int, lynch::PosInfo>::iterator iter1;
	std::map<int, lynch::PosInfo>::iterator iter2;
	lynch::PosInfo *Tag;
	lynch::Position tempP;
	for (iter1 = Chessboard->begin(); iter1 != Chessboard->end(); iter1++) {
		tempP = DecodePos(iter1->first);
		int i = tempP.X; int j = tempP.Y;
		if (i == 0 || j == 0 || i == Size - 1 || j == Size - 1) {
			if ((i == 0 || i == Size - 1) && (j == 0 || j == Size - 1)) {
				//边缘点有2个邻居
				if (i == 0 && j == 0) {
					Tag = &Chessboard->at(EncodePos(i + 1, j)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i, j + 1)); iter1->second.Neighbors->at(1) = Tag;
				}
				else if (i == 0 && j == Size - 1) {
					Tag = &Chessboard->at(EncodePos(i, j - 1)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i + 1, j - 1)); iter1->second.Neighbors->at(1) = Tag;
					Tag = &Chessboard->at(EncodePos(i + 1, j)); iter1->second.Neighbors->at(2) = Tag;
				}
				else if (i == Size - 1 && j == 0) {
					Tag = &Chessboard->at(EncodePos(i, j + 1)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j + 1)); iter1->second.Neighbors->at(1) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j)); iter1->second.Neighbors->at(2) = Tag;
				}
				else {
					Tag = &Chessboard->at(EncodePos(i, j - 1)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j)); iter1->second.Neighbors->at(1) = Tag;
				}
			}
			else {
				//边缘点有4个邻居
				if (i == 0) {
					Tag = &Chessboard->at(EncodePos(i, j - 1)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i + 1, j - 1)); iter1->second.Neighbors->at(1) = Tag;
					Tag = &Chessboard->at(EncodePos(i + 1, j)); iter1->second.Neighbors->at(2) = Tag;
					Tag = &Chessboard->at(EncodePos(i, j + 1)); iter1->second.Neighbors->at(3) = Tag;
				}
				else if (i == Size - 1) {
					Tag = &Chessboard->at(EncodePos(i, j - 1)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i, j + 1)); iter1->second.Neighbors->at(1) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j + 1)); iter1->second.Neighbors->at(2) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j)); iter1->second.Neighbors->at(3) = Tag;
				}
				else if (j == 0) {
					Tag = &Chessboard->at(EncodePos(i + 1, j)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i, j + 1)); iter1->second.Neighbors->at(1) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j + 1)); iter1->second.Neighbors->at(2) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j)); iter1->second.Neighbors->at(3) = Tag;
				}
				else {
					Tag = &Chessboard->at(EncodePos(i, j - 1)); iter1->second.Neighbors->at(0) = Tag;
					Tag = &Chessboard->at(EncodePos(i + 1, j - 1)); iter1->second.Neighbors->at(1) = Tag;
					Tag = &Chessboard->at(EncodePos(i + 1, j)); iter1->second.Neighbors->at(2) = Tag;
					Tag = &Chessboard->at(EncodePos(i - 1, j)); iter1->second.Neighbors->at(3) = Tag;
				}
			}
		}
		else {
			//在中部的点位有6个邻居
			Tag = &Chessboard->at(EncodePos(i, j - 1)); iter1->second.Neighbors->at(0) = Tag;
			Tag = &Chessboard->at(EncodePos(i + 1, j - 1)); iter1->second.Neighbors->at(1) = Tag;
			Tag = &Chessboard->at(EncodePos(i + 1, j)); iter1->second.Neighbors->at(2) = Tag;
			Tag = &Chessboard->at(EncodePos(i, j + 1)); iter1->second.Neighbors->at(3) = Tag;
			Tag = &Chessboard->at(EncodePos(i - 1, j + 1)); iter1->second.Neighbors->at(4) = Tag;
			Tag = &Chessboard->at(EncodePos(i - 1, j)); iter1->second.Neighbors->at(5) = Tag;
		}
	}
}

HexAgent::~HexAgent()
{
	if (Chessboard)
		delete Chessboard;
	if (Checkboard)
		delete Checkboard;
	if (MyLastMove)
		delete MyLastMove;
	if (OpLastMove)
		delete OpLastMove;
}

int HexAgent::EncodePos(int x, int y)
{
	return (y * GameSize + x);
}

lynch::Position HexAgent::DecodePos(int code)
{
	lynch::Position Pos;
	Pos.X = code % GameSize;
	Pos.Y = (code - Pos.X) / GameSize;
	return Pos;
}

void HexAgent::BoardReset(std::map<int, bool> * tag)
{
	std::map<int, bool>::iterator iter;
	for (iter = tag->begin(); iter != tag->end(); iter++) {
		iter->second = false;
	}
}

void HexAgent::OpDistanceSetZero(lynch::PosInfo * PosI, int Edge)
{
	lynch::PosInfo *FloatPointer = PosI;
	std::vector<lynch::PosInfo*> temp;
	Checkboard2->at(FloatPointer->Key) = true;
	while (true) {
		for (int i = 0; i < FloatPointer->Neighbors->size(); i++) {
			if (FloatPointer->Neighbors->at(i)->Occupation == 2) {
				if (Checkboard2->at(FloatPointer->Neighbors->at(i)->Key) == false) {
					FloatPointer->Neighbors->at(i)->OpDistance[Edge] = 0;
					temp.push_back(FloatPointer->Neighbors->at(i));					
				}
			}
		}
		if (temp.size() == 0)
			break;
		else {
			if (Checkboard2->at(temp.at(temp.size() - 1)->Key) == false) {
				FloatPointer = temp.at(temp.size() - 1);
				Checkboard2->at(FloatPointer->Key) = true;
			}
			temp.pop_back();
		}
	}
	BoardReset(Checkboard2);
}

void HexAgent::Receive(int Key, int i)
{
	lynch::PosInfo *PosI = &(Chessboard->at(Key));
	int y = PosI->PosY;
	int x = PosI->PosX;

	PosI->Occupation = i;

	if (x == 0) {
		PosI->MyDistance[0] = 0;
		MyDistanceSetZero(PosI, 0);
	}
	else if (x == GameSize - 1) {
		PosI->MyDistance[1] = 0;
		MyDistanceSetZero(PosI, 1);
	}

	if (y == 0) {
		PosI->OpDistance[0] = 0;
		OpDistanceSetZero(PosI, 0);
	}
	else if (y == GameSize - 1) {
		PosI->OpDistance[1] = 0;
		OpDistanceSetZero(PosI, 1);
	}

	for (int i = 0; i < PosI->Neighbors->size(); i++) {
		if (PosI->Neighbors->at(i)->MyDistance[0] == 0)
			PosI->MyDistance[0] = 0;
		if (PosI->Neighbors->at(i)->MyDistance[1] == 0)
			PosI->MyDistance[1] = 0;
		if (PosI->Neighbors->at(i)->OpDistance[0] == 0)
			PosI->OpDistance[0] = 0;
		if (PosI->Neighbors->at(i)->OpDistance[1] == 0)
			PosI->OpDistance[1] = 0;
	}

	if (TouchDown1 == false) {
		if (y == 0)TouchDown1 = true;
	}
	if (TouchDown2 == false) {
		if (y == GameSize - 1) TouchDown2 = true;
	}
	//更新上一步的落子
	if (i == 2) {
		OpLastMove->X = x;
		OpLastMove->Y = y;
	}
	else {
		MyLastMove->X = x;
		OpLastMove->Y = y;
	}
}

void HexAgent::MyDistanceSetZero(lynch::PosInfo * PosI, int Edge)
{
	lynch::PosInfo *FloatPointer = PosI;
	std::vector<lynch::PosInfo*> temp;
	Checkboard2->at(FloatPointer->Key) = true;
	while (true) {
		for (int i = 0; i < FloatPointer->Neighbors->size(); i++) {
			if (FloatPointer->Neighbors->at(i)->Occupation == 1) {
				if (Checkboard2->at(FloatPointer->Neighbors->at(i)->Key) == false) {
					FloatPointer->Neighbors->at(i)->MyDistance[Edge] = 0;
					temp.push_back(FloatPointer->Neighbors->at(i));
				}
			}
		}
		if (temp.size() == 0)
			break;
		else {
			if (Checkboard2->at(temp.at(temp.size() - 1)->Key) == false) {
				FloatPointer = temp.at(temp.size() - 1);
				Checkboard2->at(FloatPointer->Key) = true;
			}
			temp.pop_back();
		}
	}
	BoardReset(Checkboard2);
}

int HexAgent::MyDetectNeighbor(int Partner, int Edge, int NeighborKeyValue, bool & isBlocked, int & Last1, int & Last2)
{
	lynch::PosInfo *PosI = &(Chessboard->at(NeighborKeyValue));
	if (PosI->Occupation == 0) {
		if (Checkboard2->at(NeighborKeyValue) == false) {
			Checkboard2->at(NeighborKeyValue) = true;
			isBlocked = false;
			if (PosI->MyDistance[Edge] < Last1) {
				Last2 = Last1;
				Last1 = PosI->MyDistance[Edge];
			}
			else {
				if (PosI->MyDistance[Edge] < Last2)
					Last2 = PosI->MyDistance[Edge];
			}
		}
	}
	else if (PosI->Occupation == Partner) {//如果连接有己方落子
		lynch::PosInfo *FloatPointer = PosI;
		std::vector<lynch::PosInfo*> temp;
		if (FloatPointer->MyDistance[Edge] == 0)
			return 1;
		while (true) {
			for (int m = 0; m < FloatPointer->Neighbors->size(); m++) {
				lynch::PosInfo *Neighbor = FloatPointer->Neighbors->at(m);
				if (Checkboard2->at(Neighbor->Key) == false) {//没有检查过才继续
					Checkboard2->at(Neighbor->Key) = true;
					if (Neighbor->Occupation == 0) {
						isBlocked = false;
						if (Neighbor->MyDistance[Edge] < Last1) {
							Last2 = Last1;
							Last1 = Neighbor->MyDistance[Edge];
						}
						else {
							if (Neighbor->MyDistance[Edge] < Last2)
								Last2 = Neighbor->MyDistance[Edge];
						}
					}
					else if (Neighbor->Occupation == Partner) {//
						if (Neighbor->MyDistance[Edge] == 0) {
							return 1;
							/*BoardPointer->MyDistance[0] = 1;
							(*CheckPointer) = true;
							goto pin1;*/
						}
						else
							temp.push_back(Neighbor);
					}
				}
			}
			if (temp.size() == 0)
				break;
			else {
				FloatPointer = temp.at(temp.size() - 1);
				temp.pop_back();
			}
		}
	}
	return 0;
}

int HexAgent::OpDetectNeighbor(int Partner, int Edge, int NeighborKeyValue, bool & isBlocked, int & Last1, int & Last2)
{
	lynch::PosInfo *PosI = &(Chessboard->at(NeighborKeyValue));
	if (PosI->Occupation == 0) {
		if (Checkboard2->at(NeighborKeyValue) == false) {
			Checkboard2->at(NeighborKeyValue) = true;
			isBlocked = false;
			if (PosI->OpDistance[Edge] < Last1) {
				Last2 = Last1;
				Last1 = PosI->OpDistance[Edge];
			}
			else {
				if (PosI->OpDistance[Edge] < Last2)
					Last2 = PosI->OpDistance[Edge];
			}
		}
	}
	else if (PosI->Occupation == Partner) {//如果连接有己方落子
		lynch::PosInfo *FloatPointer = &(Chessboard->at(NeighborKeyValue));
		std::vector<lynch::PosInfo*> temp;
		if (FloatPointer->OpDistance[Edge] == 0)
			return 1;
		while (true) {
			for (int m = 0; m < FloatPointer->Neighbors->size(); m++) {
				lynch::PosInfo *Neighbor = FloatPointer->Neighbors->at(m);
				if (Checkboard2->at(Neighbor->Key) == false) {//没有检查过才继续
					Checkboard2->at(Neighbor->Key) = true;
					if (Neighbor->Occupation == 0) {
						isBlocked = false;
						if (Neighbor->OpDistance[Edge] < Last1) {
							Last2 = Last1;
							Last1 = Neighbor->OpDistance[Edge];
						}
						else {
							if (Neighbor->OpDistance[Edge] < Last2)
								Last2 = Neighbor->OpDistance[Edge];
						}
					}
					else if (Neighbor->Occupation == Partner) {//
						if (Neighbor->OpDistance[Edge] == 0) {
							return 1;
							/*BoardPointer->MyDistance[0] = 1;
							(*CheckPointer) = true;
							goto pin1;*/
						}
						else
							temp.push_back(Neighbor);
					}
				}
			}
			if (temp.size() == 0)
				break;
			else {
				FloatPointer = temp.at(temp.size() - 1);
				temp.pop_back();
			}
		}
	}
	return 0;
}

lynch::PosInfo* HexAgent::Find(char letter, int y)
{
	int x = ChartoInt(letter);
	return &(Chessboard->at(EncodePos(x, y)));
}

void HexAgent::Receive(std::string des, int i)
{
	int y = ChartoInt(des.c_str()[0]);
	int x = stoi(des.substr(1));
	lynch::PosInfo *PosI = &(Chessboard->at(EncodePos(x, y)));
	
	PosI->Occupation = i;
	if (i == 1) {
		if (x == 0) {
			PosI->MyDistance[0] = 0;
			MyDistanceSetZero(PosI, 0);
		}
		else if (x == GameSize - 1) {
			PosI->MyDistance[1] = 0;
			MyDistanceSetZero(PosI, 1);
		}
	}
	else {
		if (y == 0) {
			PosI->OpDistance[0] = 0;
			OpDistanceSetZero(PosI, 0);
		}
		else if (y == GameSize - 1) {
			PosI->OpDistance[1] = 0;
			OpDistanceSetZero(PosI, 1);
		}
	}

	for (int i = 0; i < PosI->Neighbors->size(); i++) {
		if (PosI->Neighbors->at(i)->MyDistance[0] == 0)
			PosI->MyDistance[0] = 0;
		if (PosI->Neighbors->at(i)->MyDistance[1] == 0)
			PosI->MyDistance[1] = 0;
		if (PosI->Neighbors->at(i)->OpDistance[0] == 0)
			PosI->OpDistance[0] = 0;
		if (PosI->Neighbors->at(i)->OpDistance[1] == 0)
			PosI->OpDistance[1] = 0;
	}

	if (TouchDown1 == false) {
		if (y == 0)TouchDown1 = true;
	}
	if (TouchDown2 == false) {
		if (y == GameSize - 1) TouchDown2 = true;
	}
	//更新上一步的落子
	if (i == 2) {
		OpLastMove->X = x;
		OpLastMove->Y = y;
	}
	else {
		MyLastMove->X = x;
		OpLastMove->Y = y;
	}
}

std::string HexAgent::Output()
{
	lynch::Position Pos;

	int BestKey = MinMaxTreeSearch();

	Receive(BestKey, 1);

	Pos = DecodePos(BestKey);

	std::string str;

	str = InttoChar(Pos.Y);
	str += std::to_string(Pos.X);

	return str;
}

void HexAgent::ShowNeighbors(char letter, int y)
{
	lynch::PosInfo* PosI = Find(letter, y);
	for (int i = 0; i < PosI->Neighbors->size(); i++) {
		std::cout << InttoChar(PosI->Neighbors->at(i)->PosY) << "," << PosI->Neighbors->at(i)->PosX << ":" << PosI->Neighbors->at(i)->Occupation << std::endl;
	}
}

void HexAgent::ShowOpLastMove()
{
	std::cout << InttoChar(OpLastMove->Y) << "," << OpLastMove->X << std::endl;
}

std::map<int, bool>* HexAgent::ShowCheckboard()
{
	return Checkboard;
}

int HexAgent::ChartoInt(char letter)
{
	return (int)letter - 65;
}

char HexAgent::InttoChar(int code)
{
	return (char)(code + 65);
}

int HexAgent::ValueFunction(lynch::PosInfo* PosI)
{
	//return PosI->TotalPotential;
	return PosI->TotalPotential;
}

void HexAgent::SortPotential(std::map<int, lynch::PosInfo> *Board, std::vector<int> *KeyContainer, std::vector<int> *PotentialContainer)
{
	std::map<int, lynch::PosInfo>::iterator Chessiter;
	for (Chessiter = Board->begin(); Chessiter != Board->end(); Chessiter++) {
		if (Chessiter->second.Occupation == 0) {//空格才参与计算
			int Value = ValueFunction(&(Chessiter->second));
			int KeyValue = Chessiter->second.Key;
			if (Value < PotentialContainer->at(0)) {
				PotentialContainer->at(2) = PotentialContainer->at(1);
				PotentialContainer->at(1) = PotentialContainer->at(0);
				PotentialContainer->at(0) = Value;
				KeyContainer->at(2) = KeyContainer->at(1);
				KeyContainer->at(1) = KeyContainer->at(0);
				KeyContainer->at(0) = KeyValue;
			}
			else if (Value < PotentialContainer->at(1)) {
				PotentialContainer->at(2) = PotentialContainer->at(1);
				PotentialContainer->at(1) = Value;
				KeyContainer->at(2) = KeyContainer->at(1);
				KeyContainer->at(1) = KeyValue;
			}
			else if (Value < PotentialContainer->at(2)) {
				PotentialContainer->at(2) = Value;
				KeyContainer->at(2) = KeyValue;
			}
		}
	}
}

int HexAgent::MinMaxTreeSearch()
{
	//double Alpha, Beta;
	int Deepth = 0;
	std::vector<int> *KeyContainer = new std::vector<int>(3);
	std::vector<int> *PotentialContainer = new std::vector<int>(3);
	PotentialContainer->at(0) = 999999; PotentialContainer->at(1) = 999999; PotentialContainer->at(2) = 999999;

	int BestValue = 999999;
	int BestKey = -1;
	std::map<int, lynch::PosInfo> *BackupChessboard = new std::map<int, lynch::PosInfo>;
	//std::map<int, lynch::PosInfo> *BackupChessboard;

	ComputePotentials();//计算当前潜力值
	BackupChessboard = SafeCopy(Chessboard);//记录当前状态
	SortPotential(Chessboard, KeyContainer, PotentialContainer);//排序 取前三的value和key
	for (int i = 0; i < 3; i++) {
		if (PotentialContainer->at(i) < 999999) {
			int NextKey = KeyContainer->at(i);
			int NextColor = 1;// 2-(LastColor+1)%2
			int TempValue = SearchTree(NextKey, NextColor, Deepth);
			if (TempValue < BestValue) {
				BestKey = NextKey;
				BestValue = TempValue;
			}
		}
		delete Chessboard;
		Chessboard = SafeCopy(BackupChessboard);
	}
	delete BackupChessboard;
	return BestKey;
}

int HexAgent::SearchTree(int LastKey, int LastColor, int & Deepth)
{
	Receive(LastKey, LastColor);
	ComputePotentials();
	lynch::PosInfo *PosI = &Chessboard->at(LastKey);
	if (LastColor == 1) {
		if (PosI->MyDistance[0] == 0 && PosI->MyDistance[1] == 0)
			return 0;
			//return -99999999;
	}
	else {
		if (PosI->OpDistance[0] == 0 && PosI->OpDistance[1] == 0)
			return 0;
			//return -99999999;
	}
	

	std::vector<int> *KeyContainer = new std::vector<int>(3);
	std::vector<int> *PotentialContainer = new std::vector<int>(3);
	PotentialContainer->at(0) = 999999; PotentialContainer->at(1) = 999999; PotentialContainer->at(2) = 999999;
	//std::map<int, lynch::PosInfo> *BackupChessboard = new std::map<int, lynch::PosInfo>;

	int BestValue = 999999;
	int BestKey = -1;
	int NextColor = 2 - ((LastColor + 1) % 2);
	std::map<int, lynch::PosInfo> *BackupChessboard = new std::map<int, lynch::PosInfo>;
	//std::map<int, lynch::PosInfo> *BackupChessboard;

	if (LastColor == 2)
		Deepth++;

	if (Deepth == SetDeepth) {
		std::map<int, lynch::PosInfo>::iterator Chessiter;
		for (Chessiter = Chessboard->begin(); Chessiter != Chessboard->end(); Chessiter++) {
			if (Chessiter->second.Occupation == 0) {
				int Value = ValueFunction(&(Chessiter->second));
				int KeyValue = Chessiter->second.Key;
				if (Value < BestValue) {
					BestValue = Value;
					BestKey = KeyValue;
				}
			}
		}
		Deepth--;
		delete BackupChessboard;
		BackupChessboard = NULL;
		return BestValue;
	}
	else {//上一步是min 这一步是max 且没有到达指定深度
		BackupChessboard = SafeCopy(Chessboard);
		SortPotential(Chessboard, KeyContainer, PotentialContainer);
		for (int i = 0; i < 3; i++) {
			if (PotentialContainer->at(i) < 999999) {
				int NextKey = KeyContainer->at(i);
				int TempValue = SearchTree(NextKey, NextColor, Deepth);
				if (TempValue < BestValue) {
					BestKey = NextKey;
					BestValue = TempValue;
				}
			}
			delete Chessboard;
			Chessboard = SafeCopy(BackupChessboard);
		}
		if(LastColor ==2)
			Deepth--;
	}

	//if (LastColor == 1) {
	//	(*BackupChessboard) = (*Chessboard);
	//	SortPotential(Chessboard, KeyContainer, PotentialContainer);
	//	//...
	//}
	//else {
	//	Deepth++;
	//	if (Deepth == SetDeepth) {
	//		//...
	//		Deepth--;
	//		return BestValue;
	//	}
	//	else {//上一步是min 这一步是max 且没有到达指定深度
	//		(*BackupChessboard) = (*Chessboard);
	//		SortPotential(Chessboard, KeyContainer, PotentialContainer);
	//		for (int i = 0; i < 3; i++) {
	//			int NextKey = KeyContainer->at(i);
	//			int TempValue = SearchTree(NextKey, NextColor, Deepth);
	//			if (TempValue < BestValue) {
	//				BestKey = NextKey;
	//				BestValue = TempValue;
	//				(*Chessboard) = (*BackupChessboard);
	//			}
	//		}
	//	}
	//}
	delete BackupChessboard;
	BackupChessboard = NULL;
	return BestValue;
}

std::map<int, lynch::PosInfo>* HexAgent::SafeCopy(std::map<int, lynch::PosInfo>* Board)
{
	std::map<int, lynch::PosInfo> * NewPointer = new std::map<int, lynch::PosInfo>;
	std::map<int, lynch::PosInfo>::iterator iter;
	for (iter = Board->begin(); iter != Board->end(); iter++) {
		lynch::PosInfo PosI;
		PosI.Key = iter->second.Key;
		PosI.PosX = iter->second.PosX;
		PosI.PosY = iter->second.PosY;
		PosI.Occupation = iter->second.Occupation;
		PosI.MyDistance[0] = iter->second.MyDistance[0];
		PosI.MyDistance[1] = iter->second.MyDistance[1];
		PosI.OpDistance[0] = iter->second.OpDistance[0];
		PosI.OpDistance[1] = iter->second.OpDistance[1];
		PosI.MyPotential = iter->second.MyPotential;
		PosI.OpPotential = iter->second.OpPotential;
		PosI.TotalPotential = iter->second.TotalPotential;
		NewPointer->insert(std::make_pair(PosI.Key, PosI));
	}

	std::map<int, lynch::PosInfo>::iterator iter1 = NewPointer->begin();
	for (iter = Board->begin(); iter != Board->end(); iter++) {
		int Num = iter->second.Neighbors->size();
		iter1->second.Neighbors = new std::vector<lynch::PosInfo *>(Num);
		for (int i = 0; i < Num; i++) {
			iter1->second.Neighbors->at(i) = &NewPointer->at(iter->second.Neighbors->at(i)->Key);
		}
		iter1++;
	}

	return NewPointer;
}

void HexAgent::MyDoubleDistance(int Edge)
{
	bool FLAG = false;
	bool *CheckPointer;
	lynch::PosInfo *BoardPointer;//指向目前要计算的点位
	int	LimitedMax = 0;
	int EdgeValue;
	if (Edge == 0)
		EdgeValue = 0;
	else
		EdgeValue = GameSize - 1;
	while (!FLAG) {
		FLAG = true;
		LimitedMax++;
		for (int i = 0; i < GameSize; i++) {
			for (int j = 0; j < GameSize; j++) {
				int KeyValue = EncodePos(i, j);
				CheckPointer = &(Checkboard->at(KeyValue));
				BoardPointer = &(Chessboard->at(KeyValue));
				if ((*CheckPointer) == false) {//当该点还没有双距离值时
					//FLAG = false;//如果有一个点没有双距离则再循环一次
					if (BoardPointer->Occupation != 0)
						(*CheckPointer) = true;//已有落子的情况，应有记录
					else {//在空格的情况下计算双距离
						if (BoardPointer->PosX == EdgeValue) {
							BoardPointer->MyDistance[Edge] = 1;//左边缘直接为1
							(*CheckPointer) = true;
							FLAG = false;
						}
						else {//不是左边缘
							bool isBlocked = true;
							int Last1 = 9999999;
							int Last2 = 9999999;
							if (BoardPointer->PosX == GameSize - 1 - EdgeValue) {//在右边缘
								for (int t = 0; t < GameSize; t++) {
									int BoarderKeyValue = EncodePos(GameSize - 1 - EdgeValue, t);//右边缘的key
									//---
									int Result = MyDetectNeighbor(1, Edge, BoarderKeyValue, isBlocked, Last1, Last2);
									if (Result == 1) {
										BoardPointer->MyDistance[Edge] = 1;
										(*CheckPointer) = true;
										FLAG = false;
										goto pin1;
									}
									//---									
								}
							}
							for (int k = 0; k < BoardPointer->Neighbors->size(); k++) {
								//---
								int Result = MyDetectNeighbor(1, Edge, BoardPointer->Neighbors->at(k)->Key, isBlocked, Last1, Last2);
								if (Result == 1) {
									BoardPointer->MyDistance[Edge] = 1;
									(*CheckPointer) = true;
									FLAG = false;
									goto pin1;
								}
								//---
							}
							if (isBlocked) {
								BoardPointer->MyDistance[Edge] = 9999999;
								(*CheckPointer) = true;
								FLAG = false;
							}
							else {
								if (Last2 == LimitedMax - 1) {
									BoardPointer->MyDistance[Edge] = LimitedMax;
									(*CheckPointer) = true;
									FLAG = false;
								}
							}
						}
					pin1:
						BoardReset(Checkboard2);
					}
				}
			}
		}
	}
	std::map<int, lynch::PosInfo>::iterator Chessiter;
	std::map<int, bool>::iterator Checkiter = Checkboard->begin();
	for (Chessiter = Chessboard->begin(); Chessiter != Chessboard->end(); Chessiter++) {
		if (Checkiter->second == false)
			Chessiter->second.MyDistance[Edge] = 9999999;
		Checkiter++;
	}
	BoardReset(Checkboard);
}

void HexAgent::OpDoubleDistance(int Edge)
{
	bool FLAG = false;
	bool *CheckPointer;
	lynch::PosInfo *BoardPointer;//指向目前要计算的点位
	int	LimitedMax = 0;
	int EdgeValue;
	if (Edge == 0)
		EdgeValue = 0;
	else
		EdgeValue = GameSize - 1;
	while (!FLAG) {
		FLAG = true;
		LimitedMax++;
		for (int i = 0; i < GameSize; i++) {
			for (int j = 0; j < GameSize; j++) {
				int KeyValue = EncodePos(j, i);
				CheckPointer = &(Checkboard->at(KeyValue));
				BoardPointer = &(Chessboard->at(KeyValue));
				if ((*CheckPointer) == false) {//当该点还没有双距离值时
					//FLAG = false;//如果有一个点没有双距离则再循环一次
					if (BoardPointer->Occupation != 0)
						(*CheckPointer) = true;//已有落子的情况，应有记录
					else {//在空格的情况下计算双距离
						if (BoardPointer->PosY == EdgeValue) {
							BoardPointer->OpDistance[Edge] = 1;//左边缘直接为1
							(*CheckPointer) = true;
							FLAG = false;
						}
						else {//不是左边缘
							bool isBlocked = true;
							int Last1 = 9999999;
							int Last2 = 9999999;
							if (BoardPointer->PosY == GameSize - 1 - EdgeValue) {//在右边缘
								for (int t = 0; t < GameSize; t++) {
									int BoarderKeyValue = EncodePos(t, GameSize - 1 - EdgeValue);//右边缘的key
																								 //---
									int Result = OpDetectNeighbor(2, Edge, BoarderKeyValue, isBlocked, Last1, Last2);
									if (Result == 1) {
										BoardPointer->OpDistance[Edge] = 1;
										(*CheckPointer) = true;
										FLAG = false;
										goto pin1;
									}
									//---									
								}
							}
							for (int k = 0; k < BoardPointer->Neighbors->size(); k++) {
								//---
								int Result = OpDetectNeighbor(2, Edge, BoardPointer->Neighbors->at(k)->Key, isBlocked, Last1, Last2);
								if (Result == 1) {
									BoardPointer->OpDistance[Edge] = 1;
									(*CheckPointer) = true;
									FLAG = false;
									goto pin1;
								}
								//---
							}
							if (isBlocked) {
								BoardPointer->OpDistance[Edge] = 9999999;
								(*CheckPointer) = true;
								FLAG = false;
							}
							else {
								if (Last2 == LimitedMax - 1) {
									BoardPointer->OpDistance[Edge] = LimitedMax;
									(*CheckPointer) = true;
									FLAG = false;
								}
							}
						}
					pin1:
						BoardReset(Checkboard2);
					}
				}
			}
		}
	}
	std::map<int, lynch::PosInfo>::iterator Chessiter;
	std::map<int, bool>::iterator Checkiter = Checkboard->begin();
	for (Chessiter = Chessboard->begin(); Chessiter != Chessboard->end(); Chessiter++) {
		if (Checkiter->second == false)
			Chessiter->second.OpDistance[Edge] = 9999999;
		Checkiter++;
	}
	BoardReset(Checkboard);
}

void HexAgent::ComputePotentials()
{
	MyDoubleDistance(0);
	MyDoubleDistance(1);
	OpDoubleDistance(0);
	OpDoubleDistance(1);

	std::map<int, lynch::PosInfo>::iterator iter;
	for (iter = Chessboard->begin(); iter != Chessboard->end(); iter++) {
		if (iter->second.Occupation == 0) {
			iter->second.MyPotential = iter->second.MyDistance[0] + iter->second.MyDistance[1];
			iter->second.OpPotential = iter->second.OpDistance[0] + iter->second.OpDistance[1];
			iter->second.TotalPotential = iter->second.MyPotential + iter->second.OpPotential;
		}
	}

}
