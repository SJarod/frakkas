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
	0, 0, 0
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
