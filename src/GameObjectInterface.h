#pragma once
#include "Matrix.h"
#include "LifecycleObject.h"
#include "Aabb.h"
#include <list>

class MeshRenderer;
class GameObject;
class Component;

class GameObjectInterface: public LifecycleObject
{
	public:
		virtual const Matrix& getTransform() const = 0;
		virtual void moveTo(const Matrix& m) = 0;
		virtual void translate(const Vector& delta) = 0;
		virtual void rotate(const Matrix& rot) = 0;
		virtual const MeshRenderer* getRenderer() = 0;
		virtual const AABB* getAreaBox() = 0;
		virtual const GameObject* getParent() const = 0 ;
		virtual void setParent(GameObject* p) = 0;
		virtual const std::list<Component*>& getComponents() const = 0;
		virtual void setActive(const bool active) = 0;

		virtual void addComponent(Component* comp) = 0;

		virtual const std::list<GameObject*>& getChildren() const = 0;
		virtual void addChild(GameObject* g) = 0;

		template <typename T>
		T* getComponentByType() = 0;

		template <typename T>
		std::list<T*> getComponentsByType() = 0;
};

