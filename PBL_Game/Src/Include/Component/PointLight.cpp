#include "PointLight.hpp"
#include "GameObject.hpp"
#include "string"

PointLight::PointLight(vec3 aAmbient, vec3 aDiffuse, vec3 aSpecular,
                       float aConstant, float aLinear, float aQuadratic, Shader &aShader, unsigned aLightNumber)
    : Component(nullptr), mAmbient(aAmbient), mDiffuse(aDiffuse),
      mSpecular(aSpecular), mConstant(aConstant), mLinear(aLinear),
      mQuadratic(aQuadratic), mShaderProgram(aShader), mLightNumber(aLightNumber)
{
    mLightNumberAsString = std::to_string(aLightNumber);
}

void PointLight::SetLightNumber(unsigned aLightNumber)
{
    mLightNumber = aLightNumber;
    mLightNumberAsString = std::to_string(aLightNumber);
}

void PointLight::SendValuesToShader()
{

    //CAN BE OPTIMIZED
    mShaderProgram.setVec3("pointLights[" + mLightNumberAsString + "].position", gameobject->transform.getPosition);
    mShaderProgram.setVec3("pointLights[" + mLightNumberAsString + "].ambient", mAmbient.x, mAmbient.y, mAmbient.z);
    mShaderProgram.setVec3("pointLights[" + mLightNumberAsString + "].diffuse", mDiffuse.x, mDiffuse.y, mDiffuse.z);
    mShaderProgram.setVec3("pointLights[" + mLightNumberAsString + "].specular", mSpecular.x, mSpecular.y, mSpecular.z);
    mShaderProgram.setFloat("pointLights[" + mLightNumberAsString + "].constant", mConstant);
    mShaderProgram.setFloat("pointLights[" + mLightNumberAsString + "].linear", mLinear);
    mShaderProgram.setFloat("pointLights[" + mLightNumberAsString + "].quadratic", mQuadratic);
}

ComponentSystem::ComponentType PointLight::GetComponentType()
{
    return ComponentSystem::PointLight;
}