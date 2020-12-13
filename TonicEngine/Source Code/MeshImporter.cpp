#include "MeshImporter.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"
#include "ModuleFileSystem.h"


#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment ( lib, "Assimp/libx86/assimp.lib" )

MeshImporter::MeshImporter(Application* app, bool start_enabled) : Module(app, start_enabled) 
{ 
	active = true; 
}

MeshImporter::~MeshImporter() 
{
}

bool MeshImporter::Init()
{
	struct aiLogStream stream;

	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);

	aiAttachLogStream(&stream);

	if (aiGetErrorString() != NULL)
	{
		LOG_C("Initializing ASSIMP");
	}
	else
	{
		LOG_C("ERROR: Could not initialize ASSIMP");
		return false;
	}

	return true;
}

update_status MeshImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool MeshImporter::CleanUp()
{
	aiDetachAllLogStreams();

	return true;
}

void MeshImporter::GenerateMesh(const char* Filename, uint tex)
{
	const aiScene* scene = aiImportFile(Filename, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes()) // Loaded correctly
	{
		// mNumMeshes iterates on mMeshes[]
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			GameObject* meshGO = App->scene_intro->CreateGO(GetName(Filename));

			aiMesh* mesh2 = scene->mMeshes[i];

			// Gets path
			meshGO->GetComponentMesh()->mData.path = Filename;

			// Vertices
			meshGO->GetComponentMesh()->mData.num_vertex = mesh2->mNumVertices;
			meshGO->GetComponentMesh()->mData.vertex = new float3[meshGO->GetComponentMesh()->mData.num_vertex];

			for (uint i = 0; i < mesh2->mNumVertices; ++i)
			{
				meshGO->GetComponentMesh()->mData.vertex[i].x = mesh2->mVertices[i].x;
				meshGO->GetComponentMesh()->mData.vertex[i].y = mesh2->mVertices[i].y;
				meshGO->GetComponentMesh()->mData.vertex[i].z = mesh2->mVertices[i].z;
			}

			// Faces
			if (mesh2->HasFaces())
			{
				meshGO->GetComponentMesh()->mData.num_index = mesh2->mNumFaces * 3;
				meshGO->GetComponentMesh()->mData.index = new uint[meshGO->GetComponentMesh()->mData.num_index];

				for (uint i = 0; i < mesh2->mNumFaces; ++i)
				{
					if (mesh2->mFaces[i].mNumIndices != 3)
					{
						LOG_C("ERROR: Geometry face with != 3 indices");
					}
					else
						memcpy(&meshGO->GetComponentMesh()->mData.index[i * 3], mesh2->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			// Normals
			if (mesh2->HasNormals())
			{
				meshGO->GetComponentMesh()->mData.normals = new float3[meshGO->GetComponentMesh()->mData.num_vertex];
				memcpy(meshGO->GetComponentMesh()->mData.normals, mesh2->mNormals, sizeof(float3) * meshGO->GetComponentMesh()->mData.num_vertex);
			}

			// UVs
			if (mesh2->HasTextureCoords(0))
			{
				meshGO->GetComponentMesh()->mData.num_tex_coords = meshGO->GetComponentMesh()->mData.num_vertex;
				meshGO->GetComponentMesh()->mData.tex_coords = new float[meshGO->GetComponentMesh()->mData.num_tex_coords * 2];

				for (int i = 0; i < meshGO->GetComponentMesh()->mData.num_tex_coords; ++i)
				{
					meshGO->GetComponentMesh()->mData.tex_coords[i * 2] = mesh2->mTextureCoords[0][i].x;
					meshGO->GetComponentMesh()->mData.tex_coords[(i * 2) + 1] = mesh2->mTextureCoords[0][i].y;
				}
			}

			
			meshGO->aabb.SetNegativeInfinity();
			meshGO->aabb.Enclose((float3*)meshGO->GetComponentMesh()->mData.vertex, meshGO->GetComponentMesh()->mData.num_vertex);
			

			// Import data to buffers
			App->renderer3D->VertexBuffer(meshGO->GetComponentMesh()->mData.vertex, meshGO->GetComponentMesh()->mData.num_vertex, meshGO->GetComponentMesh()->mData.id_vertex);
			App->renderer3D->IndexBuffer(meshGO->GetComponentMesh()->mData.index, meshGO->GetComponentMesh()->mData.num_index, meshGO->GetComponentMesh()->mData.id_index);
			App->renderer3D->TextureBuffer(meshGO->GetComponentMesh()->mData.tex_coords, meshGO->GetComponentMesh()->mData.num_tex_coords, meshGO->GetComponentMesh()->mData.id_tex_coords);

			if (tex == 0 && Filename == "Assets/BakerHouse.fbx")
				meshGO->GetComponentTexture()->texture = texture;

			else
				meshGO->GetComponentTexture()->texture = App->scene_intro->LoadNewTexture(tex);
		}

		aiReleaseImport(scene);
		LOG_C("Succesfully loaded mesh with path: %s", Filename);
	}

	else
		LOG_C("ERROR: Cannot load scene %s", Filename);
}

bool MeshImporter::CustomSave(const char* name, Mesh* mesh_values, const char* buffer)
{
	bool ret = false;
	uint ranges[4] = { mesh_values->num_index, mesh_values->num_vertex, mesh_values->num_normals, mesh_values->num_tex_coords };
	uint size = sizeof(ranges) + sizeof(uint) * mesh_values->num_index + sizeof(float) * mesh_values->num_vertex * 3 + sizeof(float) * mesh_values->num_normals * 3 + sizeof(float) * mesh_values->num_tex_coords * 2;

	char* fileBuffer = new char[size]; // Allocate
	char* cursor = fileBuffer;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	cursor += bytes; 

	// Store indices
	bytes = sizeof(uint) * mesh_values->num_index;
	memcpy(cursor, mesh_values->index, bytes);
	cursor += bytes;
	
	//Store vertices
	bytes = sizeof(uint) * mesh_values->num_vertex * 3;
	memcpy(cursor, mesh_values->vertex, bytes);
	cursor += bytes;

	//Store normals
	bytes = sizeof(uint) * mesh_values->num_normals * 3;
	memcpy(cursor, mesh_values->normals, bytes);
	cursor += bytes;

	//Store texture coordinates
	bytes = sizeof(uint) * mesh_values->num_tex_coords * 2;
	memcpy(cursor, mesh_values->tex_coords, bytes);
	cursor += bytes;

	ret = App->file_system->CustomFileSave(buffer, fileBuffer, size, MESHES_CUSTOM_FOLDER, name, "mymesh");

	if (!ret)
	{
		LOG_C("Failed exporting %s.mymesh into Library/Meshes floder", name);
	}
	else
	{
		LOG_C("Success: file %s.mymesh saved into %s", name, MESHES_CUSTOM_FOLDER);
	}

	delete[] fileBuffer;
	fileBuffer = nullptr;
	cursor = nullptr;
	return ret;
}

bool MeshImporter::CustomLoad(const char* own_buffer, Mesh* mesh_values)
{
	bool ret = false;
	const char* cursor = own_buffer;


	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	mesh_values->num_index = ranges[0];
	mesh_values->num_vertex = ranges[1];
	mesh_values->num_normals = ranges[2];
	mesh_values->num_tex_coords = ranges[3];

	cursor += bytes;
	bytes = sizeof(uint) * mesh_values->num_index;
	mesh_values->index = new uint[mesh_values->num_index];
	memcpy(mesh_values->index, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh_values->num_vertex * 3;
	mesh_values->vertex = new float3[mesh_values->num_vertex];
	memcpy(mesh_values->vertex, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh_values->num_normals * 3;
	mesh_values->normals = new float3[mesh_values->num_normals];
	memcpy(mesh_values->normals, cursor, bytes);

	cursor += bytes;
	bytes = sizeof(float) * mesh_values->num_tex_coords * 2;
	mesh_values->tex_coords = new float[mesh_values->num_tex_coords * 2];
	memcpy(mesh_values->tex_coords, cursor, bytes);

	// Generate buffers with all mesh info
	App->renderer3D->VertexBuffer(mesh_values->vertex, mesh_values->num_vertex, mesh_values->vertex_ID);
	App->renderer3D->IndexBuffer(mesh_values->index, mesh_values->num_index, mesh_values->index_ID);
	App->renderer3D->TextureBuffer(mesh_values->tex_coords, mesh_values->num_tex_coords, mesh_values->tex_coords_ID);

	RELEASE_ARRAY(own_buffer);
	cursor = nullptr;

	ret = true;
	return ret;
}

string MeshImporter::GetName(const char* path)
{
	// Finding the last slash of the path
	std::string originalString = path;
	std::size_t findSlash = originalString.find_last_of("/\\");

	// Erasing everything previous to the slash
	std::string stringWithPoint = originalString.substr(findSlash + 1);

	// Finding the point
	std::size_t findPoint = stringWithPoint.find_last_of(".");

	// Returns the name of the file
	return stringWithPoint.substr(0, findPoint);
}

