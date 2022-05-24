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
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
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
	0, 0, 0
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
	game/assets/Textures/gold.jpg

>entity
>childs
	0
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
>StaticDraw
>enabled
	1
>meshPath
	ProceduralSphere
>texturePath
	game/assets/Textures/gold.jpg

>entity
>childs
	0
>name
	NewEntity_4
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
	game/assets/Sounds/GameWin.mp3
>loop
	0
>volume
	1
