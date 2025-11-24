Build and run the os
```
docker build -t salsaos .
docker run --rm -v "$PWD":/os:Z salsaos
qemu-system-x86_64 -cdrom iso/build/salsaos.iso -drive file=disk.img,format=raw,index=0,media=disk
```
