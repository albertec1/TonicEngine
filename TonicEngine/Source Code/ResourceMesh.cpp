#include "ResourceMesh.h"

void ResourceMesh::LoadOnMemory()
{
	App->mesh_imp->CustomLoad(this->GetLibraryFile(), &mesh_data);
	this->references++;
}

void ResourceMesh::FreeMemory()
{
	//delete all basicaly
}
