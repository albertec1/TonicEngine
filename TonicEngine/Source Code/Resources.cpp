#include "Resources.h"

Resource::Resource(uint uuid, RESOURCES_TYPE type)
{

}

Resource::~Resource()
{

}

RESOURCES_TYPE Resource::GetType() const
{
	return type;
}
