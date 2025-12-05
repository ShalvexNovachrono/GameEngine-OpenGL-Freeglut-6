#include "../include/world_container.h"
#include "../include/GameObject.h"
#include "../include/r_window.h"
#include "../include/component/Camera.h"

world_container::world_container(r_window* rw) : rw(rw) {
}

void world_container::init() {
    rw->getInputRef().addKey('U');
    rw->getInputRef().addKey('P');
    
    addGameObject(new GameObject(rw, "Camera ", 0));
    gameObjects.back()->addComponent(make_unique<Camera>());
    
    
    
    for (int i = 1; i < 1000; i++) {
        addGameObject(new GameObject(rw, "GameObject " + to_string(i), i));
    }
    
}

void world_container::update() {
    
    if (rw->getInputRef().wasKeyJustPressed('P')) {
        gameObjects[0]->setActivity(false);
    }

    if (rw->getInputRef().wasKeyJustPressed('U')) {
        gameObjects[0]->setActivity(true);
    }

    
    for (GameObject* gameObject : gameObjects) {
        if (!gameObject->getActivity()) continue;
        if (gameObject->isDestroyed()) continue;
        gameObject->update();
    }

    for (GameObject* gameObject : gameObjects) {
        gameObject->updateRemoveComponents();
    }

    gameObjects.erase(
        remove_if(
            gameObjects.begin(),
            gameObjects.end(),
            [](GameObject* obj) { return obj->isDestroyed(); }
        ),
        gameObjects.end()
    );
}

void world_container::addGameObject(GameObject* gameObject) {
    gameObjects.push_back(gameObject);
}
