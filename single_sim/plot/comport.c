// adrport.c - Serial Port Handler
// Copyright MMI, MMII by Sisusypro Incorporated

// Permission is hereby granted to freely copy, 
// modify, utilize and distribute this example in 
// whatever manner you desire without restriction.


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "comport.h"
static int fd = 0;


// opens the serial port
// return code:
//   > 0 = fd for the port
//   -1 = open failed
int OpenAdrPort( )
{
  char sPortName[64]="/dev/ttyS0";

  // make sure port is closed
  CloseAdrPort();
  
  fd = open(sPortName, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd < 0)
    printf("open error %d %s\n", errno, strerror(errno));
  else {
    struct termios my_termios;
    tcgetattr(fd, &my_termios);
    
    tcflush(fd, TCIFLUSH);
    
    my_termios.c_cflag = B9600 | CS8 |CREAD | CLOCAL | HUPCL;
    
    cfsetospeed(&my_termios, B9600);
    tcsetattr(fd, TCSANOW, &my_termios);
  }
  
  return fd;
}




// writes zero terminated string to the serial port
// return code:
//   >= 0 = number of characters written
//   -1 = write failed
int WriteAdrPort(char* psOutput)
{
  int iOut;
  if (fd < 1){
    printf(" port is not open\n");
    return -1;
  } 

  iOut = write(fd, psOutput, strlen(psOutput));


  /*
  if (iOut < 0)
    printf("write error %d %s\n", errno, strerror(errno));
  else
    printf("wrote %d chars: %s\n", iOut, psOutput);
  */


  return iOut;
} 




// read string from the serial port
// return code:
//   >= 0 = number of characters read
//   -1 = read failed
int 
ReadAdrPort(char* psResponse, int iMax)
{
  int iIn;

  // printf("in ReadAdrPort iMax=%d\n", iMax);

  if (fd < 1)    {
    printf(" port is not open\n");
    return -1;
  } 

  strncpy (psResponse, "N/A", iMax<4?iMax:4);

  iIn = read(fd, psResponse, iMax-1);

  if (iIn < 0)  {
    if (errno == EAGAIN)
      return 0;
    else
      printf("read error %d %s\n", errno, strerror(errno));
  }
  else {
    psResponse[iIn<iMax?iIn:iMax] = '\0';
    // printf("read %d chars: %s\n", iIn, psResponse);
  }
  
  return iIn;
}





// closes the serial port
int
CloseAdrPort()
{
  if (fd > 0)    close(fd);
}



int 
Doit(char Command[], int second)
{ 

  char Rcom[254]="R";
  char sResult[254];
  int iSpot = 0;

  // printf("%s", Command);

  iSpot = strlen(Command);
  Command[iSpot] = 0x0d; 
  Command[iSpot+1] = 0x00; 
  if (WriteAdrPort(Command) < 0) return 0;

  sleep( 2 ); 

  if (ReadAdrPort(sResult,254) > 0)
    printf("****Response is %s\n", sResult);




  // printf(",   %s  was accepted.\n", Rcom);

  iSpot = strlen(Rcom);
  Rcom[iSpot] = 0x0d; 
  Rcom[iSpot+1] = 0x00; 
  if (WriteAdrPort(Rcom) < 0) return 0;

  sleep( second ); 

  if (ReadAdrPort(sResult,254) > 0)
    printf("****Response is %s\n", sResult);

  return 0;
}




int 
Do_online()
{ 
  char sResult[254];
  int iSpot = 0;

  char Command[254]= "F";

  // printf("%s  was accepted\n", Command);

  iSpot = strlen(Command);
  Command[iSpot] = 0x0d; 
  Command[iSpot+1] = 0x00; 
  if (WriteAdrPort(Command) < 0) return 0;

  sleep(1); 

  if (ReadAdrPort(sResult,254) > 0)
    printf("****Response is %s\n", sResult);

  return 0;
}


int 
Do_clear()
{ 
  char sResult[254];
  int iSpot = 0;

  char Command[254]= "C";

  //  printf("%s  was accepted\n", Command);

  iSpot = strlen(Command);
  Command[iSpot] = 0x0d; 
  Command[iSpot+1] = 0x00; 
  if (WriteAdrPort(Command) < 0) return 0;

  sleep(1); 

  if (ReadAdrPort(sResult,254) > 0)
    printf("****Response is %s\n", sResult);

  return 0;
}







