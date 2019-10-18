#pragma once

// core
#include "SgOglLib/Application.h"
#include "SgOglLib/Window.h"
#include "SgOglLib/Log.h"
#include "SgOglLib/Color.h"
#include "SgOglLib/OpenGl.h"

// buffer
#include "SgOglLib/buffer/Vao.h"
#include "SgOglLib/buffer/Fbo.h"
#include "SgOglLib/buffer/BufferLayout.h"

// camera
#include "SgOglLib/camera/LookAtCamera.h"

// input
#include "SgOglLib/input/MouseInput.h"

// light
#include "SgOglLib/light/DirectionalLight.h"
#include "SgOglLib/light/PointLight.h"

// math
#include "SgOglLib/math/Transform.h"

// particle
#include "SgOglLib/particle/Particle.h"
#include "SgOglLib/particle/ParticleEmitter.h"

// resource
#include "SgOglLib/resource/Material.h"
#include "SgOglLib/resource/Mesh.h"
#include "SgOglLib/resource/Model.h"
#include "SgOglLib/resource/ModelManager.h"
#include "SgOglLib/resource/ShaderManager.h"
#include "SgOglLib/resource/ShaderProgram.h"
#include "SgOglLib/resource/TextureManager.h"

// scene
#include "SgOglLib/scene/Component.h"
#include "SgOglLib/scene/Entity.h"
#include "SgOglLib/scene/Node.h"
#include "SgOglLib/scene/RenderConfig.h"
#include "SgOglLib/scene/Scene.h"
#include "SgOglLib/scene/SceneLoader.h"
#include "SgOglLib/scene/TerrainComponent.h"

// scene - components
#include "SgOglLib/scene/component/Components.h"

// state
#include "SgOglLib/state/State.h"
#include "SgOglLib/state/StateStack.h"

// terrain
#include "SgOglLib/terrain/Terrain.h"
