------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)


-----------------------------
-- Create and add Renderer --
-----------------------------

ForwardRenderer.new(1, scene)
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

scene:SetAmbientIntensity(Vec3.new(1.4, 1.4, 1.4))


---------------------
-- Create Entities --
---------------------

plane = modelManager:GetModel("res/primitive/plane1/plane1.obj")
sphere = modelManager:GetModel("res/primitive/sphere/sphere.obj")
dome = modelManager:GetModel("res/model/Dome/dome.obj")
jade = modelManager:GetMaterialByName("jade")
gold = modelManager:GetMaterialByName("gold")

e0 = ecs:CreateEntity()
ecs:AddModelComponent(e0, plane, false)
ecs:AddTransformComponent(e0, Vec3.new(0.0, 150.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(1000.0, 1.0, 1000.0))

e1 = ecs:CreateEntity()
ecs:AddModelComponent(e1, sphere, false)
ecs:AddTransformComponent(e1, Vec3.new(0.0, 240.0, 380.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(4.0, 4.0, 4.0))
ecs:AddMaterialComponent(e1, gold)

e2 = ecs:CreateEntity()
ecs:AddModelComponent(e2, dome, false)
ecs:AddTransformComponent(e2, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(5000.0, 5000.0, 5000.0))
ecs:AddSkydomeComponent(e2)
