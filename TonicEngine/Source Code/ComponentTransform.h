#ifndef __C_TRANSFORM_H__
#define __C_TRANSFORM_H__

#include "Component.h"
#include "glmath.h"

class GameObject;

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* gameObject);
	virtual ~ComponentTransform();

	void Draw();
	float4x4 GetGlobalTransform()const;
	void IsTransformComponentActive(GameObject* go);

public:
	vec3 position;
	vec3 rotation;
	vec3 scale;

private:
	float4x4 globalTransform;
};

#endif
