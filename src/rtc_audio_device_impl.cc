#include "rtc_audio_device_impl.h"
#include "rtc_base/logging.h"

namespace libwebrtc {

AudioDeviceImpl::AudioDeviceImpl(
      rtc::scoped_refptr<webrtc::AudioDeviceModule> audio_device_module,rtc::Thread* worker_thread)
      : audio_device_module_(audio_device_module),worker_thread_(worker_thread) {}

AudioDeviceImpl::~AudioDeviceImpl() {
  RTC_LOG(INFO) << __FUNCTION__ << ": dtor ";
}

int32_t AudioDeviceImpl::Init() {
  return audio_device_module_->Init();
}

int32_t AudioDeviceImpl::Terminate() {
  return audio_device_module_->Terminate();
}

bool AudioDeviceImpl::Initialized() const {
  return audio_device_module_->Initialized();
}

int16_t AudioDeviceImpl::PlayoutDevices() {
  return audio_device_module_->PlayoutDevices();
}

int16_t AudioDeviceImpl::RecordingDevices() {
  return audio_device_module_->RecordingDevices();
}

int32_t AudioDeviceImpl::PlayoutDeviceName(uint16_t index,
                                           char name[kAdmMaxDeviceNameSize],
                                           char guid[kAdmMaxGuidSize]) {
  return worker_thread_->Invoke<int32_t>(RTC_FROM_HERE, [&] {
    RTC_DCHECK_RUN_ON(worker_thread_);
    return audio_device_module_->PlayoutDeviceName(index, name, guid);
  });
}

int32_t AudioDeviceImpl::RecordingDeviceName(uint16_t index,
                                             char name[kAdmMaxDeviceNameSize],
                                             char guid[kAdmMaxGuidSize]) {
  return worker_thread_->Invoke<int32_t>(RTC_FROM_HERE, [&] {
    RTC_DCHECK_RUN_ON(worker_thread_);
    return audio_device_module_->RecordingDeviceName(index, name, guid);
  });
}

int32_t AudioDeviceImpl::GetPlayoutDevice(char name[kAdmMaxDeviceNameSize],
                                          char guid[kAdmMaxGuidSize]) {
  return audio_device_module_->GetPlayoutDevice(name, guid);
}

int32_t AudioDeviceImpl::GetRecordingDevice(char name[kAdmMaxDeviceNameSize],
                                          char guid[kAdmMaxGuidSize]) {
  return audio_device_module_->GetRecordingDevice(name, guid);
}
int32_t AudioDeviceImpl::SetPlayoutDevice(uint16_t index) {
  return worker_thread_->Invoke<int32_t>(RTC_FROM_HERE, [&] {
    RTC_DCHECK_RUN_ON(worker_thread_);
    return audio_device_module_->SetPlayoutDevice(index);
  });
}

int32_t AudioDeviceImpl::SetPlayoutDevice(WindowsDeviceType device){
  webrtc::AudioDeviceModule::WindowsDeviceType type;
  if (device == libwebrtc::RTCAudioDevice::WindowsDeviceType::kDefaultCommunicationDevice) {
    type = webrtc::AudioDeviceModule::WindowsDeviceType::kDefaultCommunicationDevice;
  }
  else{
    type = webrtc::AudioDeviceModule::WindowsDeviceType::kDefaultDevice;
  }

  return audio_device_module_->SetPlayoutDevice(type);
}

int32_t AudioDeviceImpl::SetRecordingDevice(uint16_t index) {
  return worker_thread_->Invoke<int32_t>(RTC_FROM_HERE, [&] {
    RTC_DCHECK_RUN_ON(worker_thread_);
    return audio_device_module_->SetRecordingDevice(index);
  });
}

int32_t AudioDeviceImpl::SetRecordingDevice(WindowsDeviceType device) {
  webrtc::AudioDeviceModule::WindowsDeviceType type;
  if (device == libwebrtc::RTCAudioDevice::WindowsDeviceType::kDefaultCommunicationDevice) {
    type = webrtc::AudioDeviceModule::WindowsDeviceType::kDefaultCommunicationDevice;
  }
  else{
    type = webrtc::AudioDeviceModule::WindowsDeviceType::kDefaultDevice;
  }

  return audio_device_module_->SetRecordingDevice(type);
}

int32_t AudioDeviceImpl::PlayoutIsAvailable(bool* available) {
  return audio_device_module_->PlayoutIsAvailable(available);
}

int32_t AudioDeviceImpl::InitPlayout() {
  return audio_device_module_->InitPlayout() ;
}

bool AudioDeviceImpl::PlayoutIsInitialized() const {
  return audio_device_module_->PlayoutIsInitialized();
}

int32_t AudioDeviceImpl::RecordingIsAvailable(bool* available) {
  return audio_device_module_->RecordingIsAvailable(available);
}

int32_t AudioDeviceImpl::InitRecording() {
  return audio_device_module_->InitRecording();
}

bool AudioDeviceImpl::RecordingIsInitialized() const {
  return audio_device_module_->RecordingIsInitialized();
}

int32_t AudioDeviceImpl::StartPlayout() {
  return audio_device_module_->StartPlayout();
}

int32_t AudioDeviceImpl::StopPlayout() {
  return audio_device_module_->StopPlayout();
}

bool AudioDeviceImpl::Playing() const {
  return audio_device_module_->Playing();
}

int32_t AudioDeviceImpl::StartRecording() {
  return audio_device_module_->StartRecording();
}

int32_t AudioDeviceImpl::StopRecording() {
  return audio_device_module_->StopRecording();
}

bool AudioDeviceImpl::Recording() const {
  return audio_device_module_->Recording();
}

int32_t AudioDeviceImpl::InitSpeaker() {
  return audio_device_module_->InitSpeaker();
}

bool AudioDeviceImpl::SpeakerIsInitialized() const {
  return audio_device_module_->SpeakerIsInitialized();
}

int32_t AudioDeviceImpl::InitMicrophone() {
  return audio_device_module_->InitMicrophone();
}

bool AudioDeviceImpl::MicrophoneIsInitialized() const {
  return audio_device_module_->MicrophoneIsInitialized();
}

int32_t AudioDeviceImpl::SpeakerVolumeIsAvailable(bool* available) {
  return audio_device_module_->SpeakerVolumeIsAvailable(available);
}

int32_t AudioDeviceImpl::SetSpeakerVolume(uint32_t volume) {
  return audio_device_module_->SetSpeakerVolume(volume);
}

int32_t AudioDeviceImpl::SpeakerVolume(uint32_t* volume) const {
  return audio_device_module_->SpeakerVolume(volume);
}

int32_t AudioDeviceImpl::MaxSpeakerVolume(uint32_t* maxVolume) const {
  return audio_device_module_->MaxSpeakerVolume(maxVolume);
}

int32_t AudioDeviceImpl::MinSpeakerVolume(uint32_t* minVolume) const {
  return audio_device_module_->MinSpeakerVolume(minVolume);
}

int32_t AudioDeviceImpl::MicrophoneVolumeIsAvailable(bool* available) {
  return audio_device_module_->MicrophoneVolumeIsAvailable(available);
}

int32_t AudioDeviceImpl::SetMicrophoneVolume(uint32_t volume) {
  return audio_device_module_->SetMicrophoneVolume(volume);
}

int32_t AudioDeviceImpl::MicrophoneVolume(uint32_t* volume) const {
  return audio_device_module_->MicrophoneVolume(volume);
}

int32_t AudioDeviceImpl::MaxMicrophoneVolume(uint32_t* maxVolume) const {
  return audio_device_module_->MaxMicrophoneVolume(maxVolume);
}

int32_t AudioDeviceImpl::MinMicrophoneVolume(uint32_t* minVolume) const {
  return audio_device_module_->MinMicrophoneVolume(minVolume);
}

int32_t AudioDeviceImpl::SpeakerMuteIsAvailable(bool* available) {
  return audio_device_module_->SpeakerMuteIsAvailable(available);
}

int32_t AudioDeviceImpl::SetSpeakerMute(bool enable) {
  return audio_device_module_->SetSpeakerMute(enable);
}

int32_t AudioDeviceImpl::SpeakerMute(bool* enabled) const {
  return audio_device_module_->SpeakerMute(enabled);
}

int32_t AudioDeviceImpl::MicrophoneMuteIsAvailable(bool* available) {
  return audio_device_module_->MicrophoneMuteIsAvailable(available);
}

int32_t AudioDeviceImpl::SetMicrophoneMute(bool enable) {
  return audio_device_module_->SetMicrophoneMute(enable);
}

int32_t AudioDeviceImpl::MicrophoneMute(bool* enabled) const {
  return audio_device_module_->MicrophoneMute(enabled);
}

int32_t AudioDeviceImpl::StereoPlayoutIsAvailable(bool* available) const {
  return audio_device_module_->StereoPlayoutIsAvailable(available);
}

int32_t AudioDeviceImpl::SetStereoPlayout(bool enable) {
  return audio_device_module_->SetStereoPlayout(enable);
}

int32_t AudioDeviceImpl::StereoPlayout(bool* enabled) const {
  return audio_device_module_->StereoPlayout(enabled);
}

int32_t AudioDeviceImpl::StereoRecordingIsAvailable(bool* available) const {
  return audio_device_module_->StereoRecordingIsAvailable(available);
}

int32_t AudioDeviceImpl::SetStereoRecording(bool enable) {
  return audio_device_module_->SetStereoRecording(enable);
}

int32_t AudioDeviceImpl::StereoRecording(bool* enabled) const {
  return audio_device_module_->StereoRecording(enabled);
}

bool AudioDeviceImpl::BuiltInAECIsAvailable() const {
  return audio_device_module_->BuiltInAECIsAvailable();
}

int32_t AudioDeviceImpl::EnableBuiltInAEC(bool enable) {
  return audio_device_module_->EnableBuiltInAEC(enable);
}

}  // namespace libwebrtc
