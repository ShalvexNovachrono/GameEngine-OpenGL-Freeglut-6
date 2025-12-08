#pragma once
#include "main.h"

class GameObject;
class r_window;

class world_container {
    vector<GameObject*> gameObjects;
    r_window* rw;
public:
    world_container(r_window* rw);
    ~world_container();

    void init();
    void update();
    void display() const;

    void addGameObject(GameObject* gameObject);
};
