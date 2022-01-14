# This Python file uses the following encoding: utf-8

# if __name__ == "__main__":
#     pass
from paramiko import SSHClient
import paramiko
import os
import sys

client = SSHClient()
client.load_system_host_keys()
client.load_host_keys('C:/Users/Nik/.ssh/known_hosts')
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())

client.connect(str(sys.argv[1]), username=str(sys.argv[2]), password=str(sys.argv[3]))

# Run a command (execute PHP interpreter)
comm = sys.argv[4]

if(comm == "hard"):
    comm = "systeminfo"
else:
    comm = "wmic product get name,version"

stdin, stdout, stderr = client.exec_command(comm)

print(type(stdin))  # <class 'paramiko.channel.ChannelStdinFile'>
print(type(stdout))  # <class 'paramiko.channel.ChannelFile'>
print(type(stderr))  # <class 'paramiko.channel.ChannelStderrFile'>

# Optionally, send data via STDIN, and shutdown when done
stdin.write('<?php echo "Hello!"; sleep(2); ?>')
stdin.channel.shutdown_write()




results = stdout.read().decode("utf8")

# Print output of command. Will wait for command to finish.
print(f'STDOUT: {results}')
print(f'STDERR: {stderr.read().decode("utf8")}')

# Get return code from command (0 is default for success)
print(f'Return code: {stdout.channel.recv_exit_status()}')

filename = sys.argv[2] + sys.argv[4] +".txt"

with open(filename, 'w') as f:
    f.write(results)

# Because they are file objects, they need to be closed
stdin.close()
stdout.close()
stderr.close()

#os.system("test.txt")


# Close the client itself
client.close()
