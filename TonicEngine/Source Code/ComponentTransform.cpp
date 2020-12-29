#include "ComponentTransform.h"
#include "ModuleGUI.h"
#include "ModuleSceneIntro.h"

ComponentTransform::ComponentTransform(GameObject* gameObject) : Component(COMPONENT_TYPE::TRANSFORM, gameObject)
{
	type = COMPONENT_TYPE::TRANSFORM;

	globalTransform = Quat::identity;	
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Draw()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Spacing();

		ImGui::Text("Position"); ImGui::SameLine(); 
		
		ImGui::PushItemWidth(65); ImGui::PushID("pos"); ImGui::DragFloat("X", &position.x, 0.5F); ImGui::PopID(); ImGui::SameLine(); 
		ImGui::PushItemWidth(65); ImGui::PushID("pos"); ImGui::DragFloat("Y", &position.y, 0.5F); ImGui::PopID(); ImGui::SameLine();
		ImGui::PushItemWidth(65);  ImGui::PushID("pos"); ImGui::DragFloat("Z", &position.z, 0.5F);ImGui::PopID();

		ImGui::Spacing();

		ImGui::Text("Rotation"); ImGui::SameLine();
		ImGui::PushItemWidth(65); ImGui::PushID("rot"); ImGui::DragFloat("X", &rotation.x, 0.5F); ImGui::PopID(); ImGui::SameLine();
		ImGui::PushItemWidth(65); ImGui::PushID("rot"); ImGui::DragFloat("Y", &rotation.y, 0.5F); ImGui::PopID(); ImGui::SameLine();
		ImGui::PushItemWidth(65); ImGui::PushID("rot"); ImGui::DragFloat("Z", &rotation.z, 0.5F); ImGui::PopID();

		ImGui::Spacing();

		ImGui::Text("Scale   "); ImGui::SameLine();
		ImGui::PushItemWidth(65); ImGui::PushID("scale"); ImGui::DragFloat("X", &scale.x, 0.5F); ImGui::PopID(); ImGui::SameLine();
		ImGui::PushItemWidth(65); ImGui::PushID("scale"); ImGui::DragFloat("Y", &scale.y, 0.5F); ImGui::PopID(); ImGui::SameLine();
		ImGui::PushItemWidth(65); ImGui::PushID("scale"); ImGui::DragFloat("Z", &scale.z, 0.5F); ImGui::PopID();
	}
}

void ComponentTransform::IsTransformComponentActive(GameObject* go)
{
	go = App->scene_intro->GOselected;

	if (go->GetComponentMesh()->active)
	{
		LOG_C("Transform Component is active");
	}
	else
	{
		LOG_C("ERROR: Transform Component is NOT active");
	}
}

void ComponentTransform::UpdateLocalTransform()
{
	localTransform = math::float4x4::FromTRS(position, rotation, scale);
}


float4x4 ComponentTransform::GetGlobalTransform()const
{
	return globalTransform;
}
