#include "Application.h"

Application::Application(const char* title, int width, int height, int fps): window(NULL), fps(fps) { 

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

	this->renderer = Renderer(this->window);
	if (!this->renderer.exist()) return;
	this->renderer.setScene(&this->scene);



	this->physicsEngine.setScene(&this->scene);
	this->physicsEngine.setSharedResources(&this->sharedResources);
	this->physicsEngine.setSystems(&this->systems);

	this->fileManager.setScene(&this->scene);
	this->fileManager.setRenderer(&this->renderer);
	this->fileManager.setPhysicsEngine(&this->physicsEngine);

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
		while (!this->getSceneChange() && this->getRunning()) {

			this->collisionHandling();

			this->handleEvents();

			this->update();

			this->render();

		}

		this->end();
	}
}

void Application::addSystem(System* sys) {
	this->systems.pushBack(sys);
}

void Application::start() {
	fileManager.loadScene("Assets/Scenes/Tests.wgf", this->systems);
	for (System* sys : systems) {
		for (Entity entity : sys->getEntitys()) {
			this->physicsEngine.addCustomCollisionResolve(entity.getId(), sys);
		}
		sys->run(&System::start);
	}
	timer.start();
}

void Application::handleEvents(){
	SDL_Event event;
	char key = 0;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case(SDL_QUIT):
			this->appRunning = false;
			break;
		case SDL_KEYDOWN:
			key = event.key.keysym.scancode - SDL_SCANCODE_A + 'a';
			if('a' <= key && key <= 'z' && !this->sharedResources.getKeyDown(key)) this->sharedResources.setKeyPressed(key);
			if (this->sharedResources.getKeyPressed(key)) this->sharedResources.setKeyDown(key, true);
			break;
		case SDL_KEYUP:
			key = event.key.keysym.scancode - SDL_SCANCODE_A + 'a';
			if ('a' <= key && key <= 'z') this->sharedResources.setKeyReleased(key);
			if ('a' <= key && key <= 'z') this->sharedResources.setKeyDown(key, false);
			break;
		default:
			break;
		}
	}
}

void Application::collisionHandling() {
	CollisionEventMap collisions = this->physicsEngine.getAllCollisions(this->physicsEngine.generateCollisionMap());
	this->physicsEngine.resolveCollision(collisions);

	for (const std::pair<int, Transform>& pair : this->scene.getComponents<Transform>()) {
		if(this->scene.getComponent<EntityFlags>(pair.first).getFlag(Dynamic))
			this->physicsEngine.applyVelocity(pair.first);
	}
}


void Application::update() {
	debugMessage(1'000'000/timer.stop());
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
	this->sceneChange = false;
}

bool Application::getSceneChange() {
	return this->sceneChange;
}

Application::~Application() {
	for (System* sys : this->systems) delete sys;
	SDL_DestroyWindow(this->window);
	SDL_Quit();
}
