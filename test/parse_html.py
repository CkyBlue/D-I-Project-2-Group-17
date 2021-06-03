import os

f = open(os.path.join(os.getcwd(), "test\\test.html"))

for line in f.readlines():
    if (line.strip() == ""): continue
    print('Client.print("' + line.strip().replace('"','\\"') + '");')