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
        cameraVelocity = 128.0,
        mouseSensitivity = 0.025,
        position = { x = 1334.0, y = 820.0, z = 227.0 },
        yaw = -178.0,
        pitch = -22.0,
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
    playground = {
        TransformComponent = {
            position = { x = 0.0, y = 150.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 1000.0, y = 1.0, z = 1000.0 },
        },
        ModelComponent = {
            path = "res/primitive/plane1/plane1.obj",
            showTriangles = false,
        },
        -- emerald, jade, obsidian, pearl, gold, silver, bronze
        -- cyan_plastic, red_rubber, chrome
        MaterialComponent = {
            name = "jade",
        },
    },
    sphere = {
        TransformComponent = {
            position = { x = 0.0, y = 200.0, z = 380.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 40.0, y = 40.0, z = 40.0 },
        },
        ModelComponent = {
            path = "res/model/Sphere/sphere.obj",
            showTriangles = false,
        },
        -- emerald, jade, obsidian, pearl, gold, silver, bronze
        -- cyan_plastic, red_rubber, chrome
        MaterialComponent = {
            name = "bronze",
        },
    },

    ------------------
    -- point lights --
    ------------------
    pointLight1 = {
        PointLightComponent = {
            position = { x = 30.0, y = 240.0, z = 657.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 10.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.0014,
            quadratic = 0.000007,
        },
    },
    pointLight2 = {
        PointLightComponent = {
            position = { x = 204.0, y = 240.0, z = -319.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 10.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.0014,
            quadratic = 0.000007,
        },
    },

    ------------------------
    -- directional lights --
    ------------------------
    --[[
    sun = {
        SunComponent = {
            sunTexturePath = "res/sun/sun.png",
            scale = 10.0,
            direction = { x = 1.0, y = -0.2, z = -0.4 },
            diffuseIntensity = { x = 1.0, y = 0.8, z = 0.6 },
            specularIntensity = { x = 1.0, y = 0.8, z = 0.6 },
        },
    },
    ]]

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
