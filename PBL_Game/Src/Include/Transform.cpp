#include "Transform.hpp"
#include <iostream>
#include <vector>

Transform::Transform()
{
  transform = glm::mat4(1.0f);
  Position = glm::vec3(0.0f);
  Scale = glm::vec3(1.0f);
  Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  Quaterion = glm::quat(Rotation);
}

Transform Transform::origin()
{
  return Transform();
}

Transform Transform::combine(Transform &other)
{
  Transform t;
  t.transform = other.transform * transform;
  return t;
}

glm::mat4 & Transform::GetTransform()
{
  return transform;
}

void Transform::ScaleTransform(GLfloat x, GLfloat y, GLfloat z)
{
  Scale *= glm::vec3(x, y, z);
  transform = glm::scale(transform, glm::vec3(x, y, z));
}

void Transform::Translate(glm::vec3 Value)
{

  Position += Value;
  transform = glm::translate(transform, Value);
}

void Transform::Rotate(float value, glm::vec3 axis)
{
  Rotation += value;

  Rotation.x = fmod(Rotation.x, 360.0f);
  Rotation.y = fmod(Rotation.y, 360.0f);
  Rotation.z = fmod(Rotation.z, 360.0f);

  transform = glm::rotate(transform, glm::radians(value), axis);
}

std::string Transform::Serialize()
{
	return glm::to_string(transform) + ";" + glm::to_string(Position) + ";" + glm::to_string(Scale) 
		+ ";" + glm::to_string(Rotation) + ";" + glm::to_string(Rotation_matrix) + ";" + glm::to_string(Quaterion);
}

void Transform::Deserialize(std::string data)
{
	std::vector<float> floats;
	data = data.substr(8);
	for (int i = 1; i < 17; i++)
	{
		int length = data.find(',');
		if (i % 4 > 0 || i == 0) {
			float i_dec = atof(data.substr(0, length).c_str());
			floats.push_back(i_dec);
			data = data.substr(length + 2);
		}
		else if (i % 4 == 0)
		{
			float i_dec = 0;
			if (i == 16) {
				i_dec = atof(data.substr(0, data.find(')')).c_str());
				floats.push_back(i_dec);
				data = data = data.substr(16);
			} else {
				i_dec = atof(data.substr(0, length - 1).c_str());
				floats.push_back(i_dec);
				data = data.substr(data.find(')') + 4);
			}
		}
	}

	int index1 = 0;
	int index2 = 0;
	for (float value : floats)
	{
		if (index2 % 4 == 0 && index2 != 0) {
			index1++;
			index2 = 0;
		}
		transform[index1][index2] = value;
	}
	floats.clear();
	for (int i = 1; i < 4; i++)
	{
		int length = data.find(',');
		if (i < 3) {
			float i_dec = atof(data.substr(0, length).c_str());
			floats.push_back(i_dec);
			data = data.substr(length + 2);
		}
		else if (i == 3) {
			int index = data.find(')');
			float i_dec = atof(data.substr(0, index).c_str());
			floats.push_back(i_dec);
			data = data.substr(index);
		}
	}
	
	for (int i = 0; i < 3; i++)
	{
		Position[i] = floats[i];
	}
	data = data.substr(7);
	floats.clear();
	for (int i = 1; i < 4; i++)
	{
		int length = data.find(',');
		if (i < 3) {
			float i_dec = atof(data.substr(0, length).c_str());
			floats.push_back(i_dec);
			data = data.substr(length + 2);
		}
		else if (i == 3) {
			int index = data.find(')');
			float i_dec = atof(data.substr(0, index).c_str());
			floats.push_back(i_dec);
			data = data.substr(index);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		Scale[i] = floats[i];
	}
	data = data.substr(7);
	floats.clear();
	for (int i = 1; i < 4; i++)
	{
		int length = data.find(',');
		if (i < 3) {
			float i_dec = atof(data.substr(0, length).c_str());
			floats.push_back(i_dec);
			data = data.substr(length + 2);
		}
		else if (i == 3) {
			int index = data.find(')');
			float i_dec = atof(data.substr(0, index).c_str());
			floats.push_back(i_dec);
			data = data.substr(index);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		Rotation[i] = floats[i];
	}
	data = data.substr(10);
	floats.clear();
	for (int i = 1; i < 17; i++)
	{
		int length = data.find(',');
		if (i % 4 > 0 || i == 0) {
			float i_dec = atof(data.substr(0, length).c_str());
			floats.push_back(i_dec);
			data = data.substr(length + 2);
		}
		else if (i % 4 == 0)
		{
			float i_dec = 0;
			if (i == 16) {
				i_dec = atof(data.substr(0, data.find(')')).c_str());
				floats.push_back(i_dec);
				data = data = data.substr(16);
			}
			else {
				i_dec = atof(data.substr(0, length - 1).c_str());
				floats.push_back(i_dec);
				data = data.substr(data.find(')') + 4);
			}
		}
	}

	for (float value : floats)
	{
		if (index2 % 4 == 0 && index2 != 0) {
			index1++;
			index2 = 0;
		}
		Rotation_matrix[index1][index2] = value;
	}
	floats.clear();
	for (int i = 1; i < 5; i++)
	{
		int length = data.find(',');
		if (i % 4 > 0 || i == 0) {
			float i_dec = atof(data.substr(0, length).c_str());
			floats.push_back(i_dec);
			data = data.substr(length + 2);
		}
		else if (i % 4 == 0)
		{
			float i_dec = atof(data.substr(0, length - 1).c_str());
			floats.push_back(i_dec);
		}
	}
	index1 = 0;
	for (float value : floats)
	{
		Quaterion[index1] = value;
		index1++;
	}
	floats.clear();
}