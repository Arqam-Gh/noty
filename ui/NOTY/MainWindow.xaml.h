#pragma once

#include "MainWindow.g.h"
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>
#include <iostream>

namespace winrt::NOTY::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        //declarations
        Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> List_f();
        std::string TextBox_disp_f();
        void FilestoSave_SelectionChanged(IInspectable const&, Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const&);
        void tb_file_save_f();
        void Save_to_file(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void RefreshButton(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void TweaksButton(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ChangePathButton(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

        // Mica/Acrylic controllers (optional)
        winrt::Microsoft::UI::Composition::SystemBackdrops::MicaController m_micaController{ nullptr };
        winrt::Microsoft::UI::Composition::SystemBackdrops::SystemBackdropConfiguration m_configuration{ nullptr };
        winrt::Microsoft::UI::Composition::SystemBackdrops::DesktopAcrylicController m_acrylicController{ nullptr };
    };
}

namespace winrt::NOTY::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {

    };
}