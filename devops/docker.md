# image
## download image
docker pull [NAME]/[CONTAINER ID]

## list local image
docker images

## remove local image
docker rmi [NAME]/[CONTAINER ID]


# create and start container
## interatcive
docker run [OPTIONS] IMAGE [COMMAND] [ARG...]
`$ sudo docker run -it --name NAME IMAGE /bin/bash`

-i 交互式  
-d 后台运行  
-t allocate a pseudo-TTY  

--rm: Once the container exits, remove the container. Helps to keep clean or hard drive.
用rm启动的container是一次性的

# start container
$ sudo docker start continer_id

### login container
* attach
docker ps 查看容器id，然后用attach
$ sudo docker attach container_id
当多个窗口同时使用该命令进入该容器时，所有的窗口都会同步显示。如果有一个窗口阻塞了，那么其他窗口也无法再进行操作。

* exec
$ sudo docker exec -it 775c7c9ee1e1 /bin/bash

**所有登陆该container的用户退出后，该container就停止了**

## detach
$ docker run -p 100:22 -d newdocker /bin/bash

从输出的 port 列，我们可以看到 0.0.0.0:100->22/tcp ，它表示该容器映射了宿主主机的 100 端口到容器的 22 端口，即 ssh 服务端。  
**这样我们就创建了一个具有 ssh 服务的容器**  

### login container
$ ssh root@127.0.0.1 -p 100
通过ssh登录，退出后，该container也不会停止

# stop container
$ sudo docker stop continer_id

# remove container
$ sudo docker rm xxx

# copy file
## host->docker
    sudo docker cp host_path containerID:container_path

$ sudo docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
e6ef3c5ef80d        newdocker           "/bin/bash"         7 minutes ago       Up 7 minutes                            focused_hawking

$ sudo docker cp a.txt e6ef3c5ef80d:/test

## docker->host
$ sudo docker cp containerID:container_path host_path

