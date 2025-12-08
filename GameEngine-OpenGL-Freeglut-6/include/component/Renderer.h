#pragma once
#include "../main.h"
#include "base_component.h"
#include "../util.h"

class Renderer : public base_component {
    util::static_mesh mesh;
    bool isMeshLoaded = false;
    util::texture_data texture;
    bool isTextureLoaded = false;
    vector<function<void()>> callbacks;
public:
    Vec4 color = { 1, 1, 1, 1 };
    
    Renderer() = default;
    
    void start() override;
    void update() override;
    void display();

    void setMesh(const string& meshName);
    void setTexture(const string& textureName);
    
    void addCallback(function<void()> callback);
    void removeCallback(function<void()> callback);
    
    Renderer* clone() override;
};
