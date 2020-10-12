#ifndef __MODULEGUI_H__
#define __MODULEGUI_H__

#include "Globals.h"
#include "Module.h"
#include "PanelManager.h"

#include "imgui-1.78/imgui.h"

#include "imgui-1.78/examples/imgui_impl_sdl.h"
#include "imgui-1.78/examples/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"

#include "PanelConfiguration.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelHierarchy.h"


class PanelConfiguration;
class PanelAbout;
class PanelConsole;
class PanelHierarchy;


class ModuleGUI : public Module
{
public:
	// CONSTRUCTOR
	ModuleGUI(Application* app, bool start_enabled = true);
	//DESTRUCTOR
	~ModuleGUI();

public:
	bool Init();
	bool Start();
	bool CleanUp();
	void Render();
	bool Draw();
	
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	
	// Random Number Generator 
	int rand = 0;
	bool getRand = false;
	
	PanelConfiguration* Pconfig = nullptr;
	PanelAbout* Pabout = nullptr;
	PanelConsole* Pconsole = nullptr;
	PanelHierarchy* Phierarchy = nullptr;

	bool quitApp = false;

	// Frame Rate
	int max_fps = 0;

private:
	ImGuiIO* io = nullptr;

	bool show_demo_window = false;
	bool show_style_editor = false;
	
	list<PanelManager*> panels;

};

#endif // !__MODULEGUI_H__

