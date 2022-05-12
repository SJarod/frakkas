
>entity
>childs
	0
>name
	Camera entity
>component_count
	2
>transform
>position
	0.0195347, 9.99953, 9.17656
>rotation
	-0.927295, 0, -0
>scale
	1, 1, 1
>Camera
>enabled
	1
>camera
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>fovy
	1.5708
>near
	0.001
>far
	1000
>FollowCamera
>enabled
	1
>offset
	0, 8, 6
>lerpFactor
	0.1

>entity
>childs
	1
>name
	Player
>component_count
	2
>transform
>position
	0.0195346, 1.99953, 3.18024
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
	PlayerFx
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
	game/assets\gold.jpg

>entity
>childs
	0
>name
	Ground
>component_count
	2
>transform
>position
	0, 0, 0
>rotation
	0, 0.820305, 0
>scale
	10, 1, 10
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
