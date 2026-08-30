#include "idp.iss"

[Setup]
AppPublisher=Muhammad Arqam Ghayour
WizardImageFile=NOTY_left.bmp
AppName=NOTY
AppVersion=1.0
SetupIconFile=Noty.ico
DefaultDirName={autopf}\NOTY
DefaultGroupName=NOTY
DisableWelcomePage=yes
OutputDir=.
OutputBaseFilename=Setup
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Files]
Source: "Noty.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "NOTY.bmp"; DestDir: "{tmp}"; Flags: dontcopy

Source: "UIFiles\*"; DestDir: "{app}\UI"; Flags: ignoreversion recursesubdirs createallsubdirs

[Tasks]
Name: "installruntimes"; Description: "Download and install Windows App SDK Runtime and Visual C++ Redistributable (required for the NOTY tray app; ~80 MB download, internet connection needed)"; GroupDescription: "Prerequisites:"; Flags: checkedonce

[Icons]
Name: "{group}\NOTY"; Filename: "{app}\UI\noty_ui.exe"
Name: "{commonstartup}\NOTY UI"; Filename: "{app}\UI\noty_ui.exe"

[Run]
Filename: "{app}\Noty.exe"; Flags: nowait skipifsilent
Filename: "{app}\UI\noty_ui.exe"; Flags: nowait skipifsilent postinstall

[Code]
var
  ChoicePage, TweaksPage, SaveOptionsPage: TWizardPage;
  RadioDefault, RadioManual: TNewRadioButton;
  CheckBullets, CheckEndline, CheckDoubleEndline: TNewCheckBox;
  RadioByName, RadioByTimestamp: TNewRadioButton;
  EditSavePath: TNewEdit;
  ErrorCode, ResultCode: Integer;

function GetLocalDocumentsPath: String;
begin
  Result := ExpandConstant('{%USERPROFILE}') + '\Documents';
end;

function IsDriveRoot(Dir: String): Boolean;
begin
  Result := False;
  if (Length(Dir) = 2) and (Dir[2] = ':') then
    Result := True;
  if (Length(Dir) = 3) and (Dir[2] = ':') and ((Dir[3] = '\') or (Dir[3] = '/')) then
    Result := True;
end;

function ResolveSavePath(PickedPath: String): String;
begin
  if IsDriveRoot(PickedPath) then
    Result := PickedPath + 'NOTY_SAVES'
  else
    Result := PickedPath;
end;

procedure BrowseButtonClick(Sender: TObject);
var
  Dir: String;
begin
  Dir := EditSavePath.Text;
  if BrowseForFolder('Select folder to save NOTY text files', Dir, False) then
    EditSavePath.Text := ResolveSavePath(Dir);
end;

procedure GithubButtonClick(Sender: TObject);
begin
  ShellExec('open', 'https://github.com/Arqam-Gh', '', '', SW_SHOWNORMAL, ewNoWait, ErrorCode);
end;

procedure CreateWelcomePage;
var
  WelcomePage: TWizardPage;
  YellowPanel: TPanel;
  HeadingLabel, DescLabel, MadeByLabel: TNewStaticText;
begin
  WelcomePage := CreateCustomPage(wpWelcome, '', '');

  YellowPanel := TPanel.Create(WelcomePage);
  YellowPanel.Parent := WelcomePage.Surface;
  YellowPanel.Left := 0;
  YellowPanel.Top := 0;
  YellowPanel.Width := WelcomePage.SurfaceWidth;
  YellowPanel.Height := (WelcomePage.SurfaceHeight * 20) div 100;

  YellowPanel.BevelOuter := bvNone;

  HeadingLabel := TNewStaticText.Create(WelcomePage);
  HeadingLabel.Parent := YellowPanel;
  HeadingLabel.Caption := 'NOTY';
  HeadingLabel.Font.Size := 20;
  HeadingLabel.Font.Style := [fsBold];
  HeadingLabel.Left := 16;
  HeadingLabel.Top := (YellowPanel.Height - HeadingLabel.Height) div 2;

  DescLabel := TNewStaticText.Create(WelcomePage);
  DescLabel.Parent := WelcomePage.Surface;
  DescLabel.WordWrap := True;
  DescLabel.AutoSize := True;
  DescLabel.Left := 16;
  DescLabel.Top := YellowPanel.Height + 20;
  DescLabel.Width := WelcomePage.SurfaceWidth - 32;
  DescLabel.Caption := 'NOTY is an app that will make your workflow easier. With one shortcut, you can save selected text from any window at any time to a .txt file. The app pastes the text and saves the file. Takes less than 1MB. Written in C++.';

  MadeByLabel := TNewStaticText.Create(WelcomePage);
  MadeByLabel.Parent := WelcomePage.Surface;
  MadeByLabel.Caption := 'Made by Muhammad Arqam Ghayour';
  MadeByLabel.Left := 16;
  MadeByLabel.Top := DescLabel.Top + DescLabel.Height + 24;
end;

procedure CreateChoicePage;
begin
  ChoicePage := CreateCustomPage(wpSelectDir, 'Setup Type', 'Choose how you want to configure NOTY');

  RadioDefault := TNewRadioButton.Create(ChoicePage);
  RadioDefault.Parent := ChoicePage.Surface;
  RadioDefault.Caption := 'Use Default Settings';
  RadioDefault.Left := 16;
  RadioDefault.Top := 16;
  RadioDefault.Width := ChoicePage.SurfaceWidth - 32;
  RadioDefault.Checked := True;

  RadioManual := TNewRadioButton.Create(ChoicePage);
  RadioManual.Parent := ChoicePage.Surface;
  RadioManual.Caption := 'Manual App Customization';
  RadioManual.Left := 16;
  RadioManual.Top := RadioDefault.Top + RadioDefault.Height + 12;
  RadioManual.Width := ChoicePage.SurfaceWidth - 32;
end;

procedure CreateTweaksPage;
var
  PreviewBullets, PreviewEndline, PreviewDouble: TNewMemo;
begin
  TweaksPage := CreateCustomPage(ChoicePage.ID, 'Output Format', 'Choose how saved text is formatted');

  CheckBullets := TNewCheckBox.Create(TweaksPage);
  CheckBullets.Parent := TweaksPage.Surface;
  CheckBullets.Caption := 'Add bullet before each saved entry';
  CheckBullets.Left := 16;
  CheckBullets.Top := 8;
  CheckBullets.Width := TweaksPage.SurfaceWidth - 32;
  CheckBullets.Checked := True;

  PreviewBullets := TNewMemo.Create(TweaksPage);
  PreviewBullets.Parent := TweaksPage.Surface;
  PreviewBullets.Left := 32;
  PreviewBullets.Top := CheckBullets.Top + CheckBullets.Height + 2;
  PreviewBullets.Width := TweaksPage.SurfaceWidth - 48;
  PreviewBullets.Height := 30;
  PreviewBullets.ReadOnly := True;
  PreviewBullets.ScrollBars := ssNone;
  PreviewBullets.TabStop := False;
  PreviewBullets.Font.Name := 'Segoe UI';
  PreviewBullets.Font.Size := 9;
  PreviewBullets.Lines.Text := Chr(8226) + ' SampleA';

  CheckEndline := TNewCheckBox.Create(TweaksPage);
  CheckEndline.Parent := TweaksPage.Surface;
  CheckEndline.Caption := 'Add single line break between entries';
  CheckEndline.Left := 16;
  CheckEndline.Top := PreviewBullets.Top + PreviewBullets.Height + 8;
  CheckEndline.Width := TweaksPage.SurfaceWidth - 32;
  CheckEndline.Checked := True;

  PreviewEndline := TNewMemo.Create(TweaksPage);
  PreviewEndline.Parent := TweaksPage.Surface;
  PreviewEndline.Left := 32;
  PreviewEndline.Top := CheckEndline.Top + CheckEndline.Height + 2;
  PreviewEndline.Width := TweaksPage.SurfaceWidth - 48;
  PreviewEndline.Height := 36;
  PreviewEndline.ReadOnly := True;
  PreviewEndline.ScrollBars := ssNone;
  PreviewEndline.TabStop := False;
  PreviewEndline.Lines.Text := 'SampleA' + #13#10 + 'SampleB';

  CheckDoubleEndline := TNewCheckBox.Create(TweaksPage);
  CheckDoubleEndline.Parent := TweaksPage.Surface;
  CheckDoubleEndline.Caption := 'Add blank line between entries (double line break)';
  CheckDoubleEndline.Left := 16;
  CheckDoubleEndline.Top := PreviewEndline.Top + PreviewEndline.Height + 8;
  CheckDoubleEndline.Width := TweaksPage.SurfaceWidth - 32;
  CheckDoubleEndline.Checked := False;

  PreviewDouble := TNewMemo.Create(TweaksPage);
  PreviewDouble.Parent := TweaksPage.Surface;
  PreviewDouble.Left := 32;
  PreviewDouble.Top := CheckDoubleEndline.Top + CheckDoubleEndline.Height + 2;
  PreviewDouble.Width := TweaksPage.SurfaceWidth - 48;
  PreviewDouble.Height := 42;
  PreviewDouble.ReadOnly := True;
  PreviewDouble.ScrollBars := ssNone;
  PreviewDouble.TabStop := False;
  PreviewDouble.Lines.Text := 'SampleA' + #13#10 + #13#10 + 'SampleB';
end;

procedure CreateSaveOptionsPage;
var
  NameLabel, LocationLabel: TNewStaticText;
  BrowseButton: TNewButton;
begin
  SaveOptionsPage := CreateCustomPage(TweaksPage.ID, 'Save Options', 'Choose file naming and save location');

  NameLabel := TNewStaticText.Create(SaveOptionsPage);
  NameLabel.Parent := SaveOptionsPage.Surface;
  NameLabel.Caption := 'How should saved files be named?';
  NameLabel.Left := 16;
  NameLabel.Top := 16;

  RadioByName := TNewRadioButton.Create(SaveOptionsPage);
  RadioByName.Parent := SaveOptionsPage.Surface;
  RadioByName.Caption := 'By name (first words of the saved text)';
  RadioByName.Left := 16;
  RadioByName.Top := NameLabel.Top + NameLabel.Height + 8;
  RadioByName.Width := SaveOptionsPage.SurfaceWidth - 32;

  RadioByTimestamp := TNewRadioButton.Create(SaveOptionsPage);
  RadioByTimestamp.Parent := SaveOptionsPage.Surface;
  RadioByTimestamp.Caption := 'By date and time stamp';
  RadioByTimestamp.Left := 16;
  RadioByTimestamp.Top := RadioByName.Top + RadioByName.Height + 8;
  RadioByTimestamp.Width := SaveOptionsPage.SurfaceWidth - 32;
  RadioByTimestamp.Checked := True;

  LocationLabel := TNewStaticText.Create(SaveOptionsPage);
  LocationLabel.Parent := SaveOptionsPage.Surface;
  LocationLabel.Caption := 'Folder to save text files in:';
  LocationLabel.Left := 16;
  LocationLabel.Top := RadioByTimestamp.Top + RadioByTimestamp.Height + 24;

  EditSavePath := TNewEdit.Create(SaveOptionsPage);
  EditSavePath.Parent := SaveOptionsPage.Surface;
  EditSavePath.Left := 16;
  EditSavePath.Top := LocationLabel.Top + LocationLabel.Height + 6;
  EditSavePath.Width := SaveOptionsPage.SurfaceWidth - 112;
  EditSavePath.Text := GetLocalDocumentsPath + '\NOTY_SAVES';

  BrowseButton := TNewButton.Create(SaveOptionsPage);
  BrowseButton.Parent := SaveOptionsPage.Surface;
  BrowseButton.Caption := 'Browse...';
  BrowseButton.Width := 85;
  BrowseButton.Height := EditSavePath.Height;
  BrowseButton.Left := EditSavePath.Left + EditSavePath.Width + 8;
  BrowseButton.Top := EditSavePath.Top;
  BrowseButton.OnClick := @BrowseButtonClick;
end;

procedure CreateFinishPageExtras;
var
  ConnectLabel, HotkeyLabel: TNewStaticText;
  GithubButton: TNewButton;
begin
  WizardForm.FinishedHeadingLabel.Caption := 'Thanks for downloading NOTY!';
  WizardForm.FinishedLabel.Caption := 'NOTY has been installed on your computer.';
  WizardForm.RunList.Top := WizardForm.RunList.Top + 20;

  HotkeyLabel := TNewStaticText.Create(WizardForm);
  HotkeyLabel.Parent := WizardForm.FinishedPage;
  HotkeyLabel.Caption := 'Hotkey to save text: Ctrl+Shift+S';
  HotkeyLabel.Left := WizardForm.FinishedLabel.Left;
  HotkeyLabel.Top := WizardForm.FinishedLabel.Top + WizardForm.FinishedLabel.Height + 28;

  GithubButton := TNewButton.Create(WizardForm);
  GithubButton.Parent := WizardForm.FinishedPage;
  GithubButton.Caption := 'GitHub';
  GithubButton.Width := 90;
  GithubButton.Height := 25;
  GithubButton.Left := WizardForm.FinishedPage.ClientWidth - GithubButton.Width - 16;
  GithubButton.Top := WizardForm.FinishedPage.ClientHeight - GithubButton.Height - 16;
  GithubButton.OnClick := @GithubButtonClick;

  ConnectLabel := TNewStaticText.Create(WizardForm);
  ConnectLabel.Parent := WizardForm.FinishedPage;
  ConnectLabel.Caption := 'Connect with me:';
  ConnectLabel.Left := GithubButton.Left - ConnectLabel.Width - 8;
  ConnectLabel.Top := GithubButton.Top + (GithubButton.Height - ConnectLabel.Height) div 2;
end;

procedure InitializeWizard;
begin
  ExtractTemporaryFile('NOTY.bmp');
  CreateWelcomePage;
  CreateChoicePage;
  CreateTweaksPage;
  CreateSaveOptionsPage;
  CreateFinishPageExtras;
  idpDownloadAfter(wpReady);
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpReady then
  begin
    idpClearFiles;
    if IsTaskSelected('installruntimes') then
    begin
      idpAddFile('https://aka.ms/windowsappsdk/2.2/2.2.0/windowsappruntimeinstall-x64.exe', ExpandConstant('{tmp}\WindowsAppRuntimeInstall-x64.exe'));
      idpAddFile('https://aka.ms/vs/17/release/vc_redist.x64.exe', ExpandConstant('{tmp}\vc_redist.x64.exe'));
    end;
  end;
end;

function ShouldSkipPage(PageID: Integer): Boolean;
begin
  Result := False;
  if RadioDefault.Checked then
  begin
    if (PageID = TweaksPage.ID) or (PageID = SaveOptionsPage.ID) then
      Result := True;
  end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
  IniPath, FinalSavePath: String;
  FinalBullets, FinalEndline, FinalDouble, FinalRenameOption: String;
  XmlContent: String;
  XmlPath: String;
begin
  if CurStep = ssPostInstall then
  begin
    IniPath := ExpandConstant('{userappdata}\NOTY\config.ini');
    ForceDirectories(ExpandConstant('{userappdata}\NOTY'));

    if RadioDefault.Checked then
    begin
      FinalBullets := '1';
      FinalEndline := '1';
      FinalDouble := '0';
      FinalRenameOption := '0';
      FinalSavePath := GetLocalDocumentsPath + '\NOTY_SAVES';
    end
    else
    begin
      if CheckBullets.Checked then FinalBullets := '1' else FinalBullets := '0';
      if CheckEndline.Checked then FinalEndline := '1' else FinalEndline := '0';
      if CheckDoubleEndline.Checked then FinalDouble := '1' else FinalDouble := '0';
      if RadioByTimestamp.Checked then FinalRenameOption := '0' else FinalRenameOption := '1';
      FinalSavePath := ResolveSavePath(EditSavePath.Text);
    end;

    ForceDirectories(FinalSavePath);

    SetIniString('Tweaks', 't_endline', FinalEndline, IniPath);
    SetIniString('Tweaks', 't_bullets', FinalBullets, IniPath);
    SetIniString('Tweaks', 't_double_endline', FinalDouble, IniPath);
    SetIniString('LocationToSaveFile', 'path', FinalSavePath, IniPath);
    SetIniString('Rename_Type', 'option', FinalRenameOption, IniPath);

    XmlContent := 
      '<Task version="1.2" xmlns="http://schemas.microsoft.com/windows/2004/02/mit/task">' + #13#10 +
      '  <RegistrationInfo>' + #13#10 +
      '    <URI>\NOTY</URI>' + #13#10 +
      '  </RegistrationInfo>' + #13#10 +
      '  <Principals>' + #13#10 +
      '    <Principal id="Author">' + #13#10 +
      '      <UserId>' + GetEnv('USERDOMAIN') + '\' + GetUserNameString + '</UserId>' + #13#10 +
      '      <LogonType>InteractiveToken</LogonType>' + #13#10 +
      '      <RunLevel>HighestAvailable</RunLevel>' + #13#10 +
      '    </Principal>' + #13#10 +
      '  </Principals>' + #13#10 +
      '  <Settings>' + #13#10 +
      '    <DisallowStartIfOnBatteries>false</DisallowStartIfOnBatteries>' + #13#10 +
      '    <StopIfGoingOnBatteries>false</StopIfGoingOnBatteries>' + #13#10 +
      '    <MultipleInstancesPolicy>IgnoreNew</MultipleInstancesPolicy>' + #13#10 +
      '    <Enabled>true</Enabled>' + #13#10 +
      '  </Settings>' + #13#10 +
      '  <Triggers>' + #13#10 +
      '    <LogonTrigger>' + #13#10 +
      '      <Enabled>true</Enabled>' + #13#10 +
      '    </LogonTrigger>' + #13#10 +
      '  </Triggers>' + #13#10 +
      '  <Actions Context="Author">' + #13#10 +
      '    <Exec>' + #13#10 +
      '      <Command>' + ExpandConstant('{app}') + '\Noty.exe</Command>' + #13#10 +
      '      <WorkingDirectory>' + ExpandConstant('{app}') + '</WorkingDirectory>' + #13#10 +
      '    </Exec>' + #13#10 +
      '  </Actions>' + #13#10 +
      '</Task>';

    XmlPath := ExpandConstant('{tmp}\task.xml');
    if SaveStringToFile(XmlPath, XmlContent, False) then
    begin
      Exec('schtasks.exe', '/create /tn "NOTY" /xml "' + XmlPath + '" /f', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;

    if IsTaskSelected('installruntimes') then
    begin
      if FileExists(ExpandConstant('{tmp}\WindowsAppRuntimeInstall-x64.exe')) then
        Exec(ExpandConstant('{tmp}\WindowsAppRuntimeInstall-x64.exe'), '--quiet', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
      if FileExists(ExpandConstant('{tmp}\vc_redist.x64.exe')) then
        Exec(ExpandConstant('{tmp}\vc_redist.x64.exe'), '/install /quiet /norestart', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    end;
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usPostUninstall then
    Exec('schtasks.exe', '/delete /tn "NOTY" /f', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
end;
