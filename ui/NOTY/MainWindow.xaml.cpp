#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <filesystem>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Interop.h>
#include <microsoft.ui.xaml.window.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

// Function for reading the files in the .notes containing folder
namespace winrt::NOTY::implementation
{
    ///////////////
    //constructor//
    ///////////////
    MainWindow::MainWindow()
    {
        InitializeComponent();

        // SET APP-ICON
        AppWindow().SetIcon(L"D:\NOTY GUI\NOTY\NOTY\Assets\AppIcon.ico");

        //----------------------------
        // XAML Elements Connecting. |
        //----------------------------
        
        ////////////////////////
        //Connecting LIST VIEW//
        ////////////////////////
        
        //List returns the IInspectable and is then passed to the ItermsSource of the List view function that then displays the LIST
        auto data = List_f();
        FilestoSave().ItemsSource(data);

    }

    ////////////////////
    //Global Variables//
    ////////////////////
    
    //var for config file path for user.
    char pathtoconfig[560];

    //c-string var for path to save the file.
    char pathtosave[400];

    // Var having the path to the file seleted in the LISTVIEW
    std::string S_File_Path;


    //////////////////////////////////////////////////////////////////////////////////////
    //function for the text box to display the text in the file, Text Box accepts String//
    //////////////////////////////////////////////////////////////////////////////////////

    // EVENT HANDLER FUNCTION {}
    void MainWindow::FilestoSave_SelectionChanged(IInspectable const&, Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const&)
    {
        if (FilestoSave().SelectedItem() != nullptr)
        {
            std::string Text = TextBox_disp_f();
            hstring Text_h = winrt::to_hstring(Text);
            TextInFileToDisplay().Text(Text_h);
        }
    }

    std::string MainWindow::TextBox_disp_f() {

        //accessing the current selected option in the LIst (file name) and saving the retured IInstpectable val returned by the function
        Windows::Foundation::IInspectable S_File_Name_II = FilestoSave().SelectedItem();

        //unboxing (ungeneralizing) the type from IInspectable to hstring.
        hstring S_File_Name_h = winrt::unbox_value<hstring>(S_File_Name_II);

        //Converting the hstring to string
        std::string S_File_Name_s = winrt::to_string(S_File_Name_h);

        //Concat the File name to the path we set to save our files to make a proper path.
        S_File_Path = pathtosave;
        S_File_Path += "\\";
        S_File_Path += S_File_Name_s;

        //opening the file in read mode to copy all content
        std::ifstream file(S_File_Path);
        
        // Exception check, that either the file has been opened or not by the ifstream.
        if (!file) {
            MessageBoxA(NULL, "FILE NOT OPENING!", "Title", MB_OK);
            return "Noty";
        }

        //a constructor is called on data obj of type string that makes a data string obj with data stored from the start marker to the end marker. i.e. text bw the markers
        //String is a class and this parametr passed construction lets us make a feeded string with data bw the markers
        std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        
        return data;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    //function for displaying the file names present in the save location in the List view//
    ////////////////////////////////////////////////////////////////////////////////////////
    Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> MainWindow::List_f() {
       
        //storing the path to appdata for config path
        SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, pathtoconfig);
        
        //completeing the path to NOTY
        strcat_s(pathtoconfig, sizeof(pathtoconfig), "\\NOTY\\config.ini");


        // use correct buffer size for pathtosave to avoid overruns
        GetPrivateProfileStringA("LocationToSaveFile", "path", "e", pathtosave, static_cast<UINT>(sizeof(pathtosave)), pathtoconfig);


        //Make a directory_iterator class object thats pointer member var should point to the the 1st file of the folder (It is OOP)
        std::filesystem::directory_iterator File_Pointer(pathtosave);

        //Def const fot the File_Pointer it makes an object that points to the end of the folder.
        std::filesystem::directory_iterator end_marker;

        //making a hstring and will store the file names in the hstring.
        //the function will return a hsting ivector taht will be captured by my ivector hsting obj, the retuned one points to teh real hstoing obj that we cannot make dir but these ivectors ones will be used to amanger and work on teh real onw
        //Windows::Foundation::Collections::IVector<hstring> List_Files_String = winrt::single_threaded_vector<hstring>();
        Windows::Foundation::Collections::IVector<Windows::Foundation::IInspectable> List_Files_String = winrt::single_threaded_vector<Windows::Foundation::IInspectable>();

        //LOOP Interating in the folder picking only .txts for the list. IT IS OOP!!
        while (File_Pointer != end_marker) {

            //Receive the "file" path string in a path object, dir iter has only one pinter as mem var and derefering it gives a dir entery obj that has methods like .pth which return the path as a path obj. So to acces t eoat I can also use ->
            //*File_Pointer.path(); is not used as of preccidence issues *(File_Pointer).path(); is correct though operator overloading you will get how -> gives a obj in return
            std::filesystem::path File_path = File_Pointer->path();

            //File_path was a path obj and by using one of its member func ext i got another path obj that now had its single str mem var having the ext name storedin it and to output that I then used .sting() that returned the ext as a string that I coudl use.

            if (File_path.extension().string() == ".txt") {
                //List_Files_String.Append(hstring(File_path.filename().wstring()));
                List_Files_String.Append(winrt::box_value(hstring(File_path.filename().wstring())));
            }
            File_Pointer++;
        }
        return List_Files_String;
    }

    /////////////////////////////////////
    //SAVE TEXT ON Pressing SAVE BUTTON//
    /////////////////////////////////////    

    void MainWindow::tb_file_save_f() {
        //Getting text from the text box and storing it in the hstring var
        hstring Text_in_TextBox_h = TextInFileToDisplay().Text();
        //cov the hstring to string
        std::string Text_in_TextBox_s = winrt::to_string(Text_in_TextBox_h);
        //Saving the text in the text box in the respective file
        std::ofstream File(S_File_Path);
        File << Text_in_TextBox_s;
    }

    //function that will run when we click the button
    void MainWindow::Save_to_file(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e){
        //Function to save teh text in file taht is in teh text box
        tb_file_save_f();
    }

    //The function will call when ever we will click the refresh button
    void MainWindow::RefreshButton(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e) {
        //calling all functions again that mmsut be called
        //Relisting all files
        List_f();
        //redisp the file content in text box
        FilestoSave_SelectionChanged(nullptr, nullptr);

    }

    //The function will call when ever we will click the tweaks button
    void MainWindow::TweaksButton(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e) {
        MessageBoxA(NULL, "Coming SOON", "Support Us", MB_OK);
    }

    //The function will call when ever we will click the Cahnge path button
    void MainWindow::ChangePathButton(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e) {
        MessageBoxA(NULL, "Coming SOON", "Support Us", MB_OK);
    }
}

namespace winrt::NOTY::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

}
