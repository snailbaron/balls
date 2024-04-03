#pragma once

#include <cstddef>
#include <memory>
#include <queue>
#include <span>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

class Entity {
public:
    explicit Entity(unsigned long long id)
        : _id(id)
    { }

    friend bool operator==(Entity lhs, Entity rhs)
    {
        return lhs._id == rhs._id;
    }

    friend bool operator!=(Entity lhs, Entity rhs)
    {
        return lhs._id != rhs._id;
    }

private:
    unsigned long long _id = 0;

    friend struct std::hash<Entity>;
};

template <> struct std::hash<Entity>{
    size_t operator()(Entity entity) const noexcept
    {
        return std::hash<unsigned long long>{}(entity._id);
    }
};

class EntityPool {
public:
    Entity create()
    {
        if (!_killed.empty()) {
            Entity e = _killed.front();
            _killed.pop();
            return e;
        }

        return Entity{_next++};
    }

    void kill(Entity e)
    {
        _killed.push(e);
    }

private:
    unsigned long long _next = 1;
    std::queue<Entity> _killed;
};

class AbstractComponentStorage {
public:
    ~AbstractComponentStorage() = default;

    virtual void kill(Entity e) = 0;
};

template <class Component>
class ComponentStorage : public AbstractComponentStorage {
public:
    const Component& component(Entity e) const
    {
        return _components.at(_entityIndices.at(e));
    }

    Component& component(Entity e)
    {
        return _components.at(_entityIndices.at(e));
    }

    std::span<const Component> components() const
    {
        return _components;
    }

    std::span<Component> components()
    {
        return _components;
    }

    std::span<const Entity> entities() const
    {
        return _entities;
    }

    Component& add(Entity e)
    {
        size_t index = _entities.size();
        _entities.push_back(e);
        _entityIndices.emplace(e, index);
        return _components.emplace_back();
    }

    void add(Entity e, Component&& component)
    {
        size_t index = _entities.size();
        _entities.push_back(e);
        _entityIndices.emplace(e, index);
        _components.push_back(std::forward<Component>(component));
    }

    void kill(Entity e) override
    {
        size_t index = _entityIndices.at(e);
        _entityIndices.erase(e);
        if (index + 1 < _entities.size()) {
            std::swap(_entities.at(index), _entities.back());
            std::swap(_components.at(index), _components.back());
            _entityIndices[_entities.at(index)] = index;
        }
    }

private:
    std::vector<Entity> _entities;
    std::vector<Component> _components;
    std::unordered_map<Entity, size_t> _entityIndices;
};

class ECS {
public:
    template <class Component>
    const Component& component(Entity e) const
    {
        return storage<Component>().component(e);
    }

    template <class Component>
    Component& component(Entity e)
    {
        return storage<Component>().component(e);
    }

    template <class Component>
    std::span<const Component> components() const
    {
        return storage<Component>().components();
    }

    template <class Component>
    std::span<Component> components()
    {
        return storage<Component>().components();
    }

    template <class Component>
    std::span<const Entity> entities() const
    {
        return storage<Component>().entities();
    }

    template <class Component>
    Component& add(Entity e)
    {
        return storage<Component>().add(e);
    }

    template <class Component>
    void add(Entity e, Component&& component)
    {
        storage<Component>().add(e, component);
    }

    Entity create()
    {
        return _entityPool.create();
    }

    void kill(Entity e)
    {
        for (const auto& [typeIndex, storage] : _componentStorages) {
            storage->kill(e);
        }
        _entityPool.kill(e);
    }

private:
    template <class Component>
    ComponentStorage<Component> storage()
    {
        return static_cast<ComponentStorage<Component>&>(
            *_componentStorages.at(typeid(Component)));
    }

    EntityPool _entityPool;
    std::unordered_map<
        std::type_index,
        std::unique_ptr<AbstractComponentStorage>
    > _componentStorages;
};