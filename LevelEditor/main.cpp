#include "Application.h"

int main(int argc, char* argv[]) {
	Application game("Level editor", "", 1280, 720, 60);
	game.run();
	return 0;
}