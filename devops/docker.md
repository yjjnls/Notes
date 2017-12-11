docker run [OPTIONS] IMAGE [COMMAND] [ARG...]


## host->docker
    sudo docker cp host_path containerID:container_path
    
$sudo docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
e6ef3c5ef80d        newdocker           "/bin/bash"         7 minutes ago       Up 7 minutes                            focused_hawking

$sudo docker cp a.txt e6ef3c5ef80d:/test

## docker->host
    sudo docker cp containerID:container_path host_path

