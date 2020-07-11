set fS=2205

AudioBeatMain %fS% 44100 "./test music/test(crossfire).wav"

::python ./Graphs/grapher.py %fS% "Spectral difference: " "%fS% - SD"
::python ./Graphs/grapher.py %fS% "Energy difference: " "%fS% - ED"