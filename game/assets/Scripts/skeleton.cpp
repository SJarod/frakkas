#include "player_movement.hpp"

#include "skeleton.hpp"

KK_COMPONENT_IMPL_BEGIN(Skeleton)

    KK_FIELD_PUSH(Skeleton, speed, EDataType::FLOAT)
    KK_FIELD_PUSH(Skeleton, attackTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Skeleton, life, EDataType::INT)
    KK_FIELD_PUSH(Skeleton, damage, EDataType::INT)

KK_COMPONENT_IMPL_END

void Skeleton::OnStart()
{
    // ROOT
    owner.get()->name = "Skeleton_" + std::to_string(owner.get()->GetID());

    rb = owner.get()->AddComponent<SphereCollider>();
    rb->isStatic = false;
    rb->constraintRotationX = true;
    rb->constraintRotationY = true;
    rb->constraintRotationZ = true;

    // CHILD
    Entity* child = GetEntityContainer().CreateEntity();
    GetEntityContainer().SetEntityParent(*child, *owner);
    child->name = "GFX";

    ad = child->AddComponent<AnimatedDraw>();
    ad->SetMesh("game/assets/Models/Skeleton/Skeleton.fbx");
    ad->SetTexture("game/assets/Textures/Dungeons_Texture_01.png", false);
    ad->SetAnimation("game/assets/Models/Skeleton/Skeleton_Idle.fbx");

    child->transform.position = Vector3(0.f, -0.3f, 0.f);
    child->transform.scale = Vector3(0.048f, 0.048f, 0.048f);

    Entity* pm = GetEntityContainer().FindEntityWithComponent<PlayerMovement>();
    if (pm)
        playerTransform = &pm->transform;
}

void Skeleton::OnUpdate()
{
    if (!IsAlive())
        Death();

    curAttackTime += Time::GetDeltaTime();

    if (playerTransform)
    {
        distFromPlayer = Vector3::Distance(playerTransform->position.get(), Position().get());

        if (distFromPlayer >= attackRange)
        {
            vecDir = Vector3::VecFromPt(Position().get(), playerTransform->position.get()).Normalize() * speed * Time::GetDeltaTime();
            translation = Vector3(vecDir.x, rb->velocity.y, vecDir.z);
            rb->velocity = translation;
        }
    }

    if (curAttackTime >= attackTime && distFromPlayer < attackRange)
    {
        Attack();
        curAttackTime = 0.f;
    }
}

void Skeleton::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_otherCollider.GetID() == "playerSword")
        --life;
}

bool Skeleton::IsAlive()
{
    return life > 0;
}

void Skeleton::Death()
{

}

void Skeleton::Attack()
{
    std::cout << "ATTACKING" << std::endl;
}