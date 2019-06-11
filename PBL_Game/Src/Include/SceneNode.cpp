#include "SceneNode.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "Component/Model.hpp"
#include "Component/AnimatedModel.hpp"
#include "PathFinding/MapTile.hpp"
#include "Component/ConeRenderer.hpp"

SceneNode::SceneNode() : local(Transform::origin()), dirty_flag(true), gameObject(nullptr)
{
}

SceneNode::SceneNode(GameObject *aGameobject) : local(Transform::origin()), dirty_flag(true), gameObject(aGameobject)
{
}

void SceneNode::AddGameObject(GameObject *aGameObject)
{
  aGameObject->node = this;
  aGameObject->transform = world;
  gameObject = aGameObject;
}

void SceneNode::RemoveChildNoRecursive(SceneNode *aSceneNode)
{
  children.erase(std::remove(children.begin(), children.end(), aSceneNode), children.end());
}

void SceneNode::AddParent(SceneNode *aSceneNode)
{
  parent = aSceneNode;
}
void SceneNode::AddChild(SceneNode *aSceneNode)
{
  children.push_back(aSceneNode);
}
void SceneNode::RemoveGameObject()
{
  delete gameObject;
  gameObject = nullptr;
}

void SceneNode::Render(Transform &parentWorld, bool aDirty_Flag)
{

  aDirty_Flag |= dirty_flag;

  if (aDirty_Flag)
  {
    world = local.combine(parentWorld);
    dirty_flag = false;
  }
  if (gameObject != nullptr)
  {

    ShapeRenderer3D *shape = (ShapeRenderer3D *)gameObject->GetComponent(ComponentSystem::ShapeRenderer3D);
    if (shape != nullptr)
    {
      shape->Draw(world.GetTransform());
    }

    MapTile *mapTile = (MapTile *)gameObject->GetComponent(ComponentSystem::MapTile);
    if (mapTile != nullptr)
    {
      mapTile->Draw(world.GetTransform());
    }

    Model *model = (Model *)gameObject->GetComponent(ComponentSystem::Model);
    if (model != nullptr)
    {
      model->Draw(world.GetTransform());
    }

    AnimatedModel *animModel = (AnimatedModel *)gameObject->GetComponent(ComponentSystem::AnimatedModel);
    if (animModel != nullptr)
    {
      animModel->Draw(world.GetTransform());
    }

	ConeRenderer *coneRenderer = (ConeRenderer *)gameObject->GetComponent(ComponentSystem::ConeRenderer);
	if (coneRenderer != nullptr)
	{
		coneRenderer->Draw(world.GetTransform());
	}

  }
  for (SceneNode *sn : children)
  {
    sn->Render(world, aDirty_Flag);
  }
}

void SceneNode::DynamicTranslate(float interpolation, float x, float y, float z)
{
  x = interpolation * x;
  y = interpolation * y;
  z = interpolation * z;

  Translate(x, y, z);
}

void SceneNode::Scale(float x, float y, float z)
{
  local.ScaleTransform(x, y, z);
  dirty_flag = true;
}

void SceneNode::Scale(float value)
{
  local.ScaleTransform(value, value, value);
  dirty_flag = true;
}

void SceneNode::Translate(float x, float y, float z)
{
  local.Translate(glm::vec3(x, y, z));
  dirty_flag = true;
}
void SceneNode::Rotate(float value, glm::vec3 axis)
{
  local.Rotate(value, axis);
  dirty_flag = true;
}

std::string SceneNode::Serialize()
{
  std::string str = "SN\n\t";
  str += "W;" + this->world.Serialize() + "\n\t";
  str += "L;" + this->local.Serialize() + "\n\t";
  if (this->parent)
    str += "P;" + std::to_string((intptr_t)this->parent) + "\n\t";
  for (SceneNode *child : this->children)
    str += "CH;" + std::to_string((intptr_t)child) + "\n\t";
  str += "O\n\t\t" + this->gameObject->Serialize() + "\n";
  return str;
}