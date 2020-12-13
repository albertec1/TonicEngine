#ifndef __C_CAMERA_H__
#define __C_CAMERA_H__

#include "Component.h"
#include "glmath.h"
#include "MathGeoLib/include/Geometry/Frustum.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "MathGeoLib/include/Geometry/Plane.h"

class GameObject;



class ComponentCamera : public Component
{
public:
	ComponentCamera(GameObject* GO = nullptr);
	~ComponentCamera();

	//bool Start();
	//bool Update();
	//bool CleanUp();

	void Look(const float3& position);



	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetFOV() const;
	float GetAspectRatio() const;


	void SetNearPlane(float distance);
	void SetFarPlane(float distance);
	void SetFOV(float fov);
	void SetAspectRatio(float ar);

	//void DrawFrustum();

	void OnUpdateTransform(const float4x4& global, const float4x4& parent_global = float4x4::identity);

	//bool Intersect(const AABB& box) const;
	//static bool Intersect(const Frustum& frustum, const AABB& box);

public:

	Frustum frustum;
	bool showFrustrum = false;
	bool update_frustum = true;

};

#endif
