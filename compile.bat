set fS=2205
set sn=grape
set song="./test music/test(%sn%).wav"

OnBeat %fS% 44100 %song%

::mkdir "./Graphs/%sn%""
::python ./Graphs/grapher.py %fS% "Difference: " "/%sn%/%fS% - SD - %sn%"
::python ./Graphs/grapher.py %fS% "Threshold: " "/%sn%/%fS% - SD Threshold - %sn%"
::python ./Graphs/grapher.py %fS% "SD Beat: " "/%sn%/%fS% - SD Beat - %sn%"