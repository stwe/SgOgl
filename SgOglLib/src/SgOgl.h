// This file is part of the SgOgl package.
// 
// Filename: SgOgl.h
// Author:   stwe
// 
// License:  MIT
// 
// 2019 (c) stwe <https://github.com/stwe/SgOgl>

#pragma once

// core
#include "SgOglLib/Application.h"
#include "SgOglLib/OpenGl.h"
#include "SgOglLib/Color.h"
#include "SgOglLib/Log.h"
#include "SgOglLib/Window.h"

// buffer
#include "SgOglLib/buffer/BufferLayout.h"
#include "SgOglLib/buffer/Fbo.h"
#include "SgOglLib/buffer/Vao.h"
#include "SgOglLib/buffer/Vbo.h"
#include "SgOglLib/buffer/WaterFbos.h"

// camera
#include "SgOglLib/camera/FirstPersonCamera.h"
#include "SgOglLib/camera/ThirdPersonCamera.h"

// ecs
#include "SgOglLib/ecs/component/Components.h"
#include "SgOglLib/ecs/factory/EntityFactory.h"
#include "SgOglLib/ecs/system/GuiRenderSystem.h"
#include "SgOglLib/ecs/system/InstancingRenderSystem.h"
#include "SgOglLib/ecs/system/ModelRenderSystem.h"
#include "SgOglLib/ecs/system/ParticleRenderSystem.h"
#include "SgOglLib/ecs/system/RenderSystem.h"
#include "SgOglLib/ecs/system/SkeletalModelRenderSystem.h"
#include "SgOglLib/ecs/system/SkyboxRenderSystem.h"
#include "SgOglLib/ecs/system/SkydomeRenderSystem.h"
#include "SgOglLib/ecs/system/TerrainRenderSystem.h"
#include "SgOglLib/ecs/system/WaterRenderSystem.h"

// imgui
#include "SgOglLib/imgui/imgui_impl_glfw.h"
#include "SgOglLib/imgui/imgui_impl_opengl3.h"

// input
#include "SgOglLib/input/MouseInput.h"
#include "SgOglLib/input/MousePicker.h"

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
#include "SgOglLib/resource/SkeletalModel.h"
#include "SgOglLib/resource/TextureManager.h"
#include "SgOglLib/resource/shaderprogram/ComputeNormalmap.h"
#include "SgOglLib/resource/shaderprogram/ComputeSplatmap.h"
#include "SgOglLib/resource/shaderprogram/DomeShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/GuiShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/InstancingShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/ModelNormalmapShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/ModelShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/ParticleShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/SkeletalModelShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/SkyboxShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/TerrainShaderProgram.h"
#include "SgOglLib/resource/shaderprogram/WaterShaderProgram.h"

// scene
#include "SgOglLib/scene/Scene.h"

// state
#include "SgOglLib/state/State.h"
#include "SgOglLib/state/StateStack.h"

// terrain
#include "SgOglLib/terrain/Terrain.h"

// water
#include "SgOglLib/water/Water.h"
