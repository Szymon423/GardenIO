from fabric import task
from fabric import Connection
import os

user = "szymon"
password = "2ndRPI@"
port = 22

GardenIO_source_path = r"build/Release/GardenIO"
GardenIO_target_path = r"/home/szymon/GardenIO"

sqlite_source_path = r"database/sqlite-autoconf-3430000.tar.gz"
sqlite_target_path = r"/home/szymon/sqlite"

@task
def Install(ctx):
    with Connection(host=ctx.host, user=user, port=port, connect_kwargs={'password': password}) as c:
        c.run("mkdir -p " + GardenIO_target_path)
        c.run("mkdir -p " + GardenIO_target_path + r"/database")
        c.put(GardenIO_source_path, GardenIO_target_path)
        c.run(GardenIO_target_path + r"/GardenIO")

@task
def InstallSQLite(ctx):
    with Connection(host=ctx.host, user=user, port=port, connect_kwargs={'password': password}) as c:
        c.sudo("sudo apt update")
        c.sudo("sudo apt install -y sqlite3")
        