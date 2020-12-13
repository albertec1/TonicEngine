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
	void UpdateLocalTransform();

public:
	float3 position;
	Quat rotation;
	float3 scale;

private:
	float4x4 globalTransform;
	float4x4 localTransform;
};

#endif
