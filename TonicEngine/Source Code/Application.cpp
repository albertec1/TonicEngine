#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleGui.h"
#include "PanelManager.h"
#include "MeshImporter.h"
#include "ModuleFileSystem.h"
#include "TextureImporter.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	gui = new ModuleGUI(this);
	mesh_imp = new MeshImporter(this);
	file_system = new ModuleFileSystem(this);
	tex_imp = new TextureImporter(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);	
	AddModule(mesh_imp);
	AddModule(tex_imp);
	AddModule(file_system);

	// Scenes
	AddModule(scene_intro);
	AddModule(gui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	list<Module*>::iterator item = list_modules.begin();

	for (; item != list_modules.end(); item = next(item))
	{
		RELEASE(*item);
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;
	LOG_C("Init Application");

	// Needed to initialize PCG (Random Number Generator Library)
	InitSeed();

	// Call Init() in all modules
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}
	
	frame_time.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = (float)frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	uint last_frame_ms = frame_time.Read();
	uint frames_on_last_update = prev_last_sec_frame_count;

	fpsVec.push_back(frames_on_last_update);
	if (fpsVec.size() > totalBars)
		fpsVec.erase(fpsVec.begin());

	msVec.push_back(last_frame_ms);
	if (msVec.size() > totalBars)
		msVec.erase(msVec.begin());

	if (framerateCap > 0)
		capped_ms = 1000 / framerateCap;
	else
		capped_ms = 0;

	if (capped_ms > 0 && last_frame_ms < capped_ms)
		SDL_Delay(capped_ms - last_frame_ms);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item) 
	{
		ret = (*item)->PreUpdate(dt);
	}

	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->Update(dt);
	}
	
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; ++item)
	{
		ret = (*item)->PostUpdate(dt);
		
	}
	
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend() && ret; ++item)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

void Application::RequestBrowser(const char* link) const
{
	LOG_C("WARNING: Browser Opened")
	ShellExecuteA(NULL, "open", link, NULL, NULL, SW_SHOWNORMAL);
}

const char* Application::GetAppName() const
{
	return appName.data();
}

const char* Application::GetOrgName() const
{
	return orgName.data();
}

void Application::ApplyAppName(const char* name)
{
	appName.assign(name);
	App->window->SetTitle(appName.data());
}

void Application::ApplyOrgName(const char* name)
{
	orgName = name;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

float Application::GetDT() const
{
	return dt;
}


