#include "Application.h"
#include "Softbody.h"

int main(int argc, char* argv[]) {

	Application game("Game", 1600, 900, 60);

	game.addSystem(new Softbody());

	game.run();
	return 0;
}