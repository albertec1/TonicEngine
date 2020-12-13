#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshImporter.h"
#include "ModuleWindow.h"
#include "ModuleSceneIntro.h"
#include "ModuleGUI.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Gizmos.h"


#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG_C("Loading 3D Renderer Context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		ret = false;
	}

	if(ret == true)
	{
		// Without this function glew doesn't initialize so the code crashes, IT MUST BE HERE
		glewInit();

		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG_C("ERROR: Unable to set VSync");

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		lights[0].Active(true);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	//Environment Color
	glClearColor(bg_color.r, bg_color.g, bg_color.b, 1.f);

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	
	App->scene_intro->Draw();
	
	std::vector<const GameObject*> objs;

	DrawBB();

	/*for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i]->oData.active)
		{
			objs[i]->Draw();
		}
	}*/
	// Drawing Panels
	App->gui->Draw();
	
	SDL_GL_SwapWindow(App->window->window);


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::VertexBuffer(float3* vertex, uint& size, uint& id_vertex)
{
	glGenBuffers(1, (GLuint*) &(id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * size, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::IndexBuffer(uint* index, uint& size, uint& id_index)
{
	glGenBuffers(1, (GLuint*) &(id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * size, index, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::TextureBuffer(float* text_coords, uint& num_text_coords, uint& id_text_coords)
{
	glGenBuffers(1, (GLuint*) &(id_text_coords));
	glBindBuffer(GL_ARRAY_BUFFER, id_text_coords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_text_coords * 2, text_coords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModuleRenderer3D::DeleteBuffer(uint& type)
{
	glDeleteBuffers(1, &(GLuint)type);
}

void ModuleRenderer3D::GenerateObject(GameObject* GO)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, GO->GetComponentMesh()->mData->mesh_data.vertex_ID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (GO->GetComponentTexture()->active)
	{
		if (GO->GetComponentTexture()->EnableHouseTexture)
			glBindTexture(GL_TEXTURE_2D, GO->GetComponentTexture()->texture);
		else if (GO->GetComponentTexture()->EnableCheckersTexture)
			glBindTexture(GL_TEXTURE_2D, App->tex_imp->checker_texture);
	}
	else
		glBindTexture(GL_TEXTURE_2D, NULL);

	glActiveTexture(GL_TEXTURE0);
	
	glBindBuffer(GL_ARRAY_BUFFER, GO->GetComponentMesh()->mData->mesh_data.tex_coords_ID);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GO->GetComponentMesh()->mData->mesh_data.index_ID);
	glDrawElements(GL_TRIANGLES, GO->GetComponentMesh()->mData->mesh_data.num_index, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}

// View Modes

void ModuleRenderer3D::WireframeView(bool active)
{
	if (active)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::DepthView(bool active)
{
	if (active)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::CullFaceView(bool active)
{
	if (active)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::LightingView(bool active)
{
	if (active)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::AlphaView(bool active)
{
	if (active)
		glEnable(GL_BLEND);

	else
		glDisable(GL_BLEND);
}

void ModuleRenderer3D::Texture2DView(bool active)
{
	if (active)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::CreateAABB(const AABB& box, const Color& color)
{
	aabb.push_back(RenderBox<AABB>(&box, color));
}

void ModuleRenderer3D::CreateOBB(const OBB& box, const Color& color)
{
	obb.push_back(RenderBox<OBB>(&box, color));
}

void ModuleRenderer3D::CreateFrustum(const Frustum& box, const Color& color)
{
	frustum.push_back(RenderBox<Frustum>(&box, color));
}

void ModuleRenderer3D::DrawBB()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);

	for (uint i = 0; i < aabb.size(); i++)
	{
		Gizmos::DrawWireBox(*aabb[i].box, aabb[i].color);
	}
	aabb.clear();

	for (uint i = 0; i < obb.size(); i++)
	{
		Gizmos::DrawWireBox(*obb[i].box, obb[i].color);
	}
	obb.clear();

	glEnd();
	glEnable(GL_LIGHTING);
}

