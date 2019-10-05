#include <string>
#include "Item.h"

Item::Item() 
{

}

Item::Item(std::string displayName)
{
	DisplayName = displayName;
}

std::string Item::Use()
{
	return DisplayName;
}