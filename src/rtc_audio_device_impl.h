#ifndef LIB_WEBRTC_AUDIO_DEVICE_IMPL_HXX
#define LIB_WEBRTC_AUDIO_DEVICE_IMPL_HXX

#include "modules/audio_device/audio_device_impl.h"
#include "modules/audio_device/include/audio_device.h"
#include "rtc_audio_device.h"
#include "rtc_base/ref_count.h"
#include "rtc_base/thread.h"

namespace libwebrtc {
class AudioDeviceImpl : public RTCAudioDevice {
 public:
  AudioDeviceImpl(
      rtc::scoped_refptr<webrtc::AudioDeviceModule> audio_device_module,
      rtc::Thread* worker_thread);

  virtual ~AudioDeviceImpl();

 public:
  // Main initializaton and termination
  int32_t Init() override;
  int32_t Terminate() override;
  bool Initialized() const override;

  // Device enumeration
  int16_t PlayoutDevices() override;

  int16_t RecordingDevices() override;

  int32_t PlayoutDeviceName(uint16_t index,
                            char name[kAdmMaxDeviceNameSize],
                            char guid[kAdmMaxGuidSize]) override;

  int32_t RecordingDeviceName(uint16_t index,
                              char name[kAdmMaxDeviceNameSize],
                              char guid[kAdmMaxGuidSize]) override;
  int32_t GetPlayoutDevice(char name[kAdmMaxDeviceNameSize],
                           char guid[kAdmMaxGuidSize]) override;
  int32_t GetRecordingDevice(char name[kAdmMaxDeviceNameSize],
                           char guid[kAdmMaxGuidSize]) override;

  // Device selection
  virtual int32_t SetPlayoutDevice(uint16_t index) override;
  virtual int32_t SetPlayoutDevice(WindowsDeviceType device) override;
  virtual int32_t SetRecordingDevice(uint16_t index) override;
  virtual int32_t SetRecordingDevice(WindowsDeviceType device) override;

  // Audio transport initialization
  virtual int32_t PlayoutIsAvailable(bool* available) override;
  virtual int32_t InitPlayout() override;
  virtual bool PlayoutIsInitialized() const override;
  virtual int32_t RecordingIsAvailable(bool* available) override;
  virtual int32_t InitRecording() override;
  virtual bool RecordingIsInitialized() const override;

  // Audio transport control
  virtual int32_t StartPlayout() override;
  virtual int32_t StopPlayout() override;
  virtual bool Playing() const override;
  virtual int32_t StartRecording() override;
  virtual int32_t StopRecording() override;
  virtual bool Recording() const override;

  // Audio mixer initialization
  virtual int32_t InitSpeaker() override;
  virtual bool SpeakerIsInitialized() const override;
  virtual int32_t InitMicrophone() override;
  virtual bool MicrophoneIsInitialized() const override;

  // Speaker volume controls
  virtual int32_t SpeakerVolumeIsAvailable(bool* available) override;
  virtual int32_t SetSpeakerVolume(uint32_t volume) override;
  virtual int32_t SpeakerVolume(uint32_t* volume) const override;
  virtual int32_t MaxSpeakerVolume(uint32_t* maxVolume) const override;
  virtual int32_t MinSpeakerVolume(uint32_t* minVolume) const override;

  // Microphone volume controls
  virtual int32_t MicrophoneVolumeIsAvailable(bool* available) override;
  virtual int32_t SetMicrophoneVolume(uint32_t volume) override;
  virtual int32_t MicrophoneVolume(uint32_t* volume) const override;
  virtual int32_t MaxMicrophoneVolume(uint32_t* maxVolume) const override;
  virtual int32_t MinMicrophoneVolume(uint32_t* minVolume) const override;

  // Speaker mute control
  virtual int32_t SpeakerMuteIsAvailable(bool* available) override;
  virtual int32_t SetSpeakerMute(bool enable) override;
  virtual int32_t SpeakerMute(bool* enabled) const override;

  // Microphone mute control
  virtual int32_t MicrophoneMuteIsAvailable(bool* available) override;
  virtual int32_t SetMicrophoneMute(bool enable) override;
  virtual int32_t MicrophoneMute(bool* enabled) const override;

  // Stereo support
  virtual int32_t StereoPlayoutIsAvailable(bool* available) const override;
  virtual int32_t SetStereoPlayout(bool enable) override;
  virtual int32_t StereoPlayout(bool* enabled) const override;
  virtual int32_t StereoRecordingIsAvailable(bool* available) const override;
  virtual int32_t SetStereoRecording(bool enable) override;
  virtual int32_t StereoRecording(bool* enabled) const override;

  virtual bool BuiltInAECIsAvailable() const override;
  virtual int32_t EnableBuiltInAEC(bool enable) override;

 private:
  rtc::scoped_refptr<webrtc::AudioDeviceModule> audio_device_module_;
  rtc::Thread* worker_thread_ = nullptr;
};

}  // namespace libwebrtc

#endif  // LIB_WEBRTC_AUDIO_DEVICE_IMPL_HXX
