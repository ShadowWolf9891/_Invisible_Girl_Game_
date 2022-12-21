#include "C_KeyboardMovement.h"
#include "Object.h"

void C_KeyboardMovement::Awake()
{
	velocity = owner->GetComponent<C_Velocity>();
}
void C_KeyboardMovement::SetMovementSpeed(float moveSpeed)
{
	this->moveSpeed = moveSpeed;
}
void C_KeyboardMovement::Update(float deltaTime)
{
	float xMove = 0.f;
	if (owner->context->input->IsKeyPressed(Input::Key::Left))
	{
		xMove = -moveSpeed;
	}
	else if (owner->context->input->IsKeyPressed(Input::Key::Right))
	{
		xMove = moveSpeed;
	}
	float yMove = 0.f;
	if (owner->context->input->IsKeyPressed(Input::Key::Up))
	{
		yMove = -moveSpeed;
	}
	else if (owner->context->input->IsKeyPressed(Input::Key::Down))
	{
		yMove = moveSpeed;
	}
	
	velocity->SetDirection(xMove, yMove);

}

