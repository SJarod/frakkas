>light
>position
	-0.6, 1, -1, 0
>ambient
	0.659292, 0.659292, 0.659292
>diffuse
	1, 0.446903, 0.446903
>specular
	0.460177, 0.221944, 0.221944
>toonShading
	1
>stepAmount
	5
>stepSize
	0.1
>specSize
	0.1
>shadow
	1
>shadowPCF
	0
>shadowDistance
	50

>entity
>childs
	1
>name
	Sphere
>component_count
	1
>transform
>position
	-20.386, -4.5, 0
>rotation
	-0.862193, 0, 0
>scale
	5.14, 5.14, 5.14
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	game/assets\Textures\red.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>SphereCollider
>enabled
	1
>isStatic
	1
>trigger
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
	1
>name
	Ground
>component_count
	1
>transform
>position
	0, -5, 0
>rotation
	0, 0, 0
>scale
	999, 1, 999
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	game/assets\Textures\gold.jpg
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	0, 0, 0
>model.material.shininess
	32

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
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
	Ball
>component_count
	2
>transform
>position
	15.4848, 0, 0
>rotation
	0, 0, 0
>scale
	1.85, 1.85, 1.85
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	game/assets\Textures\yellow.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.shininess
	32
>SphereCollider
>enabled
	1
>isStatic
	0
>trigger
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
	1
>name
	Cube
>component_count
	1
>transform
>position
	0, -1, 0
>rotation
	0, 0, 0
>scale
	4.1, 4.1, 4.1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	game/assets\Textures\blue.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.shininess
	32

>entity
>childs
	0
>name
	Collider
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
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
	Camera
>component_count
	2
>transform
>position
	0, 0, 0
>rotation
	-0.820305, 0, 0
>scale
	1, 1, 1
>Camera
>enabled
	1
>near
	0.1
>far
	1000
>fovY
	90
>FollowCamera
>enabled
	1
>offset
	0, 20, 16
>lerpFactor
	0.08

>entity
>childs
	1
>name
	Player
>component_count
	2
>transform
>position
	0, 5, 20
>rotation
	0, 0, 0
>scale
	4, 4, 4
>PlayerMovement
>enabled
	1
>walkSpeed
	400
>runSpeed
	1000
>leftPower
	0.3
>rightPower
	0.3
>SphereCollider
>enabled
	1
>isStatic
	0
>trigger
	0
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

>entity
>childs
	0
>name
	FX
>component_count
	3
>transform
>position
	0, -0.985, 0
>rotation
	0, 0, 0
>scale
	0.012, 0.012, 0.012
>PlayerMeshRotation
>enabled
	1
>PlayerAnimation
>enabled
	1
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Hero\Hero.fbx
>texturePath
	game/assets\Textures\Dungeons_Texture_03.png
>model.material.ambient
	0.8, 0.8, 0.8
>model.material.diffuse
	1, 1, 1
>model.material.specular
	1, 1, 1
>model.material.shininess
	32
>skmodel
>count
	2
>transform
>position
	0, 0, 0
>rotation
	1.93729, 0, 0
>scale
	222, 222, 222
>boneID
	35
>socketMesh
	game/assets/Models/Hero/HeroSword.fbx
>socketTexture
	game/assets\Textures\Dungeons_Texture_03.png
>transform
>position
	0, 0, 0
>rotation
	-1.53589, -2.93214, 0
>scale
	100, 100, 100
>boneID
	12
>socketMesh
	game/assets/Models/Hero/HeroShield.fbx
>socketTexture
	game/assets\Textures\Dungeons_Texture_03.png
>animGraph
>count
	6
>file
	game/assets\Models\Hero\Hero_Idle_1.fbx
>file
	game/assets\Models\Hero\Hero_Walk.fbx
>file
	game/assets\Models\Hero\Hero_Run.fbx
>file
	game/assets\Models\Hero\Hero_Slash_1.fbx
>file
	game/assets\Models\Hero\Hero_Slash_3.fbx
>file
	game/assets\Models\Hero\Hero_Slash_4.fbx
