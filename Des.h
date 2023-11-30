//
// Created by 65224 on 2023/4/18.
// Des算法的主要函数
//
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "SwitchTable.h"
#include "TextSwitch.h"

#ifndef DESENCRYPT_DES_H
#define DESENCRYPT_DES_H

char keyfile[50];
char cipherfile[50];
char plaintextfile[50];
char initvecfile[50];

// 密钥
char keybuf[9];
// 十六进制
char keyhex[17];
// 当前密钥的二进制
char keybin[65];

// 用于密钥的生成
// key的前28位
char c[29];
// key的后28位
char d[29];
char cd[57];

// 初始化向量
char vechex[17];
char vecbin[65];

// 读取密钥
void readKey(char *filename)
{
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    fgets(keyhex, 17, (FILE *)fp);
    keyhex[16] = '\0';
    fclose(fp);
}

// 读取初始化向量
void readInitVec(char *filename)
{
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    fgets(vechex, 17, (FILE *)fp);
    vechex[16] = '\0';
    fclose(fp);
}

// 字符串合并
void strMerge(char *str1, char *str2, char *res)
{
    int len = strlen(str1);
    for (size_t i = 0; i < len; i++)
    {
        res[i] = str1[i];
        res[len + i] = str2[i];
    }
}

// 初始ip置换
void initSwitch(char *m)
{
    char temp[64];
    strcpy(temp, m);
    for (int i = 0; i < 64; i++)
    {
        m[i] = temp[ip_table[i] - 1];
    }
}

// ip逆置换
void reIPSwitch(char *m)
{
    char temp[65];
    strcpy(temp, m);
    for (int i = 0; i < 64; i++)
    {
        m[i] = temp[ipre_table[i] - 1];
    }
}

// s盒置换
void sSwitch(char *str, char *end)
{
    char s[7];
    s[6] = '\0';
    char hang[3];
    hang[2] = '\0';
    int h;
    char lie[5];
    lie[4] = '\0';
    int l;
    int afters[8];
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            s[j] = str[6 * i + j];
        }
        hang[0] = s[0];
        hang[1] = s[5];
        for (size_t k = 0; k < 4; k++)
        {
            lie[k] = s[1 + k];
        }

        // 二进制转十进制
        h = bin2dec(hang);
        l = bin2dec(lie);

        // 完成s盒转换
        afters[i] = s_table[i][h][l];
    }
    char buf[5];
    buf[4] = '\0';
    for (size_t i = 0; i < 8; i++)
    {
        // 将s盒变换后的数字转成二进制，并补位成四位
        itoa(dec2bin(afters[i]), buf, 10);
        int lenofbuf = strlen(buf);
        switch (lenofbuf)
        {
        case 4:
            break;
            // 进行补位
        case 3:
            buf[3] = buf[2];
            buf[2] = buf[1];
            buf[1] = buf[0];
            buf[0] = '0';
            break;
        case 2:
            buf[2] = buf[0];
            buf[3] = buf[1];
            buf[0] = '0';
            buf[1] = '0';
            break;
        case 1:
            buf[3] = buf[0];
            buf[0] = '0';
            buf[1] = '0';
            buf[2] = '0';
            break;
        default:
            break;
        }
        for (size_t j = 0; j < 4; j++)
        {
            end[4 * i + j] = buf[j];
        }
    }
}

// E扩展运算
void extend(char *r, char *e)
{
    for (int i = 0; i < 48; i++)
    {
        e[i] = r[e_table[i] - 1];
    }
}

// p置换
void pSwitch(char *start, char *end)
{
    for (size_t i = 0; i < 32; i++)
    {
        end[i] = start[p_table[i] - 1];
    }
}

// ls移位
void lsSwitch(char *str, int round)
{
    int times = ls_table[round - 1];
    char temp[28];
    strcpy(temp, str);
    for (size_t i = 0; i < 28; i++)
    {
        str[i] = temp[(i + times) % 28];
    }
}

// 异或运算
// 参数分别为异或的两个数，返回的结果和异或的位数
void Xor(char *a, char *b, char *res, int len)
{
    char ret[len + 1];
    ret[len] = '\0';
    for (size_t i = 0; i < len; i++)
    {
        if (a[i] == b[i])
        {
            ret[i] = '0';
        }
        else
        {
            ret[i] = '1';
        }
    }
    strcpy(res, ret);
}

// 子密钥生成函数
void keyGen(char *key, int round, char *afterkey)
{
    // 如果是第一轮加密，进行pc1置换
    if (round == 1)
    {
        char temp[65];
        strcpy(temp, key);
        // pc1置换
        for (size_t i = 0; i < 56; i++)
        {
            cd[i] = temp[pc1_table[i] - 1];
        }
        cd[56] = '\0';
        c[28] = '\0';
        d[28] = '\0';
    }

    // 将cd分解为c和d
    for (size_t i = 0; i < 28; i++)
    {
        c[i] = cd[i];
        d[i] = cd[28 + i];
    }

    // 进行ls移位
    lsSwitch(c, round);
    lsSwitch(d, round);
    c[28] = '\0';
    d[28] = '\0';
    // 将移位后的c、d合并到cd
    strMerge(c, d, cd);
    cd[56] = '\0';
    // pc2置换
    for (size_t i = 0; i < 48; i++)
    {
        afterkey[i] = cd[pc2_table[i] - 1];
    }
}

// des加密
void des(char *in, char *out)
{
    int n = 2021141530140 % 10;
    // 读取密钥
    readKey(keyfile);
    // 密钥转换成二进制
    hex2bin(keyhex, keybin);
    keybin[64] = '\0';

    char temp[65];
    strcpy(temp, in);
    temp[64] = '\0';

    // 进行ip置换
    initSwitch(temp);
    char l[33];
    char r[33];
    // 将输入分为左右两部分
    for (size_t i = 0; i < 32; i++)
    {
        l[i] = temp[i];
        r[i] = temp[32 + i];
    }

    l[32] = '\0';
    r[32] = '\0';
    // 进行十六轮加密
    for (size_t round = 1; round < 17; round++)
    {
        // printf("\n\n");
        // printf("*****************************************\n");
        // printf("*                                       *\n");
        // printf("*              des round %d             *\n", round);
        // printf("*                                       *\n");
        // printf("*****************************************\n");
        // printf("\n\n");
        // printf("in:\t%s\n", in);
        // printf("L :\t%s\n", l);
        // printf("R :\t%s\n", r);

        // e扩展运算
        char er[49];
        er[48] = '\0';
        extend(r, er);
        // printf("exr:\t%s\n", er);

        // 生成轮密钥
        char key1[49];
        key1[48] = '\0';
        keyGen(keybin, round, key1);
        strcpy(keybin, key1);

        // 密钥与E（R）异或得到结果res
        char res[49];
        res[48] = '\0';
        Xor(keybin, er, res, 48);
        // printf("kxer:\t%s\n", res);

        // 进行s盒置换
        char afters[64];
        sSwitch(res, afters);
        // printf("sw:\t%s\n", afters);

        // 再进行p置换
        char afterp[33];
        afterp[32] = '\0';
        pSwitch(afters, afterp);
        // printf("ps:\t%s\n", afterp);

        // 两边再异或
        char r0[33];
        r0[32] = '\0';
        Xor(l, afterp, r0, 32);
        // printf("l%d:\t%s\n", round, r);
        // printf("r%d:\t%s\n", round, r0);
        strMerge(r, r0, temp);
        // if (round - 1 == n)
        // {
        //     printf("round %d des: %s\n", round - 1, temp);
        // }

        // printf("out:\t%s\n", in);
        strcpy(l, r);
        strcpy(r, r0);
    }
    // 执行最后一步换位
    strMerge(r, l, temp);
    // 进行逆ip置换
    reIPSwitch(temp);
    strcpy(out, temp);
}

// des解密
// 过程与加密一样，只是子密钥反过来使用
void dedes(char *in, char *out)
{
    int n = 2021141530140 % 10;
    char temp[65];
    strcpy(temp, in);
    temp[64] = '\0';

    char keys[16][68];
    // 读取密钥
    readKey(keyfile);
    // 密钥转换成二进制
    hex2bin(keyhex, keybin);
    // 进行ip置换
    initSwitch(temp);
    char l[33];
    char r[33];
    // 将输入分为左右两部分
    for (size_t i = 0; i < 32; i++)
    {
        l[i] = temp[i];
        r[i] = temp[32 + i];
    }

    l[32] = '\0';
    r[32] = '\0';
    // 生成十六轮密钥
    for (size_t i = 0; i < 16; i++)
    {
        keyGen(keybin, i + 1, keys[i]);
        keys[i][48] = '\0';
    }
    // 解密
    for (size_t round = 1; round < 17; round++)
    {

        // e扩展运算
        char er[49];
        er[48] = '\0';
        extend(r, er);
        // printf("exr:\t%s\n", er);

        // 密钥与E（R）异或得到结果res
        char res[49];
        res[48] = '\0';
        Xor(keys[16 - round], er, res, 48);
        // printf("kxer:\t%s\n", res);

        // 进行s盒置换
        char afters[64];
        sSwitch(res, afters);
        // printf("sw:\t%s\n", afters);

        // 再进行p置换
        char afterp[33];
        afterp[32] = '\0';
        pSwitch(afters, afterp);
        // printf("ps:\t%s\n", afterp);

        // 两边再异或
        char r0[33];
        r0[32] = '\0';
        Xor(l, afterp, r0, 32);
        // printf("l%d:\t%s\n", round, r);
        // printf("r%d:\t%s\n", round, r0);
        strMerge(r, r0, temp);
        // printf("out:\t%s\n", in);
        // if (round - 1 == n)
        // {
        //     printf("round %d des: %s\n", round - 1, temp);
        // }
        strcpy(l, r);
        strcpy(r, r0);
    }
    // 执行最后一步换位
    strMerge(r, l, temp);
    // 进行逆ip置换
    reIPSwitch(temp);
    strcpy(out, temp);
}

#endif // DESENCRYPT_DES_H