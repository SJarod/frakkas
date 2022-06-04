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
	Camera
>component_count
	2
>transform
>position
	-5.11021e-05, 4.48, 8.00008
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
	0, 8, 8
>lerpFactor
	0.08

>entity
>childs
	0
>name
	Ground
>component_count
	2
>transform
>position
	0, -5, 0
>rotation
	-0, -0, -0
>scale
	10, 1, 10
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	game/assets/gold.jpg

>entity
>childs
	0
>name
	Player
>component_count
	3
>transform
>position
	-5.1102e-05, -3.52, 8.43376e-05
>rotation
	3.13084e-07, 2.89636e-05, -1.15537e-08
>scale
	1, 1, 1
>SphereCollider
>enabled
	1
>isStatic
	0
>trigger
	0
>StaticDraw
>enabled
	1
>meshPath
	ProceduralSphere
>texturePath
	game/assets/gold.jpg
>PlayerMovement
>enabled
	1
>startPosition
	0, 0, 0
>speed
	800
>leftPower
	0.3
>rightPower
	0.3

>entity
>childs
	0
>name
	RumbleTrigger
>component_count
	3
>transform
>position
	4.4, -1.7, 0
>rotation
	-0, -0, -0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>meshPath
	ProceduralCube
>texturePath
	game/assets\blue.png
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	1
>RumbleTrigger
>enabled
	1

>entity
>childs
	0
>name
	NewEntity_5
>component_count
	1
>transform
>position
	0, 0, -2.08841
>rotation
	0, 0, 0
>scale
	1, 1, 1
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Golem\Golem.fbx
>texturePath
	game/assets\Models\Golem\Textures\FantasyRivals_Texture_02_D.png
>animationPath
	game/assets\Models\Golem\Golem_Death.fbx

>entity
>childs
	0
>name
	NewEntity_5
>component_count
	1
>transform
>position
	0, 0, -420.243
>rotation
	0, 0, 0
>scale
	1, 1, 1
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Golem\Golem.fbx
>texturePath
	game/assets\Models\Golem\Textures\FantasyRivals_Texture_02_D.png
>animationPath
	game/assets\Models\Golem\Golem_Hit.fbx

>entity
>childs
	0
>name
	NewEntity_5
>component_count
	1
>transform
>position
	0, 0, -245.01
>rotation
	0, 0, 0
>scale
	1, 1, 1
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Golem\Golem.fbx
>texturePath
	game/assets\Models\Golem\Textures\FantasyRivals_Texture_02_D.png
>animationPath
	game/assets\Models\Golem\Golem_Idle.fbx

>entity
>childs
	0
>name
	NewEntity_5
>component_count
	1
>transform
>position
	0, 0, -135.633
>rotation
	0, 0, 0
>scale
	1, 1, 1
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Golem\Golem.fbx
>texturePath
	game/assets\Models\Golem\Textures\FantasyRivals_Texture_02_D.png
>animationPath
	game/assets\Models\Golem\Golem_Stomp.fbx

>entity
>childs
	0
>name
	NewEntity_5
>component_count
	1
>transform
>position
	0, 0, 103.508
>rotation
	0, 0, 0
>scale
	1, 1, 1
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Golem\Golem.fbx
>texturePath
	game/assets\Models\Golem\Textures\FantasyRivals_Texture_02_D.png
>animationPath
	game/assets\Models\Golem\Golem_Walk.fbx

>entity
>childs
	0
>name
	NewEntity_13
>component_count
	1
>transform
>position
	254.338, 0, 0
>rotation
	0, 0, 0
>scale
	100, 100, 100
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Hero+Skel\Hero\Hero.fbx
>texturePath
	game/assets\Textures\Dungeons_Texture_01.png
>animationPath
	game/assets\Models\Hero+Skel\Hero\Hero_Walk.fbx

>entity
>childs
	0
>name
	NewEntity_14
>component_count
	1
>transform
>position
	-269.523, 0, 0
>rotation
	0, 0, 0
>scale
	100, 100, 100
>AnimatedDraw
>enabled
	1
>meshPath
	game/assets\Models\Hero+Skel\Skeleton\Skeleton.fbx
>texturePath
	game/assets\Textures\Dark_texture.png
>animationPath
	game/assets\Models\Hero+Skel\Skeleton\Skeleton_Run.fbx

>entity
>childs
	0
>name
	Sphere_15
>component_count
	1
>transform
>position
	-52.3249, 0, 0
>rotation
	0, 0, 0
>scale
	26.2, 26.2, 26.2
>StaticDraw
>enabled
	1
>meshPath
	ProceduralSphere
>texturePath
	game/assets\Textures\red.png

>entity
>childs
	0
>name
	Cube_16
>component_count
	1
>transform
>position
	64.5034, 0, 0
>rotation
	0, 0, 0
>scale
	25.6, 25.6, 25.6
>StaticDraw
>enabled
	1
>meshPath
	ProceduralIcoSphere
>texturePath
	game/assets\Textures\blue.png

>entity
>childs
	0
>name
	NewEntity_17
>component_count
	1
>transform
>position
	0, 0, -130.634
>rotation
	0, 0, 0
>scale
	0.01, 0.01, 0.01
>StaticDraw
>enabled
	1
>meshPath
	game/assets\Maps\Dark_map.fbx
>texturePath
	game/assets\Textures\Dark_texture.png

>entity
>childs
	0
>name
	NewEntity_18
>component_count
	1
>transform
>position
	0, 0, 63.4186
>rotation
	0, 0, 0
>scale
	0.01, 0.01, 0.01
>StaticDraw
>enabled
	1
>meshPath
	game/assets\Maps\Desert_map.fbx
>texturePath
	game/assets\Textures\Desert_texture.png

>entity
>childs
	0
>name
	NewEntity_19
>component_count
	1
>transform
>position
	-110.863, 0, 0
>rotation
	0, 0, 0
>scale
	0.01, 0.01, 0.01
>StaticDraw
>enabled
	1
>meshPath
	game/assets\Maps\Forest_map.fbx
>texturePath
	game/assets\Textures\Forest_Texture.png

>entity
>childs
	0
>name
	NewEntity_20
>component_count
	1
>transform
>position
	117.86, 0, 0
>rotation
	0, 0, 0
>scale
	0.01, 0.01, 0.01
>StaticDraw
>enabled
	1
>meshPath
	game/assets\Maps\Winter_map.fbx
>texturePath
	game/assets\Textures\Winter_map.png

>entity
>childs
	0
>name
	NewEntity_18
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>SoundComponent
>enabled
	1
>soundPath
	game/assets\Sounds\Teleportatoin.wav
>loop
	0
>volume
	1
