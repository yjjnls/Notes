docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
$ sudo docker run -t -i newdocker /bin/bash

-i 交互式  
-d 后台运行  
-t allocate a pseudo-TTY  

## 进入docker
* attach
docker ps 查看容器id，然后用attach
$ sudo docker attach container_id
当多个窗口同时使用该命令进入该容器时，所有的窗口都会同步显示。如果有一个窗口阻塞了，那么其他窗口也无法再进行操作。

* exec
$ sudo docker exec -it 775c7c9ee1e1 /bin/bash

# copy file
## host->docker
    sudo docker cp host_path containerID:container_path

$ sudo docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
e6ef3c5ef80d        newdocker           "/bin/bash"         7 minutes ago       Up 7 minutes                            focused_hawking

$ sudo docker cp a.txt e6ef3c5ef80d:/test

## docker->host
$ sudo docker cp containerID:container_path host_path

