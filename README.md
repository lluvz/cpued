**The project is still in development, which means it is not complete!**
## Usage
Run as a super user e.g. root  

To check cpu information including frequency, voltage, offset voltage and power,run `sudo ./cpued -c` or `sudo ./cpued --check`
example output:  
`Family: 0x06   Model:  0x8e    Threads: 4`  
`Frequency: cpu0:700.073MHz cpu1:700.000MHz cpu2:699.986MHz cpu3:700.036MHz`  
`Voltage: 0.640869v      Power:3.090210w`    
`Core: -125.000000mv     iGPU: 0.000000mv    Cache: -125.000000mv    Agent: 0.000000mv       Analog: 0.000000mv`  
At present, showing voltage and power is only support for some of the intel chips.  

**Warning: undervolting may cause permanent damage to your hardware!**  
To write a offset voltage, run `sudo ./cpued -wv {device} {voltage}`  
{device} can be `core`,`cache`,`igpu`,`agent` or `analog`  
The unit of voltage is mv.  
For example, to undervolt cpu core by -125mv, run
`sudo ./cpued -wv core -125`
On some platform, to make the offset voltage on cpu to take effect, writing to both core and cache is needed.