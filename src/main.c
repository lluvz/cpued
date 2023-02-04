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
void help(){
    printf("-c or --check\t: check the current cpu status ,e.g. voltage, frequency...\n");
    printf("-wv or --write_voltage\t: write offset voltage (unit: mv) to specific device.\n");
    printf("-rv or --read_voltage\t: read offset voltage set to the devices.\n");
}
int main(int argc,char*argv[]){
    if(argc==1){
        help();
        return 0;
    }
    if(!(strcmp(argv[1],"--help")&&strcmp(argv[1],"-h"))){
        help();
    }
    else if(!(strcmp(argv[1],"--check")&&strcmp(argv[1],"-c"))){
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
        //reading voltage & power
        if(check_ability()<0){
            return -1;
        }
        else{
            printf("Voltage: %fv\t",rdvol());
            printf("Power:%fw\n",rd_power(100));
            printf("To read the offset voltage, use '-rv' or '--read_voltage'. It may cause potential damage to unsupported device.\n");
        }
    }
    else if(!(strcmp(argv[1],"-rv")&&strcmp(argv[1],"--read_voltage"))){
        if(check_ability()<0){
            return -1;
        }
        else{
            printf("Core: %fmv\t",rd_adj_volt(0));
            printf("iGPU: %fmv\t",rd_adj_volt(1));
            printf("Cache: %fmv\t",rd_adj_volt(2));
            printf("Agent: %fmv\t",rd_adj_volt(3));
            printf("Analog: %fmv\n",rd_adj_volt(4));
        }
    }
    else if(!(strcmp(argv[1],"-wv")&&strcmp(argv[1],"--write_voltage"))){
        if(check_ability()<0) return -1;
        if(argc<3){
            printf("Please specify the device to modify.\n");
            return -1;
        }
        else if(argc<4){
            printf("Please specify the offset voltage to apply.\n");
            return -1;
        }
        else{
            enum DEVICE device;
            if(!strcmp(argv[2],"core")) device=0;
            else if(!strcmp(argv[2],"igpu")) device=1;
            else if(!strcmp(argv[2],"cache")) device=2;
            else if(!strcmp(argv[2],"agent")) device=3;
            else if(!strcmp(argv[2],"analog")) device=4;
            float voltage=atof(argv[3]);
            adj_volt(device,voltage);
            printf("The offset voltage is set to %fv, which may not perfectly match the input one.\n",rd_adj_volt(device));
        }
    }
    else if(!(strcmp("-rdmsr",argv[1]))){
        if(argc<3){
            printf("Please specify the address to read.\n");
        }
        else{
            print_bin64(rdmsr(atoi(argv[2])));
        }
    }
    else{
        printf("Please input the valid arguments.\n");
        help();
    }
}
