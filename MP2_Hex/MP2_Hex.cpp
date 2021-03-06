// MP2_Hex.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include <vector>
#include "HexAgent.h"
#include <string>

using namespace std;
//const int GameSize = 11;

int main(int argc, char *argv[])
{
	int GameSize;
	string Color, OpInput;
	Color = argv[1];
	GameSize = stoi(argv[2]);

	HexAgent HAgent(GameSize, 2);

	if (Color == "RED") {
		while (true) {
			string str = HAgent.Output();
			cout << str << endl;
			cin >> OpInput;
			if (OpInput == "exit")
				break;
			HAgent.Receive(OpInput, 2);
		}
	}
	else if (Color == "BLUE") {
		while (true){
			cin >> OpInput;
			if (OpInput == "exit")
				break;
			HAgent.Receive(OpInput, 2);
			string str = HAgent.Output();
			cout << str << endl;
		}
	}
	//system("pause");
    return 0;
}

