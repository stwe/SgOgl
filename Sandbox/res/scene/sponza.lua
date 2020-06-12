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
]]

-- deferred with skybox

--[[
DeferredRenderer.new(3, scene)
SkyboxRenderer.new(2, scene)
SunRenderer.new(1, scene)
]]

-- forward with skydome

SkydomeRenderer.new(3, scene)
SunRenderer.new(2, scene)
ForwardRenderer.new(1, scene)

-- deferred with skydome

--[[
DeferredRenderer.new(3, scene)
SkydomeRenderer.new(2, scene)
SunRenderer.new(1, scene)
]]

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(1197.0, 171.0, -23.0), -179.0, 1.0, scene)
firstPersonCamera:SetCameraVelocity(200.0)
firstPersonCamera:SetMouseSensitivity(0.1)

------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")
scene:SetAmbientIntensity(Vec3.new(0.2, 0.2, 0.2))

--------------------
-- Load resources --
--------------------

sponza = modelManager:GetModel("res/model/Sponza/sponza.obj")
dome = modelManager:GetModel("res/model/Dome/dome.obj")
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

-- sponza

sponzaEntity = ecs:CreateEntity()
ecs:AddModelComponent(sponzaEntity, sponza, false)
ecs:AddTransformComponent(sponzaEntity, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(1.0, 1.0, 1.0))

-- sun

sunEntity = ecs:CreateEntity()
ecs:AddSunComponent(sunEntity,
    Vec3.new(1.0, -0.2, -0.4), -- direction
    Vec3.new(1.0, 0.8, 0.6),   -- diffuseIntensity
    Vec3.new(0.6, 0.6, 0.6),   -- specularIntensity
    sunTextureId,
    10.0
)

-- point lights

p1 = ecs:CreateEntity()
ecs:AddPointLightComponent(p1,
    Vec3.new(-945.0, 20.0, -89.0), -- position
    Vec3.new(0.2, 0.2, 0.2),       -- ambientIntensity
    Vec3.new(10.0, 1.0, 1.0),      -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),       -- specularIntensity
    1.0, 0.007, 0.002              -- constant, linear, quadratic
)

p2 = ecs:CreateEntity()
ecs:AddPointLightComponent(p2,
    Vec3.new(-64.0, 20.0, -89.0), -- position
    Vec3.new(0.2, 0.2, 0.2),      -- ambientIntensity
    Vec3.new(1.0, 1.0, 10.0),     -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),      -- specularIntensity
    1.0, 0.007, 0.002             -- constant, linear, quadratic
)

-- skybox

skyboxEntity = ecs:CreateEntity()
ecs:AddCubemapComponent(skyboxEntity, skyboxCubemapId)

-- skydome

skydomeEntity = ecs:CreateEntity()
ecs:AddModelComponent(skydomeEntity, dome, false)
ecs:AddTransformComponent(skydomeEntity, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 5000.0, 5000.0))
ecs:AddSkydomeComponent(skydomeEntity)
