#include "C_InstanceID.h"

unsigned int C_InstanceID::count = 0;


unsigned int C_InstanceID::Get() const
{
	return id;
}