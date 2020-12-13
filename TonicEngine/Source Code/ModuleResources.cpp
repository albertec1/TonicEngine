#include "ModuleResources.h"

ModuleResources::ModuleResources(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleResources::~ModuleResources()
{
	//init importer?
}

bool ModuleResources::Init()
{
	return false;
}

bool ModuleResources::Start()
{
	return false;
}

update_status ModuleResources::Update()
{
	return update_status();
}

bool ModuleResources::CleanUp()
{
	return false;
}

Resource* ModuleResources::CreateNewResource(const char* assetsFile, RESOURCES_TYPE type)
{
	return nullptr;
} 
//void M_Resources::SaveChangedResources()
//{
//	for (std::map<uint64, Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
//	{
//		if (it->second->needs_save == true)
//		{
//			SaveResource(it->second);
//			it->second->needs_save = false;
//		}
//	}
//}
//
//uint M_Resources::DeleteResource(uint64 ID, bool deleteAsset)
//{
//	//TODO: update folder resource and remove this one from the contained list
//	uint instances = UnloadResource(ID);
//
//	//Remove the resource from the database.
//	std::map<uint64, ResourceBase>::iterator libraryIt = resourceLibrary.find(ID);
//	if (libraryIt != resourceLibrary.end())
//	{
//		if (deleteAsset)
//		{
//			Engine->fileSystem->Remove(libraryIt->second.assetsFile.c_str());
//			Engine->fileSystem->Remove(libraryIt->second.assetsFile.append(".meta").c_str());
//		}
//		Engine->fileSystem->Remove(libraryIt->second.libraryFile.c_str());
//		resourceLibrary.erase(libraryIt);
//	}
//
//	return instances;
//}
//
//void M_Resources::SaveResource(Resource* resource, bool saveMeta)
//{
//	char* buffer = nullptr;
//	uint size = 0;
//
//	switch (resource->GetType())
//	{
//	case(ResourceType::FOLDER): { size = Importer::Folders::Save((R_Folder*)resource, &buffer); break; }
//	case(ResourceType::MESH): { size = Importer::Meshes::Save((R_Mesh*)resource, &buffer); break; }
//	case(ResourceType::TEXTURE): { size = Importer::Textures::Save((R_Texture*)resource, &buffer); break; }
//	case(ResourceType::MATERIAL): { size = Importer::Materials::Save((R_Material*)resource, &buffer); break; }
//	case(ResourceType::ANIMATION): { size = Importer::Animations::Save((R_Animation*)resource, &buffer); break;	}
//	case(ResourceType::ANIMATOR_CONTROLLER): { size = Importer::Animators::Save((R_AnimatorController*)resource, &buffer);	break; }
//	case(ResourceType::MODEL): { size = Importer::Models::Save((R_Model*)resource, &buffer); break; }
//	case(ResourceType::PARTICLESYSTEM): { size = Importer::Particles::Save((R_ParticleSystem*)resource, &buffer); break; }
//	case(ResourceType::SHADER): { size = Importer::Shaders::Save((R_Shader*)resource, &buffer); break; }
//	case(ResourceType::SCENE): { size = Importer::Scenes::Save((R_Scene*)resource, &buffer); break; }
//	}
//
//	if (size > 0)
//	{
//		Engine->fileSystem->Save(resource->GetLibraryFile(), buffer, size);
//
//		if (!resource->isExternal)
//			Engine->fileSystem->Save(resource->GetAssetsFile(), buffer, size);
//		if (saveMeta) //Model internal resources should not override meta content
//			SaveMetaInfo(*resource->baseData);
//
//		RELEASE_ARRAY(buffer);
//	}
//}
//
//uint64 M_Resources::SaveResourceAs(Resource* resource, const char* directory, const char* fileName)
//{
//	//TODO:   SaveResourceAs would override any existing resource with that name, and not remove its library content.
//
//	//Creating a new empty resource just to add it into the database
//	std::string path = directory + std::string("/").append(name); //TODO: Append extension...?
//	Resource* newResource = CreateNewResource(path.c_str(), resource->GetType(), fileName);
//
//	//We save the content of the old resource with the baseData of the new resource and restore it later
//	ResourceBase& oldBase = resourceLibrary[resource->GetID()];
//	resource->baseData = newResource->baseData;
//
//	SaveResource(resource);
//	//TODO: Save meta data
//
//	//Load the newly saved data into the new resource
//	uint64 newID = newResource->GetID();
//	UnloadResource(newID);
//
//	//Restore the old resource data
//	resource->baseData = &oldBase;
//
//	return newID;
//}
//
//uint M_Resources::UnloadResource(uint64 ID)
//{
//	uint64 instances = 0;
//
//	std::map<uint64, Resource*>::iterator it = resources.find(ID);
//	if (it != resources.end())
//	{
//		//TODO: at this moment no resource is freeing any memory
//		it->second->FreeMemory();
//		instances = it->second->instances;
//		RELEASE(it->second);
//		resources.erase(it); //TODO: Can this break after releasing "second"?
//	}
//	return instances;
//}
//
//ResourceType M_Resources::GetTypeFromFileExtension(const char* path) const
//{
//	std::string ext = "";
//	Engine->fileSystem->SplitFilePath(path, nullptr, nullptr, &ext);
//
//	static_assert(static_cast<int>(ResourceType::UNKNOWN) == 10, "Code Needs Update");
//
//	if (ext == "FBX" || ext == "fbx")
//		return ResourceType::MODEL;
//	if (ext == "tga" || ext == "png" || ext == "jpg" || ext == "TGA" || ext == "PNG" || ext == "JPG")
//		return ResourceType::TEXTURE;
//	if (ext == "shader")
//		return ResourceType::SHADER;
//	if (ext == "particles")
//		return ResourceType::PARTICLESYSTEM;
//	if (ext == "shader")
//		return ResourceType::SHADER;
//	if (ext == "scene")
//		return ResourceType::SCENE;
//	if (ext == "anim")
//		return ResourceType::ANIMATION;
//	if (ext == "animator")
//		return ResourceType::ANIMATOR_CONTROLLER;
//
//	return Engine->fileSystem->IsDirectory(path) ? ResourceType::FOLDER : ResourceType::UNKNOWN;
//}