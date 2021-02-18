#环境搭建(基于debian)

###albert

下载安装包

>https://software.opensuse.org/download.html?project=home:manuelschneid3r&package=albert

安装

>sudo dpkg -i albert_0.16.1_amd64.deb
>sudo apt install albert

修复错误

>sudo apt --fix-broken install

卸载

>apt-get remove package_name 只卸载软件包

>apt-get purge package_name 卸载并清除软件包的配置


###gcc

安装

>sudo apt-get install gcc 

>sudo apt-get install make

>sudo apt-get install gdb

>sudo apt-get install build-essential  

>sudo aptitude install g++

###pip

安装

>sudo apt-get install python python-pip python-dev libssl-dev libffi-dev

>sudo apt-get install python-pip




###pwntools

安装

>pip install --ignore-installed psutil --user

>sudo pip install -U setuptools

>sudo pip install --upgrade pip

>sudo pip install --upgrade pwntools


###git

安装

>apt-get update

>apt-get install git

>git config --global user.name "bee"

>git config --global user.email "youremail@qq.com"


###peda 

安装

>git clone https://github.com/longld/peda.git ~/peda

>echo "source ~/peda/peda.py" >> ~/.gdbinit

###改变root终端颜色和普通用户颜色一样

>sudo cp ~/.bashrc /root/.bashrc
