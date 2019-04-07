#include "IComponent.h"
#include <vector>
#include <algorithm>

#include "pch.h"

using namespace std;

string tag;
Transform* transform;
GameObject* parent;
vector<GameObject*> children;
vector<IComponent*> components;

GameObject::GameObject()
{

}

void GameObject::Start()
{
	for_each(components.begin(), components.end(), [](IComponent* component)
	{
		component->Start();
	});

	for_each(children.begin(), children.end(), [](GameObject* child)
	{
		child->Start();
	});
}

void GameObject::Update()
{
	for_each(components.begin(), components.end(), [](IComponent* component)
	{
		component->Update();
	});

	for_each(children.begin(), children.end(), [](GameObject* child)
	{
		child->Update();
	});
}