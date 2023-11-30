//
// Created by 65224 on 2023/4/18.
// 分组密码的操作模式
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "TextSwitch.h"
#include "Des.h"

#ifndef DESENCRYPT_OPMODE_H
#define DESENCRYPT_OPMODE_H
#define DATA_SIZE (5 * 1024) // 生成的测试数据大小为5kb
// 分组字符串
char strbuff[9];
// 分组——16进制
char pkthex[17];
// 当前分组数据的二进制
char pktbin[65];

// 8位的分组
char ebhex[3];
char ebbin[9];
// 8位的密文分组
char ebcipherbin[9];
char ebcipherhex[3];

// 密文二进制
char cipherbin[65];
// 密文十六进制
char cipherhex[17];

// 随机生成测试数据
void initTxtData()
{
    FILE *fp;
    char *data;
    int i;

    srand(time(NULL)); // 初始化随机数种子

    // 分配内存，用于存储随机生成的测试数据
    data = (char *)malloc(DATA_SIZE);
    if (data == NULL)
    {
        printf("memory allocate fail！\n");
        return;
    }

    // 生成随机测试数据
    for (i = 0; i < DATA_SIZE; i++)
    {
        data[i] = rand() % 256; // 生成0到255之间的随机数
    }

    // 将测试数据以16进制格式写入文件
    fp = fopen(plaintextfile, "wb");
    if (fp == NULL)
    {
        printf("can not open file\n");
        return;
    }
    for (i = 0; i < DATA_SIZE; i++)
    {
        fprintf(fp, "%02X", toupper((unsigned char)data[i])); // 将每个字节转换为2位16进制数，并写入文件
    }
    fclose(fp);
    printf("generate 5mb data successfully\n");
}

// 分组密码操作模式
// 电子密码本模式
void des_ecb()
{
    cipherbin[64] = '\0';
    cipherhex[16] = '\0';

    // 打开明文文件
    FILE *fp = NULL;
    fp = fopen(plaintextfile, "r");
    while (fgets(pkthex, 17, (FILE *)fp))
    {
        hex2bin(pkthex, pktbin);
        des(pktbin, cipherbin);
        bin2hex(cipherbin, cipherhex);
        writeFile(cipherfile, cipherhex);
    }
    fclose(fp);

    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*   des of ECB mode encrypt successfully!!  *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// ECB模式解密
void dedes_ecb()
{
    cipherbin[64] = '\0';
    cipherhex[16] = '\0';

    // 打开明文文件
    FILE *fp = NULL;
    fp = fopen(cipherfile, "r");
    while (fgets(pkthex, 17, (FILE *)fp))
    {
        hex2bin(pkthex, pktbin);
        dedes(pktbin, cipherbin);
        bin2hex(cipherbin, cipherhex);
        writeFile(plaintextfile, cipherhex);
    }
    fclose(fp);

    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*  des of ECB mode  decrypt  successfully!! *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// 密码分组链接模式
void des_cbc()
{
    cipherbin[64] = '\0';
    cipherhex[16] = '\0';

    // 读取初始化向量
    readInitVec(initvecfile);

    // 将初始化向量转成二进制
    hex2bin(vechex, vecbin);

    // 与输入按位异或
    FILE *fp = NULL;
    fp = fopen(plaintextfile, "r");
    char in[65];

    while (fgets(pkthex, 17, (FILE *)fp))
    {
        // 明文分组转二进制
        hex2bin(pkthex, pktbin);
        // 明文分组与前一密文或初始化向量的异或得到输入
        Xor(vecbin, pktbin, in, 64);
        in[64] = '\0';
        // 将输入经过des加密
        des(in, cipherbin);
        // 将密文作为与下一明文分组异或的链接
        strcpy(vecbin, cipherbin);
        // 将密文转为十六进制写入密文文件
        bin2hex(cipherbin, cipherhex);
        writeFile(cipherfile, cipherhex);
    }
    fclose(fp);
    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*   des of CBC mode encrypt successfully!!  *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// CBC模式解密
void dedes_cbc()
{
    cipherbin[64] = '\0';
    cipherhex[16] = '\0';

    // 读取初始化向量
    readInitVec(initvecfile);

    // 将初始化向量转成二进制
    hex2bin(vechex, vecbin);

    FILE *fp = NULL;
    fp = fopen(cipherfile, "r");
    char in[65];

    while (fgets(cipherhex, 17, (FILE *)fp))
    {
        // 密文分组转二进制
        hex2bin(cipherhex, cipherbin);
        // des解密
        dedes(cipherbin, in);
        in[64] = '\0';
        // 与前一密文或初始化向量的异或得到明文分组
        Xor(vecbin, in, pktbin, 64);
        // 将密文作为与下一明文分组异或的链接
        strcpy(vecbin, cipherbin);
        // 将明文转为十六进制写入明文文件
        bin2hex(pktbin, pkthex);
        writeFile(plaintextfile, pkthex);
    }

    fclose(fp);
    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*  des of CBC mode  decrypt  successfully!! *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// 8位密码反馈模式
void des_cfb()
{
    // 8位操作模式
    ebcipherbin[9] = '\0';
    ebcipherhex[3] = '\0';
    // 读取初始化向量
    readInitVec(initvecfile);
    // 将初始化向量转成二进制
    hex2bin(vechex, vecbin);
    // 与输入按位异或
    FILE *fp = NULL;
    fp = fopen(plaintextfile, "r");
    char in[65];
    while (fgets(ebhex, 3, (FILE *)fp))
    {
        // 将明文转为二进制
        hex2bin(ebhex, ebbin);
        in[64] = '\0';
        // 将移位寄存器内容加密得到输入
        des(vecbin, in);
        // 将输入的前八位与8位明文异或得到密文
        Xor(in, ebbin, ebcipherbin, 8);
        // 将密文转为二进制并写入文件
        bin2hex(ebcipherbin, ebcipherhex);
        writeFile(cipherfile, ebcipherhex);

        // 移动移位寄存器
        char temp[65];
        temp[64] = '\0';
        strcpy(temp, vecbin);
        for (size_t i = 0; i < 56; i++)
        {
            vecbin[i] = temp[8 + i];
        }
        for (size_t i = 56; i < 64; i++)
        {
            vecbin[i] = ebcipherbin[i - 56];
        }
    }
    fclose(fp);
    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*   des of CFB mode encrypt successfully!!  *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// cfb模式解密
void dedes_cfb()
{
    // 8位操作模式
    ebbin[9] = '\0';
    ebhex[3] = '\0';
    // 读取初始化向量
    readInitVec(initvecfile);
    // 将初始化向量转成二进制
    hex2bin(vechex, vecbin);
    // 与输入按位异或
    FILE *fp = NULL;
    fp = fopen(cipherfile, "r");
    char in[65];
    while (fgets(ebcipherhex, 3, (FILE *)fp))
    {
        // 将密文转为二进制
        hex2bin(ebcipherhex, ebcipherbin);
        in[64] = '\0';
        // 将移位寄存器内容加密得到输入
        des(vecbin, in);
        // 将输入的前八位与8位密文异或得到明文
        Xor(in, ebcipherbin, ebbin, 8);
        // 将明文转为二进制并写入文件
        bin2hex(ebbin, ebhex);
        writeFile(plaintextfile, ebhex);

        // 移动移位寄存器
        char temp[65];
        temp[64] = '\0';
        strcpy(temp, vecbin);
        for (size_t i = 0; i < 56; i++)
        {
            vecbin[i] = temp[8 + i];
        }
        for (size_t i = 56; i < 64; i++)
        {
            vecbin[i] = ebcipherbin[i - 56];
        }
    }
    fclose(fp);
    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*  des of CFB mode  decrypt  successfully!! *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// 8位输出反馈模式
void des_ofb()
{
    cipherbin[64] = '\0';
    cipherhex[16] = '\0';
    // 读取初始化向量
    readInitVec(initvecfile);
    // 将初始化向量转成二进制
    hex2bin(vechex, vecbin);
    FILE *fp = NULL;
    fp = fopen(plaintextfile, "r");
    char in[65];
    while (fgets(pkthex, 17, (FILE *)fp))
    {
        hex2bin(pkthex, pktbin);
        des(vecbin, in);
        in[64] = '\0';
        strcpy(vecbin, in);
        Xor(in, pktbin, cipherbin, 64);
        bin2hex(cipherbin, cipherhex);
        writeFile(cipherfile, cipherhex);
    }
    fclose(fp);
    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*   des of OFB mode encrypt successfully!!  *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}

// ofb解密模式
void dedes_ofb()
{
    pktbin[64] = '\0';
    pkthex[16] = '\0';
    // 读取初始化向量
    readInitVec(initvecfile);
    // 将初始化向量转成二进制
    hex2bin(vechex, vecbin);
    FILE *fp = NULL;
    fp = fopen(cipherfile, "r");
    char in[65];
    while (fgets(cipherhex, 17, (FILE *)fp))
    {
        hex2bin(cipherhex, cipherbin);
        des(vecbin, in);
        in[64] = '\0';
        strcpy(vecbin, in);
        Xor(in, cipherbin, pktbin, 64);
        bin2hex(pktbin, pkthex);
        writeFile(plaintextfile, pkthex);
    }
    fclose(fp);
    // printf("\n\n");
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*  des of OFB mode  decrypt  successfully!! *\n");
    printf("*                                           *\n");
    printf("*********************************************\n");
}
#endif // DESENCRYPT_SWITCHTABLE_H