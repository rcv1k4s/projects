#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>   
#include<sys/ioctl.h>
#include<unistd.h>  
#include<iostream>
#include<fstream>
#include<errno.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int receive_image(int socket)
{ 
// Start function 

int buffersize = 0, recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat;

char imagearray[10241],verify = '1';
FILE *image;

//Find the size of the image
do{
stat = read(socket, &size, sizeof(int));
}while(stat<0);

printf("Packet received.\n");
printf("Packet size: %i\n",stat);
printf("Image size: %i\n",size);
printf(" \n");

char buffer[] = "Got it";

//Send our verification signal
do{
stat = write(socket, &buffer, sizeof(int));
}while(stat<0);

printf("Reply sent\n");
printf(" \n");
image = fopen("/home/viki/Documents/RTOS/retest/client/rimageName.jpeg", "w");

if( image == NULL) {
printf("Error has occurred. Image file could not be opened\n");
return -1; }

//Loop while we have not received the entire file yet


int need_exit = 0;
struct timeval timeout = {10,0};

fd_set fds;
int buffer_fd, buffer_out;

while(recv_size < size) {
//while(packet_index < 2){

    FD_ZERO(&fds);
    FD_SET(socket,&fds);

    buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout);

    if (buffer_fd < 0)
       printf("error: bad file descriptor set.\n");

    if (buffer_fd == 0)
       printf("error: buffer read timeout expired.\n");

    if (buffer_fd > 0)
    {
        do{
               read_size = read(socket,imagearray, 10241);
            }while(read_size <0);

            printf("Packet number received: %i\n",packet_index);
        printf("Packet size: %i\n",read_size);


        //Write the currently read data into our image file
         write_size = fwrite(imagearray,1,read_size, image);
         printf("Written image size: %i\n",write_size); 

             if(read_size !=write_size) {
                 printf("error in read write\n");    }


             //Increment the total number of bytes read
             recv_size += read_size;
             packet_index++;
             printf("Total received image size: %i\n",recv_size);
             printf(" \n");
             printf(" \n");
    }

}


  fclose(image);
  printf("Image successfully Received!\n");
  return 1;
  }

int send_image(int socket, char *k)
{	


   FILE *picture_send;
   int size_send, read_size_send, stat_send, packet_index_send;
   char send_buffer[10240], read_buffer[256]; 
   packet_index_send = 1;


   picture_send = fopen(k,"r");
   printf("Getting Picture Size\n");   

   if(picture_send == NULL) {
        printf("Error Opening Image File"); } 

   fseek(picture_send, 0, SEEK_END);
   size_send = ftell(picture_send);
   fseek(picture_send, 0, SEEK_SET);
   printf("Total Picture size: %i\n",size_send);

   //Send Picture Size
   printf("Sending Picture Size\n");
   write(socket, (void *)&size_send, sizeof(int));

   //Send Picture as Byte Array
   printf("Sending Picture as Byte Array\n");

   do { //Read while we get errors that are due to signals.
      stat_send=read(socket, &read_buffer , 255);
      printf("Bytes read: %i\n",stat_send);
   } while (stat_send < 0);

   printf("Received data in socket\n");
   printf("Socket data: %c\n", read_buffer);

   while(!feof(picture_send)) {
   //while(packet_index_send = 1){
      //Read from the file into our send buffer
      read_size_send = fread(send_buffer, 1, sizeof(send_buffer)-1, picture_send);

      //Send data through our socket 
      do{
        stat_send = write(socket, send_buffer, read_size_send);  
      }while (stat_send < 0);

      printf("Packet Number: %i\n",packet_index_send);
      printf("Packet Size Sent: %i\n",read_size_send);     
      printf(" \n");
      printf(" \n");


      packet_index_send++;  

      //Zero out our send buffer
      bzero(send_buffer, sizeof(send_buffer));
     }
    }



int main(int argc , char *argv[])
    {
      int socket_desc;
  struct sockaddr_in server;
  char *parray;
char *image=argv[1];


  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);

  if (socket_desc == -1) {
  printf("Could not create socket");
  }

  memset(&server,0,sizeof(server));
  server.sin_addr.s_addr = inet_addr("127.0.1.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( 8889 );

  //Connect to remote server
  if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
  cout<<strerror(errno);
  close(socket_desc);
  puts("Connect Error");
  return 1;
  }

  puts("Connected\n");
/*
*/
static const char *input =argv[2];
    char *garbage = NULL;
    long value = 0;

    errno = 0;

    value = strtol(input, &garbage, 0);
printf("%ld",value);
int i[1];
i[0]=value;


write(socket_desc,i,1);
  send_image(socket_desc, image);

  receive_image(socket_desc);
//
Mat imag;
 imag = imread( image, 1 );


 Mat gray_image;
gray_image= imread("rimageName.jpeg",1);

 namedWindow( "simageName.jpeg", WINDOW_AUTOSIZE );
 namedWindow( "Gray image", WINDOW_AUTOSIZE );

 imshow( "simageName.jpeg", imag );
 imshow( "Gray image", gray_image );

 waitKey(0);


  close(socket_desc);

  return 0;
    }
