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
-- Config ParticleSystem --
---------------------------

particleSystem = ParticleSystem.new("quads", scene)

particleProperties = ParticleProperties.new()
particleProperties.position = Vec3.new(0.0, 0.0, 0.0)
particleProperties.velocity = Vec3.new(0.0, 0.0, 0.0)
particleProperties.velocityVariation = Vec3.new(3.0, 1.0, 1.0)
particleProperties.colorBegin = Vec4.new(254.0 / 255.0, 212.0 / 255.0, 123.0 / 255.0, 1.0)
particleProperties.colorEnd = Vec4.new(254.0 / 255.0, 109.0 / 255.0, 41.0 / 255.0, 1.0)
particleProperties.sizeBegin = 1.0
particleProperties.sizeEnd = 0.0
particleProperties.sizeVariation = 0.3
particleProperties.lifeTime = 1.0

---------------------
-- Create Entities --
---------------------

particleSystemEntity = ecs:CreateEntity()
ecs:AddParticleSystemComponent(particleSystemEntity, particleSystem)
ecs:AddInputComponent(particleSystemEntity, "PressP")

---------------
-- Functions --
---------------

function PressP(entity, mouseX, mouseY)
    particleProperties.position.x = 0.0
    particleProperties.position.y = 0.0
    particleProperties.position.x = (mouseX + particleProperties.position.x) * math.random(100)
    particleProperties.position.y = (mouseY + particleProperties.position.y) * math.random(100)
    particleSystem:Emit(particleProperties)
end
