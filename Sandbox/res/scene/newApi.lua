-----------
-- Scene --
-----------

SetAmbientIntensity(scene, vec3.new(0.4, 0.4, 0.4))

-------------
-- Cameras --
-------------

fpc = FirstPersonCamera.new("first_person_camera1", app, vec3.new(1334.0, 820.0, 227.0), -178.0, -22.0)
tpc = ThirdPersonCamera.new("third_person_camera1", app, vec3.new(1334.0, 820.0, 227.0))

SetFirstPersonCameraAsCurrent(scene, fpc)
--SetThirdPersonCameraAsCurrent(scene, tpc)

--------------
-- Entities --
--------------


--------------
-- Renderer --
--------------

