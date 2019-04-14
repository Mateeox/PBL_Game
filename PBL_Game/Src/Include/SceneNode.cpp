#include "SceneNode.hpp"
#include "ShapeRenderer3D.hpp"

SceneNode::SceneNode() : local(Transform::origin()), dirty_flag(true), gameObject(nullptr)
{
}

SceneNode::SceneNode(GameObject *aGameobject) : local(Transform::origin()), dirty_flag(true), gameObject(aGameobject)
{
}

void SceneNode::AddGameObject(GameObject *aGameObject)
{
  aGameObject->transform = world;
  gameObject = aGameObject;
}
void SceneNode::AddChild(SceneNode *aSceneNode)
{
  children.push_back(aSceneNode);
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
  }
  for (SceneNode *sn : children)
  {
    sn->Render(world, aDirty_Flag);
  }
}

void SceneNode::Scale(float x, float y, float z)
{
  local.ScaleTransform(x, y, z);
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