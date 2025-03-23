#include "Renderer.h"

SDL_Rect Renderer::cameraTransform(Transform tf, const Sprite& sp) {
	tf.position -= this->cameraOffset;
	if (this->cameraFollowEntity.getId() != -1)
		tf.position -= this->scene->getComponent<Transform>(this->cameraFollowEntity.getId()).position;
	SDL_Rect target;
	tf.position += sp.offset;
	float xScale = this->scene->getWidth() / this->cameraWidth;
	float yScale = this->scene->getHeight() / this->cameraHeight;
	target.x = tf.position.x * xScale;
	target.y = tf.position.y * yScale;
	target.w = sp.width * tf.scale.x  * xScale;
	target.h = sp.height * tf.scale.y * yScale;
	return target;
}

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

		Sprite& sp = this->scene->getComponent<Sprite>(entity.getId());

		SDL_Rect target = cameraTransform(tf, sp);

		if (SDL_RenderCopy(this->renderer, this->sprites[sp.spriteIndex], &sp.texturePortion, &target) != 0) {
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

void Renderer::addEntity(Entity entity) {
	Transform tf = this->scene->getComponent<Transform>(entity.getId());
	this->entitys[tf.position.z].pushBack(entity);
}

void Renderer::removeEntity(Entity entity) {
	Transform tf = this->scene->getComponent<Transform>(entity.getId());
	this->entitys[tf.position.z].erase(this->entitys[tf.position.z].find(entity));
}

float Renderer::getCameraWidth() {
	return this->cameraWidth;
}

float Renderer::getCameraHeight() {
	return this->cameraHeight;
}

void Renderer::setCameraWidth(float size) {
	this->cameraWidth = size;
}

void Renderer::setCameraHeight(float size) {
	this->cameraHeight = size;
}

Entity Renderer::getCameraFollowEntity() {
	return this->cameraFollowEntity;
}

void Renderer::setCameraFollowEntity(Entity e) {
	this->cameraFollowEntity = e;
}

Vector3D Renderer::getCameraOffset() {
	return this->cameraOffset;
}

void Renderer::setCameraOffset(Vector3D offset) {
	this->cameraOffset = offset;
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(this->renderer);
}
