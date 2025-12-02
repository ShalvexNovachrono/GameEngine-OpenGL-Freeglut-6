#pragma once
#include "main.h"
#include "idh.h"

class r_window;
using namespace std;

class GameObject {
private:
    bool IsDestroyed = false;
public:
    float& deltaTime;
    Input* input;
    r_window* rw;

    string NAME;
    int ID;

    GameObject(float& dt, Input* in, r_window* _rw, string name, int id) : deltaTime(dt), input(in), rw(_rw) {
		NAME = name;
		ID = id;
    }
};