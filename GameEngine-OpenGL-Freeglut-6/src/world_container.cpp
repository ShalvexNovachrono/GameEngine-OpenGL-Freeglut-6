#include "../include/world_container.h"
#include "../include/GameObject.h"
#include "../include/r_window.h"
#include "../include/component/Camera.h"
#include "../include/component/Renderer.h"
#include "../include/component/Transform.h"

world_container::world_container(r_window* rw) : rw(rw) {
}

void world_container::init() {
    
    util::input->addKey('U');
    util::input->addKey('P');
    util::input->addKey(VK_DELETE);
	
    util::mesh_collection_instance.loadStaticMeshToHolder("Cube", "assets/Cube.obj");
    util::mesh_collection_instance.loadStaticMeshToHolder("Stanford Bunny", "assets/bunny.obj");
    util::mesh_collection_instance.loadStaticMeshToHolder("House", "assets/Residential Buildings 001.obj");
    util::mesh_collection_instance.loadStaticMeshToHolder("911GT2", "assets/Porsche_911_GT2.obj");
    util::mesh_collection_instance.loadStaticMeshToHolder("Ground", "assets/Flat-Ground-Grass.obj");
    util::textures_data_collection_instance.loadTextureToHolder("testImage", "assets/Shalvex Novachrono.jpg");
    util::textures_data_collection_instance.loadTextureToHolder("testImage2", "assets/Square swirls.png");
    util::textures_data_collection_instance.loadTextureToHolder("Test Image", "assets/0000.BMP");
    //util::textures_data_collection_instance.loadTextureToHolder("Test Grid", "assets/testGrid.png");
    util::textures_data_collection_instance.loadTextureToHolder("GroundImage", "assets/Flat-Ground-Grass.jpg");
    
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
        renderer->setTexture("testImage");
    }


    

    gameObjects[4]->addComponent(make_unique<Renderer>());
    gameObjects[4]->transform->setWorldPosition({-25, 10, 25});
    
    if (Renderer* renderer = gameObjects[4]->getComponent<Renderer>()) {
        renderer->setMesh("911GT2");
        renderer->setTexture("Test Image");
    }
}

void world_container::update() {
    
    if (util::input->wasKeyJustPressed('P')) {
        gameObjects[0]->setActivity(false);
    }

    if (util::input->wasKeyJustPressed('U')) {
        gameObjects[0]->setActivity(true);
    }

    if (util::input->wasKeyJustPressed(VK_DELETE)) {
        if (gameObjects.size() > 1) {
            gameObjects.back()->setDestroyed();
        }
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
