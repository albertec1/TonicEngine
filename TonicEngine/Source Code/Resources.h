#pragma once

#include "Globals.h"
#include "Application.h"ç

enum class RESOURCES_TYPE
{
	UNKNOWN = -1,
	TEXTURE,
	MESH
};

class Resource
{
public: 
	Resource() {}
	virtual ~Resource() {}

	inline RESOURCES_TYPE GetType() const
	{
		return type;
	}
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

	//serialization methods still missing

private:
	uint UUID = 0;
	uint references = 0;
	RESOURCES_TYPE type = RESOURCES_TYPE::UNKNOWN;
	std::string name = " ";
	std::string assets_file = " ";
	std::string library_file = " ";
};