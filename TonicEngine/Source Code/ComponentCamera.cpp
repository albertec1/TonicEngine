#include "ComponentCamera.h"
#include "ModuleInput.h"
#include "ModuleGUI.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"

#include "MathGeoLib/include/Math/MathFunc.h"

ComponentCamera::ComponentCamera(GameObject* GO) : Component(COMPONENT_TYPE::CAMERA, GO)
{
	type = COMPONENT_TYPE::CAMERA;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = DegToRad(60.0f);

	SetAspectRatio(1.3f);
	update_frustum = true;
}

ComponentCamera::~ComponentCamera(){}

float ComponentCamera::GetNearPlane() const
{
	return frustum.nearPlaneDistance;
}

float ComponentCamera::GetFarPlane() const
{
	return frustum.farPlaneDistance;
}

float ComponentCamera::GetFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::SetNearPlane(float distance)
{
	if (distance > 0 && distance < frustum.farPlaneDistance)
	{
		frustum.nearPlaneDistance = distance;
	}
		
	update_frustum = true;
}

void ComponentCamera::SetFarPlane(float distance)
{
	if (distance > 0 && distance > frustum.nearPlaneDistance)
	{
		frustum.farPlaneDistance = distance;
	}
		
	update_frustum = true;
}

void ComponentCamera::SetFOV(float fov)
{
	fov *= DEGTORAD;
	frustum.verticalFov = fov;
	update_frustum = true;
}

void ComponentCamera::SetAspectRatio(float ar)
{
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * ar);
	update_frustum = true;
}

void ComponentCamera::Look(const float3& position)
{
	float3 vector = position - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, vector.Normalized(), frustum.up, float3::unitY);

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
	update_frustum = true;
}

void ComponentCamera::OnUpdateTransform(const float4x4& global, const float4x4& parent_global)
{
	frustum.front = global.WorldZ();
	frustum.up = global.WorldY();

	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat quat = Quat::identity;
	global.Decompose(position, quat, scale);

	frustum.pos = position;
	
	update_frustum = true;
}