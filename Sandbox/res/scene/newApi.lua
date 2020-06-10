------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

ForwardRenderer.new(1, scene)
SunRenderer.new(2, scene)
SkydomeRenderer.new(10, scene)

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
jade = modelManager:GetMaterialByName("jade")
gold = modelManager:GetMaterialByName("gold")
sunTextureId = textureManager:LoadTexture("res/sun/sun.png")

---------------------
-- Create Entities --
---------------------

e0 = ecs:CreateEntity()
ecs:AddModelComponent(e0, plane, false)
ecs:AddTransformComponent(e0, Vec3.new(0.0, 150.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 1.0, 5000.0))

e1 = ecs:CreateEntity()
ecs:AddModelComponent(e1, sphere, false)
ecs:AddTransformComponent(e1, Vec3.new(0.0, 240.0, 380.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(4.0, 4.0, 4.0))
ecs:AddMaterialComponent(e1, gold)

e2 = ecs:CreateEntity()
ecs:AddModelComponent(e2, dome, false)
ecs:AddTransformComponent(e2, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 5000.0, 5000.0))
ecs:AddSkydomeComponent(e2)

e3 = ecs:CreateEntity()
ecs:AddPointLightComponent(e3,
    Vec3.new(30.0, 240.0, 657.0), -- position
    Vec3.new(0.2, 0.2, 0.2),      -- ambientIntensity
    Vec3.new(10.0, 1.0, 1.0),     -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),      -- specularIntensity
    1.0, 0.0014, 0.000007         -- constant, linear, quadratic
)
ecs:AddUpdateComponent(e3, "UpdatePointLight")

e4 = ecs:CreateEntity()
ecs:AddPointLightComponent(e4,
    Vec3.new(204.0, 240.0, -319.0), -- position
    Vec3.new(0.2, 0.2, 0.2),        -- ambientIntensity
    Vec3.new(1.0, 1.0, 10.0),       -- diffuseIntensity
    Vec3.new(1.0, 1.0, 1.0),        -- specularIntensity
    1.0, 0.0014, 0.000007           -- constant, linear, quadratic
)
ecs:AddUpdateComponent(e4, "UpdatePointLight")

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

---------------
-- Functions --
---------------

val = 0.0

function UpdatePointLight(entity, dt)
    val = val + dt
    p = ecs:GetPointLightComponent(entity)
    p.position.x = p.position.x + (math.sin(val) * 4.0)
end
