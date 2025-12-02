#pragma once
#include "main.h"
#include "idh.h"

class engine;
using namespace std;

class GameObject {
private:
    bool IsDestroyed = false;
public:
    float& deltaTime;
    Input* input;
    engine* _engine;

    string NAME;
    int ID;

    GameObject() = default;
};