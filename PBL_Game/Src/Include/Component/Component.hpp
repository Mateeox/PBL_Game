#pragma once

class GameObject;

namespace ComponentSystem
{
enum ComponentType
{

	Error,
	Renderer,
	Collider,
	ShapeRenderer3D,
	Model,
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

	virtual ComponentType GetComponentType() = 0;
	GameObject *gameobject;
};

} // namespace ComponentSystem