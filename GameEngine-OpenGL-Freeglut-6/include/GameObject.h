#pragma once
#include "main.h"
#include "idh.h"
#include "component/base_component.h"

class Transform;

class GameObject {
    friend class r_window;
    bool destroyed = false;
    bool active = true;
    int id = -1;
    vector<unique_ptr<base_component>> components;
public:
    r_window* rw;
    Input* input;
    Transform* transform = nullptr;

    string name;

    GameObject(r_window* rw, const string& name, const int& id);
    GameObject(const GameObject& other);
    GameObject(GameObject&& other) noexcept;
    GameObject& operator=(const GameObject& other);
    GameObject& operator=(GameObject&& other) noexcept;
    
    ~GameObject();
    void update() const;
    void display() const;
    
    void setDestroyed();
    bool isDestroyed() const;
    /// <summary>
    /// Sets the activity state (paused/running).
    /// </summary>
    /// <param name="value">True to run, false to pause.</param>
    void setActivity(bool value);
    bool getActivity() const;

    void addComponent(unique_ptr<base_component> component);
    template <typename UniqueComponentType>
    void addComponent();

    void removeComponent(int componentID) const;
    void removeAllComponents() const;

    void updateComponents() const;
    void updateRemoveComponents();

    template <typename UniqueComponentType>
    bool hasComponent(UniqueComponentType* Component);
    template <typename UniqueComponentType>
    UniqueComponentType* getComponent() const ;
    vector<unique_ptr<base_component>>& getComponents();

    int getID() const;
};
