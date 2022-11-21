#include "C_Mass.h"
#include "Object.h"


//The mass is 100 by default, and that is how much mass an average human will have.
void C_Mass::SetMass(float m)
{
	this->mass = m;
}

float C_Mass::GetMass()
{
	return this->mass;
}
