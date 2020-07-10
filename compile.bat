python compiler.py -f ./src/AudioBeatMain.cpp -d ./src/headers -r ./AudioBeatMain.exe -e AccelerateFFT -o /EHsc /wd4244 /Fo.\objects\ -DUSE_KISS_FFT


set fS=2205

AudioBeatMain %fS% 44100 "./test music/test(crossfire).wav"

::python grapher.py %fS% "Spectral difference: " "%fS% - SD"
::python grapher.py %fS% "Energy difference: " "%fS% - ED"