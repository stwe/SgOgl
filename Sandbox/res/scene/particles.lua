------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

ForwardRenderer.new(1, scene)
ParticleSystemRenderer.new(0, scene)

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(6.0, 20.0, -27.0), 102.0, -16.0, scene)
firstPersonCamera:SetCameraVelocity(48.0)
firstPersonCamera:SetMouseSensitivity(0.1)

------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")

--------------------
-- Load resources --
--------------------

plane = modelManager:GetModel("res/primitive/plane1/plane1.obj")

---------------------------
-- Create ParticleSystem --
---------------------------

textureId = textureManager:LoadTexture("res/particle/fire.png")

particleSystem = ParticleSystem.new("fire", textureId, 8, scene)
particleSystem:SetParticlesPerSecond(24.0)
particleSystem:SetSpeed(1.0)
particleSystem:SetGravityEffect(-0.1)
particleSystem:SetLifeTime(2.0)
particleSystem:SetMaxScale(10.0)

--particleSystem = ParticleSystem.new("quads", textureId, 8, 8.0, 1.0, -0.1, 6.0, 8.0, scene)

---------------------
-- Create Entities --
---------------------

particleSystemEntity = ecs:CreateEntity()
ecs:AddParticleSystemComponent(particleSystemEntity, particleSystem)
ecs:AddUpdateComponent(particleSystemEntity, "GenerateParticles")

planeEntity = ecs:CreateEntity()
ecs:AddModelComponent(planeEntity, plane, false)
ecs:AddTransformComponent(planeEntity, Vec3.new(0.0, 0.0, 0.0), Vec3.new(0.0, 0.0, 0.0), Vec3.new(250.0, 1.0, 250.0))

---------------
-- Functions --
---------------

function GenerateParticles(entity, dt)
    particleSystem:GenerateParticles(dt, Vec3.new(0.0, 20.0, 0.0))
end
