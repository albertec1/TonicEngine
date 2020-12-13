#pragma once

#include "Globals.h"
#include "Module.h"

#define CHECKERS_WIDTH 128
#define CHECKERS_HEIGHT 128

struct Texture
{
	uint id, height, width;
	std::string path;
};

class TextureImporter : public Module
{
public:
	TextureImporter(Application* app, bool start_enabled = true);
	~TextureImporter();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:

	uint CreateTexture(const void* texture, const char* path, uint width, uint height, int format, uint format2);
	uint CreateEmptyTexture() const;

	uint GenerateTexture(const char* path);
	void GenerateCheckersTexture();

	//Saves with dds format an already loaded texure, then calls the fileSystem to store it in disk as a new file.
	void CustomSave(const char* path, char** buffer);

	void CustomLoad(const char* path, Texture* tex);

	uint texture = 0;
	uint checker_texture = 0;

};