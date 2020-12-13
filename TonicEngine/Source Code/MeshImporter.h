#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Application.h"
#include "Module.h"

struct Mesh
{
	//index
	uint index_ID = 0;
	uint num_index = 0;
	uint* index = nullptr;
	//vertex
	uint vertex_ID = 0;
	uint num_vertex = 0;
	float3* vertex = nullptr;
	//texcoords
	uint tex_coords_ID = 0;
	uint num_tex_coords = 0;
	float* tex_coords = nullptr;
	//normals
	uint normals_ID = 0;
	uint num_normals = 0;
	float3* normals = nullptr;
};

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

	//TODO2 - Custom file format:
	//Create a function named ImportMesh that calls all the necessary methods to load the mesh.
	//It should store the header information (ammount of vertices, indices, etc...), 
	//mesh array data, and other precalculated data into a buffer of some sort.
	//ImportMesh must create a cursor and move it while storing things into the array to keep track of the position where it has to save data.
	//Once the data is stored into the buffer, ImportMesh should save and return it.
	bool CustomSave(const char* name, Mesh* mesh_values , const char* buffer);
	
	//create the function load to read the information out of the custom file.
	bool CustomLoad(const char* own_buffer, Mesh* mesh_values);
public:

	bool active = true;

	uint texture = 0;
	uint checker_texture = 0;
};

#endif