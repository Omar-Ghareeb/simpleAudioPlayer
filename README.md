# Simple Audio Player


Simple Audio Player is a dual-instance, cross-platform desktop audio player built with the C++ JUCE framework. It provides two independent player interfaces within a single window, each with its own playlist, waveform display, and playback controls. The application supports common audio formats and features session persistence, remembering the last played track and position for each player upon restart.

## Features

- **Dual Player Interface**: Two fully independent audio players in one application, each with its own controls and playlist.
- **Waveform Visualization**: Real-time audio waveform display with a playhead indicating the current playback position.
- **Comprehensive Playback Controls**:
    - Play, pause, and seek functionality.
    - Jump to the beginning or end of a track.
    - Variable playback speed control (from 0.1x to 2.0x).
    - Standard looping and precise A-B (segment) looping.
    - Quick forward (+10s) and rewind (-10s) buttons.
    - Volume and mute controls.
- **Playlist Management**:
    - Load audio files (`.wav`, `.mp3`, `.aiff`) into a playlist.
    - Add or remove tracks.
    - Navigate the playlist with next/previous track buttons.
    - Click any track in the list to play it immediately.
- **Audio Effects & Markers**:
    - A toggleable reverb effect.
    - Ability to add markers at specific timestamps and instantly jump to them.
- **Metadata Display**: Shows the title, artist, and duration for the currently loaded audio file.
- **Session Persistence**: Automatically saves and reloads the last played file and its position for each player on shutdown and startup.

## Key Components

The project is structured into three main classes:

- **`PlayerAudio`**: The core audio engine that handles all backend logic. This includes loading files, audio transport (play/pause/seek), volume adjustments, speed changes, effects processing, and playlist management.
- **`PlayerGUI`**: Represents the complete user interface for a single player instance. It contains all the buttons, sliders, the waveform display, and the playlist view. It listens for user input and communicates these actions to the `PlayerAudio` engine.
- **`MainComponent`**: The top-level component that hosts the application. It instantiates two `PlayerGUI` instances, arranges them in the main window, and mixes their audio outputs. It also manages saving and loading the session state for both players.

## How to Build

This project is built using the JUCE framework.

### Prerequisites
- A modern C++ compiler (MSVC, Clang, or GCC).
- The [JUCE framework](https://juce.com/get-juce).

### Steps
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/omar-ghareeb/simpleaudioplayer.git
    cd simpleaudioplayer
    ```
2.  **Open in Projucer:**
    Launch the Projucer application (part of the JUCE framework) and open the `simpleAudioPlayer.jucer` file from the cloned repository.

3.  **Configure Paths:**
    In the Projucer, go to the "Modules" section from the left-hand panel. Make sure the `juce_...` modules' paths are correctly set to the location of the `modules` directory in your JUCE installation.

4.  **Generate Project Files:**
    Select your desired exporter (e.g., "Visual Studio 2022") and click the "Save Project and Open in IDE..." button. This will generate the native project files in the `Builds/` directory.

5.  **Build and Run:**
    Open the generated solution/project in your IDE (e.g., Visual Studio or Xcode) and compile and run the application.
