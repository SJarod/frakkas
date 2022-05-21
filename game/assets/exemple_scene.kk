>light
>position
	1, 1, 1, 0
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
	-0, 0.750492, -0
>scale
	20, 1, 20
>StaticDraw
>enabled
	1
>model
>name
	ProceduralCube
>textureName
	game/assets\gold.jpg
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	0

>entity
>childs
	0
>name
	Camera
>component_count
	3
>transform
>position
	0.502511, 8.4844, 15.8626
>rotation
	-0.855285, 2.52264e-08, -2.90239e-08
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
	0, 10.7, 9.3
>lerpFactor
	0.08
>FPSMovement
>enabled
	1

>entity
>childs
	0
>name
	BigGround
>component_count
	2
>transform
>position
	0, -6.6, 0
>rotation
	-0, -0, -0
>scale
	30, 1.5, 30
>StaticDraw
>enabled
	1
>model
>name
	ProceduralCube
>textureName
	game/assets\gold.jpg
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	0

>entity
>childs
	0
>name
	Camera
>component_count
	3
>transform
>position
	0.502511, 5.7844, 15.2626
>rotation
	-0.743507, 3.02586e-08, -2.7824e-08
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
	0, 8, 8.7
>lerpFactor
	0.08
>FPSMovement
>enabled
	1

>entity
>childs
	0
>name
	BigGround
>component_count
	2
>transform
>position
	0, -6.6, 0
>rotation
	-0, -0, -0
>scale
	30, 1.5, 30
>StaticDraw
>enabled
	1
>model
>name
	ProceduralCube
>textureName
	game/assets\gold.jpg
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	0

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
	800
>leftPower
	0.3
>rightPower
	0.3

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
>name
	ProceduralSphere
>textureName
	game/assets\gold.jpg

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
	-0, -0, -0
>scale
	0.5, 2, 0.5
>BoxCollider
>enabled
	1
>isStatic
	1
>trigger
	1
