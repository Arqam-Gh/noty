# NOTY

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-00599C)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-active-brightgreen)

**Remainder: NOT VIBE CODED**

Whatever text is selected by the user in any window, pressing the hotkey **Ctrl+Alt+S** makes NOTY create a .txt file at the user-specified save location, copy the text, paste it into the file, save it, and close it — saving you the repetitive steps of note-taking. Until the user creates a new file with **Ctrl+Alt+N**, all text will keep saving into the same .txt file with proper naming and formatting.

## What others don't have

Unlike other vibe-coded or web-stack-heavy apps, NOTY uses under 1MB of RAM. It is:

- Written in C++ with efficient memory management.
- Built on the Win32 API to make the program faster.
- Given a GUI made in WinUI3 (XAML/C++) to keep it as efficient as possible.

NOTY automates the repetitive note-saving steps for you. It runs in the background, takes minimal resources (~1MB of RAM), and does the same job more efficiently than most apps on the market.

## What the GUI offers

With an easy-to-use interface, the GUI lets you:

- See all the .txt files saved so far, view them, edit them, and save changes.
- Change the formatting used when saving text. *(Under development)*

## Manual

| Action | Hotkey |
|---|---|
| Save selected text | **Ctrl+Alt+S** |
| Create a new file | **Ctrl+Alt+N** |
| Exit the program | **Ctrl+Alt+X** |

## Notification system

Whenever you save a file, create a new one, or start/exit NOTY, a notification appears to confirm the action. The notification never steals focus and is small enough to notice without pulling you away from your work.

## Features

- Text can be selected in any window.
- Text is saved as plain .txt files — you can later hand these to an AI to turn into properly formatted PDF notes.
- Two naming methods for your .txt files:
  - By the current time/date
  - By the initial words of the text you copy
- Formatting options available:
  - New line after each save
  - Bullet points before each entry
  - Double new line between entries
- Notifications for save, new file, errors, and program start/stop.
- GUI for faster editing of notes.

## How it flows

```
Select text  -->  Press Ctrl+Alt+S  -->  NOTY reads clipboard
                                              |
                                              v
                                   Applies your naming/formatting
                                              |
                                              v
                          Makes/writes to .txt file  -->  Toast confirms save  -->  Saves and closes the file
```

## Details

- Core app: C++, Win32 API (clipboard, global hotkeys, custom toast window with WM_PAINT)
- Settings stored in a config.ini file in AppData
- Installer: Inno Setup
- Tray/settings UI: WinUI3

## Installing

Download the installer from the Releases page and run it. It sets everything up for you.

## Notes

This is a personal project, built and debugged by me from scratch — including the clipboard handling, the toast notification system, and the installer. If it helps you a little, it will pay off my hard work.

**FEEDBACK = Buying me a COFFEE**

**Email:** arqamgh4@gmail.com
