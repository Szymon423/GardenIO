# GardenIO server

## Compilation
to compile there are few steps to proceed
### At first lounch You have to create docker image. Go ./Docker folder and run
```sh
docker build --tag armhf-compiler-image .
```

### Clone essential repos
```sh
cd external
```
```sh
git clone https://github.com/nlohmann/json.git
```
```sh
git clone https://github.com/gabime/spdlog.git
```

### To Run Docker container use:

If you're running this from WSL use:
```sh
./runDocker.sh -w
```
If you're running this on Linux use
```sh
./runDocker.sh
```

### Run:

To compile in release mode use:
```sh
./makeArm.sh
```
In debug:
```sh
./makeArm.sh -d
```

### And to exit:
```sh
exit
```

## Deploy
Run:
```sh
fab -H 192.168.0.203 Install
```
