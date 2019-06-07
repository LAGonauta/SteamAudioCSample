#pragma once

#include <alure2.h>
#include <memory>

#include "common.h"
#include "BinauralRenderer.hpp"
#include "EnvironmentalRenderer.hpp"
#include "Structs.hpp"
#include "SteamAudioDecoder.hpp"
#include "Environment.hpp"

class AudioSource
{
private:
  std::shared_ptr<BinauralRenderer> m_renderer{ nullptr };
  std::shared_ptr<EnvironmentalRenderer> m_env_renderer{ nullptr };
  std::shared_ptr<Environment> m_env{ nullptr };
  alure::Context m_al_context;
  alure::Source m_al_source;
  std::shared_ptr<SteamAudioDecoder> m_decoder;
  float GetMean(const float* data, const size_t dataSize);

  IPLSource m_source{ };
  IPLVector3 m_listenerPosition{ };
  IPLVector3 m_listenerAhead{ };
  IPLVector3 m_listenerUp{ };
  IPLfloat32 m_radius = 1;
  IPLDirectOcclusionMode m_occlusionMode = IPL_DIRECTOCCLUSION_NONE;
  IPLDirectOcclusionMethod m_occlusionMethod = IPL_DIRECTOCCLUSION_RAYCAST;

public:
  AudioSource(std::shared_ptr<BinauralRenderer> renderer, std::shared_ptr<EnvironmentalRenderer> envRenderer, std::shared_ptr<Environment> env, alure::Context al_context);
  ~AudioSource();

  void Play(std::string filename);

  void Stop();

  bool isPlaying();

  void SetPosition(const IPLSource& source);
  void SetListener(const IPLVector3& listenerPosition, const IPLVector3& listenerAhead, const IPLVector3& listenerUp);
  void SetRadius(IPLfloat32 radius);
  IPLfloat32 GetRadius();
  void SetOcclusionMode(IPLDirectOcclusionMode mode);
  IPLDirectOcclusionMode GetOcclusionMode();
  void SetOcclusionMethod(IPLDirectOcclusionMethod mode);
  IPLDirectOcclusionMethod GetOcclusionMethod();
};