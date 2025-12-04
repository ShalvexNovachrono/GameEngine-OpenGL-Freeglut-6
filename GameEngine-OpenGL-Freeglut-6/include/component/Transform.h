#pragma once
#include "base_component.h"
#include "../extra_math.h"

class GameObject;

class Transform : public base_component {
    GameObject* parent = nullptr;
    vector<GameObject*> children;
public:
    Vec3 position;
    Vec3 rotation;
    Vec3 scale;
    
    Transform();
    Transform(const Transform& other);
    Transform(Transform&& other) noexcept;

    ~Transform();

    void setParent(GameObject* parentGameObject, bool internal = false);
    GameObject& getParent() const;
    
    void addChild(GameObject* child, bool internal = false);
    void removeChild(const GameObject* child, bool internal = false);
    void start() override;
    void update() override;
    Transform* clone() override;
};

