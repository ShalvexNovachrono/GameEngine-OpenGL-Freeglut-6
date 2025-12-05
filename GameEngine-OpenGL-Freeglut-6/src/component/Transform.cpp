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

Vec3 Transform::getLocalPosition() const {
    if (parent == nullptr) return position;
    return position - parent->transform->getLocalPosition();
}

void Transform::setLocalPosition(const Vec3& position) {
    if (parent == nullptr) this->position = position;
    else this->position = position + parent->transform->getLocalPosition();
}

Vec3 Transform::getWorldPosition() const {
    return position;
}

void Transform::setWorldPosition(const Vec3& position) {
    this->position = position;
}

Vec3 Transform::getLocalRotation() const {
    if (parent == nullptr) return rotation;
    return rotation - parent->transform->getLocalRotation();
}

void Transform::setLocalRotation(const Vec3& rotation) {
    if (parent == nullptr) this->rotation = rotation;
    else this->rotation = rotation + parent->transform->getLocalRotation();
}

Vec3 Transform::getWorldRotation() const {
    return rotation;
}

void Transform::setWorldRotation(const Vec3& rotation) {
    this->rotation = rotation;
}

void Transform::getBasis(Vec3& outForward, Vec3& outRight, Vec3& outUp) const {
    float pitch = rotation.x;
    float yaw   = rotation.y;
    float roll  = rotation.z;

    Vec3 worldUp    = Vec3::Up();
    Vec3 baseFwd    = Vec3::Forward();
    Vec3 baseRight  = Vec3::Right();

    // yaw
    Vec3 forward = Vec3::rotate3d(baseFwd,   worldUp, yaw);
    Vec3 right   = Vec3::rotate3d(baseRight, worldUp, yaw);

    // pitch
    forward = Vec3::rotate3d(forward, right, pitch);
    Vec3 up = Vec3::Cross(right, forward).Normalize();

    // roll
    right = Vec3::rotate3d(right, forward, roll);
    up    = Vec3::rotate3d(up,    forward, roll);

    outForward = forward.Normalize();
    outRight   = right.Normalize();
    outUp      = up.Normalize();
}

Vec3 Transform::getForward() const {
    Vec3 f, r, u;
    getBasis(f, r, u);
    return f;
}

Vec3 Transform::getRight() const {
    Vec3 f, r, u;
    getBasis(f, r, u);
    return r;
}

Vec3 Transform::getUp() const {
    Vec3 f, r, u;
    getBasis(f, r, u);
    return u;
}
