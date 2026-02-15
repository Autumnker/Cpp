/*===============================================
*   文件名称：test.c
*   创 建 者：hqyj    
*   创建日期：2025年07月28日
*   描    述：
================================================*/
#include <stdio.h>
#include <string.h>

#define NUM 4069

int main(int argc, char const *argv[])
{
    // 打开文件
    FILE *fp = fopen("./debug/src.txt", "w");
    if (NULL == fp)
    {
        perror("fopen");
        return -1;
    }

    // 循环写入内容
    char buf[NUM] = {'\0'};
    for (int i = 0; i < 26; ++i)
    {
        memset(buf, '\0', NUM);
        for (int j = 0; j < NUM; ++j)
        {
            if(j % 78 == 0 && j > 0)
            {
                buf[j] = '\n';    
            }
            else
            {
                buf[j] = (char)('A' + i);
            }
        }
        if (-1 == fwrite(buf, NUM, 1, fp))
        {
            perror("fwrite");
            fclose(fp);
            return -1;
        }
    }

    // 关闭文件描述符
    fclose(fp);

    return 0;
}
