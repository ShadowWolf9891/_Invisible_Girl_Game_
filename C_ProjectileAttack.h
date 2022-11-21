#pragma once
#ifndef C_ProjectileAttack_h
#define C_ProjectileAttack_h

#include "Component.h"
#include "C_Animation.h"
#include "Input.h"
#include "ObjectCollection.h"
#include "C_RemoveObjectOnCollisionEnter.h"
#include "C_Events.h"

class C_ProjectileAttack : public Component
{
public:
	C_ProjectileAttack(Object* owner) : Component(owner), projectileVelocity(400.f), isAttacking(false)
	{

	};
	virtual ~C_ProjectileAttack() = default;

	void Awake() override;
	void Start() override;

	void Update(float deltaTime) override;
	
	void SpawnProjectile();
	
	void SetIsAttacking(bool isAttacking) { this->isAttacking = isAttacking; };
	bool GetIsAttacking() { return this->isAttacking; };
	
	
private:

	static std::unordered_map<FacingDirection, sf::IntRect> textureDirectionBindings;
	static std::unordered_map<FacingDirection, sf::Vector2f> offsetDirectionBindings;
	static std::unordered_map<FacingDirection, sf::Vector2f> velocityDirectionBindings;

	std::shared_ptr<C_Animation> animation;
	std::shared_ptr<C_Direction> direction;
	std::shared_ptr<C_Events> eStack;
	int projectileTextureID;
	float projectileVelocity;

	bool isAttacking;
	
};
#endif /* C_ProjectileAttack_h */