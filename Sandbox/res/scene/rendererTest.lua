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
        cameraVelocity = 96.0,
        mouseSensitivity = 0.1,
        position = { x = 675.0, y = 800.0, z = 800.0 },
        yaw = -126.0,
        pitch = -24.0,
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
            scale = { x = 4000.0, y = 1.0, z = 4000.0 },
        },
        ModelComponent = {
            path = "res/primitive/plane1/plane1.obj",
            showTriangles = false,
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
        },
    },
    -- point lights
    pointLight1 = {
        PointLightComponent = {
            position = { x = -50.0, y = 4.0, z = 40.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 2.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.007,
            quadratic = 0.0002,
        },
    },
    pointLight2 = {
        PointLightComponent = {
            position = { x = 10.0, y = 4.0, z = 50.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 2.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.007,
            quadratic = 0.0002,
        },
    },
    modelWithPointLight = {
        TransformComponent = {
            position = { x = 6.0, y = 0.0, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 4.0, y = 4.0, z = 4.0 },
        },
        ModelComponent = {
            path = "res/model/Streetlamp/streetlamp.obj",
            showTriangles = false,
        },
        PointLightComponent = {
            position = { x = 4.0, y = 9.0, z = 0.0 },
            ambientIntensity = { x = 0.2, y = 0.2, z = 0.2 },
            diffuseIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
            constant = 1.0,
            linear = 0.007,
            quadratic = 0.0002,
        },
    },
    -- directional lights
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
            direction = { x = -0.5, y = -1.0, z = 0.0 },
            diffuseIntensity = { x = 0.4, y = 0.4, z = 0.4 },
            specularIntensity = { x = 1.0, y = 1.0, z = 1.0 },
        },
    },
    -- skybox/skydome, gui
    --[[
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
    ]]
    skydome = {
        TransformComponent = {
            position = { x = 0.0, y = 0.0, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
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
            position = { x = -0.5, y = 0.5, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 0.125, y = 0.125, z = 0.125 },
        },
    },
    gui2 = {
        GuiComponent = {
            guiTexturePath = "res/gui/healthIcon.png",
        },
        TransformComponent = {
            position = { x = 0.5, y = 0.5, z = 0.0 },
            rotation = { x = 0.0, y = 0.0, z = 0.0 },
            scale = { x = 0.125, y = 0.125, z = 0.125 },
        },
    },
}

--------------
-- Renderer --
--------------

renderer = {
    --ForwardRenderer = { priority = 0, name = "ForwardRenderSystem" },
    DeferredRenderer = { priority = 999, name = "DeferredRenderSystem" },
    --SkyboxRenderer = { priority = 10, name = "SkyboxRenderSystem" },
    SkydomeRenderer = { priority = 10, name = "SkydomeRenderSystem" },
    SunRenderer = { priority = 1, name = "SunRenderSystem" },
    GuiRenderer = { priority = 0, name = "GuiRenderSystem" }
}
