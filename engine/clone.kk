
>entity
>childs
	4
>name
	GolemClone
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Golem
>enabled
	0
>attackRange
	20
>chargeTime
	2.5
>attackTime
	0.5
>riseLegTime
	1
>hurtEffectTime
	0.1
>hurtEffectTint
	0.7, 0.5, 0, 1
>deathTime
	4
>EnemyStats
>enabled
	0
>baseLife
	12
>baseDamage
	6
>baseSpeed
	450
>baseAttackSpeed
	0.333
>baseScore
	20
>increaseRatio
	0.5
>SphereCollider
>enabled
	0
>cTrigger
	0
>cStatic
	0
>cKinematic
	0
>cDynamic
	1
>posX
	0
>posY
	0
>posZ
	0
>rotX
	1
>rotY
	1
>rotZ
	1
>Life
>enabled
	0
>life
	12
>invincibilityTime
	0.75
>EnemyMeshRotation
>enabled
	0

>entity
>childs
	0
>name
	GFX
>component_count
	1
>transform
>position
	0, -0.3, 0
>rotation
	0, 0, 0
>scale
	0.08, 0.08, 0.08
>AnimatedDraw
>enabled
	0
>meshPath
	game/assets/Models/Golem/Golem.fbx
>texturePath
	game/assets/Textures/Golem_Texture.png
>skmodel.material.ambient
	0.8, 0.8, 0.8
>skmodel.material.diffuse
	1, 1, 1
>skmodel.material.specular
	1, 1, 1
>skmodel.material.emissive
	0, 0, 0
>skmodel.material.tint
	0, 0, 0, 1
>skmodel.material.shininess
	32
>skmodel
>count
	0
>animGraph
>count
	5
>file
	game/assets/Models/Golem/Golem_Idle.fbx
>file
	game/assets/Models/Golem/Golem_Death.fbx
>file
	game/assets/Models/Golem/Golem_Stomp.fbx
>file
	game/assets/Models/Golem/Golem_Walk.fbx
>file
	game/assets/Models/Golem/Golem_Hit.fbx

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 7.4, 0
>rotation
	0, 0, 0
>scale
	4.3, 5.73333, 2.29333
>BoxCollider
>enabled
	0
>cTrigger
	0
>cStatic
	0
>cKinematic
	1
>cDynamic
	0
>posX
	0
>posY
	0
>posZ
	0
>rotX
	0
>rotY
	0
>rotZ
	0

>entity
>childs
	0
>name
	AttackGFX
>component_count
	1
>transform
>position
	0, 0.2, 0
>rotation
	1.5708, 0, 0
>scale
	7.5, 7.5, 7.5
>StaticDraw
>enabled
	0
>meshPath
	ProceduralQuad
>texturePath
	game/assets/Textures/Golem_Stomp.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	0, 0, 0
>model.material.emissive
	1, 1, 1
>model.material.tint
	0, 0, 0, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Sounds
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Hit.mp3
>loop
	0
>volume
	30
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Roar.mp3
>loop
	0
>volume
	50
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Walk.mp3
>loop
	1
>volume
	30
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\swords\Hit_Golem.mp3
>loop
	0
>volume
	10
>Sound
>enabled
	1
>soundPath
	game\assets\Sounds\golem\Golem_Death.mp3
>loop
	0
>volume
	50
