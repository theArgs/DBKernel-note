## 1 Ubuntu虚拟机安装

环境使用的是Ubuntu-18.04.6-desktop-amd64.iso。

### 1.1 创建虚拟机

1. 第一步创建新的虚拟机。

   <img src="img\00_1.png" alt="00_1" width="50%" height="50%" />

2. 选择自定义（高级）。

   <img src="img\00_2.png" alt="00_2"  />

3. 选择硬件兼容性。

   <img src="img\00_3.png" alt="00_3"  />

4. 选择稍后安装操作系统。

   <img src="img\00_4.png" alt="00_4"  />

5. 选择客户机操作系统。

   <img src="img\00_5.png" alt="00_5"  />

6. 自定义虚拟机名称。

   <img src="img\00_6.png" alt="00_6"  />

7. 指定处理器数量。

   <img src="img\00_7.png" alt="00_7"  />

8. 指定虚拟机内存大小。

   <img src="img\00_8.png" alt="00_8"  />

9. 使用网络地址转换。

   <img src="img\00_9.png" alt="00_9"  />

10. 选择I/O控制器类型。

    <img src="img\00_10.png" alt="00_10"  />

11. 选择磁盘类型。

    <img src="img\00_11.png" alt="00_11"  />

12. 选择磁盘。

    <img src="img\00_12.png" alt="00_12"  />

13. 指定磁盘容量（可以大一点）。

    <img src="img\00_13.png" alt="00_13"  />

14. 指定磁盘文件。

    <img src="img\00_14.png" alt="00_14"  />

15. 已准备好。

    <img src="img\00_15.png" alt="00_15"  />

### 1.2 安装操作系统

1. 编辑虚拟机设置

   <img src="img\01_1.png" alt="01_1" width="67%" height="67%"  />

2. 选择已经下载好的操作系统ISO映像文件。

   <img src="img\01_2.png" alt="01_2" width="67%" height="67%" />

3. 保存后开启此虚拟机，之后选择安装Ubuntu。

   <img src="img\01_5.png" alt="01_5" width="67%" height="67%" />

4. 键盘布局选择Chinese并继续。

   <img src="img\01_6.png" alt="01_6" width="67%" height="67%" />

5. 这里选择了最小安装。

   <img src="img\01_7.png" alt="01_7" width="67%" height="67%" />

6. 选择安装类型为清除整个磁盘并安装Ubuntu，然后点击Install Now。

   <img src="img\01_8.png" alt="01_8" width="67%" height="67%" />

7. 确认安装。

   <img src="img\01_9.png" alt="01_9" width="67%" height="67%" />

8. 输入个人信息，然后Continue。

   <img src="img\01_10.png" alt="01_10" width="67%" height="67%" />

9. 安装完成后重启。

### 1.3 设置静态IP地址

1. 修改虚拟网络编辑器配置。

   <img src="img\02_1.png" alt="02_1" width="67%" height="67%" />

   选择Vmnet8 Net网络连接方式，随意设置子网IP，点击NAT设置页面。

   <img src="img\02_2.png" alt="02_2" width="67%" height="67%" />

   设置子网掩码和网关，后续使用。

   <img src="img\02_3.png" alt="02_3" width="67%" height="67%" />

   设置起始和结束IP地址。

   <img src="img\02_8.png" alt="02_8" width="67%" height="67%" />

   修改虚拟机网络适配器为VMnet8（NAT模式）。

   <img src="img\02_7.png" alt="02_7" width="67%" height="67%" />

2. 修改宿主机VMnet8网卡设置。

   <img src="img\02_4.png" alt="02_4" width="80%" height="80%" />

3. 修改虚拟机静态IP。

   <img src="img\02_5.png" alt="02_5" width="50%" height="50%" />

   <img src="img\02_6.png" alt="02_6" width="67%" height="67%" />

4. 重启网络服务。测试可以ping通网络和主机。

   <img src="img\02_9.png" alt="02_9" width="80%" height="80%" />

### 1.4 更换阿里云源

**备份/etc/apt/sources.list文件**

``` shell
sudo mv /etc/apt/sources.list /etc/apt/sources.list.backup
```

**新建/etc/apt/sources.list文件并添加内容**

``` shell
sudo vi /etc/apt/sources.list
```

``` shell
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
```

**保存后执行如下命令**

``` shell
sudo apt update
sudo apt upgrade
```

### 1.5 安装SSH

```shell
sudo apt install openssh-server
```

``` shell
sudo service ssh start
```

## 2 VS Code

### 2.1 安装VS Code

### 2.2 安装Remote-SSH插件

<img src="img\03_1.png" alt="03_1" width="67%" height="67%" />

### 2.3 配置Remote-SSH

使用快捷键`Ctrl+Shift+p`打开面板，输入ssh，选择Remote-SSH:Settings。

<img src="img\03_2.png" alt="03_2" width="80%" height="80%" />

找到并勾选`Always reveal the SSH login terminal`选项。

<img src="img\03_3.png" alt="03_3" width="80%" height="80%" />

填写config文件。

<img src="img\03_4.png" alt="03_4" width="80%" height="80%" />

在打开的配置文件中填入如下内容：

``` shell
# Read more about SSH config files: https://linux.die.net/man/5/ssh_config
Host ubuntu
    HostName 192.168.1.3
    User pannnn
```

### 2.4 连接并打开文件夹

点击SSH TARGETS下服务器的按钮进行连接，按照提示输入密码。

<img src="img\03_5.png" alt="03_5" width="80%" height="80%" />

连接后即可打开新的终端和文件夹作为工作区。

<img src="img\03_6.png" alt="03_6" width="80%" height="80%" />









