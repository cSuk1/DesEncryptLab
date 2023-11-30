//
// Created by 65224 on 2023/4/18.
// 进制与字符串转化
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef DESENCRYPT_TEXTSWITCH_H
#define DESENCRYPT_TEXTSWITCH_H

// 字符串转十六进制
void str2hex(char *str, char *hex)
{
    char hexbuf[65];
    char hexstr[65] = {0};
    // 将读取的字符串转化成十六进制
    for (size_t i = 0; i < strlen(str); i++)
    {
        sprintf(hexbuf, "%02X", str[i]);
        strncat(hexstr, hexbuf, 2);
    }
    hexstr[64] = '\0';
    strcpy(hex, hexstr);
}

// 2进制转10进制
int bin2dec(char *bin)
{
    int res = 0;
    int len = strlen(bin);
    char temp[2];
    temp[0] = '0';
    temp[1] = '1';
    for (size_t i = 0; i < len; i++)
    {
        if (bin[len - i - 1] == temp[1])
        {
            res += pow(2, i);
        }
        else
        {
            continue;
        }
    }
    return res;
}

// 十进制转二进制
int dec2bin(int n)
{
    int sum = 0;
    int y, x = 1; // y表示余数，x为叠加的系数
    while (n != 0)
    {
        y = n % 2;
        sum += x * y;
        x *= 10;
        n /= 2;
    }
    return sum;
}

// 十六进制转二进制
void hex2bin(char *hex, char *bin)
{
    int len = strlen(hex);
    char buff[65];
    char binbuff[65] = {0};
    for (int i = 0; i < len; i++)
    {
        switch (hex[i])
        {
        case '0':
            strcpy(buff, "0000");
            strncat(binbuff, buff, 4);
            break;
        case '1':
            strcpy(buff, "0001");
            strncat(binbuff, buff, 4);
            break;
        case '2':
            strcpy(buff, "0010");
            strncat(binbuff, buff, 4);
            break;
        case '3':
            strcpy(buff, "0011");
            strncat(binbuff, buff, 4);
            break;
        case '4':
            strcpy(buff, "0100");
            strncat(binbuff, buff, 4);
            break;
        case '5':
            strcpy(buff, "0101");
            strncat(binbuff, buff, 4);
            break;
        case '6':
            strcpy(buff, "0110");
            strncat(binbuff, buff, 4);
            break;
        case '7':
            strcpy(buff, "0111");
            strncat(binbuff, buff, 4);
            break;
        case '8':
            strcpy(buff, "1000");
            strncat(binbuff, buff, 4);
            break;
        case '9':
            strcpy(buff, "1001");
            strncat(binbuff, buff, 4);
            break;
        case 'a':
        case 'A':
            strcpy(buff, "1010");
            strncat(binbuff, buff, 4);
            break;
        case 'b':
        case 'B':
            strcpy(buff, "1011");
            strncat(binbuff, buff, 4);
            break;
        case 'c':
        case 'C':
            strcpy(buff, "1100");
            strncat(binbuff, buff, 4);
            break;
        case 'd':
        case 'D':
            strcpy(buff, "1101");
            strncat(binbuff, buff, 4);
            break;
        case 'e':
        case 'E':
            strcpy(buff, "1110");
            strncat(binbuff, buff, 4);
            break;
        case 'f':
        case 'F':
            strcpy(buff, "1111");
            strncat(binbuff, buff, 4);
            break;
        default:
            printf("err hex\n");
            break;
        }
    }
    binbuff[65] = '\0';
    strcpy(bin, binbuff);
}

// 二进制转十六进制
void bin2hex(char *bin, char *hex)
{
    int len = strlen(bin) / 4;
    char buf[5];
    buf[4] = '\0';
    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            buf[j] = bin[i * 4 + j];
        }
        int dec = bin2dec(buf);
        switch (dec)
        {
        case 0:
            hex[i] = '0';
            break;
        case 1:
            hex[i] = '1';
            break;
        case 2:
            hex[i] = '2';
            break;
        case 3:
            hex[i] = '3';
            break;
        case 4:
            hex[i] = '4';
            break;
        case 5:
            hex[i] = '5';
            break;
        case 6:
            hex[i] = '6';
            break;
        case 7:
            hex[i] = '7';
            break;
        case 8:
            hex[i] = '8';
            break;
        case 9:
            hex[i] = '9';
            break;
        case 10:
            hex[i] = 'A';
            break;
        case 11:
            hex[i] = 'B';
            break;
        case 12:
            hex[i] = 'C';
            break;
        case 13:
            hex[i] = 'D';
            break;
        case 14:
            hex[i] = 'E';
            break;
        case 15:
            hex[i] = 'F';
            break;
        default:
            break;
        }
    }
}

// 写入文件
void writeFile(char *filename, char *txt)
{
    FILE *fp = NULL;
    fp = fopen(filename, "a+");
    fputs(txt, fp);
    fclose(fp);
}

#endif // DESENCRYPT_TEXTSWITCH_H
