#include "../../include/component/Transform.h"
#include "../../include/GameObject.h"


Transform::Transform() {
    position = Vec3(0, 0, 0);
    rotation = Vec3(0, 0, 0);
    scale = Vec3(1, 1, 1);
}

Transform::Transform(const Transform& other) {
    parent = other.parent;
    children = other.children;
    position = other.position;
    rotation = other.rotation;
    scale = other.scale;
}

Transform::Transform(Transform&& other) noexcept {
    parent = other.parent;
    children = other.children;
    position = other.position;
    rotation = other.rotation;
    scale = other.scale;
}

Transform::~Transform() {
}

void Transform::setParent(GameObject* parentGameObject, bool internal) {
    if (parent == parentGameObject) return;

    if (parent != nullptr) {
        parent->transform->removeChild(gameObject, true);
    }

    parent = parentGameObject;
    
    if (!internal && parent != nullptr) {
        parent->transform->addChild(gameObject, true);
    }
}

GameObject& Transform::getParent() const {
    return *parent;
}

void Transform::addChild(GameObject* child, bool internal) {
    children.push_back(child);
    
    if (!internal && child != nullptr) {
        child->transform->setParent(gameObject, true);
    }
}

void Transform::removeChild(const GameObject* child, bool internal) {
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            children.erase(it);
            break;
        }
    }

    if (!internal && child != nullptr) {
        child->transform->setParent(nullptr, true);
    }
}

void Transform::start() {
    base_component::start();
}


void Transform::update() {
    base_component::update();
}

Transform* Transform::clone() {
    return new Transform(*this);
}

