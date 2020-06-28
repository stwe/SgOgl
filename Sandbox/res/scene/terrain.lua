---------------
-- Functions --
---------------

function RandomFloat(lower, greater)
    return lower + math.random()  * (greater - lower);
end

function CreateGrassInstancesData()
    instancesData = {}

    plantHeight = 8.0

    i = 1000000
    c = 0

    while (c < i)
    do
        x = RandomFloat(-3980.0, 3980.0)
        z = RandomFloat(-3980.0, 3980.0)
        height = terrainConfig:GetHeightAt(x, z, RandomFloat(0.01, 0.02), RandomFloat(0.3, 0.5))

        if (height > 0.0) then
            t = Transform.new()
            t.position = Vec3.new(x, height + plantHeight, z)
            t.rotation = Vec3.new(180.0, 0.0, 0.0)
            t.scale = Vec3.new(plantHeight)

            instancesData[c] = t

            c = c + 1
        end
    end

    return instancesData
end

function CreateTreeInstancesData()
    instancesData = {}

    plantHeight = 36.0

    i = 250000
    c = 0

    while (c < i)
    do
        x = RandomFloat(-3980.0, 3980.0)
        z = RandomFloat(-3980.0, 3980.0)
        height = terrainConfig:GetHeightAt(x, z, RandomFloat(0.07, 0.08), 0.9)

        if (height > 0.0) then
            t = Transform.new()
            t.position = Vec3.new(x, height + plantHeight, z)
            t.rotation = Vec3.new(180.0, 0.0, 0.0)
            t.scale = Vec3.new(plantHeight)

            instancesData[c] = t

            c = c + 1
        end
    end

    return instancesData
end

------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

SkyboxRenderer.new(3, scene)
SunRenderer.new(2, scene)
TerrainQuadtreeRenderer.new(1, scene)
--TerrainQuadtreeWfRenderer.new(1, scene)
InstancingRenderer.new(0, scene)

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(-40.0, 140.0, -33.0), 24.0, -2.0, scene)
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

grass = modelManager:GetModel("res/model/Grass/grassmodel.obj")
tree = modelManager:GetModel("res/model/Tree_02/billboardmodel.obj")
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

-- grass instances

grassEntity = ecs:CreateEntity()
ecs:AddModelInstancesComponent(grassEntity, grass, false, true, CreateGrassInstancesData())

-- tree instances

treeEntity = ecs:CreateEntity()
ecs:AddModelInstancesComponent(treeEntity, tree, false, true, CreateTreeInstancesData())

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
