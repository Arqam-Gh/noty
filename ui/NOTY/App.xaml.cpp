#include "pch.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <microsoft.ui.xaml.window.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::NOTY::implementation
{
    /// <summary>
    /// Initializes the singleton application object.  This is the first line of authored code
    /// executed, and as such is the logical equivalent of main() or WinMain().
    /// </summary>
    App::App()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                OutputDebugString(errorMessage.c_str());
                OutputDebugString(L"\n");
            }
        });
#endif
    }

    /// <summary>
    /// Invoked when the application is launched.
    /// </summary>
    /// <param name="e">Details about the launch request and process.</param>
    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        //Get window Object for noty
        window = make<MainWindow>();
        //Make a IWN class to get the handle of win, by cov te window obj to IWN
        winrt::com_ptr<IWindowNative> Handle_IWN_copy = window.as<::IWindowNative>();
        //Make a Handle container ptr
        HWND Win_OS_Handle = nullptr;
        //Put the Handle
        Handle_IWN_copy->get_WindowHandle(&Win_OS_Handle);
        //Coverting the HWND to a WindowID type so it could fit as an pram for getting teh appWindow obj
        Microsoft::UI::WindowId OS_WinID = Microsoft::UI::GetWindowIdFromWindow(Win_OS_Handle);
        //Use appWin for noty to rsize it
        // 
        // direct resize
        //Microsoft::UI::Windowing::AppWindow::GetFromWindowId(OS_WinID).Resize({900,100});
        // 
        Microsoft::UI::Windowing::AppWindow OS_appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(OS_WinID);
        OS_appWindow.Resize({800,600});
        // Extend content into title bar so Mica/Acrylic can show behind the title area like native Win11 apps
        try
        {
            OS_appWindow.TitleBar().ExtendsContentIntoTitleBar(true);
        }
        catch (...) { }
        
        //Activiate Window
        window.Activate();
    }
}
