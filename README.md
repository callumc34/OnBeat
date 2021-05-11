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
4. Download FMOD studio from: https://www.fmod.com/download and copy the dlls and libraries provided to the working directory along with the FMOD include headers
5. Download the FFTW library for windows using `curl ftp://ftp.fftw.org/pub/fftw/fftw-3.3.5-dll64.zip -o fftw.zip`
   - Copy the dll and library files provided to the working directory along with the FFTW include header to vendor/gist/src/
6. Download the ultralight SDK from https://github.com/ultralight-ux/ultralight/releases
   - Copy the dll and library files to the working directory and copy the include folders to the vendor/ultralight folder.
6. Build using MSVC and you're done!

