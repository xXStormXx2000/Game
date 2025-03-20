#include "Renderer.h"

Renderer::Renderer(SDL_Window* window): window(window), renderer(NULL), scene(nullptr) {

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		debugMessage("SDL_image could not initialize! SDL_image Error: " << IMG_GetError());
		return;
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == NULL) {
		debugMessage("Renderer could not be created! SDL_Error: " << SDL_GetError());
		return;
	}
}

Renderer::Renderer(): window(NULL), renderer(NULL), scene(nullptr) {
}

void Renderer::setScene(Scene* scene) {
	this->scene = scene;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
	this->window = other.window;
	this->renderer = other.renderer;
	this->scene = other.scene;
	other.window = NULL;
	other.renderer = NULL;
	other.scene = nullptr;
	return *this;
}

bool Renderer::exist() {
	return this->renderer != NULL;
}

void Renderer::render() {
	SDL_RenderClear(this->renderer);

	for (auto entityList: this->entitys) for (Entity entity: entityList.second) {
		EntityFlags flags = this->scene->getComponent<EntityFlags>(entity.getId());
		if (!flags.getFlag(Active) || !flags.getFlag(Visible)) continue;
		assert(this->scene != nullptr && "Renderer can't find scene");
		Transform& tf = this->scene->getComponent<Transform>(entity.getId());

		Sprite& sprite = this->scene->getComponent<Sprite>(entity.getId());

		SDL_Rect target;
		target.x = tf.position.x + sprite.offset.x;
		target.y = tf.position.y + sprite.offset.y;
		target.w = sprite.width * tf.scale.x;
		target.h = sprite.height * tf.scale.y;

		if (SDL_RenderCopy(this->renderer, this->sprites[sprite.spriteIndex], &sprite.texturePortion, &target) != 0) {
			debugMessage("SDL_RenderCopy Error: " << SDL_GetError());
		}
	}
	SDL_RenderPresent(this->renderer);
}

void Renderer::createTexture(const std::string& path) {
	SDL_Surface* surface = IMG_Load(path.c_str());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	if(texture == NULL) debugMessage("SDL_image Error: " << IMG_GetError());

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	this->sprites.pushBack(texture);
	SDL_FreeSurface(surface);
}

void Renderer::destroyTextures() {
	for (SDL_Texture* texture : this->sprites) {
		SDL_DestroyTexture(texture);
	}
	this->sprites.empty();
}

void Renderer::setEntitys(DrawMap& newEntitys) {
	this->entitys = std::move(newEntitys);
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(this->renderer);
}
