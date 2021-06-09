import os

f = open(os.path.join(os.getcwd(), "test\\highcharts.js"))
f2 = open(os.path.join(os.getcwd(), "output.txt"), "w")

output = ""
for line in f.readlines():
    if (line.strip() == ""): continue
    output += line.strip().replace("\\", "\\\\").replace('"','\\"') + " "
    if (len(output) > 3000):
        f2.write('"' + output + '"\\' + "\n")
        output = ""

f2.write('"' + output + '"')