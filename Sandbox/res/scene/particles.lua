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

particleRoot = ParticleRoot.new()
particleRoot.position = Vec3.new(0.0, 0.0, 0.0)
particleRoot.velocity = Vec3.new(0.0, 30.0, 0.0)
particleRoot.gravityEffect = 1.0
particleRoot.lifeTime = 4.0
particleRoot.rotation = 45.0
particleRoot.scale = 4.0

---------------------
-- Create Entities --
---------------------

particleSystemEntity = ecs:CreateEntity()
ecs:AddParticleSystemComponent(particleSystemEntity, particleSystem)
ecs:AddInputComponent(particleSystemEntity, "LeftMouseButtonPressed")

---------------
-- Functions --
---------------

function LeftMouseButtonPressed(entity, mouseX, mouseY)
    print("Emit Particle")
    particleSystem:Emit(particleRoot)
end
