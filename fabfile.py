from fabric import task
from fabric import Connection
import os

user = "szymon"
# user = r"pi"
# compile = r"Release"
compile = r"Debug"
password = r"2ndRPI@"
# password = r"1stRPI@"
port = 22

GardenIO_source_path = r"build/" + compile + r"/GardenIO"
GardenIO_target_path = r"/home/" + user  + r"/GardenIO"

sqlite_source_path = r"database/sqlite-autoconf-3430000.tar.gz"
sqlite_target_path = r"/home/" + user  + r"/sqlite"

@task
def Install(ctx):
    with Connection(host=ctx.host, user=user, port=port, connect_kwargs={'password': password}) as c:
        c.run("mkdir -p " + GardenIO_target_path)
        c.run("mkdir -p " + GardenIO_target_path + r"/database")
        c.run("mkdir -p " + GardenIO_target_path + r"/configuration")
        c.put("genericDevices.json", GardenIO_target_path + "/configuration/genericDevices.json")
        c.put("devices.json", GardenIO_target_path + "/configuration/devices.json")
        c.put(GardenIO_source_path, GardenIO_target_path)
        c.run(GardenIO_target_path + r"/GardenIO")

@task
def InstallPrerequisites(ctx):
    with Connection(host=ctx.host, user=user, port=port, connect_kwargs={'password': password}) as c:
        c.sudo("apt update")
        c.sudo("apt install -y sqlite3")
        c.sudo("apt install -y mosquitto mosquitto-clients")
        c.sudo("apt install -y libspdlog-dev")
        c.sudo("apt install -y libmodbus-dev")
        c.sudo("apt install -y nlohmann-json3-dev")
        c.sudo("adduser $USER dialout")

