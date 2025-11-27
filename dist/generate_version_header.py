import sys

major, minor, patch = sys.argv[1:4]

template = "include/Version.h.in"
output = "include/Version.h"

content = open(template).read()
content = content.replace("@MAJOR@", major)
content = content.replace("@MINOR@", minor)
content = content.replace("@PATCH@", patch)

with open(output, "w") as f:
    f.write(content)

print(f"Generated Version.h → {major}.{minor}.{patch}")
