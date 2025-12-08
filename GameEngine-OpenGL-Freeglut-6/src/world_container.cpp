#include "../include/world_container.h"
#include "../include/GameObject.h"
#include "../include/r_window.h"
#include "../include/component/Camera.h"
#include "../include/component/Renderer.h"
#include "../include/component/Transform.h"

world_container::world_container(r_window* rw) : rw(rw) {
}

void world_container::init() {
    rw->getInputRef().addKey('U');
    rw->getInputRef().addKey('P');
	
    util::mesh_holder_instance.loadStaticMeshToHolder("Cube", "assets/Cube.obj");
    util::mesh_holder_instance.loadStaticMeshToHolder("Stanford Bunny", "assets/bunny.obj");
    util::mesh_holder_instance.loadStaticMeshToHolder("House", "assets/Residential Buildings 001.obj");
    util::mesh_holder_instance.loadStaticMeshToHolder("Ground", "assets/Flat-Ground-Grass.obj");
    util::textures_holder_instance.loadTextureToHolder("testImage", "assets/Shalvex Novachrono.jpg");
    util::textures_holder_instance.loadTextureToHolder("testImage2", "assets/Square swirls.png");
    util::textures_holder_instance.loadTextureToHolder("Test Image", "assets/Test Image.jpg");
    //util::textures_holder_instance.loadTextureToHolder("Test Grid", "assets/testGrid.png");
    util::textures_holder_instance.loadTextureToHolder("GroundImage", "assets/Flat-Ground-Grass.jpg");
    
    addGameObject(new GameObject(rw, "Camera ", 0));
    gameObjects.back()->addComponent(make_unique<Camera>());
    
    
    
    for (int i = 1; i < 10; i++) {
        addGameObject(new GameObject(rw, "GameObject " + to_string(i), i));
    }

    gameObjects[1]->addComponent(make_unique<Renderer>());
    gameObjects[1]->transform->setWorldPosition({0, -10, 0});
    
    if (Renderer* renderer = gameObjects[1]->getComponent<Renderer>()) {
        renderer->setMesh("Ground");
        renderer->setTexture("GroundImage");
    }

    

    gameObjects[2]->addComponent(make_unique<Renderer>());
    
    if (Renderer* renderer = gameObjects[2]->getComponent<Renderer>()) {
        renderer->setMesh("Cube");
        renderer->setTexture("testImage");
    }

    gameObjects[3]->addComponent(make_unique<Renderer>());
    gameObjects[3]->transform->setWorldPosition({50, -10, 0});
    
    if (Renderer* renderer = gameObjects[3]->getComponent<Renderer>()) {
        renderer->setMesh("House");
        renderer->setTexture("Test Image");
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
            [](const GameObject* obj) { return obj->isDestroyed(); }
        ),
        gameObjects.end()
    );
}

void world_container::display() const {
    for (GameObject* gameObject : gameObjects) {
        if (!gameObject->getActivity()) continue;
        if (gameObject->isDestroyed()) continue;
        gameObject->display();
    }
}

void world_container::addGameObject(GameObject* gameObject) {
    gameObjects.push_back(gameObject);
}
