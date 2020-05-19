-----------
-- Scene --
-----------

scene = {
    ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
}

-------------
-- Cameras --
-------------

cameras = {
    firstCamera = {
        type = "first",
        cameraVelocity = 4.0,
        mouseSensitivity = 0.1,
        position = { x = 16.0, y = 6.0, z = 10.0 },
        yaw = -146.0,
        pitch = -0.2,
        current = true,
    },
    secondCamera = {
        type = "third",
        cameraVelocity = 4.0,
        position = {},
        playerPosition = { x = 0.0, y = 0.0, z = 0.0 },
        playerRotationY = {},
        yaw = 20.0,
        pitch = 0.0,
        current = false,
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
            scale = { x = 10.0, y = 1.0, z = 10.0 },
        },
        ModelComponent = {
            path = "res/primitive/plane1/plane1.obj",
            showTriangles = false,
            fakeNormals = false,
        },
    },
    rock = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 1.0, y = 1.0, z = 1.0 },
        },
        ModelComponent = {
            path = "res/model/Rock/rock.obj",
            showTriangles = false,
            fakeNormals = false,
        },
    },
    -- point lights
    pointLight1 = {
        PointLightComponent = {
            position = { x = -3.0, y = 0.3, z = 0.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.22,
            quadratic = 0.2,
        },
    },
    pointLight2 = {
        PointLightComponent = {
            position = { x = 3.0, y = 0.3, z = 0.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.22,
            quadratic = 0.2,
        },
    },
    modelWithPointLight = {
        TransformComponent = {
            position = { x = 6.0, y = 0.0, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 1.0, y = 1.0, z = 1.0 },
        },
        ModelComponent = {
            path = "res/model/Streetlamp/streetlamp.obj",
            showTriangles = false,
            fakeNormals = false,
        },
        PointLightComponent = {
            position = { x = 4.0, y = 9.0, z = 0.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.022,
            quadratic = 0.0019,
        },
    },
    -- directional lights
    directionalLight1 = {
        DirectionalLightComponent = {
            direction = { x = 0.5, y = -1.0, z = 0.0 },
            diffuseIntensity = { x = 1.0, y = 10.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
        },
    },
    sun = {
        SunComponent = {
            sunTexturePath = "res/sun/sun.png",
            scale = 10.0,
            direction = { x = -0.5, y = -1.0, z = 0.0 },
            diffuseIntensity = { x = 10.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
        },
    },
    -- other
    skybox = {
        CubemapComponent = {
            right = "res/skybox/sky1/sRight.png",
            left = "res/skybox/sky1/sLeft.png",
            up = "res/skybox/sky1/sUp.png",
            down = "res/skybox/sky1/sDown.png",
            back = "res/skybox/sky1/sBack.png",
            front = "res/skybox/sky1/sFront.png",
        },
    },
}

--------------
-- Renderer --
--------------

renderer = {
    ForwardRenderer = "ForwardRenderSystem",
    SkyboxRenderer = "SkyboxRenderSystem",
    SunRenderer = "SunRenderSystem",
}
