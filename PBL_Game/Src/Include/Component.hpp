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

	public:

	void SetGameObject(GameObject * gamobj)
	{
		gameobject = gamobj;
	}

  virtual ComponentType GetComponentType() =0;


	GameObject* gameobject;
};

}