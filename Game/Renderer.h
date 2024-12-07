#pragma once

#include "SDL.h"
#include "Debug.h"
#include "SDL_image.h"
#include "DynamicArray.h"
#include <map>
#include "Entity.h"
#include "Scene.h"
#include <string>

using DrawMap = std::map<float, DynamicArray<Entity>>;

class Renderer {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Scene* scene;

	DrawMap entitys;
public:
	Renderer(SDL_Window* window);
	Renderer();

	void setScene(Scene*);

	Renderer& operator=(Renderer&&) noexcept;

	bool exist();
	void render();

	SDL_Texture* createTexture(int, int);
	void addToTexture(const std::string&, SDL_Texture*);
	static void destroyTextures(CompMap<Sprite>&);

	void setEntitys(DrawMap&);

	~Renderer();
};

