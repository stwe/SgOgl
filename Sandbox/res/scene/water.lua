------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

skyboxRenderer = SkyboxRenderer.new(2, scene)
sunRenderer = SunRenderer.new(1, scene)
WaterRenderer.new(0, scene)

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(1334.0, 820.0, 227.0), -178.0, -22.0, scene)
firstPersonCamera:SetCameraVelocity(128.0)
firstPersonCamera:SetMouseSensitivity(0.025)

------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")
scene:SetAmbientIntensity(Vec3.new(0.2, 0.2, 0.2))

--------------------
-- Load resources --
--------------------

-- sun

sunTextureId = textureManager:LoadTexture("res/sun/sun.png")

-- skybox

a = {}
a[1] = "res/skybox/sky1/sRight.png"
a[2] = "res/skybox/sky1/sLeft.png"
a[3] = "res/skybox/sky1/sUp.png"
a[4] = "res/skybox/sky1/sDown.png"
a[5] = "res/skybox/sky1/sBack.png"
a[6] = "res/skybox/sky1/sFront.png"

skyboxCubemapId = textureManager:GetCubemapId(a)

-- water

local xWaterPos = 0.0
local zWaterPos = 0.0
local waterHeight = 0.0
local waterTileSize = 5000.0

ocean = Water.new(
    "ocean",
    applicationContext,
    xWaterPos, zWaterPos,
    waterHeight,
    Vec3.new(waterTileSize, 1.0, waterTileSize),
    "res/water/waterDUDV.png", "res/water/normal.png",
    scene
)

ocean:AddRendererToReflectionTexture(skyboxRenderer)
ocean:AddRendererToReflectionTexture(sunRenderer)

ocean:AddRendererToRefractionTexture(skyboxRenderer)
ocean:AddRendererToRefractionTexture(sunRenderer)

---------------------
-- Create Entities --
---------------------

-- sun

sunEntity = ecs:CreateEntity()
ecs:AddSunComponent(sunEntity,
    Vec3.new(1.0, -0.2, -0.4), -- direction
    Vec3.new(1.0, 0.8, 0.6),   -- diffuseIntensity
    Vec3.new(1.0, 0.8, 0.6),   -- specularIntensity
    sunTextureId,
    10.0
)

-- skybox

skyboxEntity = ecs:CreateEntity()
ecs:AddCubemapComponent(skyboxEntity, skyboxCubemapId)

-- water

oceanEntity = ecs:CreateEntity()
ecs:AddWaterComponent(oceanEntity, ocean)
ecs:AddTransformComponent(oceanEntity, Vec3.new(xWaterPos, waterHeight, zWaterPos), Vec3.new(0.0, 0.0, 0.0), Vec3.new(waterTileSize, 1.0, waterTileSize))
