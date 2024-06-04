#!/bin/bash

cp mini-p2p /home/ec/node0
for i in {1,2,3,4,5}
do
	sshpass -p zhangzhao scp mini-p2p  zz@10.11.7.7$i:/home/zz/ec/node$i
done

