------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

-- forward with skybox

--[[
SkyboxRenderer.new(3, scene)
SunRenderer.new(2, scene)
ForwardRenderer.new(1, scene)
GuiRenderer.new(0, scene)
]]

-- deferred with skybox

--[[
DeferredRenderer.new(3, scene)
SkyboxRenderer.new(2, scene)
SunRenderer.new(1, scene)
GuiRenderer.new(0, scene)
]]

-- forward with skydome

SkydomeRenderer.new(3, scene)
SunRenderer.new(2, scene)
ForwardRenderer.new(1, scene)
GuiRenderer.new(0, scene)

-- deferred with skydome

--[[
DeferredRenderer.new(3, scene)
SkydomeRenderer.new(2, scene)
SunRenderer.new(1, scene)
GuiRenderer.new(0, scene)
]]

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(1334.0, 820.0, 227.0), -178.0, -22.0, scene)
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

e0 = ecs:CreateEntity()
ecs:AddModelComponent(e0, plane, false)
ecs:AddTransformComponent(e0, Vec3.new(0.0, 150.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 1.0, 5000.0))

-- sphere

e1 = ecs:CreateEntity()
ecs:AddModelComponent(e1, sphere, false)
ecs:AddTransformComponent(e1, Vec3.new(0.0, 240.0, 380.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(4.0, 4.0, 4.0))
ecs:AddMaterialComponent(e1, gold)

-- skydome

e2 = ecs:CreateEntity()
ecs:AddModelComponent(e2, dome, false)
ecs:AddTransformComponent(e2, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 5000.0, 5000.0))
ecs:AddSkydomeComponent(e2, "skydome")

-- a point light

e3 = ecs:CreateEntity()
ecs:AddPointLightComponent(e3,
    Vec3.new(30.0, 240.0, 657.0), -- position
    Vec3.new(0.2, 0.2, 0.2),      -- ambientIntensity
    Vec3.new(10.0, 1.0, 1.0),     -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),      -- specularIntensity
    1.0, 0.0014, 0.000007         -- constant, linear, quadratic
)
ecs:AddUpdateComponent(e3, "UpdatePointLight")

-- another point light

e4 = ecs:CreateEntity()
ecs:AddPointLightComponent(e4,
    Vec3.new(204.0, 240.0, -319.0), -- position
    Vec3.new(0.2, 0.2, 0.2),        -- ambientIntensity
    Vec3.new(1.0, 1.0, 10.0),       -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),        -- specularIntensity
    1.0, 0.0014, 0.000007           -- constant, linear, quadratic
)
ecs:AddUpdateComponent(e4, "UpdatePointLight")

-- the sun

--[[
e5 = ecs:CreateEntity()
ecs:AddDirectionalLightComponent(e5,
    Vec3.new(1.0, -0.2, -0.4), -- direction
    Vec3.new(1.0, 0.8, 0.6),   -- diffuseIntensity
    Vec3.new(1.0, 0.8, 0.6)    -- specularIntensity
)
]]

e6 = ecs:CreateEntity()
ecs:AddSunComponent(e6,
    Vec3.new(1.0, -0.2, -0.4), -- direction
    Vec3.new(1.0, 0.8, 0.6),   -- diffuseIntensity
    Vec3.new(1.0, 0.8, 0.6),   -- specularIntensity
    sunTextureId,
    10.0
)

-- skybox

e7 = ecs:CreateEntity()
ecs:AddCubemapComponent(e7, skyboxCubemapId)

-- gui

e8 = ecs:CreateEntity()
ecs:AddGuiComponent(e8, foodGuiId)
ecs:AddTransformComponent(e8, Vec3.new(0.9, 0.9, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.031, 0.031, 1.0))

-- another gui

e9 = ecs:CreateEntity()
ecs:AddGuiComponent(e9, healthGuiId)
ecs:AddTransformComponent(e9, Vec3.new(0.9, 0.8, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.031, 0.031, 1.0))

-- point light with model

e10 = ecs:CreateEntity()
ecs:AddModelComponent(e10, lamp, false)
ecs:AddTransformComponent(e10, Vec3.new(-1400.0, 150.0, 11.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(40.0, 40.0, 40.0))
ecs:AddPointLightComponent(e10,
    Vec3.new(-1400.0, 400.0, 11.0), -- position
    Vec3.new(0.2, 0.2, 0.2),        -- ambientIntensity
    Vec3.new(1.0, 1.0, 10.0),       -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),        -- specularIntensity
    1.0, 0.0014, 0.000007           -- constant, linear, quadratic
)

---------------
-- Functions --
---------------

val = 0.0

function UpdatePointLight(entity, dt)
    val = val + dt
    p = ecs:GetPointLightComponent(entity)
    p.position.x = p.position.x + (math.sin(val) * 4.0)
end
