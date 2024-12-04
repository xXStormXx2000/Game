#include "Application.h"

Application::Application(const char* title, int width, int height, int fps): window(NULL), fps(fps) { 
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		debugMessage("SDL could not initialize! SDL_Error: " << SDL_GetError());
		return;
	}
	this->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (this->window == NULL) {
		debugMessage("Window could not be created! SDL_Error: " << SDL_GetError());
		SDL_Quit();
		return;
	}

	this->renderer = Renderer(this->window, &this->scene);
	if (!this->renderer.exist()) {
		SDL_DestroyWindow(this->window);
		SDL_Quit();
		return;
	}
	this->physicsEngine = PhysicsEngine(&this->scene, &this->systems, &this->sharedResources);
	this->fileManager = FileManager(&this->scene, &this->renderer, &this->physicsEngine);
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
	fileManager.loadScene("Assets/Scenes/Test.wgf", this->systems);
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
	char keys = 0;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case(SDL_QUIT):
			this->appRunning = false;
			break;
		case SDL_KEYDOWN:
			keys = event.key.keysym.scancode - SDL_SCANCODE_A + 'a';
			if('a' <= keys && keys <= 'z') this->sharedResources.setKeyPressed(keys, true);
			break;
		case SDL_KEYUP:
			keys = event.key.keysym.scancode - SDL_SCANCODE_A + 'a';
			if ('a' <= keys && keys <= 'z') this->sharedResources.setKeyPressed(keys, false);
			break;
		default:
			break;
		}
	}
}

void Application::collisionHandling() {
	CollisionEventMap col;
	for (int i = 0; i < 10; i++) {
		col = this->physicsEngine.checkAndResolveAllCollisions();
		if (col.size() == 0) break;
	}
	for (const std::pair<int, Transform>& pair : this->scene.getComponents<Transform>()) {
		this->physicsEngine.applyVelocity(pair.first);
	}
}


void Application::update() {

	this->sharedResources.setDeltaTime(timer.stop());
	debugMessage(timer.stop() / 1'000);
	if (this->sharedResources.getDeltaTime() < 1'000'000/fps) std::this_thread::sleep_for(std::chrono::microseconds(int(1'000'000/fps - this->sharedResources.getDeltaTime())));
	this->sharedResources.setDeltaTime(timer.stop()/1'000'000);
	this->timer.start();

	for (System* sys : this->systems) {
		sys->run(&System::preUpdate);
		sys->run(&System::update);
		sys->run(&System::postUpdate);
	}
	for (Entity entity : this->scene.getEntitys()) {
		EntityFlags& ef = this->scene.getComponent<EntityFlags>(entity.getId());
		ef.setMovedX(false);
		ef.setMovedY(false);
	}
	//this->sharedResources.resetKeysPressed();
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
