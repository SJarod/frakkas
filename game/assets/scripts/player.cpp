#include "player_movement.hpp"

#include "player.hpp"

KK_COMPONENT_IMPL_BEGIN(Player)

    KK_FIELD_PUSH(Player, life, EDataType::INT)

KK_COMPONENT_IMPL_END

void Player::OnStart()
{
    // ROOT
    owner.get()->name = "Player";

    rb = owner.get()->AddComponent<SphereCollider>();
    rb->isStatic = false;
    rb->constraintRotationX = true;
    rb->constraintRotationY = true;
    rb->constraintRotationZ = true;
    owner.get()->AddComponent<PlayerMovement>();

    // CHILD
    Entity* child = GetEntityContainer().CreateEntity();
    GetEntityContainer().SetEntityParent(*child, *owner);
    child->name = "GFX";

    ad = child->AddComponent<AnimatedDraw>();
    ad->SetMesh("game/assets/Models/Hero/Hero.fbx");
    ad->SetTexture("game/assets/Textures/Dungeons_Texture_01.png", false);
    ad->SetAnimation("game/assets/Models/Hero/Hero_Idle_1.fbx");

    child->transform.position = Vector3(0.f, -0.3f, 0.f);
    child->transform.scale = Vector3(0.05f, 0.05f, 0.05f);
}

void Player::OnUpdate()
{
    if (!IsAlive())
        Death();
}

void Player::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_otherCollider.GetID() == "skeletonSword")
        --life;

    if (i_otherCollider.GetID() == "golemw")
        --life;
}

bool Player::IsAlive()
{
    return life > 0;
}

void Player::Death()
{

}

void Player::Attack()
{
    std::cout << "ATTACKING" << std::endl;
}