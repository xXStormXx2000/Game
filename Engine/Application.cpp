#include "Application.h"

Application::Application(const char* title, const char* icon, int width, int height, int fps): window(NULL), fps(fps) {

	if(this->audioManager.failure()) return;

	if (SDL_Init(SDL_INIT_EVENTS) < 0) {
		debugMessage("SDL could not initialize! SDL_Error: " << SDL_GetError());
		return;
	}
	

	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (this->window == NULL) {
		debugMessage("Window could not be created! SDL_Error: " << SDL_GetError());
		return;
	}
	if (icon != "") {
		SDL_Surface* iconSurface = IMG_Load(icon);
		if (iconSurface == NULL) {
			debugMessage("Icon could not be loaded! SDL_Error: " << IMG_GetError());
			return;
		}
		SDL_SetWindowIcon(this->window, iconSurface);
		SDL_FreeSurface(iconSurface);
	}

	this->sharedResources.setWindowSize(width, height);

	this->renderer = Renderer(this->window);
	if (!this->renderer.exist()) return;
	this->renderer.setScene(&this->scene);
	this->renderer.setSharedResources(&this->sharedResources);

	this->fileManager.setScene(&this->scene);
	this->fileManager.setRenderer(&this->renderer);
	this->fileManager.setPhysicsEngine(&this->physicsEngine);

	this->physicsEngine.setSharedResources(&this->sharedResources);
	this->physicsEngine.setSystems(&this->systems);

	this->addComponent<EntityFlags>();
	this->addComponent<Transform>();
	this->addComponent<Collider>();
	this->addComponent<Sprite>();

	this->appRunning = true;

}

bool Application::getRunning() {
	return this->appRunning;
}

void Application::run() {
	for (System* system : this->systems) {
		system->setScene(&this->scene);
		system->setSharedResources(&this->sharedResources);
	}

	while (this->getRunning()) {
		this->start();
		while (!this->sharedResources.getSceneChange() && this->getRunning()) {

			this->handleEvents();

			this->update();

			this->collisionHandling();

			this->render();

		}

		this->end();
	}
}

void Application::setStartScene(const std::filesystem::path& path){
	this->sharedResources.setSceneFilePath(path);
}

void Application::start() {
	fileManager.loadScene(this->sharedResources.getSceneFilePath(), this->systems, this->componentsTypes);
	this->renderer.setCameraWidth(this->sharedResources.getWindowWidth());
	this->renderer.setCameraHeight(this->sharedResources.getWindowHeight());
	for (System* sys : systems) {
		for (Entity entity : sys->getEntitys()) {
			this->physicsEngine.addCustomCollisionResolve(entity.getId(), sys);
		}
		sys->run(&System::start);
	}
	timer.start();
}

void Application::handleEvents(){
	SDL_Event SDLevent;
	char key = 0;

	auto helperKeydown = [&](int translateKey, int low, int high)->void {
		translateKey += low;
		if (low <= translateKey && translateKey <= high
			&& !this->sharedResources.getKeyDown(translateKey))
			this->sharedResources.setKeyPressed(translateKey);

		if (this->sharedResources.getKeyPressed(translateKey))
			this->sharedResources.setKeyDown(translateKey, true);
	};

	auto helperKeyup = [&](int translateKey, int low, int high)->void {
		translateKey += low;
		if (low <= translateKey && translateKey <= high)
			this->sharedResources.setKeyReleased(translateKey);

		if (low <= translateKey && translateKey <= high)
			this->sharedResources.setKeyDown(translateKey, false);
	};

	while (SDL_PollEvent(&SDLevent)) {
		switch (SDLevent.type) {
		case(SDL_QUIT):
			this->appRunning = false;
			break;
		case SDL_KEYDOWN:
			key = SDLevent.key.keysym.scancode;
			helperKeydown(key - SDL_SCANCODE_A, 'a', 'z');
			helperKeydown(key - SDL_SCANCODE_1, '1', '9');
			helperKeydown(key - SDL_SCANCODE_0, '0', '0');
			helperKeydown(key - SDL_SCANCODE_RETURN, 0, 47);
			helperKeydown(key - SDL_SCANCODE_RETURN + 47, 58, 96);
			if (this->sharedResources.getTextInputState() && key == SDL_SCANCODE_BACKSPACE) {
				this->sharedResources.popBackTextInput();
			}

			break;
		case SDL_KEYUP:
			key = SDLevent.key.keysym.scancode;
			helperKeyup(key - SDL_SCANCODE_A, 'a', 'z');
			helperKeyup(key - SDL_SCANCODE_1, '1', '9');
			helperKeyup(key - SDL_SCANCODE_0, '0', '0');
			helperKeyup(key - SDL_SCANCODE_RETURN, 0, 47);
			helperKeyup(key - SDL_SCANCODE_RETURN + 47, 58, 96);

			break;
		case SDL_TEXTINPUT:
			if (this->sharedResources.getTextInputState()) {
				this->sharedResources.concatenateTextInput(SDLevent.text.text);
			}
			break;
		default:
			break;
		}
	}
	
	this->sharedResources.updateMouseState();
}

void Application::collisionHandling() {
	this->physicsEngine.addAndRemoveEntitys();
	CollisionEventMap collisions = this->physicsEngine.getAllCollisions();
	this->physicsEngine.resolveCollision(collisions);

	for (const std::pair<Entity, Component*>& pair : this->scene.getComponents<Transform>()) {
		if(this->scene.getComponent<EntityFlags>(pair.first).getFlag(Dynamic))
			this->physicsEngine.applyVelocity(pair.first);
	}
}


void Application::update() {
	if (timer.stop() < 1'000'000/fps) SDL_Delay(int(1'000/fps - timer.stop()/1'000));
	
	this->sharedResources.setDeltaTime(timer.stop()/1'000'000);
	this->timer.start();

	for (System* sys : this->systems) {
		sys->run(&System::preUpdate);
		sys->run(&System::update);
		sys->run(&System::postUpdate);
	}

	for (Entity entity : this->scene.getEntitys()) {
		EntityFlags& ef = this->scene.getComponent<EntityFlags>(entity.getId());
		ef.setFlag(MovedX, false);
		ef.setFlag(MovedY, false);
	}

	this->sharedResources.resetKeysPressed();
	this->sharedResources.resetKeysReleased();
}


void Application::render() {
	this->renderer.render();
}

void Application::end() {
	for (System* sys : systems) {
		sys->run(&System::end);
	}
	this->sharedResources.setSceneChange(false);
}

Application::~Application() {
	for (System* sys : this->systems) delete sys;
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
