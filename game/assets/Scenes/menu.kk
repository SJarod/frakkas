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
	0
>stepAmount
	4
>stepSize
	0.1
>specSize
	0.1
>shadow
	0
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
	0.1, -0.6, -4.1
>rotation
	-1.42768, -0.0122173, 0
>scale
	1, 1, 1
>Camera
>enabled
	1
>near
	1
>far
	300
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
	game/assets/Textures/gold.jpg

>entity
>childs
	0
>name
	MenuManager
>component_count
	5
>transform
>position
	0, 0, 0
>rotation
	0, 0, 0
>scale
	1, 1, 1
>Button
>enabled
	1
>position
	30.9, 73
>scale
	300, 40
>text
	QUIT
>textScale
	2.1
>idle
	0, 0, 0, 1
>hovered
	0.353982, 0.296029, 0.296029, 1
>activated
	0.783186, 0.741601, 0.741601, 1
>MenuScript
>enabled
	1
>Panel
>enabled
	1
>position
	31.7, 4.7
>scale
	275.4, 50
>color
	0, 0, 0, 0.563
>Text
>enabled
	1
>position
	34.5, 6.1
>scale
	2.6, 1
>text
	FRAKKARENA
>color
	1, 1, 1, 1
>ImageButton
>enabled
	1
>position
	30, 25.6
>scale
	300, 150
>imageFlip
	0
>Texture
	game/assets/Textures/play_menu_button.jpg
>idle
	0.8, 0.8, 0.8, 1
>hovered
	1, 1, 1, 1
>activated
	1, 1, 0.2, 1
