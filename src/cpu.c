#include<stdio.h>
#include<malloc.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<math.h>
#include<time.h>
#define v2off(v) ((unsigned long long)((int)roundf((float)v*1.024)<<21)&0x00000000ffffffff)
#define off2v(off) ((float)((int)off>>21))/1.024
int check_ability(){
    if(open("/dev/cpu/0/msr",O_RDONLY)<0){
        fprintf(stderr,"Reading msr failed, is this command executed by superuser?\n");
        return -1;
    }
    else return 0;
}
int*cpuid(int eax,int ecx){
    int*output=malloc(32*4);
    asm("movl $0,%%ebx\n\t"
    "movl $0,%%edx\n\t"
    "cpuid\n\t"
    :"=a"(output[0]),"=b"(output[1]),"=c"(output[2]),"=d"(output[3])
    :"a"(eax),"c"(ecx));
    return output;
}
unsigned long long rdmsr(int addr){
    unsigned long long result;
    int fd=open("/dev/cpu/0/msr",O_RDONLY);
    if(fd<0){
        fprintf(stderr,"Writing msr failed, is this command executed by superuser?\n");
        return -1;
    }
    pread(fd,&result,8,addr);
    close(fd);
    return result;
}
int wrmsr(int addr,unsigned long long data){
    int fd=open("/dev/cpu/0/msr",O_WRONLY);
    if(fd<0){
        fprintf(stderr,"Reading msr failed, is this command executed by superuser?\n");
        return -1;
    }
    pwrite(fd,&data,8,addr);
    close(fd);
    return 0;
}
#define rdvol() (float)(short)(rdmsr(408)>>32)/8192.0
int get_threads(){
    FILE*f_threads=popen("grep -c MHz /proc/cpuinfo","r");
    int threads;
    fscanf(f_threads,"%d",&threads);
    pclose(f_threads);
    return threads;
}
void get_freq(int threads,float*data){
    FILE*f_freq=popen("grep MHz /proc/cpuinfo","r");
    for(int i=0;i<threads;i++){
        char buf[100];
        fgets(buf,100,f_freq);
        char*aft=memchr(buf,':',100);
        sscanf(aft+1,"%f",data+i);
    }
    pclose(f_freq);
}
enum DEVICE{
    core,iGPU,cache,agent,analog,digital
};
unsigned long long wrbase=0x8000001100000000;
unsigned long long rdbase=0x8000001000000000;
unsigned long long devbase=0x10000000000;
void adj_volt(enum DEVICE device,float voltage){
    wrmsr(0x150,wrbase+devbase*device+v2off(voltage));
}
float rd_adj_volt(enum DEVICE device){
    wrmsr(0x150,rdbase+devbase*device);
    return off2v(rdmsr(0x150));
}
unsigned rd_family(){
    unsigned eax=cpuid(1,0)[0];
    unsigned result=(eax>>8)&0xf;
    if(result==0xf) result=(eax>>20)&0xff;
    return result;
}
unsigned rd_model(){
    unsigned eax=cpuid(1,0)[0];
    unsigned result=(eax>>4)&0xf;
    unsigned family=(eax>>8)&0xf;
    if(family==0x6 || family==0xf){
        result+=eax>>12&0xf0;
    }
    return result;
}
float rd_power(int interval){//The unit of interval is ms.
    struct timespec req,rem;
    req.tv_sec=0,req.tv_nsec=interval*1000000;
    unsigned long long unit_msr=rdmsr(0x606);
    float unit=powf(0.5,(unit_msr>>8)&31);
    unsigned long long e_msr1=rdmsr(0x611);
    int flag=nanosleep(&req,&rem);
    unsigned long long e_msr2=rdmsr(0x611);
    if(flag<0){
        interval-=rem.tv_nsec/1000000;
    }
    unsigned energy=e_msr2-e_msr1;
    float power=(float)energy*unit*1000.0/(float)interval;
    return power;
}