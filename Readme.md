# NOTY

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-active-brightgreen)

NOTY is a small Windows app I made that saves selected text to a file with one hotkey. Select any text anywhere, press the hotkey, and it gets copied and saved to a .txt file. No need to open notepad, copy, paste, save. One shortcut does it.

Takes less than 1MB. Written in C++ using the raw Windows API.

![NOTY demo](demo.gif)

## How it works

You select text in any window (browser, PDF, chat, anything), then press:

- **Ctrl+Alt+S** — saves the selected text to your file
- **Ctrl+Alt+N** — starts a new file
- **Ctrl+Alt+X** — exits the program

When you save, a small toast notification pops up in the corner near the taskbar to tell you it worked.

## Features

- Global hotkeys, works from any window, you don't need NOTY to be focused
- Saves as plain .txt files
- Two ways to name your files:
  - By time (each save gets a timestamped file)
  - By the first text you copy (renames the file based on what you first save)
- Tweaks you can turn on:
  - New line after each save
  - Bullet points before each entry
  - Double new line (extra blank line between entries)
- Toast notifications for save, new file, errors, and program start/stop
- A small tray UI (built separately in WinUI3) to change settings without editing config files by hand

## Why I made this

I wanted a fast way to dump text I come across into a file without breaking my flow. Opening notepad every time and doing copy-paste-save got annoying, so I built this instead.

## How it flows

```
Select text  -->  Press Ctrl+Alt+S  -->  NOTY reads clipboard
                                              |
                                              v
                                   Applies your naming/tweaks
                                              |
                                              v
                                    Writes to .txt file  -->  Toast confirms save
```

## Tech

- Core app: C++, Win32 API (clipboard, global hotkeys, custom toast window with WM_PAINT)
- Settings stored in a config.ini file in AppData
- Installer: Inno Setup
- Tray/settings UI: WinUI3

## Installing

Download the installer from the Releases page and run it. It sets things up for you, including an option to launch NOTY automatically when you log in to Windows.

## Notes

This is a personal project, built and debugged by me from scratch, including the clipboard handling, toast notification system, and the installer.