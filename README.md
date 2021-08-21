# OnBeat

<p align="center">
<img src="logo/logo.png" width=50%>
</p>

## About OnBeat
OnBeat is an early development rhythm game inspired by Osu Mania.
Onset detection is used to automatically generate a beatmap for any file given and lets the user play straight away.

Currently OnBeat lacks a lot of functionality - feel free to help - check out the roadmap here - https://trello.com/b/t8s2mLv0/onbeat-hazel

## How to build and run
1. Clone this repository to your machine using `git clone --recursive https://callumc34/OnBeat`
   - You may need to run `git submodule init` within the vendor/Hazel directory to ensure all submodules are initialised
2. Build Hazel using the provided instructions in the Hazel readme: https://github.com/TheCherno/Hazel#readme
3. Copy any lib files outputted to Lib/ or provide your own link directory using MSVC
4. Download:
   - https://www.fmod.com/download
   - FFTW library for windows using `curl ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll64.zip -o fftw.zip`
   - https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip
5. Move the .lib files to Lib/* and the dll files to the working directory
6. Move the include files to vendor/ 
7. Build using MSVC and you're done!

