# Getting started
Open Visual Studio 2022 (This might work with different version of Visual Studio but I can't guarantee it).

Under "Get started" click on **Clone repository**.

Under "Repository location" write: https://github.com/xXStormXx2000/Game.git and click **Clone**.

# How to use the engine
You have to write the main function correctly because this engine uses SDL.

Like this:
```
int main(int argc, char* argv[]) {
    return 0;
}
```

To run the engine you have to create a instant of the application class and run it.

Like this:
```
#include "Application.h"

int main(int argc, char* argv[]) {
    int windowWidth = 1100, windowHeight = 800;
    int fps = 60;

    Application game("GameName", "WindowIcon.png", windowWidth, windowHeight, fps);
    game.run();

    return 0;
}
```

You can add and remove systems/Component by calling the addSystem/addComponent method.

Like this:
```
#include "Application.h"
#include "MySystemOne.h"
#include "MySystemTwo.h"
#include "MyComponent.h"

int main(int argc, char* argv[]) {
    int windowWidth = 1100, windowHeight = 800;
    int fps = 60;

    Application game("GameName", "WindowIcon.png", windowWidth, windowHeight, fps);

    game.addSystem<MySystemOne>();
    game.addSystem<MySystemTwo>();
    game.addComponent<MyComponent>();

    game.run();
    return 0;
}
```
## Creating custom system
You can also create your own systems by inheriting from the "System" class. 
There's are functions you can overload when implementing your system but none of them are required.

Like this:
```
#pragma once
#include "System.h"

class MySystemOne : public System {
public:
    void onCollision(const CollisionEvent&);

    void start(Entity);
  
    void preUpdate(Entity);
    void update(Entity);
    void postUpdate(Entity);
  
    void end(Entity);
};
```
## Creating custom component
You can create your own custom components by inheriting from the "Component" class.
You wil need to overwrite the "readFile" function and the "writeFile" function.

Like this:

MyComponent.h:
```

#pragma once
#include "Components.h"

class MyComponent : public Component {
public:
    float myDataOne;
    Vector3D myDataTwo;

    Component* readFile(std::ifstream&, std::string&);
    void writeFile(std::ofstream&);
};
```
MyComponent.cpp:
```
#include "MyComponent.h"

Component* MyComponent::readFile(std::ifstream& file, std::string& str) {
    if (str == "MyComponentName") {
        MyComponent* data = new MyComponent();
        file >> data->myDataOne;
        file >> data->myDataTwo.x >> data->myDataTwo.y >> data->myDataTwo.z;
        return dynamic_cast<Component*>(data);
    }
    return nullptr;
}

void MyComponent::writeFile(std::ofstream& file) {
    file << "MyComponentName" << std::endl;
    file << data->myDataOne << std::endl;
    file << data->myDataTwo.x << ' ' << data->myDataTwo.y << ' ' << data->myDataTwo.z << ' ' << std::endl;
}
```

