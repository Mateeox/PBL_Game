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

    pointLightStringMap.insert(std::pair<std::string, std::string>("position", "pointLights[" + mLightNumberAsString + "].position"));
    pointLightStringMap.insert(std::pair<std::string, std::string>("ambient", "pointLights[" + mLightNumberAsString + "].ambient"));
    pointLightStringMap.insert(std::pair<std::string, std::string>("diffuse", "pointLights[" + mLightNumberAsString + "].diffuse"));
    pointLightStringMap.insert(std::pair<std::string, std::string>("specular", "pointLights[" + mLightNumberAsString + "].specular"));
    pointLightStringMap.insert(std::pair<std::string, std::string>("constant", "pointLights[" + mLightNumberAsString + "].constant"));
    pointLightStringMap.insert(std::pair<std::string, std::string>("linear", "pointLights[" + mLightNumberAsString + "].linear"));
    pointLightStringMap.insert(std::pair<std::string, std::string>("quadratic", "pointLights[" + mLightNumberAsString + "].quadratic"));
}

void PointLight::SetLightNumber(unsigned aLightNumber)
{
    mLightNumber = aLightNumber;
    mLightNumberAsString = std::to_string(aLightNumber);
    SetLightMapStringValues(mLightNumberAsString);
}

void PointLight::SetLightMapStringValues(std::string aNumber)
{

    pointLightStringMap.at("position") = "pointLights[" + aNumber + "].position";
    pointLightStringMap.at("ambient") = "pointLights[" + aNumber + "].ambient";
    pointLightStringMap.at("diffuse") = "pointLights[" + aNumber + "].diffuse";
    pointLightStringMap.at("specular") = "pointLights[" + aNumber + "].specular";
    pointLightStringMap.at("constant") = "pointLights[" + aNumber + "].constant";
    pointLightStringMap.at("linear") = "pointLights[" + aNumber + "].linear";
    pointLightStringMap.at("quadratic") = "pointLights[" + aNumber + "].quadratic";
}

void PointLight::SendValuesToShader()
{
    mShaderProgram.setVec3(pointLightStringMap.at("position"), gameobject->transform.getPosition);
    mShaderProgram.setVec3(pointLightStringMap.at("ambient"), mAmbient.x, mAmbient.y, mAmbient.z);
    mShaderProgram.setVec3(pointLightStringMap.at("diffuse"), mDiffuse.x, mDiffuse.y, mDiffuse.z);
    mShaderProgram.setVec3(pointLightStringMap.at("specular"), mSpecular.x, mSpecular.y, mSpecular.z);
    mShaderProgram.setFloat(pointLightStringMap.at("constant"), mConstant);
    mShaderProgram.setFloat(pointLightStringMap.at("linear"), mLinear);
    mShaderProgram.setFloat(pointLightStringMap.at("quadratic"), mQuadratic);
}

ComponentSystem::ComponentType PointLight::GetComponentType()
{
    return ComponentSystem::PointLight;
}