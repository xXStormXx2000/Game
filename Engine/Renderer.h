#pragma once

#include "SDL.h"
#include "Debug.h"
#include "SDL_image.h"
#include "DynamicArray.h"
#include <map>
#include "Entity.h"
#include "Scene.h"
#include "SharedResources.h"
#include <string>

struct Tile
{
	short tileX, tileY;
	int posX, posY;
};
using DrawMap = std::map<float, DynamicArray<Entity>>;
struct TileSet {
	Vector3D offset = { 0,0,0 }; // pixels
	int tileWidth = 0, tileHeight = 0; // pixels
	int textureTileWidth = 0, textureTileHeight = 0; // pixels
	int spriteIndex = -1;
	float depht = 0;
	DynamicArray<Tile> tiles;
	TileSet readFile(std::ifstream&);
	void writeFile(std::ofstream&);
};

class Renderer {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Scene* scene;
	SharedResources* sharedResources;

	DynamicArray<SDL_Texture*> sprites;
	DynamicArray<TileSet> tileSets;


	float cameraWidth = 0, cameraHeight = 0;
	Vector3D cameraOffset = { 0, 0, 0 };
	Entity cameraFollowEntity = -1;

	SDL_Rect cameraTransform(Transform, const Sprite&);

	DrawMap entitys;

	SDL_Texture* font;
	struct TextDrawCall {
		std::string text;
		Vector3D pos;
	};
	DynamicArray<TextDrawCall> textToDraw;
	void drawText(const std::string&, Vector3D);

public:
	Renderer(SDL_Window* window);
	Renderer();

	void setScene(Scene*);

	Renderer& operator=(Renderer&&) noexcept;

	bool exist();
	void render();

	void createTexture(const std::string&);
	void destroyTextures();

	void setEntitys(DrawMap&);

	void setTileSets(DynamicArray<TileSet>&);

	void addEntity(Entity);
	void removeEntity(Entity);

	float getCameraWidth();
	float getCameraHeight();

	void setCameraWidth(float);
	void setCameraHeight(float);

	Entity getCameraFollowEntity();
	void setCameraFollowEntity(Entity);

	Vector3D getCameraOffset();
	void setCameraOffset(Vector3D);

	void addTextToDraw(const std::string&, Vector3D);

	void setSharedResources(SharedResources*);

	~Renderer();
};

