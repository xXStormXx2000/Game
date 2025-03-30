#include "Application.h"
#include "GUI.h"
int main(int argc, char* argv[]) {
	Application editor("Level editor", "", 1280, 720, 60);
	editor.setStartScene("Scene.wgf");
	GUI* gui = new GUI;
	gui->setGamePath("../TestGame");
	editor.addSystem(gui);
	editor.run();
	return 0;
}