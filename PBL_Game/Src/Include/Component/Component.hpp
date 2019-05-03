#pragma once
#include <string>
class GameObject;

namespace ComponentSystem
{
enum ComponentType
{

	Error,
	Renderer,
	Collider,
	ShapeRenderer3D,
	AnimatedModel,
	ModelMesh

};

class Component
{

  public:
	Component(GameObject *obj) : gameobject(nullptr) {}

	void SetGameObject(GameObject *gamobj)
	{
		gameobject = gamobj;
	}
	std::string Serialize() {
		return std::to_string(this->GetComponentType());
	}
	virtual ComponentType GetComponentType() = 0;
	GameObject *gameobject;
};

} // namespace ComponentSystem