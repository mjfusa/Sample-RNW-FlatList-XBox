#include "pch.h"
#include INCLUDE_FILE_X(MODULE.h)

#include "ReactNotificationService.h"
#include <winrt/Windows.Security.ExchangeActiveSyncProvisioning.h>
#include <winrt/Windows.ApplicationModel.Background.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <CppWinRTIncludes.h>
#include <winrt/base.h>

#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Core.h>
#include "xinput.h"
#include <thread>

using namespace NAMESPACE; 
using namespace winrt;
using namespace Microsoft::ReactNative;
using namespace Windows::Security::ExchangeActiveSyncProvisioning;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Background;
using namespace Windows::System;

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Core;

  winrt::hstring MODULE::DeviceModel() noexcept {
    auto info = EasClientDeviceInformation();
    return info.SystemProductName();
  }

  // Test signaling of background task with App Trigger
  void MODULE::TestFireBackgroundTask() noexcept {
    TestFireBackgroundTaskAsync();
  }

  IAsyncAction MODULE::TestFireBackgroundTaskAsync() noexcept {
    auto result = _AppTrigger.RequestAsync();
    co_return;
  }

  void MODULE::RegisterNativeJsTaskHook(std::string const &str_taskName) noexcept {
    registerNativeJsTaskHook(str_taskName);
  }

  IAsyncAction MODULE::registerNativeJsTaskHook(std::string const &str_taskName) noexcept {
    auto taskName = str_taskName;

    auto accessStatus = co_await BackgroundExecutionManager::RequestAccessAsync();
    if ((BackgroundAccessStatus::AlwaysAllowed == accessStatus) ||
        (BackgroundAccessStatus::AllowedSubjectToSystemPolicy == accessStatus)) {
      auto taskRegistrations{BackgroundTaskRegistration::AllTasks()};

      for (const auto &taskRegistration : taskRegistrations) {
        // Assuming one background task. Just un-register all tasks when registering a new one.
        // This prevents needing to worry about the name of the task changing and leaving zombie tasks.
        taskRegistration.Value().Unregister(false);
      }

      BackgroundTaskBuilder builder;
      builder.Name(winrt::to_hstring(taskName));
      // builder.SetTrigger(_AppTrigger); // Test signaling of background task with App Trigger
      builder.SetTrigger(SystemTrigger(SystemTriggerType::TimeZoneChange, false));
      auto registration = builder.Register();
    }

    co_return;
  }

  void MODULE::Init() noexcept {
    InitAsync();
  }

  void MODULE::EmitJSEvent1(int value) noexcept {
    m_reactContext.EmitJSEvent(L"RCTDeviceEventEmitter", L"EmitKeystrokeEvent", value);
  }


  void MODULE::OnKeyDown(IInspectable const &sender, KeyEventArgs const &e) noexcept {
    DWORD key = (DWORD) e.VirtualKey();

    switch (key) {
        // For keyboard testing without XBOX Controller
        case 0x41: //VK_A:
          e.Handled(true);
          EmitJSEvent1(key);
      break;
        case 0x000000c3: // Key mapping on PC for XBox A Key
        case VK_PAD_A:
        e.Handled(true);
        EmitJSEvent1(key);
      break;
        case 0x000000c4: // Key mapping on PC for XBox B Key
        case VK_PAD_B:
          e.Handled(true);
          EmitJSEvent1(key);
      break;
      case VK_PAD_X:
        break;
      case VK_PAD_Y:
        break;
      case VK_PAD_DPAD_UP:
        break;
      case VK_PAD_DPAD_DOWN:
        break;
      case VK_PAD_DPAD_LEFT:
        break;
    case VK_PAD_DPAD_RIGHT:
        break;
    }


  }

  bool init = false;

  IAsyncAction MODULE::InitAsync() noexcept {
    Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(
        CoreDispatcherPriority::Normal,
        [this]() {
          if (!init) {
          auto window = Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow();
          window.KeyDown({this, &MODULE::OnKeyDown});
          init = true;
          }
        });
    co_return;
  }


  void MODULE::Initialize(winrt::Microsoft::ReactNative::ReactContext const &reactContext) noexcept {
    m_reactContext = reactContext;
    //_AppTrigger = ApplicationTrigger(); // Test signaling of background task with App Trigger

    const ReactNotificationId<int> backgroundNotification{L"NativeModuleClass", L"BackgroundNotification"};

    m_reactContext.Notifications().Subscribe(
        backgroundNotification, [this](IInspectable const &sender, ReactNotificationArgs<int> const &args) {
          IReactPropertyBag res = unbox_value<IReactPropertyBag>(sender);
          auto taskPropName = ReactPropertyBagHelper::GetName(nullptr, L"TaskNameProperty");
          auto value = res.Get(taskPropName);
          auto taskName = unbox_value<hstring>(value);
          OutputDebugStringW((to_hstring(L"Background Task: ") + taskName + to_hstring("\n")).data());

          m_reactContext.CallJSFunction(L"AppRegistry", L"startHeadlessTask", 1, winrt::to_string(taskName));
        });
  }
