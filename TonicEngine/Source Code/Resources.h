#pragma once

#include "Globals.h"
#include "Application.h"

enum class RESOURCES_TYPE
{
	UNKNOWN = -1,
	TEXTURE,
	MESH, 
	SCENE
};

class Resource
{
public: 
	Resource(RESOURCES_TYPE type);
	virtual ~Resource();

	inline uint GetUUID() const
	{
		return UUID;
	}
	inline const char* GetAssetsFile() const
	{
		return assets_file.c_str();
	}
	inline const char* GetLibraryFile() const
	{
		return library_file.c_str();
	}
	inline uint GetReferenceCount() const
	{
		return references;
	}
	RESOURCES_TYPE GetType() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	

public:
	uint UUID = 0;
	uint references = 0;
	RESOURCES_TYPE type = RESOURCES_TYPE::UNKNOWN;
	std::string name = " ";
	std::string assets_file = " ";
	std::string library_file = " ";
};