
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
	0, 0, 0
>scale
	1, 1, 1
>Camera
>enabled
	1
>near
	0.001
>far
	1000
>fovY
	90
>FollowCamera
>enabled
	1
>offset
	0, 8, 6
>lerpFactor
	0.08

>entity
>childs
	2
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
>PlayerMovement
>enabled
	1
>startPosition
	0, 0, 0
>speed
	10

>entity
>childs
	0
>name
	PlayerFX
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>model
>mesh_path
	ProceduralSphere
>texture_path
	

>entity
>childs
	0
>name
	PlayerTrigger
>component_count
	1
>transform
>position
	0, 1, 0
>rotation
	0, 0, 0
>scale
	0.5, 2, 0.5
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	1

>entity
>childs
	0
>name
	Ground
>component_count
	2
>transform
>position
	0, -3.7, 0
>rotation
	0, 0.750492, 0
>scale
	20, 1, 20
>StaticDraw
>enabled
	1
>model
>mesh_path
	ProceduralCube
>texture_path
	
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	0
