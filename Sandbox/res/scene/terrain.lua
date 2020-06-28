---------------
-- Functions --
---------------

val = 0.0

function UpdatePointLight(entity, dt)
    val = val + dt
    p = ecs:GetPointLightComponent(entity)
    p.position.x = p.position.x + (math.sin(val) * 4.0)
end

function CreatePlantInstancesData()
    PlantInstancesData = {}
    for i = 0, 9, 1 do
        t = Transform.new()

        t.position = Vec3.new(0.0 + i * 4, 10.0, 0.0)
        t.rotation = Vec3.new(180.0, 0.0, 0.0)
        t.scale = Vec3.new(10.0)

        PlantInstancesData[i] = t
    end

    return PlantInstancesData
end

------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

SkyboxRenderer.new(4, scene)
SunRenderer.new(3, scene)
TerrainQuadtreeRenderer.new(2, scene)
--TerrainQuadtreeWfRenderer.new(2, scene)
ForwardRenderer.new(1, scene)
InstancingRenderer.new(0, scene)

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(-15.0, 227.0, -0.05), 24.0, -4.0, scene)
firstPersonCamera:SetCameraVelocity(48.0)
firstPersonCamera:SetMouseSensitivity(0.1)

------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")
scene:SetAmbientIntensity(Vec3.new(0.3, 0.3, 0.3))

--------------------
-- Load resources --
--------------------

plant = modelManager:GetModel("res/model/Plant_01/billboardmodel.obj")
sunTextureId = textureManager:LoadTexture("res/sun/sun.png")

a = {}
a[1] = "res/skybox/sky1/sRight.png"
a[2] = "res/skybox/sky1/sLeft.png"
a[3] = "res/skybox/sky1/sUp.png"
a[4] = "res/skybox/sky1/sDown.png"
a[5] = "res/skybox/sky1/sBack.png"
a[6] = "res/skybox/sky1/sFront.png"

skyboxCubemapId = textureManager:GetCubemapId(a)

terrainConfig = TerrainConfig.new(applicationContext, scene)
terrainConfig.scaleXz = 8000.0
terrainConfig.scaleY = 1700.0
terrainConfig.rootNodes = 12
terrainConfig.normalStrength = 60.0
terrainConfig.lodRanges = { 1750, 874, 386, 192, 100, 50, 0, 0 }
terrainConfig.use16BitHeightmap = true
terrainConfig:InitMapsAndMorphing("res/heightmap/ruhpolding/Ruhpolding8km.png")
terrainConfig:InitTextures(
    "res/terrain/terrain0/Grass (Hill).jpg",
    "res/terrain/terrain1/Grass.jpg",
    "res/terrain/terrain1/moss.jpg",
    "res/terrain/terrain0/Cliff (Layered Rock).jpg"
);

terrain = TerrainQuadtree.new(scene, terrainConfig)

---------------------
-- Create Entities --
---------------------

-- terrain

terrainEntity = ecs:CreateEntity()
ecs:AddTerrainQuadtreeComponent(terrainEntity, terrain)

-- plant instances

plantEntity = ecs:CreateEntity()
ecs:AddModelInstancesComponent(plantEntity, plant, false, true, CreatePlantInstancesData())

-- a point light

plightEntity0 = ecs:CreateEntity()
ecs:AddPointLightComponent(plightEntity0,
    Vec3.new(0.0, 40.0, 0.0), -- position
    Vec3.new(0.2, 0.2, 0.2),  -- ambientIntensity
    Vec3.new(0.2, 0.2, 10.0), -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),  -- specularIntensity
    1.0, 0.0022, 0.0019       -- constant, linear, quadratic
)
ecs:AddUpdateComponent(plightEntity0, "UpdatePointLight")

-- sun

sunEntity = ecs:CreateEntity()
ecs:AddSunComponent(sunEntity,
    Vec3.new(-0.35, -0.077, -0.077), -- direction
    Vec3.new(1.0, 0.8, 0.6),         -- diffuseIntensity
    Vec3.new(1.0, 0.8, 0.6),         -- specularIntensity
    sunTextureId,
    10.0
)

-- skybox

skyboxEntity = ecs:CreateEntity()
ecs:AddCubemapComponent(skyboxEntity, skyboxCubemapId)
