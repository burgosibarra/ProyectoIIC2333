#pragma once

struct process;

typedef struct process Process;

struct process
{
    int process_id;
    char* process_name;
    //Atributos
};

int num;

char** file_direction;




Process* process_init(int process_id, char* process_name);
void cr_mount(char* memory_path);
void cr_ls_process();
int cr_exists(int process_id, char* file_name);
void cr_ls_files(int process_id);
void cr_start_process(int process_id, char* process_name);
void cr_finish_process(int process_id);