#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Application.h"
#include "Module.h"

class MeshImporter : public Module
{

public:
	MeshImporter(Application* app, bool start_enabled = true);
	~MeshImporter();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void GenerateMesh(const char* path, uint tex = 0);
	string GetName(const char* path);

	//Custom file format:
	//Create a function named ImportMesh that calls all the necessary methods to load the mesh.
	//It should store the header information (ammount of vertices, indices, etc...), 
	//mesh array data, and other precalculated data into a buffer of some sort.
	//ImportMesh must create a cursor and move it while storing things into the array to keep track of the position where it has to save data.
	//Once the data is stored into the buffer, ImportMesh should return it.

	//With the previous buffer full of the mesh data, make a method (Save) to create file and save it.
	
	//create the function load to read the information out of the custom file.

public:

	bool active = true;

	uint texture = 0;
	uint checker_texture = 0;

};

#endif