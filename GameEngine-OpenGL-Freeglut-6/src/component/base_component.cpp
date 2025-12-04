#include "../../include/component/base_component.h"

base_component::base_component(const base_component& other) :
	componentID(other.componentID),
	gameObject(other.gameObject),
	removeComponent(other.removeComponent),
	disableComponent(other.disableComponent) {
}
base_component::base_component(base_component&& other) noexcept :
	componentID(other.componentID),
	gameObject(other.gameObject),
	removeComponent(other.removeComponent),
	disableComponent(other.disableComponent) {
}

base_component& base_component::operator=(const base_component& other) {
	if (this == &other) return *this;
	componentID = other.componentID;
	gameObject = other.gameObject;
	removeComponent = other.removeComponent;
	disableComponent = other.disableComponent;
	return *this;
}

base_component& base_component::operator=(base_component&& other) noexcept {
	if (this == &other)
		return *this;
	componentID = other.componentID;
	gameObject = other.gameObject;
	removeComponent = other.removeComponent;
	disableComponent = other.disableComponent;
	return *this;
}

base_component::~base_component() {
}

void base_component::start() {
}

void base_component::update() {
}

base_component* base_component::clone() {
	return new base_component(*this);
}

void base_component::setGameObject(GameObject* gm, const int& id) {
	if (gameObject == nullptr) {
		gameObject = gm;
		componentID = id;
	}
}

GameObject& base_component::getGameObject() const {
	return *gameObject;
}

int base_component::getComponentID() const {
	return componentID;
}

bool base_component::isRemoveComponent() const {
	return removeComponent;
}

void base_component::setRemoveComponent(const bool& value) {
	if (removeComponent == false) removeComponent = value;
}

bool base_component::isDisableComponent() const {
	return disableComponent;
}

void base_component::setDisableComponent(const bool& value) {
	disableComponent = value;
}
