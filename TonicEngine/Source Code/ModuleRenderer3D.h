#ifndef __ModuleRenderer3D__
#define __ModuleRenderer3D__

#include "Application.h"
#include "Light.h"
#include "GameObject.h"

#define MAX_LIGHTS 8

//struct MeshObj;

template <typename Box>
struct RenderBox
{
	RenderBox(const Box* box, const Color& color) : box(box), color(color)
	{}

	const Box* box;
	Color color;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void VertexBuffer(float3* vertex, uint& size, uint& id_vertex);
	void IndexBuffer(uint* index, uint& size, uint& id_index);
	void TextureBuffer(float* text_coords, uint& num_text_coords, uint& id_text_coords);
	void DeleteBuffer(uint& type);


	void CreateAABB(const AABB& box, const Color& color);
	void CreateOBB(const OBB& box, const Color& color);
	void GenerateObject(GameObject* GO);

	// View Mode
	void WireframeView(bool active);
	void DepthView(bool active);
	void CullFaceView(bool active);
	void LightingView(bool active);
	void AlphaView(bool active);
	void Texture2DView(bool active);
	void DrawBB();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	std::vector<RenderBox<AABB>> aabb;
	std::vector<RenderBox<OBB>> obb;

	Color bg_color = { 0.f, 0.f, 0.f };

};

#endif 