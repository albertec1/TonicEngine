#pragma once

#include "Resources.h"
#include "TextureImporter.h"

class ResourceTexture : public Resource
{
public:
	ResourceTexture();
	~ResourceTexture();

	void LoadOnMemory();
	void FreeMemory();

	Texture tex;
};