//#pragma once
//
//#include "Module.h"
//#include "Globals.h"
//#include "Resources.h"
//#include "Timer.h"
//#include "MathGeoLib\include\Algorithm\Random\LCG.h"
//
//#include <map>
//
//class ModuleResources : public Module
//{
//public:
//	ModuleResources(Application* app, bool start_enabled = true);
//	virtual ~ModuleResources();
//		
//	bool Init() override;	//importers init
//	bool Start() override;	//load all assets
//	update_status Update(); //update assets
//	bool CleanUp() override;//save resources and free memory
//
//	//LoadAllAssets()
//	//LoadAssetBase() --> checks whether the 
//
//	uint CreateResource(const char* buffer, RESOURCES_TYPE type);
//	void DeleteResource(uint ID);
//	
//	const Resource* RequestResource(uint uid) const;
//	Resource* RequestResource(uint uid);
//	
//	uint ImportFileFromAssets(const char* path);
//
//	inline uint GetNewID() { return random.Int(); }
//
//private:
//	Resource* CreateNewResource(const char* assetsFile, RESOURCES_TYPE type);
//
//public:
//	std::map<uint, Resource> resourceLibrary;
//
//	Timer updateAssets_timer;
//	Timer saveChangedResources_timer;
//	LCG random;
//};