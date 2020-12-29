#include "Resources.h"

Resource::Resource(RESOURCES_TYPE type)
{

}

Resource::~Resource()
{
}

RESOURCES_TYPE Resource::GetType() const
{
	return type;
}
