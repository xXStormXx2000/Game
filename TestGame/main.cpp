#include "Application.h"
#include "Softbody.h"
#include "SoftbodyNode.h"
#include "somData.h"

int main(int argc, char* argv[]) {
	
	Application game("Game", "Assets/Art/smil.png", 1100, 800, 60);

	game.setStartScene("Assets/Scenes/Tests.wgf");

	game.addSystem<SoftbodyNode>();
	game.addSystem<Softbody>();
	game.addComponent<somData>();

	game.run();
	return 0;
}