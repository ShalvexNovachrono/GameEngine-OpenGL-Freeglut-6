#pragma once
#include "../main.h"
#include "base_component.h"
#include "../util.h"

class Renderer : public base_component {
    util::static_mesh mesh;
    util::texture_data texture;
    vector<function<void()>> callbacks;
public:
    Renderer();
    
    void start() override;
    void update() override;
    
    void addCallback(function<void()> callback);
    void removeCallback(function<void()> callback);
    
    Renderer* clone() override;
};
