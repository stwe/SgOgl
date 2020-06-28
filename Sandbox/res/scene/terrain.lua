------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

SkyboxRenderer.new(3, scene)
SunRenderer.new(2, scene)
ForwardRenderer.new(1, scene)
InstancingRenderer.new(0, scene)

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(-65.0, 44.0, -18.0), 20.0, -16.0, scene)
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

plane = modelManager:GetModel("res/primitive/plane1/plane1.obj")
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

---------------------
-- Create Entities --
---------------------

-- plane

planeEntity = ecs:CreateEntity()
ecs:AddModelComponent(planeEntity, plane, false)
ecs:AddTransformComponent(planeEntity, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(250.0, 1.0, 250.0))

-- plants

PlantInstances = {}
for i = 0, 9, 1 do
    t = Transform.new()

    t.position = Vec3.new(0.0 + i * 4, 10.0, 0.0)
    t.rotation = Vec3.new(180.0, 0.0, 0.0)
    t.scale = Vec3.new(10.0)

    PlantInstances[i] = t
end

plantEntity = ecs:CreateEntity()
ecs:AddModelInstancesComponent(plantEntity, plant, false, true, PlantInstances)

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

---------------
-- Functions --
---------------

val = 0.0

function UpdatePointLight(entity, dt)
    val = val + dt
    p = ecs:GetPointLightComponent(entity)
    p.position.x = p.position.x + (math.sin(val) * 4.0)
end
