# Sample React Native for Windows - Flatlist - XBox controller key handling.

## Prerequisites
**Required React Native for Windows Version:** react-native-windows_v0.64.0-preview.12 or greater

## Installation
1. Clone React Native for Windows repo using tag ```react-native-windows_v0.64.0-preview.12``` 

2. In a seperate folder, clone this repo.

3. Copy this folder to ```react-native-windows\packages\microsoft-reactnative-sampleapps\``` in React Native for Windows repo cloned in above step. This will overwrite ```index.windows.js```.
4. Load solution FlatListXbox into Visual Studio.
5. Set target architecture to ```x64``` (default is ARM).
6. Build and run app.

## How it works
The app installs a hook on for the OnKeyDown event. This is needed to capture the 'X'. 'Y' and 'B' (Back / Escape) buttons an XBox controller. Note that the framework includes processing for the 'A' and DPad buttons.






