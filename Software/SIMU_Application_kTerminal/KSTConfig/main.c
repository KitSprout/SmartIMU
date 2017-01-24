#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#include "rs232.h"

#define BUF_SIZE 256

int com_port = 3 - 1;   /* COM3 */
int baudrate = 115200;  /* 115200 */
char mode[]  = {'8', 'N', '1', 0};

void KSTCommand( char* pString )
{
    int lens = 0;
    unsigned char recvString[BUF_SIZE] = {0};

    RS232_SendBuf(com_port, pString, strlen(pString));
    RS232_SendBuf(com_port, "\r\n", 2);
    Sleep(100);

    lens = RS232_PollComport(com_port, recvString, BUF_SIZE);
    printf(recvString);
    printf("\n");
    Sleep(100);
}

int main()
{
    char pString[128] = {0};

    /* Open COM Port */
    if (!RS232_OpenComport(com_port, baudrate, mode)) {
        printf("Open COM Port Success\n\n");

        do {
            printf("KST Command : ");
            scanf("%s", pString);
            KSTCommand(pString);
        } while(strcmp("EXIT", pString) != 0);
    }
    else {
        printf("Open COM Port Error\n");
    }

    /* Close COM Port */
    RS232_CloseComport(com_port);
    printf("\n\nClose COM Port\n");

    return(0);
}
