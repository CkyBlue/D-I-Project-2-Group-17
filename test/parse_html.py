import os

f = open(os.path.join(os.getcwd(), "test\\test.html"))

output = ""
for line in f.readlines():
    if (line.strip() == ""): continue
    output += line.strip().replace('"','\\"')  + " "
    if (len(output) > 80):
        print('client.print("' + output + '");')
        output = ""

print('client.print("' + output + '");')