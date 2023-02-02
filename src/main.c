#include"cpu.c"
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
void print_bin64(unsigned long long bin){
    unsigned long long a=1;
    a=a<<63;
    for(int i=0;i<=63;i++){
        printf("%d",(bin&(a>>i))&&1);
        if(i%8==7) printf(" ");
    }
    printf("\n");
}
int main(int argc,char*argv[]){
    if(argc==1){
        printf("-c --check : check the current cpu status ,e.g. voltage frequency...\n");
        return 0;
    }
    if(!(strcmp(argv[1],"--check")&&strcmp(argv[1],"-c"))){
        //reading voltage
        float vol=rdvol();
        if(vol<0.0) printf("Reading msr failed.\nVoltage and power info can't be read.\nIs this command executed by the superuser, or is msr module loaded into the kernel?");
        else{
            printf("Voltage: %fv\t",rdvol());
            printf("Power:%fw\n",rd_power(100));
            printf("Core: %fmv\t",rd_adj_volt(0));
            printf("iGPU: %fmv\t",rd_adj_volt(1));
            printf("Cache: %fmv\t",rd_adj_volt(2));
            printf("Agent: %fmv\t",rd_adj_volt(3));
            printf("Analog: %fmv\n",rd_adj_volt(4));
        }
        //reading the number of threads
        int threads=get_threads();
        printf("Family: 0x%02x\tModel: 0x%02x\t",rd_family(),rd_model());
        printf("Threads: %d\n",threads);
        //reading frequency on every thread
        printf("Frequency: ");
        float freq[threads];
        get_freq(threads,freq);
        for(int i=0;i<threads;i++) printf("cpu%d:%.3fMHz ",i,freq[i]);
        printf("\n");
    }
    else if(!(strcmp(argv[1],"-wv")&&strcmp(argv[1],"--write_voltage"))){
        if(check_ability()<0) return -1;
        if(argc<3){
            printf("Please specify the device to modify.\n");
            return 0;
        }
        else if(argc<4){
            printf("Please specify the offset voltage to apply.");
            return 0;
        }
        else{
            enum DEVICE device;
            if(!strcmp(argv[2],"core")) device=0;
            else if(!strcmp(argv[2],"gpu")) device=1;
            else if(!strcmp(argv[2],"cache")) device=2;
            else if(!strcmp(argv[2],"agent")) device=3;
            else if(!strcmp(argv[2],"analog")) device=4;
            float voltage=atof(argv[3]);
            adj_volt(device,voltage);
            printf("The offset voltage is set to %fv, which may not perfectly match the input one.",rd_adj_volt(device));
        }
    }
    else if(!(strcmp("-rdmsr",argv[1]))){
        if(argc<3){
            printf("Please specify the address to read.");
        }
        else{
            print_bin64(rdmsr(atoi(argv[2])));
        }
    }
    else{
        printf("Please input the valid arguments.\n");
    }
}