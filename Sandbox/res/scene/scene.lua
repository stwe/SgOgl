

-------------
-- Cameras --
-------------

cameras = {
    firstCamera = {
        type = "first",
        cameraVelocity = 4.0,
        mouseSensitivity = 0.1,
        position = { x = 5.0, y = 3.0, z = 3.0 },
        yaw = -152.0,
        pitch = -16.0,
        current = true
    },
    secondCamera = {
        type = "third",
        cameraVelocity = 4.0,
        position = {},
        playerPosition = { x = 0.0, y = 0.0, z = 0.0 },
        playerRotationY = {},
        yaw = 20.0,
        pitch = 0.0,
        current = false
    },
}


--------------
-- Entities --
--------------

entities = {
    playground = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 3.0, y = 3.0, z = 3.0 },
        },
        ModelComponent = {
            path = "res/primitive/plane1/plane1.obj",
            showTriangles = false,
            fakeNormals = false,
        }
    },
    sphere = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 1.0, y = 1.0, z = 1.0 },
        },
        ModelComponent = {
            path = "res/model/Rock/rock.obj",
            showTriangles = false,
            fakeNormals = false,
        }
    }
}


--------------
-- Renderer --
--------------

renderer = {
    ForwardRenderer = "ForwardRenderSystem",
    SkyboxRenderer = "SkyboxRenderSystem"
}
