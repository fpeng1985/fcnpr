#!/usr/bin/env python
import sys
import subprocess
import resource

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

iterations=10

try:
    for i in range(1,iterations):
        subprocess.check_output(["build/main", "-i", "circuits/C17.v", "-o", "output.json"])
except subprocess.CalledProcessError:
    eprint("Failure to execute process")

average_exec_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime / iterations

print(average_exec_time)
