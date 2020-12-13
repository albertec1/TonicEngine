#pragma once

#include "Module.h"
#include "Globals.h"
#include "Resources.h"

#include <map>

class ModuleResources : public Module
{
	ModuleResources();
	~ModuleResources();

	uint CreateResource(const char* buffer, RESOURCES_TYPE type);
	void DeleteResource(uint ID);

	uint ImportFileFromAssets(const char* path);

	std::map<uint, Resource*> resources;
};