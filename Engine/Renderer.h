#pragma once

#include "SDL.h"
#include "Debug.h"
#include "SDL_image.h"
#include "DynamicArray.h"
#include <map>
#include <unordered_set>
#include "Entity.h"
#include "Scene.h"
#include "SharedResources.h"
#include <string>
#include <filesystem>

struct Tile
{
	short tileX, tileY;
	int posX, posY;

	bool operator==(const Tile& other) const;
};
namespace std {
	template<>
	struct hash<Tile> {
		typedef Tile argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& obj) const {
			return uint64_t(obj.posX)<<32 + uint64_t(obj.tileY);
		}
	};
}

struct TileSet {
	Vector3D offset = { 0,0,0 }; // pixels
	int tileWidth = 0, tileHeight = 0; // pixels
	int textureTileWidth = 0, textureTileHeight = 0; // pixels
	int spriteIndex = -1;
	float depht = 0;
	std::unordered_set<Tile> tiles;
	TileSet readFile(std::ifstream&);
	void writeFile(std::ofstream&);
};

using DrawMap = std::map<float, std::pair<std::unordered_set<Entity>, std::unordered_set<int>>>;

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

	Vector3D cameraPosTransform(Vector3D);

	DrawMap entitys;

	SDL_Texture* font;
	struct TextDrawCall {
		std::string text;
		Vector3D pos;
	};

public:
	Renderer(SDL_Window* window);
	Renderer();

	void setScene(Scene*);

	Renderer& operator=(Renderer&&) noexcept;

	bool exist();
	void render();

	void createTexture(const std::filesystem::path&);
	void destroyTextures();

	void addTileSet(TileSet&);

	TileSet& getTileSets(int);

	void addEntity(Entity);
	void addEntity(Entity, float);
	void removeEntity(Entity);

	float getCameraWidth();
	float getCameraHeight();

	void setCameraWidth(float);
	void setCameraHeight(float);

	Entity getCameraFollowEntity();
	void setCameraFollowEntity(Entity);

	Vector3D getCameraOffset();
	void setCameraOffset(Vector3D);

	void drawText(const std::string&, Vector3D);

	void setSharedResources(SharedResources*);

	SDL_Renderer* getRenderer();

	~Renderer();
};

