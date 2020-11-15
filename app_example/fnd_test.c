#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#define fnd_dev "/dev/fnd"
#define MAXFND 4
unsigned char asc_to_fnd(int n)
{
    unsigned char c;
    switch (n)
    {
    case 0:
        c = 0xC0;
        break;
    case 1:
        c = 0xF9;
        break;
    case 2:
        c = 0xA4;
        break;
    case 3:
        c = 0xB0;
        break;
    case 4:
        c = 0x99;
        break;
    case 5:
        c = 0x92;
        break;
    case 6:
        c = 0x82;
        break;
    case 7:
        c = 0xD8;
        break;
    case 8:
        c = 0x80;
        break;
    case 9:
        c = 0x98;
        break;
    default:
        c = 0xFF;
        break;
    }
    printf("asc_to_find : %d, return : %d \n", n, c);
    return c;
}
int main(int argc, char *av[])
{
    int n = 0, count = 0, dev = 0;
    int len = 0;
    int i = 0;
    unsigned char buf[MAXFND + 1];
    unsigned char new_buf[MAXFND + 1]; //ADD

    dev = open(fnd_dev, O_RDWR);
    if (dev < 0)
    {
        fprintf(stderr, "cannot open FND (%d)", dev);
        exit(2);
    }
    while (1)
    {
        memset(buf, 0x00, sizeof(buf));
        memset(new_buf, 0x00, sizeof(new_buf));
        i = 0;

        printf("###############################\n");
        printf(" FND TEST\n");
        printf(" press any numbers\n");
        printf(" 'q' to exit \n");
        printf("###############################\n");
        fgets(buf, sizeof(buf) + 1, stdin);
        len = strlen(buf);
        buf[len - 1] = '\0';
        if (!strcmp(buf, "q"))
        {
            printf("FND TEST END \n");
            close(dev);
            return 0;
        }
        for (n = MAXFND - len; n < MAXFND; n++)
        {
            new_buf[n+1] = asc_to_fnd((buf[i] - 48));
            i++;
            printf("new_buf : %08X \n", new_buf);
        }
        //unsigned char data[MAXFND] = {0xfc, 0x60, 0xda, 0x4f, 0x66, 0xb6, 0xbe, 0xe4};
        //unsigned char data[MAXFND] = {0xF9, 0xA4, 0xB0, 0x99, 0xFF, 0x00, 0xFF, 0x00};
        write(dev, new_buf, MAXFND);
    }
}
    unsigned char c;