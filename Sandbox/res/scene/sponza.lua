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
        cameraVelocity = 200.0,
        mouseSensitivity = 0.1,
        position = { x = 1197.0, y = 171.0, z = -23.0 },
        yaw = -179.0,
        pitch = 1.0,
        current = true,
    },
}

--------------
-- Entities --
--------------

entities = {

    ------------
    -- models --
    ------------
    sphere = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 1.0, y = 1.0, z = 1.0 },
        },
        ModelComponent = {
            path = "res/model/Sponza/sponza.obj",
            showTriangles = false,
        },
    },

    ------------------
    -- point lights --
    ------------------
    pointLight1 = {
        PointLightComponent = {
            position = { x = -945.0, y = 20.0, z = -89.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 10.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.007,
            quadratic = 0.002,
        },
    },
    pointLight2 = {
        PointLightComponent = {
            position = { x = -64.0, y = 20.0, z = -89.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 10.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.007,
            quadratic = 0.002,
        },
    },

    ------------------------
    -- directional lights --
    ------------------------
    sun = {
        SunComponent = {
            sunTexturePath = "res/sun/sun.png",
            scale = 10.0,
            direction = { x = 1.0, y = -0.2, z = -0.4 },
            diffuseIntensity = { x = 1.0, y = 0.8, z = 0.6 },
            specularIntensity = { x = 0.6, y = 0.6, z = 0.6 },
        },
    },

    ---------------------------
    -- skybox, skydome, guis --
    ---------------------------
    skydome = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 5000.0, y = 5000.0, z = 5000.0 },
        },
        ModelComponent = {
            path = "res/model/Dome/dome.obj",
            showTriangles = false,
        },
        SkydomeComponent = {},
    },
}

--------------
-- Renderer --
--------------

renderer = {

    DeferredRenderer = { priority = 99, name = "DeferredRenderSystem" },
    --SkyboxRenderer = { priority = 0, name = "SkyboxRenderSystem" },

    SkydomeRenderer = { priority = 10, name = "SkydomeRenderSystem" },
    SunRenderer = { priority = 2, name = "SunRenderSystem" },
    --ForwardRenderer = { priority = 1, name = "ForwardRenderSystem" },
}
