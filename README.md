## 智能农场

### 简介
采集农场环境信息,进行数据处理,并通过蓝牙发送至网关,网关发送至云平台,实现农场物联网的实现.这里只包含节点部分.  
基于野火指南者开发板, 各个模块通过杜邦线相连接.

### 功能实现:  
1. 采集温湿度,光照值,烟雾值  
2. 将采集信息进行实时显示  
3. 若温度值超标, 通过继电器控制风扇(电机模拟)打开  
4. 若光照值过低, 打开灯  
5. 若温度值超标, 光照值过低, 烟雾值超标, 均进行语音播报  
6. 通过蓝牙与网关互相通信, 发送采集数据, 可接收指令控制风扇和灯


### 手残框图:
[![Dxx0mj.png](https://s3.ax1x.com/2020/12/07/Dxx0mj.png)](https://imgchr.com/i/Dxx0mj)