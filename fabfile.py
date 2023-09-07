from fabric import task
from fabric import Connection
import os

user = "user"
password = "password"
port = 22

GardenIO_source_path = r"build/Release/GardenIO"
GardenIO_target_path = r"/home/szymon/GardenIO"

@task
def Install(ctx):
    with Connection(host=ctx.host, user=user, port=port, connect_kwargs={'password': password}) as c:
        c.run("mkdir -p " + GardenIO_target_path)
        c.put(GardenIO_source_path, GardenIO_target_path)
        c.run(GardenIO_target_path + r"/GardenIO")