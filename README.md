Open Visual Studio 2022 (This might work with different version of Visual Studio but I can't guarantee it).

Under "Get started" click on "Clone repository".

Under "Repository location" write: "https://github.com/xXStormXx2000/Game.git" and click "Clone".


You can create your own custom components by inheriting from the "Component" class.
You wil need to overwrite the "readFile" function and the "writeFile" function.
Then you can add it to the application by using "addComponent<MyComponent>()" method.


You can also create your own systems by inheriting from the "System" class.
Then you can add it to the application by using "addSystem<MySystem>()" method.
