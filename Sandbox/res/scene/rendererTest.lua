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
        cameraVelocity = 256.0,
        mouseSensitivity = 0.1,
        position = { x = 2633.0, y = 923.0, z = 328.0 },
        yaw = -178.0,
        pitch = -10.0,
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

local xWaterPos = 0.0
local zWaterPos = 0.0
local waterHeight = 0.0
local waterTileSize = 5000.0

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
    },
    rock = {
        TransformComponent = {
            position = { x = 0.0, y = 150.0, z = 380.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 40.0, y = 40.0, z = 40.0 },
        },
        ModelComponent = {
            path = "res/model/Rock/rock.obj",
            showTriangles = false,
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
            diffuseIntensity = { x = 1.0, y = 10.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.0014,
            quadratic = 0.000007,
        },
    },
    modelWithPointLight = {
        TransformComponent = {
            position = { x = 6.0, y = 150.0, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 40.0, y = 40.0, z = 40.0 },
        },
        ModelComponent = {
            path = "res/model/Streetlamp/streetlamp.obj",
            showTriangles = false,
        },
        PointLightComponent = {
            position = { x = -98.0, y = 400.0, z = 11.0 },
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
    directionalLight1 = {
        DirectionalLightComponent = {
            direction = { x = 0.5, y = -1.0, z = 0.0 },
            diffuseIntensity = { x = 0.7, y = 0.7, z = 0.7 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
        },
    },
    ]]
    sun = {
        SunComponent = {
            sunTexturePath = "res/sun/sun.png",
            scale = 10.0,
            direction = { x = 1.0, y = -0.2, z = -0.4 },
            diffuseIntensity = { x = 1.0, y = 0.8, z = 0.6 },
            specularIntensity = { x = 1.0, y = 0.8, z = 0.6 },
        },
    },
    ---------------------------
    -- skybox, skydome, guis --
    ---------------------------
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
    gui1 = {
        GuiComponent = {
            guiTexturePath = "res/gui/foodIcon.png",
        },
        TransformComponent = {
            position = { x = 0.9, y = 0.9, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 0.031, y = 0.031, z = 1.0 },
        },
    },
    gui2 = {
        GuiComponent = {
            guiTexturePath = "res/gui/healthIcon.png",
        },
        TransformComponent = {
            position = { x = 0.9, y = 0.8, z = 0.0 },
            --rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 0.031, y = 0.031, z = 1.0 },
        },
    },
    -----------
    -- water --
    -----------
    ocean = {
        WaterComponent = {
            xPosition = xWaterPos,
            zPosition = zWaterPos,
            height = waterHeight,
            tileSize = { x = waterTileSize, y = 1.0, z = waterTileSize },
            dudvTexturePath = "res/water/waterDUDV.png",
            normalMapTexturePath = "res/water/normal.png",
            renderToReflectionTexture = { [0] = "SkydomeRenderSystem", [1] = "SunRenderSystem", [2] = "ForwardRenderSystem" },
            renderToRefractionTexture = { [0] = "SkydomeRenderSystem", [1] = "SunRenderSystem", [2] = "ForwardRenderSystem" },
        },
        TransformComponent = {
            position = { x = xWaterPos, y = waterHeight, z = zWaterPos },
            scale = { x = waterTileSize, y = 1.0, z = waterTileSize },
        },
    },
}

--------------
-- Renderer --
--------------

renderer = {

    --DeferredRenderer = { priority = 99, name = "DeferredRenderSystem" },
    --SkyboxRenderer = { priority = 0, name = "SkyboxRenderSystem" },

    SkydomeRenderer = { priority = 10, name = "SkydomeRenderSystem" },
    WaterRenderer = { priority = 4, name = "WaterRenderSystem" },
    SunRenderer = { priority = 2, name = "SunRenderSystem" },
    ForwardRenderer = { priority = 1, name = "ForwardRenderSystem" },
    GuiRenderer = { priority = 0, name = "GuiRenderSystem" },
}
