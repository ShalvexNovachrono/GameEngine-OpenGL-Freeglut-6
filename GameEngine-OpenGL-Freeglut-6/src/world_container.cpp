#include "../include/world_container.h"
#include "../include/GameObject.h"
#include "../include/r_window.h"

world_container::world_container(r_window* rw) : rw(rw) {
}

void world_container::init() {
    for (int i = 0; i < 1000; i++) {
        addGameObject(new GameObject(rw, "GameObject " + to_string(i), i));
    }
    
}

void world_container::update() {    
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
