#include "ResourceMesh.h"

ResourceMesh::ResourceMesh() : Resource(RESOURCES_TYPE::MESH)
{

}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::LoadOnMemory()
{
	App->mesh_imp->CustomLoad(this->GetLibraryFile(), &mesh_data);
	this->references++;
}

void ResourceMesh::FreeMemory()
{
	//delete all basicaly
}
