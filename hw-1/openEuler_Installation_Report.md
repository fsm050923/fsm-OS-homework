# OpenEuler 系统安装报告

## 1. 概述
本报告详细记录了在 VMware 虚拟机上安装 openEuler 操作系统的步骤和过程。openEuler 是一个开源的企业级 Linux 发行版，适用于服务器和云计算环境。

---

## 2. 准备工作

### 2.1 下载 VMware
首先，需要下载并安装 VMware Workstation 或 VMware Player。VMware 是一款功能强大的虚拟机软件，允许用户在单一物理机上运行多个操作系统。

- **下载地址**: [VMware 官网](https://www.vmware.com/)
- **版本**: VMware Workstation 16 Pro

### 2.2 下载 openEuler 镜像
接下来，下载 openEuler 的 ISO 镜像文件。

- **下载地址**: [openEuler 官网](https://openeuler./)
- **版本**: openEuler 20.03 LTS

---

## 3. 安装步骤

### 3.1 创建虚拟机
1. 打开 VMware Workstation。
2. 点击 "Create a New Virtual Machine"。
3. 选择 "Typical" 配置类型，点击 "Next"。
4. 选择 "Installer disc image file (ISO)"，并浏览到下载的 openEuler ISO 文件，点击 "Next"。
5. 选择操作系统类型为 "Linux"，版本为 "Other Linux 5.x kernel 64-bit"，点击 "Next"。
6. 命名虚拟机并选择存储位置，点击 "Next"。
7. 设置磁盘容量，建议至少 20GB，点击 "Next"。
8. 点击 "Finish" 完成虚拟机创建。

### 3.2 安装 openEuler
1. 启动虚拟机，进入 openEuler 安装界面。
2. 选择 "Install openEuler 20.03 LTS" 开始安装。
3. 选择语言和键盘布局，点击 "Continue"。
4. 配置网络和主机名，点击 "Done"。
5. 选择安装源，默认使用本地 ISO 文件，点击 "Done"。
6. 选择软件包，建议选择 "Server with GUI" 以便使用图形界面，点击 "Done"。
7. 配置磁盘分区，可以选择自动分区或手动分区，点击 "Done"。
8. 开始安装，点击 "Begin Installation"。
9. 设置 root 密码和创建用户，点击 "Done"。
10. 等待安装完成，点击 "Reboot" 重启系统。

### 3.3 初始配置
1. 系统重启后，登录 openEuler。
2. 更新系统软件包：
   ```bash
   sudo dnf update -y
   ```
3. 配置防火墙和 SELinux（如果需要）：
   ```bash
   sudo systemctl enable firewalld
   sudo systemctl start firewalld
   sudo setenforce 1
   ```

---

## 4. 结论
通过以上步骤，成功在 VMware 虚拟机上安装并配置了 openEuler 操作系统。openEuler 提供了强大的功能和良好的性能，适用于各种服务器和云计算场景。

---

## 5. 参考
- [VMware 官方文档](https://docs.vmware.com/)
- [openEuler 官方文档](https://openeuler.org/en/docs/)

---

