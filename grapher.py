import matplotlib.pyplot as plt
import re
import sys

dpi = plt.rcParams["figure.dpi"]
plt.rcParams["figure.figsize"] = (1920.0/float(dpi),
 1080.0/(3.0 * float(dpi)))

logFile = open("./frames.log")
txt = logFile.read()

frameSize = int(sys.argv[1])

samplesPerSecond = 44100 / frameSize

frames = [int(frame) for frame in re.findall("(?<=frame \[)[0-9]+(?=])", txt)]

perChannel = max(frames)

times = [frame/samplesPerSecond for frame in frames] \
[:perChannel]


vals = [float(val) for val in re.findall(
	f"(?<={sys.argv[2]})(.*)(?=\n)", txt)]

vals1 = vals[:perChannel]
vals2 = vals[perChannel:]

print(times)
print(vals1, vals2)

plt.plot(times, vals1, label="Channel 1 - " + sys.argv[2])
plt.plot(times, vals2, label="Channel 2 - " + sys.argv[2])

plt.xticks(range(0, 26))
plt.subplots_adjust(left=0, right=1)
plt.legend()
#plt.show()
plt.savefig(f"{sys.argv[3]}.png")