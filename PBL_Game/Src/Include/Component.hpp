#pragma once

class GameObject;

namespace ComponentSystem{
enum ComponentType{

Error,
Renderer,
Collider,
ShapeRenderer3D

};

class Component {

	GameObject* gameobject;

	public:
	Component(GameObject * obj): gameobject(nullptr){}

	void SetGameObject(GameObject * gamobj)
	{
		gameobject = gamobj;
	}
	
	ComponentType GetComponentType()
	{
		return ComponentType::Error;
	}

};

}