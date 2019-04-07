#include "GameObject.h"
#include <string>
#include <vector>

using namespace std;

class Game {
	public:
		Game(string filePath);
		void Start();
		void Update();
	private:
		void Deserialize();
};