#include "Application.h"
#include "Softbody.h"
#include "SoftbodyNode.h"
#include "somData.h"
#include "Controler.h"

int main(int argc, char* argv[]) {
	
	Application game("Game", "Assets/Art/smil.png", 1280, 720, 60);

	game.setStartScene("Assets/Scenes/Tests.wgf");

	game.addSystem<SoftbodyNode>();
	game.addSystem<Softbody>();
	game.addComponent<somData>();
	game.addSystem<Controler>();

	game.run();
	return 0;
}