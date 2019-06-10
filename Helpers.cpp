#pragma once

#include "common.h"
#include "Helpers.hpp"
#include "Structs.hpp"
#include "Externals/soxr-code/src/soxr.h"

static auto runtimeSpec = soxr_runtime_spec(0);
static auto qualityResampleQuality = soxr_quality_spec(SOXR_VHQ, 0);

std::shared_ptr<AudioBuffer> get_audio(std::string filename, alure::Context al_context, size_t finalSampleRate)
{
  auto dec = al_context.createDecoder(filename);
  if (dec->getFrequency() == finalSampleRate && dec->getSampleType() == alure::SampleType::Float32)
  {
    std::vector<float> audioData(dec->getLength() * get_channel_quantity(dec->getChannelConfig()));
    dec->read(audioData.data(), dec->getLength());
    return std::make_shared<AudioBuffer>(audioData, alure::SampleType::Float32, dec->getChannelConfig());
  }

  std::vector<ALbyte> audioData(alure::FramesToBytes(dec->getLength(), dec->getChannelConfig(), dec->getSampleType()));
  dec->read(audioData.data(), dec->getLength());

  auto inputSampleType = dec->getSampleType();
  auto inputLength = dec->getLength();
  auto inputSize = audioData.size();
  auto inputFrequency = dec->getFrequency();
  auto inputChannelConfig = dec->getChannelConfig();

  std::vector<float> resampledAudio(inputSize / get_sample_size(inputSampleType) * finalSampleRate / inputFrequency + 1);

  auto audioSpec = soxr_io_spec(get_sox_type(inputSampleType), SOXR_FLOAT32_I);
  auto resampler = soxr_create(inputFrequency, finalSampleRate, get_channel_quantity(inputChannelConfig), nullptr, &audioSpec, &qualityResampleQuality, &runtimeSpec);

  size_t j = 0;
  for (size_t i = 0,
    block = 262144,
    idone = 0,
    odone = 0,
    m = alure::BytesToFrames(audioData.size(), inputChannelConfig, inputSampleType);
    i < m;)
  {
    if (block + i > m)
    {
      block = m - i;
    }
    soxr_process(resampler, audioData.data() + alure::FramesToBytes(i, inputChannelConfig, inputSampleType),
      block, &idone,
      resampledAudio.data() + j, resampledAudio.size() - j, &odone);

    i += idone;
    j += odone * get_channel_quantity(inputChannelConfig);
    if (odone == 0)
    {
      break;
    }
  }
  resampledAudio.resize(j);

  soxr_delete(resampler);
  return std::make_shared<AudioBuffer>(resampledAudio, alure::SampleType::Float32, inputChannelConfig);
}

IPLVector3 rotate_clockwise_over_zero(double radius, std::chrono::milliseconds run_duration, double angularVelocity)
{
  return IPLVector3
  {
    (IPLfloat32)(std::cos(run_duration.count() * angularVelocity / 1000) * radius),
    0,
    -(IPLfloat32)(std::sin(run_duration.count() * angularVelocity / 1000) * radius)
  };
}

IPLVector3 rotate_clockwise_over_zero(double radius, uint64_t run_duration, double angularVelocity)
{
  return IPLVector3
  {
    (IPLfloat32)(std::cos(run_duration * angularVelocity / 1000) * radius),
    0,
    -(IPLfloat32)(std::sin(run_duration * angularVelocity / 1000) * radius)
  };
}

size_t get_channel_quantity(alure::ChannelConfig config)
{
  switch (config)
  {
  case alure::ChannelConfig::Mono:
    return 1;
  case alure::ChannelConfig::Stereo:
    return 2;
  default:
    throw std::exception("Channel type not implemented.");
  }
}

size_t get_sample_size(alure::SampleType type)
{
  switch (type)
  {
  case alure::SampleType::Float32:
    return 4;
  case alure::SampleType::Int16:
    return 2;
  case alure::SampleType::Mulaw:
  case alure::SampleType::UInt8:
    return 1;
  }
}

soxr_datatype_t get_sox_type(alure::SampleType type)
{
  switch (type)
  {
  case alure::SampleType::Float32:
    return SOXR_FLOAT32_I;
  case alure::SampleType::Int16:
    return SOXR_INT16_I;
  default:
    throw std::exception("Sample type not supported");
  }
}

size_t get_real_input_size(alure::SampleType type, size_t size)
{
  switch (type)
  {
  case alure::SampleType::Float32:
    return size;
  case alure::SampleType::Int16:
    return size / 2;
  default:
    throw std::exception("Not supported");
  }
}