/*
 * Ejercicio 5 del TP de Cola de mensajes
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <signal.h>

#define MENSAJE "DATA PARA PROCESO"
#define MQ_PATH "/MQ_TD30"  

   int err, leido;
   char buff[1024];   
   mqd_t mqd = 0; 
   struct mq_attr attr, attr_rcv;
   struct sigevent sev;   
   int retval;


void lee_mensaje(int a){ 

   write (0, "\nleer mensaje\n", sizeof("\nleer mensaje\n"));

   
   //-- Lee en cola de mensajes --------------------------
   leido = mq_receive(mqd, buff, attr_rcv.mq_msgsize, 0);
      if (( leido < 0 )){
         printf ("error en mq_receive()");
         exit(-1); }

      printf("Mensaje leido: %s\n", buff);
}

/**
 * Función que se activa cuando el proceso recibe la señal SIGUSR1, finaliza el proceso
 * @param a PID del proceso???
 */
void finaliza_proceso(int a){ 

   err = mq_close(mqd);
   if (( err < 0 )){
      write (0, "\nerror en mq_close()\n", sizeof("\nerror en mq_close()\n"));
      exit(0);   }
   
   write (0, "\nCola de mensajes cerrada\n", sizeof("\nCola de mensajes cerrada\n"));

   err = mq_unlink(MQ_PATH);
   if(err == -1){
      write (0, "\nerror en mq_unlink()\n", sizeof("\nerror en mq_unlink()\n"));
      exit(0);  }

   write (0, "\nCola de mensajes eliminada\n", sizeof("\nCola de mensajes eliminada\n"));

   write (0, "\nTerminando proceso...\n", sizeof("\nTerminando proceso...\n"));

   exit(0);
}


int main() {
   
   signal(SIGINT, lee_mensaje);    //Ctrl + C; escribe MENSAJE a la cola de mensajes
   signal(SIGUSR1, finaliza_proceso);  //termina; cierra la cola de mensajes y finaliza el proceso

   printf("Soy el proceso: %d \n ",getpid()); //gets the pid of the process

   attr.mq_msgsize = sizeof(buff); //max length of each message
   attr.mq_maxmsg = 5; //max quantity of messages
   
   // abrir de cola de mensajes
    mqd = mq_open(MQ_PATH, O_RDWR | O_CREAT | O_NONBLOCK, 0666, &attr); //abre cola de mensajes, modo lectura/escritura... 
    //crear si no existe y no bloqueante
   if (mqd < 0) {
      printf ("error en mq_open()");   
      exit(-1);  }

   printf("Cola de mensajes creada\n");

 
  if (mq_getattr(mqd, &attr_rcv) == -1) {
      printf ("error en mq_getattr()");
      exit(-1); }
   printf("Nro max. de mensajes en cola de mensajes: %ld\n", attr_rcv.mq_maxmsg);
   printf("Longitud max. de mensaje: %ld\n", attr_rcv.mq_msgsize);
   printf("Nros de mensajes pendientes en cola de mensajes: %ld\n", attr_rcv.mq_curmsgs);


   while(1){//espera las señales

       
   }  //fin while

   exit(0);
}
