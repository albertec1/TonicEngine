#ifndef __Application__
#define __Application__

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"


#include <vector>
#include <list>
#include <string>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleSceneIntro;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleGUI;

class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModuleGUI* gui = nullptr;

public: 

	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* link) const;

	const char* GetAppName() const;
	const char* GetOrgName() const;
	void ApplyAppName(const char* name);
	void ApplyOrgName(const char* name);
	string appName;
	string orgName;

	// Frame Rate
	int getFrameRateCap();
	void setFrameRateCap(int cap);

	/*uint					frame_count = 0;
	Timer					startup_time;
	Timer					frame_time;
	Timer					last_sec_frame_time;
	uint					last_sec_frame_count = 0;
	uint					prev_last_sec_frame_count = 0;
	uint					framerate_cap = 0;
	uint					maxFPS = 60;*/

	int framerateCap = 60;
	Timer	ms_timer;
	float	dt;

	float GetDT() const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;

#endif __Application__