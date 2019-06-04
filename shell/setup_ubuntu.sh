#! /bin/bash

#bootmode
sudo systemctl set-default multi-user.target	#graphical.target/startx

#utils
sudo apt-get install net-tools
sudo apt-get install ssh-server
sudo apt-get install vim	#gitee/study/shell/vimrc
sudo apt-get install python3.7

#programing
sudo apt-get install build-essential
sudo apt-get install exuberant-ctags
sudo apt-get install cscope

#git
sudo apt-get install git
git config --global core.editor "vim"
git config --golbal user.email "646099228@qq.com"
git config --golbal user.name  "coolen"

#samba
sudo apt-get install samba
sudo apt-get install smbclient
vim /etc/samba/smb.conf
#[Share]
#   path = /home/coolen/Share
#   browseable = yes
#   public = yes
#   writable = yes
sudo service smbd restart
chmod -R go+rwx /home/coolen/share
