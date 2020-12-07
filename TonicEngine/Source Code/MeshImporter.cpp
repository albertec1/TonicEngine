#include "MeshImporter.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleGUI.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"


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