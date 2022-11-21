#include "C_ProjectileAttack.h"
#include "Object.h"

std::unordered_map<FacingDirection, sf::IntRect> C_ProjectileAttack::textureDirectionBindings = {}; //Static (Same for all projectile attacks)
std::unordered_map<FacingDirection, sf::Vector2f> C_ProjectileAttack::offsetDirectionBindings = {};
std::unordered_map<FacingDirection, sf::Vector2f> C_ProjectileAttack::velocityDirectionBindings = {};

void C_ProjectileAttack::Awake()
{
	animation = owner->GetComponent<C_Animation>();
	direction = owner->GetComponent<C_Direction>();
	eStack = owner->GetComponent<C_Events>();
}
void C_ProjectileAttack::Start()
{
	projectileTextureID = owner->context->textureAllocator->Add(
		owner->context->workingDir->Get() + "Assets/Character/20.08b - Bow Combat 3.1/weapon sprites/aro_comn_v01.png");

	if (textureDirectionBindings.size() == 0)
	{
		textureDirectionBindings.emplace(FacingDirection::Up, sf::IntRect(sf::Vector2(0, 0), sf::Vector2(16, 16)));
		textureDirectionBindings.emplace(FacingDirection::Down, sf::IntRect(sf::Vector2(64, 0), sf::Vector2(16, 16)));
		textureDirectionBindings.emplace(FacingDirection::Left, sf::IntRect(sf::Vector2(0, 16), sf::Vector2(16, 16)));
		textureDirectionBindings.emplace(FacingDirection::Right, sf::IntRect(sf::Vector2(16, 16), sf::Vector2(-16, 16)));
	}

	if (offsetDirectionBindings.size() == 0)
	{
		offsetDirectionBindings.emplace(FacingDirection::Up, sf::Vector2f());
		offsetDirectionBindings.emplace(FacingDirection::Left, sf::Vector2f(-8.f, 3.f));
		offsetDirectionBindings.emplace(FacingDirection::Down, sf::Vector2f(-3.f, 15.f));
		offsetDirectionBindings.emplace(FacingDirection::Right, sf::Vector2f(8.f, 3.f));
	}

	if (velocityDirectionBindings.size() == 0)
	{
		velocityDirectionBindings.emplace(FacingDirection::Up, sf::Vector2f(0.f, -1.f));
		velocityDirectionBindings.emplace(FacingDirection::Left, sf::Vector2f(-1.f, 0.f));
		velocityDirectionBindings.emplace(FacingDirection::Down, sf::Vector2f(0.f, 1.f));
		velocityDirectionBindings.emplace(FacingDirection::Right, sf::Vector2f(1.f, 0.f));
	}

	//Animation action is an alias for std::function<void(void)>; allowing us to store SpawnProjectile as a callable object. 
	//Because it is non-static, it requires a reference to the object that owns the function, this.
	animation->AddAnimationAction(AnimationState::BowShootStraight, FacingDirection::Up, 7, std::bind(&C_ProjectileAttack::SpawnProjectile, this));
	animation->AddAnimationAction(AnimationState::BowShootStraight, FacingDirection::Left, 7, std::bind(&C_ProjectileAttack::SpawnProjectile, this));
	animation->AddAnimationAction(AnimationState::BowShootStraight, FacingDirection::Down, 7, std::bind(&C_ProjectileAttack::SpawnProjectile, this));
	animation->AddAnimationAction(AnimationState::BowShootStraight, FacingDirection::Right, 7, std::bind(&C_ProjectileAttack::SpawnProjectile, this));
	
}
void C_ProjectileAttack::Update(float deltaTime)
{
	
}
//Changed from Private to Public
void C_ProjectileAttack::SpawnProjectile()
{
	FacingDirection faceDir = direction->Get();

	// Create new object.
	std::shared_ptr<Object> projectile = std::make_shared<Object>(owner->context);

	// Set objects position to owners position
	projectile->transform->SetPosition(owner->transform->GetPosition() + offsetDirectionBindings.at(faceDir));;

	// Add sprite component to object
	auto projSprite = projectile->AddComponent<C_Sprite>();
	projSprite->Load(projectileTextureID);
	projSprite->SetDrawLayer(DrawLayer::Entities);
	projSprite->SetSortOrder(100);
	projSprite->SetTextureRect(textureDirectionBindings.at(faceDir)); // Use the direction to set the texture rect.

	//Set velocity based on direction
	auto velocity = projectile->AddComponent<C_Velocity>();
	velocity->SetDirection(velocityDirectionBindings.at(faceDir));
	velocity->SetAccelerationRate(1);
	velocity->SetFrictionCoefficant(0);
	velocity->SetMaxVelocity(400.f);
	
	auto mass = projectile->AddComponent<C_Mass>();
	mass->SetMass(1);

	//Add collider component
	auto collider = projectile->AddComponent<C_BoxCollider>();
	collider->SetSize(16, 16);
	collider->SetLayer(CollisionLayer::Projectile);

	projectile->AddComponent<C_RemoveObjectOnCollisionEnter>();//Add component for destroying projectile

	owner->context->objects->Add(projectile);// Add object to collection
}