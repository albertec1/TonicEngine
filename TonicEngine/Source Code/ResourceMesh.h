#pragma once

#include "Resources.h"
#include "MeshImporter.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ResourceMesh : public Resource
{
public:
	ResourceMesh();
	~ResourceMesh();

	void LoadOnMemory();
	void FreeMemory();

	Mesh mesh_data;

};