#include "C_Camera.h"
#include "Object.h"


void C_Camera::LateUpdate(float deltaTime)
{
	sf::View view = owner->context->window->GetView();

	const sf::Vector2f& targetPos = owner->transform->GetPosition();
	view.setCenter(sf::Vector2(targetPos.x, targetPos.y));

	owner->context->window->SetView(view);

}