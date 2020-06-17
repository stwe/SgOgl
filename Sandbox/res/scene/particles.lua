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
firstPersonCamera:SetCameraVelocity(128.0)
firstPersonCamera:SetMouseSensitivity(0.025)

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

textureId = textureManager:LoadTexture("res/particle/particleStar.png")

--[[
particleSystem = ParticleSystem.new("quads", textureId, scene)
particleSystem:SetParticlesPerSecond(100.0)
particleSystem:SetSpeed(5.0)
particleSystem:SetGravityEffect(1.0)
particleSystem:SetLifeTime(3.0)
particleSystem:SetMaxScale(1.0)
]]

particleSystem = ParticleSystem.new("quads", textureId, 100.0, 5.0, 1.0, 3.0, 1.0, scene)

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
