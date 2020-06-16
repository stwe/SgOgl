------------------
-- Create Scene --
------------------

scene = Scene.new(applicationContext)

-----------------------------
-- Create and add Renderer --
-----------------------------

ParticleSystemRenderer.new(1, scene)

----------------------------
-- Create and add Cameras --
----------------------------

firstPersonCamera = FirstPersonCamera.new("first_person_camera1", applicationContext, Vec3.new(0.0, -35.0, -90.0), 90.0, 20.0, scene)
firstPersonCamera:SetCameraVelocity(10.0)
firstPersonCamera:SetMouseSensitivity(0.0125)

------------------
-- Config Scene --
------------------

scene:SetCurrentCamera("first_person_camera1")

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

---------------
-- Functions --
---------------

function GenerateParticles(entity, dt)
    particleSystem:GenerateParticles(dt, Vec3.new(0.0, 0.0, 0.0))
    particleSystem:GenerateParticles(dt, Vec3.new(50.0, 0.0, 0.0))
end
