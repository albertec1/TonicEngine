#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "Component.h"
#include "glmath.h"
#include "Color.h"
#include "ResourceMesh.h"

class GameObject;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* gameObject);
	virtual ~ComponentMesh();

	bool Update();
	void Draw();
	bool DrawFaceNormals(GameObject* m, bool active = false);
	bool DrawVertexNormals(GameObject* m, bool active = false);

	void IsMeshComponentActive(GameObject* go);

	const AABB& GetAABB() const;
	const OBB& GetOBB() const;


public:

	bool showFaceNormals = false;
	bool showVertexNormals = false;

	float faceLenght = 1.0f;
	Color faceColor = Red;

	float vertexLenght = 1.0f;
	Color vertexColor = Blue;

	AABB aabb;
	OBB obb;
	
	ResourceMesh* mData = nullptr;
};

#endif
