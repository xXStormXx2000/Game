#include "Application.h"
#include "Softbody.h" 

int main(int argc, char* argv[]) {
	
	Application game("Game", "Assets/Art/smil.png", 1100, 800, 60);

	game.addSystem(new Softbody());
	
	game.run();
	return 0;
}