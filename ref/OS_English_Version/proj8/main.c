#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define TLB_SIZE 16
#define FRAME_NUMBER 256
#define FRAME_SIZE 256
#define PAGE_TABLE_SIZE 256

typedef struct TLB{
    int latest_used;
    int page_number;
    int frame_number;
}tlb_t;

typedef struct PAGE_TABLE{
    int valid;
    int frame_number;
}page_table_t;

typedef struct memory{
    int latest_used;
    char data[FRAME_SIZE];
}mem_t;

tlb_t tlb[TLB_SIZE];
page_table_t page_table[PAGE_TABLE_SIZE];
mem_t mem[FRAME_NUMBER];

void init();
void close_file();
void LRU_tlb(int, int);
void LRU_mem(int, int);

int time, tlb_hit_number, page_fault_number, count;
FILE* addr, *out, *bin;

int address;



int main(int argc, char*argv[]){
    assert(argc == 2);
    init();
    addr = fopen(argv[1], "r");
    out = fopen("out.txt", "w");
    bin = fopen("BACKING_STORE.bin", "rb");
    fscanf(addr, "%d", &address);
    while(!feof(addr)){
        time++;
        count++;
        int offset = address & 0x000000ff;
        int page_number = (address >> 8) & 0x000000ff;
        int in_tlb = 0;
        int page_fault = 1;
        int frame_id;
        for(int i = 0;i < TLB_SIZE;++i){
            if(page_number == tlb[i].page_number){
                tlb_hit_number ++;
                in_tlb = 1;
                page_fault = 0;
                frame_id = tlb[i].frame_number;
                mem[frame_id].latest_used = time;
                tlb[i].latest_used = time;
                LRU_tlb(page_number, frame_id);
                break;
            }
        }
         

        if(in_tlb == 0 && page_table[page_number].valid){
            page_fault = 0;
            frame_id = page_table[page_number].frame_number;
            mem[frame_id].latest_used = time;
            LRU_tlb(page_number, frame_id);
        }


        if(page_fault){
            page_fault_number ++;
            page_table[page_number].valid = 1;
            //printf("%d\n", page_number);
            int min_time=1e9, min_index=0;
            for(int i = 0;i < FRAME_NUMBER;++i){
                if(mem[i].latest_used < min_time){
                    min_index = i;
                    min_time = mem[i].latest_used;
                }
            }
            for(int i = 0;i < PAGE_TABLE_SIZE;++i){
                if(page_table[i].frame_number == min_index && page_table[i].valid){
                    page_table[i].valid = 0;
                    break;
                }
            }
            LRU_mem(min_index, page_number);
            frame_id = min_index;
            page_table[page_number].frame_number = frame_id;
            LRU_tlb(page_number,frame_id);

        }
        //printf("file opened---\n");
        //printf("%d %d", frame_id, offset);
        int data = mem[frame_id].data[offset];

        fprintf(out, "Virtual address: %d Physical address: %d Value: %d\n", address, frame_id * FRAME_SIZE + offset, data);
        fscanf(addr, "%d", &address);
    }

    double tlb_hit_rate = (double)tlb_hit_number / count;
    double page_fault_rate = (double)page_fault_number / count;
    printf("TLB hit rate: [%d], page fault rate : [%f]\n", tlb_hit_number, page_fault_rate);
    close_file();
    return 0;
}

void init(){
    for(int i = 0;i < TLB_SIZE;++i){
        tlb[i].frame_number = -1;
        tlb[i].latest_used = -1;
        tlb[i].latest_used = 0;
    }
    for(int i = 0;i < PAGE_TABLE_SIZE;++i){
        page_table[i].frame_number = -1;
        page_table[i].valid = 0;
    }
    for(int i = 0;i < FRAME_NUMBER;++i){
        mem[i].latest_used = 0;
    }
    time = tlb_hit_number = page_fault_number = count = 0;
    printf("space initialized----\n");
}


void LRU_tlb(int page_id, int frame_id){
    int min_time=1e9, min_index=0;
    for(int i = 0;i < TLB_SIZE;++i){
        if(tlb[i].latest_used < min_time){
            min_time = tlb[i].latest_used;
            min_index = i;
        }
    }
    tlb[min_index].frame_number = frame_id;
    tlb[min_index].page_number = page_id;
    tlb[min_index].latest_used = time;
    return;
}


void LRU_mem(int frame_number, int page_number){
    mem[frame_number].latest_used = time;
    fseek(bin, page_number * FRAME_SIZE, SEEK_SET);
    fread(mem[frame_number].data, sizeof(char), FRAME_SIZE, bin);
    return;
}

void close_file(){
    fclose(bin);
    fclose(addr);
    fclose(out);
    return;
}