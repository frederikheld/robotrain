import subprocess

# prints directly to terminal:
subprocess.run(["sh", "hello-world.sh"])

# stores result, prints with byte prefix:
result = subprocess.check_output(["sh", "hello-world.sh"])
print(result)

# passes result to another shell script:
subprocess.run(["sh", "parrot.sh", result])