# GardenIO server

## Compilation
to compile there are few steps to proceed
1. At first lounch You have to create docker image. Go through wsl to ./Docker folder and run
```sh
docker build --tag armhf-compiler-image .
```

2. From wsl run at ./GardenIO/ run:
```sh
./runDocker.sh
```

3.  Run:
```sh
./makeArm.sh
```

4. And to exit run:
```sh
exit
```

## Deploy
Run:
```sh
fab -H 192.168.0.203 Install
```