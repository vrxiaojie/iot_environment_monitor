<div style="text-align:center;background:linear-gradient(135deg, #2E3192 0%, #1BFFFF 100%);padding:40px 20px;margin:20px 0;border-radius:20px;box-shadow:0 10px 30px rgba(0,0,0,0.3)">
<h1 style="color:#fff;font-size:2.5em;font-weight:900;margin:0;text-shadow:2px 2px 4px rgba(0,0,0,0.5);line-height:1.2">
 <span style="background:linear-gradient(45deg, #FBB03B, #D4145A);-webkit-background-clip:text;-webkit-text-fill-color:transparent;background-clip:text;text-shadow:none">多合一IoT</span> <span style="color:#00ff99">桌面环境监测仪</span>
</h1>
<div style="height:3px;width:200px;background:linear-gradient(90deg, #FBB03B, #00ff99, #D4145A);margin:15px auto;border-radius:2px"></div>
<h2 style="color:#e0e0e0;font-size:1.3em;font-weight:400;margin:15px 0 0 0;text-shadow:1px 1px 2px rgba(0,0,0,0.3)">
💡 全方位环境监测，尽在掌握 💡
</h2>
<div style="margin-top:20px">
<span style="display:inline-block;background:rgba(255,255,255,0.2);color:#fff;padding:8px 16px;margin:5px;border-radius:20px;font-size:0.9em;backdrop-filter:blur(10px)">🌡️ 温湿度</span>
<span style="display:inline-block;background:rgba(255,255,255,0.2);color:#fff;padding:8px 16px;margin:5px;border-radius:20px;font-size:0.9em;backdrop-filter:blur(10px)">💨 CO₂ & VOC</span>
<span style="display:inline-block;background:rgba(255,255,255,0.2);color:#fff;padding:8px 16px;margin:5px;border-radius:20px;font-size:0.9em;backdrop-filter:blur(10px)">☁️ 数据上云</span>
<span style="display:inline-block;background:rgba(255,255,255,0.2);color:#fff;padding:8px 16px;margin:5px;border-radius:20px;font-size:0.9em;backdrop-filter:blur(10px)">📊 可视化</span>
</div>
</div>

## 🔗 网站链接    

1. 项目演示视频
[开源一款小巧的物联网桌面环境监测仪](https://www.bilibili.com/video/BV1NAxJzVEtF/)

2. 以下是教学视频，后续还会继续在B站更新的，这边更新链接可能不会特别及时，请在B站**订阅合集、关注我 ~**
[WSL+VSCode+ESP-IDF安装 | 项目编译 程序烧录教程](https://www.bilibili.com/video/BV1wuxJzuEwY/)

3. 我的技术文档中心，你可以在这里查看文字版的教程：
https://wiki.vrxiaojie.top/

4. 原理图、PCB开源：[立创开源广场-IoT桌面环境监测仪](https://oshwhub.com/vrxiaojie/all-in-one-iot-desktop-environme)

## 项目亮点
✅全方位环境数据监测，专业级传感器方案
✅强大的本地可视化与智能交互系统
✅WIFI连接、MQTT协议数据上云
✅私有化搭建MQTT Broker和InfluxDB数据库
✅自动电源管理，支持旁路充电、锂电池与USB供电自动切换

## 如何烧录程序？
### 使用flash download tool手动烧录
（1）在[GitHub Releases页面](https://github.com/vrxiaojie/iot_environment_monitor/releases)下载最新版本的程序，包括`bootloader.bin`、`partition-table.bin`、`iot_env_monitor.bin`

（2）点击下载 [Flash 下载工具](https://dl.espressif.com/public/flash_download_tool.zip)

（3）将环境监测仪开机，并通过USB type-c连接至电脑，启动`flash_download_tool.exe`，选择芯片 `ESP32S3`，点击OK

（4）参数配置如下：
| 文件名              | 地址    |
| ------------------- | ------- |
| bootloader.bin      | 0x0     |
| partition-table.bin | 0x8000  |
| iot_env_monitor.bin | 0x10000 |

SPI Flash配置为`80MHz`、`QIO`

（5）点击START按钮开始烧录。稍作等待，显示FINISH时，即为烧录成功，此时屏幕是黑屏的，可**先关掉电源再打开电源**，就能看到主界面了

### 基于VSCode ESP-IDF插件的自动烧录
（1）确保你已经成功地在VSCode上安装了ESP-IDF，并能成功编译官方提供的example工程。如果你没有配置，请惨见我的技术文档中心 [安装ESP-IDF（基于VSCode插件）](https://wiki.vrxiaojie.top/iot_env_monitor_guide/1-%E9%A1%B9%E7%9B%AE%E7%BC%96%E8%AF%91/%E5%AE%89%E8%A3%85ESP-IDF%EF%BC%88%E5%9F%BA%E4%BA%8EVSCode%E6%8F%92%E4%BB%B6%EF%BC%89.html)

（2）clone本项目
```shell
git clone https://github.com/vrxiaojie/iot_environment_monitor.git
```

（3）使用VSCode `文件`--`打开文件夹`--`选择本项目的文件夹`

（4）点击底部栏的小扳手图标(build)，等待编译完成

（5）烧录。如果你是在WSL中进行的，需要绑定usb设备到WSL，请安装 [wsl usb manager](https://github.com/nickbeth/wsl-usb-manager)，具体请参阅我的 技术文档中心 [基于VSCode插件的自动烧录](https://wiki.vrxiaojie.top/iot_env_monitor_guide/2-%E7%A8%8B%E5%BA%8F%E7%83%A7%E5%BD%95/%E5%9F%BA%E4%BA%8EVSCode%E6%8F%92%E4%BB%B6%E7%9A%84%E8%87%AA%E5%8A%A8%E7%83%A7%E5%BD%95.html)

## 如何贡献

非常欢迎你的参与！无论是提交 Bug、完善文档、优化代码，还是新增功能，都是对项目的巨大帮助。

你可以通过以下方式贡献：

- 提交 Issue：反馈问题、提供建议或提出新功能需求。
- 提交 Pull Request：修复 Bug、改进实现、补充文档或新增模块。

贡献流程：

1) Fork 仓库，并从 main 创建分支
2) 本地开发并自测（能正常编译、Wi‑Fi 能够扫描/连接、LCD 正常显示、传感器正常读取等）
3) 遵循代码规范与提交规范，编写必要的注释
4) 提交 Pull Request，并在描述中清晰写明背景、变更点

感谢每一位贡献者的时间与努力！也欢迎你在 GitHub 为项目点一颗 Star，或将它分享给更多同好。项目贡献者列表可在 GitHub 的 Contributors 页面查看。

## 常见问题
    
1. Q：连接WIFI时，屏幕会闪烁、卡顿？
A：正常现象。由于使用的屏幕是RGB接口的，内部不自带显存，需要通过ESP32的SPI PSRAM作为显存，用ESP32一直给它传输数据。当WiFi启动连接时，需要进行一系列密集的Flash操作。由于ESP32-S3的Flash和PSRAM是共享SPI总线，在写Flash期间，PSRAM的访问会被暂时挂起。RGB LCD控制器因此无法及时读取帧缓冲区数据，导致传输到屏幕的数据流出现中断或错位。为了解决这个问题，在程序中已通过降低频率、重置屏幕等方式尽可能地避免显示错误，但因此带来的副作用是会出现持续几秒的屏幕闪烁、卡顿现象，当WIFI连接过程结束后，就会恢复正常显示。


2. Q：为什么我系统显示的时间不对？
A：在连接WIFI后，系统将自动使用NTP对时，且每30分钟自动同步一次。请注意，断电后时间信息将会被清除。

3. Q：续航时间多久？如何提高电池续航？
A：使用一节满电的亿纬18650锂电池（标称容量3300mAh），屏幕亮度20%（常亮），数据采样周期5s，此时系统功耗约500mW，理论续航时间约为24小时。通过在设置页面开启省电模式、设置屏幕超时休眠、提高数据采样周期等方式，均可大幅降低系统功耗。此外，本产品支持类似笔记本电脑的电源管理模式的旁路供电模式，当接入USB供电时，电源管理芯片将自动切换至外部供电，同时为电池充电，有效减少电池反复充放电损耗

4. Q：本项目元器件容易用烙铁焊接吗？
A：本项目的传感器为DFN和LGA封装，AW32001是WLCSP封装，因此无法使用电烙铁焊接这些器件！**建议使用加热台/热风枪+钢网或用立创SMT焊接传感器及AW32001**，除此之外的部分均可用烙铁焊接。

5. Q：为什么WIFI名称中会显示方框？
A：这样的WIFI名称通常是含有除了英文字母、英文标点和数字之外的字符，如中文、emoji符号等。这些字符并未被写入字库，且考虑到Flash空间的问题，以后也不会添加中文支持，请使用英文WIFI名。

6. Q：为什么扫描不到我的WIFI？
A：ESP32-S3只支持2.4GHz，不支持5GHz，请检查你的WIFI是否为2.4GHz频段。另外，建议将路由器的双频合一功能关闭，再试着扫描一遍。

7. Q：为什么手动开关一次电源之后，立即开启WIFI，无法自动连接到上一次保存的WIFI？
A：手动关掉设备电源，设备在路由器那边并不会主动断开连接，因此再次开机连接时，路由器会认为该设备已在线而导致设备连接失败。解决方法是：多等几分钟，或在路由器后台踢掉ESP32。

