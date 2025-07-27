#include "Renderer.h"


TileSet TileSet::readFile(std::ifstream& file) {
	TileSet ts;
	file >> ts.offset.x >> ts.offset.y >> ts.offset.z;
	file >> ts.tileWidth >> ts.tileHeight;
	file >> ts.textureTileWidth >> ts.textureTileHeight;
	file >> ts.spriteIndex;
	file >> ts.depht;
	int size;
	file >> size;
	for (int i = 0; i < size; i++) {
		Tile t;
		file >> t.tileX >> t.tileY >> t.posX >> t.posY;
		ts.tiles.insert(t);
	}
	return ts;
}

void TileSet::writeFile(std::ofstream& file) {
	file << this->offset.x << ' ' << this->offset.y << ' ' << this->offset.z << '\n';
	file << this->tileWidth << ' ' << this->tileHeight << '\n';
	file << this->textureTileWidth << ' ' << this->textureTileHeight << '\n';
	file << this->spriteIndex << '\n';
	file << this->depht << '\n';
	file << this->tiles.size() << '\n';
	for (const Tile& tile: this->tiles) {
		file << tile.tileX << tile.tileY << tile.posX << tile.posY << '\t';
	}
}



Vector3D Renderer::cameraPosTransform(Vector3D absPos) {
	absPos -= this->getCameraOffset();
	if (this->getCameraFollowEntity() != -1)
		absPos -= this->scene->getComponent<Transform>(this->getCameraFollowEntity().getId()).position;
	float xScale = this->sharedResources->getWindowWidth() / this->cameraWidth;
	float yScale = this->sharedResources->getWindowHeight() / this->cameraHeight;
	return absPos.hadamardProduct({ xScale, yScale, 1 });
}

Renderer::Renderer(SDL_Window* window): window(window), renderer(NULL), scene(nullptr), font(NULL), sharedResources(nullptr) {

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		debugMessage("SDL_image could not initialize! SDL_image Error: " << IMG_GetError());
		return;
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == NULL) {
		debugMessage("Renderer could not be created! SDL_Error: " << SDL_GetError());
		return;
	}

	SDL_Surface* surface = IMG_Load("../Engine/Font.png");

	this->font = SDL_CreateTextureFromSurface(this->renderer, surface);
	if (this->font == NULL) debugMessage("SDL_image Error: " << IMG_GetError());

	SDL_SetTextureBlendMode(this->font, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(surface);
}

Renderer::Renderer(): window(NULL), renderer(NULL), scene(nullptr), font(NULL), sharedResources(nullptr) {
}

void Renderer::setScene(Scene* scene) {
	this->scene = scene;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept {
	this->window = other.window;
	this->renderer = other.renderer;
	this->scene = other.scene;
	this->font = other.font;
	other.window = NULL;
	other.renderer = NULL;
	other.scene = nullptr;
	other.font = nullptr;
	return *this;
}

bool Renderer::exist() {
	return this->renderer != NULL;
}

void Renderer::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(this->renderer);
	SDL_RenderClear(this->renderer);
	float xScale = this->sharedResources->getWindowWidth() / this->cameraWidth;
	float yScale = this->sharedResources->getWindowHeight() / this->cameraHeight;
	for (auto [depht, entityList] : this->entitys) {
		for (int tileSetID: entityList.second) {
			TileSet& tileSet = this->tileSets[tileSetID];
			float w = tileSet.tileWidth * xScale;
			float h = tileSet.tileHeight * yScale;
			for (const Tile& tile : tileSet.tiles) {
				SDL_Rect target;
				Vector3D pos = cameraPosTransform({ float(tile.posX*tileSet.tileWidth), float(tile.posY*tileSet.tileHeight), 0 });
				target.x = pos.x;
				target.y = pos.y;
				target.w = round(w);
				target.h = round(h);

				SDL_Rect texturePortion;
				texturePortion.x = tileSet.textureTileWidth * tile.tileX;
				texturePortion.y = tileSet.textureTileHeight * tile.tileY;
				texturePortion.w = tileSet.textureTileWidth;
				texturePortion.h = tileSet.textureTileHeight;

				if (SDL_RenderCopy(this->renderer, this->sprites[tileSet.spriteIndex], &texturePortion, &target) != 0) {
					debugMessage("SDL_RenderCopy Error: " << SDL_GetError());
				}
			}
		}
		for (Entity entity: entityList.first) {
			EntityFlags flags = this->scene->getComponent<EntityFlags>(entity.getId());
			if (!flags.getFlag(Active) || !flags.getFlag(Visible)) continue;
			assert(this->scene != nullptr && "Renderer can't find scene");
			Transform& tf = this->scene->getComponent<Transform>(entity.getId());

			Sprite& sp = this->scene->getComponent<Sprite>(entity.getId());

			SDL_Rect target;
			Vector3D pos = cameraPosTransform(tf.position + sp.offset);
			target.x = pos.x;
			target.y = pos.y;
			target.w = round(sp.width * tf.scale.x * xScale);
			target.h = round(sp.height * tf.scale.y * yScale);
			if (SDL_RenderCopy(this->renderer, this->sprites[sp.spriteIndex], &sp.texturePortion, &target) != 0) {
				debugMessage("SDL_RenderCopy Error: " << SDL_GetError());
			}
		}
	}
}

void Renderer::createTexture(const std::filesystem::path& path) {
	SDL_Surface* surface = IMG_Load(path.string().c_str());

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

void Renderer::addTileSet(TileSet& tileSet) {
	this->tileSets.pushBack(std::move(tileSet));
	this->entitys[tileSet.depht].second.insert(this->tileSets.size() - 1);
}

TileSet& Renderer::getTileSets(int num) {
	return this->tileSets[num];
}

void Renderer::addEntity(Entity entity, float depth) {
	this->entitys[depth].first.insert(entity);
}

void Renderer::addEntity(Entity entity) {
	Transform tf = this->scene->getComponent<Transform>(entity.getId());
	this->entitys[tf.position.z].first.insert(entity);
}

void Renderer::removeEntity(Entity entity) {
	Transform tf = this->scene->getComponent<Transform>(entity.getId());
	this->entitys[tf.position.z].first.erase(this->entitys[tf.position.z].first.find(entity));
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

Vector3D Renderer::getCameraPos() {
	Vector3D pos = this->getCameraOffset();
	if (this->getCameraFollowEntity() != -1)
		pos += this->scene->getComponent<Transform>(this->getCameraFollowEntity().getId()).position;
	return pos;
}

void Renderer::setSharedResources(SharedResources* sr) {
	this->sharedResources = sr;
}

SDL_Renderer* Renderer::getRenderer() {
	return this->renderer;
}

void Renderer::drawText(const std::string& text, Vector3D pos) {
	// pos.z is the scale
	int line = 0, x = 0;
	for (int i = 0; i < text.size(); i++) {
		char c = text.at(i);
		if (c == '\n') {
			line++;
			x = 0;
			continue;
		}
		SDL_Rect target;
		target.y = (22*line) * pos.z + int(pos.y);
		target.w = 16 * pos.z;
		target.h = 21 * pos.z;

		SDL_Rect texturePortion;
		texturePortion.y = -1;
		texturePortion.w = 16;
		texturePortion.h = 21;
		if ('A' <= c && c <= 'Z') {
			texturePortion.y = 0;
			texturePortion.x = 16 * (c-'A');
		}
		if ('a' <= c && c <= 'z') {
			texturePortion.y = 21;
			texturePortion.x = 16 * (c - 'a');
			if(c == 'l') x -= 3 * pos.z;
		}
		if ('0' <= c && c <= '9') {
			texturePortion.y = 42;
			texturePortion.x = 16 * (c - '0');
		}
		target.x = x + int(pos.x) * pos.z;
		if (texturePortion.y == -1) {
			texturePortion.y = 63;

			texturePortion.x = 6 * (c - ' ');
			texturePortion.w = 6;

			x += 8 * pos.z;
			target.w = 6 * pos.z;
			if (c == '\'') x -= 4 * pos.z;
		} else {
			x += 17 * pos.z;
			if (c == 'l') x -= 3 * pos.z;
		}
		if (SDL_RenderCopy(this->renderer, this->font, &texturePortion, &target) != 0) {
			debugMessage("SDL_RenderCopy Error: " << SDL_GetError());
		}
	}
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(this->renderer);
}

bool Tile::operator==(const Tile& other) const {
	return this->posX == other.posX && this->posY == other.posY;
}
