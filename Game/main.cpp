#include "Application.h"
#include "Softbody.h"
#include "SoftbodySpawner.h"
#include "somData.h"

int main(int argc, char* argv[]) {
	
	Application game("Game", "Assets/Art/smil.png", 1100, 800, 60);

	game.addSystem<Softbody>();
	game.addSystem<SoftbodySpawner>();
	game.addComponent<somData>();

	game.run();
	return 0;
}