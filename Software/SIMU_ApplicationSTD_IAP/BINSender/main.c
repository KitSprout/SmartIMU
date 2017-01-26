#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#include "rs232.h"

#define BUF_SIZE    4096
#define FILE_NAME   "SIMU_ApplicationSTD_IAP-GPIO.bin"

int com_port = 3 - 1;   /* COM3 */
int baudrate = 115200;  /* 115200 */
char mode[]  = {'8', 'N', '1', 0};

int main()
{
    FILE *fs;

    uint8_t i;

    uint8_t fileInfo[16] = {0};
    uint8_t filebuff[BUF_SIZE] = {0};

    uint32_t *fileSize = &fileInfo[4];
    uint32_t *recvLens = &fileInfo[8];
    uint32_t *sendCRC  = &fileInfo[12];

    uint32_t quotient  = 0;
    uint32_t remainder = 0;

    uint32_t count  = 0;
    uint32_t offset = 0;

    fileInfo[0] = 'K';
    fileInfo[1] = 'S';
    fileInfo[2] = 'U';
    fileInfo[3] = 'L';
    *recvLens   = 64;

    /* Open Binary File */
    fs = fopen(FILE_NAME, "rb");
    if (fs == NULL) {
        printf("Open File Error\n\n");
        return 1;
    }
    else {
        printf("Open File Success\n\n");
        fseek(fs, 0, SEEK_END);
        *fileSize = ftell(fs);
        rewind(fs);
        fread(filebuff, *fileSize, 1, fs);

        *sendCRC = 0;
        for (i = 0; i < 12; i++) {
            *sendCRC += fileInfo[i];
        }
        quotient  = *fileSize / *recvLens;
        remainder = *fileSize % *recvLens;

        printf("file size = %d bytes\n", *fileSize);
        printf("\n");
        while (count <= *fileSize) {
            printf(" %02X", filebuff[count++]);
            if ((count % 16) == 0) {
                printf("\n");
            }
        }
        printf("\n\n");
    }
    fclose(fs);

    /* Open COM Port */
    if (!RS232_OpenComport(com_port, baudrate, mode)) {
        printf("Open COM Port Success\n\n");

        RS232_SendBuf(com_port, fileInfo, 16);
        Sleep(100);

        while (quotient--) {
            printf("[offset] = %d\n", offset);
            RS232_SendBuf(com_port, filebuff + offset, *recvLens);
            offset += *recvLens;
            Sleep(100);
        }
        printf("[offset] = %d\n", offset);
        RS232_SendBuf(com_port, filebuff + offset, remainder);
    }
    RS232_CloseComport(com_port);
    printf("\n\nClose COM Port\n");

    return(0);
}
