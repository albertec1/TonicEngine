#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Application.h"
#include "Module.h"
#include "Math.h" // Include MathGeoLib


class MeshImporter : public Module
{

public:
	MeshImporter(Application* app, bool start_enabled = true);
	~MeshImporter();

	bool Init();

	update_status Update(float dt);

	bool CleanUp();

	void LoadMesh(const char* Filename);


public:

	bool enabled = true;

};

#endif