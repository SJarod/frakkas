
>entity
>childs
	0
>name
	Camera entity
>component_count
	2
>transform
>position
	0, 2, 5
>rotation
	0, 0, 0
>scale
	1, 1, 1
>CameraComponent
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
>FPSMovement
>enabled
	1

>entity
>childs
	0
>name
	Bagpack
>component_count
	2
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
	game/assets/bp.fbx
>texture_path
	game/assets/bp.jpg
>SphereCollider
>enabled
	1
>isTrigger
	0
>isStatic
	0
>radius
	1

>entity
>childs
	0
>name
	Super Bagpack
>component_count
	2
>transform
>position
	0, 5, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>StaticDraw
>enabled
	1
>model
>mesh_path
	game/assets/bp.fbx
>texture_path
	game/assets/bp.jpg
>SphereCollider
>enabled
	1
>isTrigger
	0
>isStatic
	0
>radius
	1

>entity
>childs
	0
>name
	Banana
>component_count
	1
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>AnimatedDraw
>enabled
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
	0, -3, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>BoxCollider
>enabled
	1
>isTrigger
	256
>isStatic
	1
>halfExtension
	10, 1, 10
>Bar
>enabled
	1
>speed
	5
>originPos
	0, 0, 0
>destPos
	2, 2, 0
