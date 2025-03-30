#include "Application.h"
#include "Interface.h"
int main(int argc, char* argv[]) {
	Application editor("Level editor", "", 1280, 720, 60);
	editor.setStartScene("Scene.wgf");

	Interface* intf = new Interface;
	intf->setGamePath("../TestGame");
	intf->setSceneFile("Tests.wgf");
	editor.addSystem(intf);

	editor.run();
	return 0;
}