## Usage
Run as a super user e.g. root  

1. check cpu status  
To check cpu information including frequency, voltage and power,run `sudo ./cpued -c` or `sudo ./cpued --check`  
Reading voltage and power is only supported on some intel platforms.  
example output:  
`Family: 0x06   Model:  0x8e    Threads: 4`  
`Frequency: cpu0:700.073MHz cpu1:700.000MHz cpu2:699.986MHz cpu3:700.036MHz`  
`Voltage: 0.640869v      Power:3.090210w`    
`To read the offset voltage, use '-rv' or '--read_voltage'. It may cause potential damage to unsupported device.`  
At present, showing voltage and power is only supported for some of the intel chips.  

2. read offset voltage  
**Warning: reading or writing offset voltage on device which do not support FIVR may cause potential problem**
To read the offset voltage, run `sudo ./cpued -rv` or `sudo ./cpued --read_voltage`  
example output:  
`Core: -125.000000mv     iGPU: 0.000000mv        Cache: -125.000000mv    Agent: 0.000000mv       Analog: 0.000000mv`  

3. write offset voltage
**Warning: addjusting voltage may cause permanent damage to your hardware!**  
To write a offset voltage, run `sudo ./cpued -wv {device} {voltage}`  
{device} can be `core`,`cache`,`igpu`,`agent` or `analog`  
The unit of {voltage} is mv.  
For example, to undervolt cpu core by -125mv, run  
`sudo ./cpued -wv core -125`  
On some platform, to make the offset voltage on cpu to take effect, writing to both core and cache is needed.

4. use script to save time 
The file cpued.sh is a script which be used to execute multiple commands.**Warning: The file is only an example, which may not suit your mechine. Please change it for your own when needed** 
To run it, execute `chmod +x cpued.sh` to make it executable. Afterwards, you can use
`sudo ./cpued.sh` to execute it(no need to rerun `chmod`). 

## Future plan  
- GUI  
- Adjusting frequency or frequency plan  
- Identify CPU that support undervolt by FIVR  
- Auto set offset voltage when starting up  
- More precise power monitor  
- Make package into `.deb`,`.rpm` or `.tar`