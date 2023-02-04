**项目仍在开发中，这意味着它可能不完整。**
## 使用方法  
请用超级用户（如root）运行该程序  

1.查看cpu状态  
要查看cpu运行状态和信息（频率，电压，功耗等），运行`sudo ./cpued -c`或者`sudo ./cpued --check`  
输出例子：  
`Family: 0x06   Model:  0x8e    Threads: 4`  
`Frequency: cpu0:700.073MHz cpu1:700.000MHz cpu2:699.986MHz cpu3:700.036MHz`  
`Voltage: 0.640869v      Power:3.090210w`    
`To read the offset voltage, use '-rv' or '--read_voltage'. It may cause potential damage to unsupported device.`  
目前，仅有部分intel处理器支持查看电压和功耗  

2.获取偏移电压  
**警告：在不支持FIVR的平台上读写偏移电压可能导致潜在问题**  
要获取偏移电压，运行`sudo ./cpued -rv`或者`sudo ./cpued --read_voltage`  
输出例子：  
`Core: -125.000000mv     iGPU: 0.000000mv        Cache: -125.000000mv    Agent: 0.000000mv       Analog: 0.000000mv`  

3.写入偏移电压
**警告：修改电压可能会造成硬件的永久损伤**  
要写入偏转电压，运行`sudo ./cpued -wv {device} {voltage}`  
{device}可以是`core`,`cache`,`igpu`,`agent`或者`analog`  
{voltage}的单位是mv  
例如，将cpu核心电压调低-125mv，运行  
`sudo ./cpued -wv core -125`  
对于某些平台的cpu，为使cpu降压生效，需要将核心(core)和缓存(cache)都调节

## 未来计划  
- 图形界面  
- 调节cpu频率或频率策略  
- 分辨cpu是否可以使用FIVR调节电压  
- 开机自动设置偏移电压  
- 更精确的功耗测量  
- 打包为`.deb`,`.rpm`或者`.tar`