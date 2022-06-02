#include "player_movement.hpp"

#include "golem.hpp"

KK_COMPONENT_IMPL_BEGIN(Golem)

    KK_FIELD_PUSH(Golem, speed, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, attackTime, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, attackRange, EDataType::FLOAT)
    KK_FIELD_PUSH(Golem, life, EDataType::INT)
    KK_FIELD_PUSH(Golem, damage, EDataType::INT)
    KK_FIELD_PUSH(Golem, alive, EDataType::BOOL)

KK_COMPONENT_IMPL_END

void Golem::OnStart()
{
    // ROOT
    owner.get()->name = "Golem_" + std::to_string(owner.get()->GetID());
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
    ad->SetMesh("game/assets/Models/Golem/Golem.fbx");
    ad->SetTexture("game/assets/Textures/Golem_Texture.png", false);
    ad->SetAnimation("game/assets/Models/Golem/Golem_Idle.fbx");

    child->transform.position = Vector3(0.f, -0.3f, 0.f);
    child->transform.scale = Vector3(0.048f, 0.048f, 0.048f);

    Entity* pm = GetEntityContainer().FindEntityWithComponent<PlayerMovement>();
    if (pm)
        playerTransform = &pm->transform;
}

void Golem::OnUpdate()
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

void Golem::OnTriggerEnter(const Collider& i_ownerCollider, const Collider& i_otherCollider)
{
    if (i_otherCollider.GetID() == "playerSword")
        --life;
}

bool Golem::IsAlive()
{
    return life > 0;
}

void Golem::Death()
{

}

void Golem::Attack()
{
    std::cout << "ATTACKING" << std::endl;
}