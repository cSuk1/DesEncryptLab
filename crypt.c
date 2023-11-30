#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include "TextSwitch.h"
#include "Des.h"
#include "OpMode.h"

void showHelp()
{
    printf("*****************************************************\n");
    printf("*-h get help                                        *\n");
    printf("*-p plain file                                      *\n");
    printf("*-k key file                                        *\n");
    printf("*-v init vector file                                *\n");
    printf("*-m encrypt or decrypt mode                         *\n");
    printf("*   encrypt:                                        *\n");
    printf("*       0 ecb encrypt                               *\n");
    printf("*       1 cbc encrypt                               *\n");
    printf("*       2 cfb encrypt                               *\n");
    printf("*       3 ofb encrypt                               *\n");
    printf("*   decrypt:                                        *\n");
    printf("*       4 ecb decrypt                               *\n");
    printf("*       5 cbc decrypt                               *\n");
    printf("*       6 cfb decrypt                               *\n");
    printf("*       7 ofb decrypt                               *\n");
    printf("*-c cipher file                                     *\n");
    printf("*exp:                                               *\n");
    printf("*crypt -p mingwen -k key [-v initvect] -m 0 -c miwen*\n");
    printf("*****************************************************\n");
}
int main(int argc, char **argv)
{
    int cnt = 20;
    // 时间测试
    clock_t start_time;
    clock_t finish_time;
    float program_time;

    // 操作模式
    int mode = -1;
    int arg_require = 9;
    int is_need_initvec = 0;
    int h = 0;
    int flag = 1;
    // 获取命令行参数
    for (int i = 1; i < argc; i++)
    {
        // 指定明文文件
        if (strcmp(argv[i], "-p") == 0)
        {
            strcpy(plaintextfile, argv[i + 1]);
            i++;
        }
        // 获取帮助
        if (strcmp(argv[i], "-h") == 0)
        {
            h = 1;
            showHelp();
        }
        // 指定密钥文件
        else if (strcmp(argv[i], "-k") == 0)
        {
            strcpy(keyfile, argv[i + 1]);
            i++;
        }
        // 指定初始化向量文件
        else if (strcmp(argv[i], "-v") == 0)
        {
            strcpy(initvecfile, argv[i + 1]);
            arg_require++;
            is_need_initvec = 1;
            i++;
        }
        // 指定操作模式
        else if (strcmp(argv[i], "-m") == 0)
        {
            mode = atoi(argv[i + 1]);
            // 随机生成测试数据
            // initTxtData();
            i++;
        }
        // 指定密文文件
        else if (strcmp(argv[i], "-c") == 0)
        {
            strcpy(cipherfile, argv[i + 1]);
            i++;
        }
    }
    // 默认则显示帮助信息
    if (argc == 1 || argc + is_need_initvec < arg_require)
    {
        if (h == 0)
        {
            printf("more args are needed, you can get help by cmd crypt -h\n");
        }

        flag = 0;
    }
    if (flag)
    {
        // 开始时间
        start_time = clock();
        // 1次加或解密，多次用于测试
        for (size_t i = 0; i < 1; i++)
        {
            switch (mode)
            {
            case 0:
                des_ecb();
                break;
            case 1:
                des_cbc();
                break;
            case 2:
                des_cfb();
                break;
            case 3:
                des_ofb();
                break;
            case 4:
                dedes_ecb();
                break;
            case 5:
                dedes_cbc();
                break;
            case 6:
                dedes_cfb();
                break;
            case 7:
                dedes_ofb();
                break;
            default:
                break;
            }
        }
        // 结束时间
        finish_time = clock();
        // 获取执行时间，以秒为单位
        program_time = (float)(finish_time - start_time);
        float rate = cnt / (program_time / 1000);
        printf("the time that mode %d consumed is: %f ms\nthe rate is %f MBps\n", mode, program_time, rate);
    }

    return 0;
}