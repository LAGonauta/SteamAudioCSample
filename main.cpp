#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include <variant>

#include <alure2.h>
#include <phonon.h>
#include "Externals/soxr-code/src/soxr.h"

#include "Context.hpp"
#include "BinauralRenderer.hpp"
#include "SteamAudioDecoder.hpp"
#include "Helpers.hpp"
#include "Structs.hpp"
#include "Scene.hpp"
#include "AudioSource.hpp"
#include "EnvironmentalRenderer.hpp"

std::unordered_map<alure::String, alure::Vector<float>> audioCache;
constexpr int selectedRate = 48000;
constexpr int selectedFramesize = 1024;
constexpr int selectedNumBuffersPerSource = 4;

int main()
{
  float pos_y = 1.0f;
  // Init OpenAL
  auto al_dev_manager = alure::DeviceManager::getInstance();
  auto al_device = al_dev_manager.openPlayback("OpenAL Soft");
  //auto al_device = al_dev_manager.openPlayback();
  auto al_context = al_device.createContext();
  alure::Context::MakeCurrent(al_context);

  // Create SteamAudio decoder
  auto context = std::make_shared<Context>();
  {
    auto const samplingrate = selectedRate;
    auto const framesize = selectedFramesize;
    IPLRenderingSettings settings{ samplingrate, framesize, IPLConvolutionType::IPL_CONVOLUTIONTYPE_PHONON };
    IPLHrtfParams hrtfParams{ IPLHrtfDatabaseType::IPL_HRTFDATABASETYPE_DEFAULT, nullptr, 0 };

    auto renderer = std::make_shared<BinauralRenderer>(context, settings, hrtfParams);
    IPLSimulationSettings simulationSettings{};
    simulationSettings.ambisonicsOrder = 1;
    simulationSettings.bakingBatchSize = 1;
    simulationSettings.irDuration = 0.5;
    simulationSettings.irradianceMinDistance = 0.1f;
    simulationSettings.maxConvolutionSources = 1;
    simulationSettings.numBounces = 1;
    simulationSettings.numDiffuseSamples = 1;
    simulationSettings.numOcclusionSamples = 1;
    simulationSettings.numRays = 1;
    simulationSettings.numThreads = 1;
    simulationSettings.sceneType = IPLSceneType::IPL_SCENETYPE_PHONON;

    auto scene = std::make_shared<Scene>(context, IPLhandle{ NULL }, simulationSettings, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    auto environment = std::make_shared<Environment>(context, IPLhandle{ NULL }, simulationSettings, scene, IPLhandle{ NULL });

    IPLAudioFormat stereo;
    stereo.channelLayoutType = IPLChannelLayoutType::IPL_CHANNELLAYOUTTYPE_SPEAKERS;
    stereo.numSpeakers = 2;
    stereo.channelLayout = IPLChannelLayout::IPL_CHANNELLAYOUT_STEREO;
    stereo.channelOrder = IPLChannelOrder::IPL_CHANNELORDER_INTERLEAVED;

    auto envRenderer = std::make_shared<EnvironmentalRenderer>(context, environment, settings, stereo);

    // Link decoder to source
    {
      auto audioSource = AudioSource(renderer, envRenderer, environment, al_context);
      std::cout << "Write a number and press enter to start." << std::endl;
      std::cin >> pos_y;
      audioSource.Play("erro.mp3");
      //audioSource.Play("piec.wav");

      auto clock = std::chrono::steady_clock();
      auto now = clock.now();
      float distance = 1;
      size_t m = 0;
      while (audioSource.isPlaying())
      {
        IPLSource source{};
        source.position = rotate_clockwise_over_zero(distance, std::chrono::duration_cast<std::chrono::milliseconds>(now - clock.now()), 1);
        source.position = IPLVector3{ source.position.x, source.position.y, source.position.z };
        audioSource.SetPosition(source);
        audioSource.SetListener(IPLVector3{ 0.0f, 0.0f, 0.0f }, IPLVector3{ 0.0f, 0.0f, -1.0f }, IPLVector3{ 0.0f, 1.0f, 0.0f });
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        if (m == 1000)
        {
          audioSource.Stop();
        }
        al_context.update();
        distance += 0.05f;
        ++m;
      }
    }

    alure::Context::MakeCurrent(nullptr);
    al_context.destroy();
    al_device.close();
  }
  context->Destroy();
  context->Cleanup();

  return 0;
}