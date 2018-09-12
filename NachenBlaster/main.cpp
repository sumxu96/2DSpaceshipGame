#include "GameController.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const string assetDirectory = ""; 

class GameWorld;

GameWorld* createStudentWorld(string assetDir = "");

int main(int argc, char* argv[])
{
	{
		string path = assetDirectory;
		if (!path.empty())
			path += '/';
		const string someAsset = "ship.tga";
		ifstream ifs(path + someAsset);
		if (!ifs)
		{
			cout << "Cannot find " << someAsset << " in ";
			cout << (assetDirectory.empty() ? "current directory" : assetDirectory) << endl;
			return 1;
		}
	}

	GameWorld* gw = createStudentWorld(assetDirectory);
	Game().run(argc, argv, gw, "NachenBlaster");
}
