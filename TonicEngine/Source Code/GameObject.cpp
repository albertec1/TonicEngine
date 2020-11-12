#include "GameObject.h"
#include "ModuleGUI.h"
#include "Component.h"
#include "ModuleSceneIntro.h"

GameObject::GameObject(std::string name)
{
	this->data.name = name;
	this->data.UUID = GenerateUUID();
	CreateComponent(COMPONENT_TYPE::TRANSFORM);
	CreateComponent(COMPONENT_TYPE::MESH);
	CreateComponent(COMPONENT_TYPE::TEXTURE);
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	/*for (int i = 0; i < componentsList.size(); ++i)
	{
		if (componentsList[i]->active)
			componentsList[i]->Update();
	}*/

	/*for (std::vector<GameObject*>::iterator it = childrenList.begin(); it != childrenList.end(); ++it)
	{
		(*it)->Update();
	}*/
}

void GameObject::CleanUp()
{
	// Deleting Components
	for (int i = 0; i < componentsList.size(); ++i)
	{
		if (componentsList[i] != nullptr && componentsList[i]->active)
			componentsList[i]->CleanUp();

		delete componentsList[i];
		componentsList[i] = nullptr;
	}

	componentsList.clear();

	// Deleting GOs
	if (GOparent != nullptr)
	{
		GOparent->RemoveChild(this);
	}

	for (uint i = 0; i < childrenList.size(); ++i)
	{
		if (childrenList[i] != nullptr)
		{
			childrenList[i]->CleanUp();
		}
	}

	childrenList.clear();
}

//void GameObject::Draw()
//{
//	for (int i = 0; i < componentsList.size(); ++i)
//	{
//		componentsList[i]->Draw();
//	}
//}

void GameObject::EnableGameObject()
{
	if (!this->data.active)
		this->data.active = true;

	for (std::vector<GameObject*>::iterator it = childrenList.begin(); it != childrenList.end(); ++it)
	{
		(*it)->EnableGameObject();
	}

}

void GameObject::DisableGameObject()
{
	if (this->data.active)
		this->data.active = false;

	for (std::vector<GameObject*>::iterator it = childrenList.begin(); it != childrenList.end(); ++it)
	{
		(*it)->DisableGameObject();
	}

}

Component* GameObject::CreateComponent(COMPONENT_TYPE type, bool active)
{
	Component* component = nullptr;

	switch (type) 
	{
	case COMPONENT_TYPE::TRANSFORM:
		component = new ComponentTransform(this);
		LOG_C("Component Transform added to the list");
		break;
	case COMPONENT_TYPE::MESH:
		component = new ComponentMesh(this);
		LOG_C("Component Mesh added to the list");
		break;
	case COMPONENT_TYPE::TEXTURE:
		component = new ComponentTexture(this);
		LOG_C("Component Texture added to the list");
		break;
	case COMPONENT_TYPE::CAMERA:
		component = new ComponentCamera(this);
		LOG_C("Component Camera added to the list");
		break;
	}

	if (component != nullptr) 
	{
		componentsList.push_back(component);
	}
		
	return component;
}

Component* GameObject::GetComponent(const COMPONENT_TYPE& type)
{
	for (std::vector<Component*>::iterator it = componentsList.begin(); it != componentsList.end(); ++it)
	{
		if (*it != nullptr && (*it)->GetComponentType() == type)
		{
			return *it;
		}	
	}
	return nullptr;
}

ComponentTransform* GameObject::GetComponentTransform()
{
	Component* transform = nullptr;
	for (std::vector<Component*>::iterator i = componentsList.begin(); i != componentsList.end(); i++)
	{
		if ((*i)->type == COMPONENT_TYPE::TRANSFORM)
		{
			return (ComponentTransform*)*i;
		}
	}
	return (ComponentTransform*)transform;
}

ComponentMesh* GameObject::GetComponentMesh()
{
	Component* mesh = nullptr;
	for (std::vector<Component*>::iterator i = componentsList.begin(); i != componentsList.end(); i++)
	{
		if ((*i)->type == COMPONENT_TYPE::MESH)
		{
			return (ComponentMesh*)*i;
		}
	}
	return (ComponentMesh*)mesh;
}

ComponentTexture* GameObject::GetComponentTexture()
{
	Component* texture = nullptr;
	for (std::vector<Component*>::iterator i = componentsList.begin(); i != componentsList.end(); i++)
	{
		if ((*i)->type == COMPONENT_TYPE::TEXTURE)
		{
			return (ComponentTexture*)*i;
		}
	}
	return (ComponentTexture*)texture;
}

ComponentCamera* GameObject::GetComponentCamera()
{
	Component* camera = nullptr;
	for (std::vector<Component*>::iterator i = componentsList.begin(); i != componentsList.end(); i++)
	{
		if ((*i)->type == COMPONENT_TYPE::CAMERA)
		{
			return (ComponentCamera*)*i;
		}
	}
	return (ComponentCamera*)camera;
}

bool GameObject::IsGameObjectActive()
{
	return data.active;
}

string GameObject::GetGameObjectName()
{
	return data.name.c_str();
}

uint GameObject::GetGameObjectId()
{
	return data.id;
}

int GameObject::GetGameObjectUUID()
{
	return data.UUID;
}

GameObject* GameObject::GetRootGameObject()
{
	return App->scene_intro->GOroot;
}

void GameObject::AddChild(GameObject* GO)
{
	if (GO->GOparent != nullptr)
		RemoveChild(GO);

	GO->GOparent = this;
	childrenList.push_back(GO);
}

void GameObject::RemoveChild(GameObject* GO)
{
	/*for (std::vector<GameObject*>::iterator it = childrenList.begin(); it != childrenList.end(); it++)
	{
		if ((*it) == GO)
		{
			childrenList.erase(it);
			break;
		}
	}*/

	for (int i = 0; i < childrenList.size(); i++)
	{
		if (childrenList[i] == GO)
		{
			childrenList.erase(childrenList.begin() + i);
		}
	}
}

int GameObject::GenerateUUID()
{
	int uuid = GenerateRandomBetween(99999999999);
	//LOG_C("rnd number is: %i", uuid); 
	return uuid;
}



