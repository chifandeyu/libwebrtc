#include "libwebrtc.h"

#include "api/scoped_refptr.h"
#include "rtc_base/ssl_adapter.h"
#include "rtc_base/thread.h"
#include "rtc_base/logging.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include "rtc_peerconnection_factory_impl.h"

namespace libwebrtc {

template< typename... Args >
std::string string_sprintf(const char* format, Args... args) {
    int length = std::snprintf(nullptr, 0, format, args...);
    if (length <= 0) {
        return "";
    }

    char* buf = new char[length + 1];
    std::snprintf(buf, length + 1, format, args...);

    std::string str(buf);
    delete[] buf;
    return str;
}

  //1 定义rtc::LogSink子对象，实现void OnLogMessage(const std::string &message)函数
class SampleLogSink:virtual public rtc::LogSink
{
public:
    SampleLogSink(const char* webrtcLogDir) {
        if (webrtcLogDir) {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            std::string strLogPath = string_sprintf("webRTC_%d%02d%02d%02d%02d%02d",
             1900 + ltm->tm_year, 
             1 + ltm->tm_mon, 
             ltm->tm_mday, 
             ltm->tm_hour, 
             ltm->tm_min, 
             ltm->tm_sec);

            strLogPath = std::string(webrtcLogDir) + "\\" + strLogPath + ".log";
            
            _streamObj.open(strLogPath.c_str());
            _bWriteFile = !_streamObj.fail();
        }
    }
    ~SampleLogSink() {
      if (_bWriteFile) {
        _streamObj << "[webRTC] ==== LOG close ====";
        _streamObj.flush();
        _streamObj.close();
      }
    }
    virtual void OnLogMessage(const std::string &message)
    {
      //do something ...
      if (_bWriteFile) {
        _streamObj << "[webRTC]" << message;
        _streamObj.flush();
      } else {
        std::cout << "[webRTC]" << message << std::endl;
      }
    }

private:
    std::ofstream _streamObj;
    bool _bWriteFile = false;
};

static bool g_is_initialized = false;
std::unique_ptr<rtc::Thread> worker_thread;
std::unique_ptr<rtc::Thread> signaling_thread;
std::unique_ptr<rtc::Thread> network_thread;
std::unique_ptr<SampleLogSink> g_logSink = nullptr;

bool LibWebRTC::Initialize(const char* webrtcLogDir /*= nullptr*/, LoggingLevel level /*= LoggingLevel::LS_INFO*/) {
  if (!g_is_initialized) {
    rtc::InitializeSSL();
    g_is_initialized = true;
    if(webrtcLogDir){
      //1. 定义日志接收对象
      g_logSink = std::make_unique<SampleLogSink>(webrtcLogDir);
      //2. 配置对象及需要的日志登记
      rtc::LoggingSeverity loggingSeverity = rtc::LoggingSeverity::WARNING;
      switch (level)
      {
      case LoggingLevel::INFO:
        loggingSeverity = rtc::LoggingSeverity::INFO;
        break;
      case LoggingLevel::WARNING:
        loggingSeverity = rtc::LoggingSeverity::WARNING;
        break;
        case LoggingLevel::LERROR:
        loggingSeverity = rtc::LoggingSeverity::LERROR;
        break;
        case LoggingLevel::NONE:
        loggingSeverity = rtc::LoggingSeverity::LS_NONE;
        break;
        case LoggingLevel::VERBOSE:
        loggingSeverity = rtc::LoggingSeverity::LS_VERBOSE;
        break;
      default:
        break;
      }
      rtc::LogMessage::AddLogToStream(g_logSink.get(), loggingSeverity);
    }
    if (worker_thread.get() == nullptr) {
      worker_thread = rtc::Thread::Create();
      worker_thread->SetName("worker_thread", nullptr);
      RTC_CHECK(worker_thread->Start()) << "Failed to start thread";
    }

    if (signaling_thread.get() == nullptr) {
      signaling_thread = rtc::Thread::Create();
      signaling_thread->SetName("signaling_thread", NULL);
      RTC_CHECK(signaling_thread->Start()) << "Failed to start thread";
    }

    if (network_thread.get() == nullptr) {
      network_thread = rtc::Thread::CreateWithSocketServer();
      network_thread->SetName("network_thread", nullptr);
      RTC_CHECK(network_thread->Start()) << "Failed to start thread";
    }
  }
  return g_is_initialized;
}

void LibWebRTC::Terminate() {
  rtc::ThreadManager::Instance()->SetCurrentThread(NULL);
  rtc::CleanupSSL();

  if (worker_thread.get() != nullptr) {
    worker_thread->Stop();
    worker_thread.reset(nullptr);
  }

  if (signaling_thread.get() != nullptr) {
    signaling_thread->Stop();
    signaling_thread.reset(nullptr);
  }

  if (network_thread.get() != nullptr) {
    network_thread->Stop();
    network_thread.reset(nullptr);
  }

  if(g_logSink.get() != nullptr){
    g_logSink.reset(nullptr);
  }
  g_is_initialized = false;
}

scoped_refptr<RTCPeerConnectionFactory>
LibWebRTC::CreateRTCPeerConnectionFactory() {
  scoped_refptr<RTCPeerConnectionFactory> rtc_peerconnection_factory =
      scoped_refptr<RTCPeerConnectionFactory>(
          new RefCountedObject<RTCPeerConnectionFactoryImpl>(
              worker_thread.get(), signaling_thread.get(),
              network_thread.get()));
  rtc_peerconnection_factory->Initialize();
  return rtc_peerconnection_factory;
}

}  // namespace libwebrtc
