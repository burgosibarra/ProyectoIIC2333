//FUNCIONES GENERALES
#include "crms_API.h"



Process* process_init(int process_id, char* process_name)
{
    Process* process = malloc(sizeof(Process));
    process -> process_id = process_id;
}

void cr_mount(char* memory_path)
  {
    //Función para guardar ruta de memoria
    file_direction = &(memory_path);      
  }

void cr_ls_process()
  {
      //Funcion que muestra los procesos en ejecucion
  }

int cr_exists(int process_id, char* file_name)
  {
      //Funcion para revisar si existe el archivo en la memoria del proceso
  }

void cr_ls_files(int process_id)
  {
      //Funcion para listar archivos dentro de la memoria del proceso
  }
  

//FUNCIONES PROCESOS
void cr_start_process(int process_id, char* process_name)
{
    //Inicia proceso con nombre process_name y id process_id
}

void cr_finish_process(int process_id)
{
    //Funcion que termina el proceso con id process_id
}


