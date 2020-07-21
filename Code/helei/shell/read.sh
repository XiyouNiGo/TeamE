#!/bin/bash



read -t 3 -p "Please input your name: " name

echo "Name is $name"

read -s -t 30 -p "Please input your age:" age
echo  -e "\n"
echo "Age is $age"

read -n 1 -t 30 -p "Please select your gender[M/F] :" gender
echo -e "\n"

echo "Sex is $gender"



#      echo -e   激活转义字符  
#read  选项  变量名
#-p   “提示信息”   等待read输入时，输出提示信息
#-t   秒数  read命令会一直等待用户输入，用此选项可指定等待时间
#-n 字符数  read命令只接受制定的字符数
#-s 隐藏输入数据
