#include "Scene.hpp"

Scene::Scene(std::shared_ptr<Context> context,
  IPLhandle computeDevice,
  IPLSimulationSettings simulationSettings,
  IPLint32 numMaterials,
  IPLMaterial* materials,
  IPLClosestHitCallback closestCallback,
  IPLAnyHitCallback anyHitCallback,
  IPLBatchedClosestHitCallback batchedClosestCallback,
  IPLBatchedAnyHitCallback batchedAnyHitCallback,
  IPLvoid* userData)
  : m_context(context), m_computeDevice(computeDevice), m_simulation_settings(simulationSettings),
  m_numMaterials(numMaterials), m_materials(materials), m_closestCallback(closestCallback),
  m_anyHitCallback(anyHitCallback), m_batchedClosestCallback(batchedClosestCallback), m_batchedAnyHitCallback(batchedAnyHitCallback),
  m_userData(userData)
{
  IPLint32 num_materials{ 0 };
  IPLMaterial* p_materials{ nullptr };
  if (m_materials == nullptr)
  {
    num_materials = default_materials.size();
    p_materials = default_materials.data();
  }
  else
  {
    num_materials = m_numMaterials;
    p_materials = m_materials;
  }

  IPLerror error = iplCreateScene(m_context->GetHandle(),
    m_computeDevice,
    m_simulation_settings,
    num_materials,
    p_materials,
    m_closestCallback,
    m_anyHitCallback,
    m_batchedClosestCallback,
    m_batchedAnyHitCallback,
    m_userData,
    &m_scene);
  if (error)
  {
    throw std::exception("Error creating Scene: " + error);
  }
}

Scene::~Scene()
{
  if (m_scene)
  {
    iplDestroyScene(&m_scene);
  }
}

IPLerror Scene::LoadScene(const std::vector<IPLbyte>& data, IPLLoadSceneProgressCallback callback)
{
  m_loadedScenes.push_back(data);
  if (m_scene)
  {
    return iplLoadScene(m_context->GetHandle(), m_simulation_settings, m_loadedScenes.back().data(), m_loadedScenes.back().size(), m_computeDevice, callback, &m_scene);
  }
  else
  {
    return IPLerror::IPL_STATUS_FAILURE;
  }
}

IPLhandle Scene::GetHandle()
{
  return m_scene;
}

Scene::Scene(Scene&& other) noexcept
{
  std::swap(m_scene, other.m_scene);
  std::swap(m_context, other.m_context);
  std::swap(m_computeDevice, other.m_computeDevice);
  std::swap(m_simulation_settings, other.m_simulation_settings);
  std::swap(m_numMaterials, other.m_numMaterials);
  std::swap(m_materials, other.m_materials);
  std::swap(m_closestCallback, other.m_closestCallback);
  std::swap(m_anyHitCallback, other.m_anyHitCallback);
  std::swap(m_batchedClosestCallback, other.m_batchedClosestCallback);
  std::swap(m_batchedAnyHitCallback, other.m_batchedAnyHitCallback);
  std::swap(m_userData, other.m_userData);
}

Scene& Scene::operator=(Scene&& other) noexcept
{
  std::swap(m_scene, other.m_scene);
  std::swap(m_context, other.m_context);
  std::swap(m_computeDevice, other.m_computeDevice);
  std::swap(m_simulation_settings, other.m_simulation_settings);
  std::swap(m_numMaterials, other.m_numMaterials);
  std::swap(m_materials, other.m_materials);
  std::swap(m_closestCallback, other.m_closestCallback);
  std::swap(m_anyHitCallback, other.m_anyHitCallback);
  std::swap(m_batchedClosestCallback, other.m_batchedClosestCallback);
  std::swap(m_batchedAnyHitCallback, other.m_batchedAnyHitCallback);
  std::swap(m_userData, other.m_userData);
  return *this;
}