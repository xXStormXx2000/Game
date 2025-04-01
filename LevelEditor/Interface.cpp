#include "Interface.h"

DynamicArray<std::filesystem::path> Interface::findFiles(const std::filesystem::path& folder) {
    if (!std::filesystem::exists(folder) || !std::filesystem::is_directory(folder)) {
        debugMessage("Invalid directory: " << folder);
        return {};
    }
    DynamicArray<std::filesystem::path> out;
    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(folder)) {
        if (std::filesystem::is_regular_file(entry.path())) {
            out.pushBack(entry.path());
        }
    }
    return out;
}

bool Interface::isNum(const std::string& str) {
    if (str.size() == 0) return false;
    for(char c: str) 
        if('0' > c || c > '9') 
            return false;
    return true;
}

void Interface::setGamePath(const std::filesystem::path& path) {
	this->gamePath = path;
}

void Interface::setSceneFile(const std::filesystem::path& file) {
    this->sceneFile = file;
}

void Interface::start(Entity) {
	cameraAspectRation = getCameraWidth() / getCameraHeight();
    artFiles = findFiles(gamePath / "Assets" / "Art");
    for (const std::filesystem::path& file : artFiles) {
        addTexture(file);
    }
	TileSet tileSet;
    tileSet.tileWidth = 32;
	tileSet.tileHeight = 32;
	tileSet.textureTileHeight = 16;
	tileSet.textureTileWidth = 16;
	tileSet.offset;
	tileSet.spriteIndex = 4;
	tileSet.depht = 100;
	addTileSet(tileSet);
    audioFiles = findFiles(gamePath / "Assets" / "Audio");
    musicFiles = findFiles(gamePath / "Assets" / "Music");

}

void Interface::update(Entity) {
    std::string sceneW = std::to_string(sceneSizeX), sceneH = std::to_string(sceneSizeY);
    
    float speed = 10;
    if (keyDown('a')) setCameraOffset(getCameraOffset() - Vector3D({ speed, 0, 0 }));
    if (keyDown('d')) setCameraOffset(getCameraOffset() + Vector3D({ speed, 0, 0 }));
    if (keyDown('w')) setCameraOffset(getCameraOffset() - Vector3D({ 0, speed, 0 }));
    if (keyDown('s')) setCameraOffset(getCameraOffset() + Vector3D({ 0, speed, 0 }));

    float x = 10, y = 10;
    if (leftMouseButton()) {
        select = -1;
        if(this->getTextInputState()) this->toggleTextInputState();
		if (!mouseInBox(x, y, 120, 63)) {
			TileSet& tileSet = getTileSet(0);
			Vector3D pos = getMousePos()+getCameraOffset();
            tileSet.tiles.insert({ 0, 0, int(pos.x)/tileSet.tileWidth, int(pos.y)/tileSet.tileHeight});
		}
        if (mouseInBox(x + 20, y + 23, 100, 21)) {
            this->toggleTextInputState();
            select = 0;
        }
        if (mouseInBox(x + 20, y + 23 * 2, 100, 21)) {
            this->toggleTextInputState();
            select = 1;
        }
    }

    if (select != -1) {
        blinkTime++;
        if (blinkTime > 30) blinkTime = 0;
    }

    switch (select) {
    case 0:
        sceneW = this->getTextInput();
        if (blinkTime <= 15) sceneW += '/';
        if (this->keyPressed(0)) {
            if(!isNum(this->getTextInput())) break;
            sceneSizeX = std::stoi(this->getTextInput());
        }
        break;
    case 1:
        sceneH = this->getTextInput();
        if (blinkTime <= 15) sceneH += '/';
        if (this->keyPressed(0)) {
            if (!isNum(this->getTextInput())) break;
            sceneSizeY = std::stoi(this->getTextInput());
        }
        break;
    default:
        break;
    }
    if (this->keyPressed(0)) {
        select = -1;
        blinkTime = 0;
        this->toggleTextInputState();
    }
    this->drawText("Scene Size", { x, y, 1 });
    this->drawText("W - " + sceneW, { x, y + 23, 1 });
    this->drawText("H - " + sceneH, { x, y + 23*2, 1 });


    if (keyDown('q')) {
        if (getCameraHeight() - speed > 1) {
            setCameraWidth(getCameraWidth() - speed * cameraAspectRation);
            setCameraHeight(getCameraHeight() - speed);
        }
    }
    if (keyDown('e')) {
        setCameraWidth(getCameraWidth() + speed * cameraAspectRation);
        setCameraHeight(getCameraHeight() + speed);
    }


    Vector3D origo = getSceneOrigin();
    Vector3D lineEnd = absPosToScenePos({ float(sceneSizeX), float(sceneSizeY), 0 });
    SDL_SetRenderDrawColor(getRenderer(), 255, 0, 0, 255);
    const SDL_Point points[5] = {{origo.x, origo.y}, {lineEnd.x, origo.y}, {lineEnd.x, lineEnd.y}, {origo.x, lineEnd.y}, {origo.x, origo.y}};
    if (SDL_RenderDrawLines(getRenderer(), points, 5)) debugMessage("SDL_RenderCopy Error: " << SDL_GetError());

}
