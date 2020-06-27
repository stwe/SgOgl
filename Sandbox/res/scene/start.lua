------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

-- forward with skybox

SkyboxRenderer.new(4, scene)
SunRenderer.new(3, scene)
ForwardRenderer.new(2, scene)
SkeletalModelRenderer.new(1, scene)
GuiRenderer.new(0, scene)
TextRenderer.new(0, scene, "res/font/calibri.ttf")

-- deferred with skybox

--[[
DeferredRenderer.new(3, scene)
SkyboxRenderer.new(2, scene)
SunRenderer.new(1, scene)
GuiRenderer.new(0, scene)
]]

-- forward with skydome

--[[
SkydomeRenderer.new(4, scene)
SunRenderer.new(3, scene)
ForwardRenderer.new(2, scene)
SkeletalModelRenderer.new(1, scene)
GuiRenderer.new(0, scene)
]]

-- deferred with skydome

--[[
DeferredRenderer.new(3, scene)
SkydomeRenderer.new(2, scene)
SunRenderer.new(1, scene)
GuiRenderer.new(0, scene)
TextRenderer.new(0, scene, "res/font/calibri.ttf")
]]

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(-375.0, 313.0, -417.0), -147.0, -16.0, scene)
firstPersonCamera:SetCameraVelocity(128.0)
firstPersonCamera:SetMouseSensitivity(0.025)
--print(firstPersonCamera:GetCameraVelocity())
--print(firstPersonCamera:GetMouseSensitivity())

thirdPersonCamera = ThirdPersonCamera.new("third_person_camera1", applicationContext, Vec3.new(1334.0, 820.0, 227.0), scene)
thirdPersonCamera:SetPlayerRotationY(45.0)
--print(thirdPersonCamera:GetPlayerRotationY())

------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")
--scene:SetCurrentCamera("third_person_camera1")

scene:SetAmbientIntensity(Vec3.new(0.2, 0.2, 0.2))

--------------------
-- Load resources --
--------------------

plane = modelManager:GetModel("res/primitive/plane1/plane1.obj")
sphere = modelManager:GetModel("res/primitive/sphere/sphere.obj")
dome = modelManager:GetModel("res/model/Dome/dome.obj")
lamp = modelManager:GetModel("res/model/Streetlamp/streetlamp.obj")
jade = modelManager:GetMaterialByName("jade")
gold = modelManager:GetMaterialByName("gold")
sunTextureId = textureManager:LoadTexture("res/sun/sun.png")
foodGuiId = textureManager:LoadTexture("res/gui/foodIcon.png")
healthGuiId = textureManager:LoadTexture("res/gui/healthIcon.png")
hero = modelManager:GetSkeletalModel("res/model/CastleGuard01/Idle.dae")

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
ecs:AddTransformComponent(planeEntity, Vec3.new(0.0, 150.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 1.0, 5000.0))

-- hero

heroEntity = ecs:CreateEntity()
ecs:AddSkeletalModelComponent(heroEntity, hero, false)
ecs:AddTransformComponent(heroEntity, Vec3.new(-500.0, 150.0, -500.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(1.0, 1.0, 1.0))

-- sphere

sphereEntity = ecs:CreateEntity()
ecs:AddModelComponent(sphereEntity, sphere, false)
ecs:AddTransformComponent(sphereEntity, Vec3.new(0.0, 240.0, 380.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(4.0, 4.0, 4.0))
ecs:AddMaterialComponent(sphereEntity, gold)

-- skydome

skydomeEntity = ecs:CreateEntity()
ecs:AddModelComponent(skydomeEntity, dome, false)
ecs:AddTransformComponent(skydomeEntity, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 5000.0, 5000.0))
ecs:AddSkydomeComponent(skydomeEntity, "skydome")

-- a point light

plightEntity0 = ecs:CreateEntity()
ecs:AddPointLightComponent(plightEntity0,
    Vec3.new(30.0, 240.0, 657.0), -- position
    Vec3.new(0.2, 0.2, 0.2),      -- ambientIntensity
    Vec3.new(10.0, 1.0, 1.0),     -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),      -- specularIntensity
    1.0, 0.0014, 0.000007         -- constant, linear, quadratic
)
ecs:AddUpdateComponent(plightEntity0, "UpdatePointLight")

-- another point light

plightEntity1 = ecs:CreateEntity()
ecs:AddPointLightComponent(plightEntity1,
    Vec3.new(204.0, 240.0, -319.0), -- position
    Vec3.new(0.2, 0.2, 0.2),        -- ambientIntensity
    Vec3.new(1.0, 1.0, 10.0),       -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),        -- specularIntensity
    1.0, 0.0014, 0.000007           -- constant, linear, quadratic
)
ecs:AddUpdateComponent(plightEntity1, "UpdatePointLight")

-- the sun

--[[
dlightEntity = ecs:CreateEntity()
ecs:AddDirectionalLightComponent(dlightEntity,
    Vec3.new(1.0, -0.2, -0.4), -- direction
    Vec3.new(1.0, 0.8, 0.6),   -- diffuseIntensity
    Vec3.new(1.0, 0.8, 0.6)    -- specularIntensity
)
]]

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

-- gui

foodEntity = ecs:CreateEntity()
ecs:AddGuiComponent(foodEntity, foodGuiId)
ecs:AddTransformComponent(foodEntity, Vec3.new(0.9, 0.9, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.031, 0.031, 1.0))

-- another gui

healthEntity = ecs:CreateEntity()
ecs:AddGuiComponent(healthEntity, healthGuiId)
ecs:AddTransformComponent(healthEntity, Vec3.new(0.9, 0.8, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.031, 0.031, 1.0))

-- point light with model

plightModelEntity = ecs:CreateEntity()
ecs:AddModelComponent(plightModelEntity, lamp, false)
ecs:AddTransformComponent(plightModelEntity, Vec3.new(-1400.0, 150.0, 11.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(40.0, 40.0, 40.0))
ecs:AddPointLightComponent(plightModelEntity,
    Vec3.new(-1400.0, 400.0, 11.0), -- position
    Vec3.new(0.2, 0.2, 0.2),        -- ambientIntensity
    Vec3.new(1.0, 1.0, 10.0),       -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),        -- specularIntensity
    1.0, 0.0014, 0.000007           -- constant, linear, quadratic
)

-- text

textEntity0 = ecs:CreateEntity()
ecs:AddTextComponent(textEntity0, "SgOgl Test", 40.0, 750.0, 0.35, Vec3.new(0.1, 0.2, 0.2))

textEntity1 = ecs:CreateEntity()
ecs:AddTextComponent(textEntity1, "Version: dev-master", 40.0, 730.0, 0.35, Vec3.new(0.1, 0.2, 0.2))

---------------
-- Functions --
---------------

val = 0.0

function UpdatePointLight(entity, dt)
    val = val + dt
    p = ecs:GetPointLightComponent(entity)
    p.position.x = p.position.x + (math.sin(val) * 4.0)
end
