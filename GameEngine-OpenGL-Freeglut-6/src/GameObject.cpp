#include "../include/GameObject.h"
#include "../include/r_window.h"
#include "../include/component/Transform.h"

GameObject::GameObject(r_window* rw, const string& name, const int& id) : rw(rw), input(&rw->getInputRef()) {
	this->id = id;
	this->name = name;
	addComponent<Transform>();
	transform = getComponent<Transform>();
}

GameObject::GameObject(const GameObject& other) : destroyed(other.destroyed), paused(other.paused), rw(other.rw), input(&rw->getInputRef()), name(other.name) {
	addComponent<Transform>();
	transform = getComponent<Transform>();
}

GameObject::GameObject(GameObject&& other) noexcept : destroyed(other.destroyed), paused(other.paused), rw(other.rw), input(&rw->getInputRef()), name(other.name) {
	addComponent<Transform>();
	transform = getComponent<Transform>();
}

GameObject& GameObject::operator=(const GameObject& other) {
	if (this == &other) return *this;
	destroyed = other.destroyed;
	paused = other.paused;
	removeAllComponents();
	updateRemoveComponents();

	for (auto& component : other.components) {
	 	components.push_back(unique_ptr<base_component>(component->clone()));
	}
	rw = other.rw;
	input = &rw->getInputRef();
	name = other.name;
	return *this;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept {
	if (this == &other) return *this;
	destroyed = other.destroyed;
	paused = other.paused;
	
	components = std::move(other.components);

	rw = other.rw;
	input = &rw->getInputRef();
	name = other.name;
	return *this;
}

GameObject::~GameObject() = default;

void GameObject::setDestroyed() { 
	if (destroyed == true) return;
	destroyed = true;
}

bool GameObject::isDestroyed() const {
	return destroyed;
}

void GameObject::setActivity(bool value) { 
	paused = !value;
}

bool GameObject::getActivity() const {
	return !paused;
}

template <typename UniqueComponentType>
void GameObject::addComponent() {
	static_assert(is_base_of<base_component, UniqueComponentType>::value, "UniqueComponentType must be derived from base_component");
	
	auto newComponentMade = make_unique<UniqueComponentType>();
	newComponentMade->setGameObject(this, components.size() - 1);
	components.push_back(move(newComponentMade));
	
}

template <typename UniqueComponentType>
UniqueComponentType* GameObject::getComponent() const {
	for (auto& component : components) {
		if (UniqueComponentType* comp = dynamic_cast<UniqueComponentType*>(component.get())) {
			return comp;
		}
	}
	return nullptr;
}

template <typename UniqueComponentType>
bool GameObject::hasComponent(UniqueComponentType* Component) {
	for (auto& component : components) {
		if (dynamic_cast<UniqueComponentType*>(component.get()) == Component) {
			return true;
		}
	}
	return false;
}

void GameObject::update() {
	updateComponents();
}

void GameObject::addComponent(unique_ptr<base_component> component) {
	components.push_back(std::move(component));
	components.back()->setGameObject(this, static_cast<int>(components.size()) - 1);
}


void GameObject::removeComponent(int componentID) const {
	for (auto& component : components) {
		if (component->getComponentID() == componentID) {
			component->setRemoveComponent(true);
		}
	}
}

void GameObject::removeAllComponents() const {
	for (auto& component : components) {
		component->setRemoveComponent(true);
	}
}

void GameObject::updateComponents() const {
	for (auto& component : components) {
		if (component.get() == nullptr) continue;
		if (component->isDisableComponent()) continue;
		if (component->isRemoveComponent()) continue;
		component->update();
	}
}

void GameObject::updateRemoveComponents() {
	if (components.empty()) return;

	for (auto it = components.begin(); it != components.end();) {
		if ((*it)->isRemoveComponent()) {
			it = components.erase(it);
		} else {
			++it;
		}
	}
}

vector<unique_ptr<base_component>>& GameObject::getComponents() {
	return components;
}

int GameObject::getID() const {
	return id;
}

