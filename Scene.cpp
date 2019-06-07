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
{
  IPLint32 num_materials{ 0 };
  IPLMaterial* p_materials{ nullptr };
  if (materials == nullptr)
  {
    num_materials = default_materials.size();
    p_materials = default_materials.data();
  }
  else
  {
    num_materials = numMaterials;
    p_materials = materials;
  }

  IPLhandle scene{ nullptr };
  IPLerror error = iplCreateScene(context->GetHandle(),
    computeDevice,
    simulationSettings,
    num_materials,
    p_materials,
    closestCallback,
    anyHitCallback,
    batchedClosestCallback,
    batchedAnyHitCallback,
    userData,
    &scene);
  if (error)
  {
    throw std::exception("Error creating Scene: " + error);
  }
  m_scene = std::make_shared<IPLhandle>(scene);

}

Scene::~Scene()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (m_scene.unique())
  {
    iplDestroyScene(m_scene.get());
  }
}

IPLhandle Scene::GetHandle()
{
  return *m_scene;
}