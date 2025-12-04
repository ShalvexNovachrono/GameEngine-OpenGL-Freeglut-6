#pragma once
class GameObject;

class base_component {
    int componentID = -1;
protected:
    GameObject* gameObject = nullptr;
    bool removeComponent = false;
    bool disableComponent = false;
public:
    base_component() = default;
    base_component(const base_component& other);
    base_component(base_component&& other) noexcept;
    base_component& operator=(const base_component& other);
    base_component& operator=(base_component&& other) noexcept;
    
    virtual ~base_component();
    virtual void start();
    virtual void update();
    virtual base_component* clone();

    /// <summary>
    /// This function sets the gameObject only if it was not set before.
    /// Also, it sets the componentID
    /// </summary>
    /// <param name="gm">game object</param>
    /// <param name="id">component id</param>
    void setGameObject(GameObject* gm, const int& id);

    GameObject& getGameObject() const;

    int getComponentID() const;

    bool isRemoveComponent() const;
    void setRemoveComponent(const bool& value);
    
    bool isDisableComponent() const;
    void setDisableComponent(const bool& value);
};