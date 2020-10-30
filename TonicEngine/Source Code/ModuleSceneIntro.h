#ifndef __ModuleSceneIntro__
#define __ModuleSceneIntro__

#include "Globals.h"
#include "Primitive.h"
#include "GameObject.h"

enum class OBJECTS3D
{
	NONE = -1,
	B_SPHERE,
	B_CUBE,
	B_CYLINDER,
	B_CONE,
	B_TREE,
	LIGHTPOST, 
	CARRIAGE, 
	GERALT,
	ROCK
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void DrawGridAndAxis();
	void Create3DObject(OBJECTS3D object);

	// DRAW CUBE INFO
	uint id_vertices = 0;
	uint id_no_dup_vertices = 1;
	uint id_indices = 2;

	GameObject* CreateGO(string objName);
	string AssignNameToGO(string name_go);
	void RemoveSelectedGO(GameObject* GO);
	void RemoveAllGO();
	void NumberOfGO();
	std::vector<GameObject*> gameobjectsList;

	GameObject* GOselected = nullptr;

	uint texture;
	uint checkers_texture;
	uint texture_shapes;
	
};


#endif __ModuleSceneIntro__