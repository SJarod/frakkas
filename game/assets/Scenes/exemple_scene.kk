>light
>position
	-0.333, 1, 1, 0
>ambient
	0.3, 0.3, 0.3
>diffuse
	1, 1, 1
>specular
	0.5, 0.5, 0.5
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
	20

>entity
>childs
	0
>name
	NewEntity_11
>component_count
	1
>transform
>position
	-7.09318, 1.52588e-05, 11.3832
>rotation
	0, 0, 0
>scale
	0.1, 0.1, 0.1
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Models/Skeleton/Skeleton.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_03.png

>entity
>childs
	0
>name
	Text
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Text
>enabled
	1
>position
	34.9, 78.6
>scale
	3.3, 1
>text
	hello world!
>color
	0, 0, 0, 1

>entity
>childs
	0
>name
	Ground
>component_count
	2
>transform
>position
	0, -6.6, 0
>rotation
	0, 0, 0
>scale
	20, 2, 20
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	game/assets/Textures/red.png

>entity
>childs
	1
>name
	Player
>component_count
	2
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
>PlayerMovement
>enabled
	1
>speed
	1000
>leftPower
	0.3
>rightPower
	0.3

>entity
>childs
	0
>name
	FX
>component_count
	2
>transform
>position
	0, -0.9, 0
>rotation
	0, -0, 0
>scale
	0.04, 0.04, 0.04
>StaticDraw
>enabled
	1
>meshPath
	game/assets/Models/Hero/Hero.fbx
>texturePath
	game/assets/Textures/Dungeons_Texture_03.png
>PlayerMeshRotation
>enabled
	1

>entity
>childs
	0
>name
	Camera
>component_count
	2
>transform
>position
	0, 7, 8
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
	0, 15, 10
>lerpFactor
	0.08
