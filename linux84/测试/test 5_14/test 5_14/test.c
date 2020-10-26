#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
//#define __DEBUG__
//int main()
//{
//	int arr[10] = { 0 };
//	int i = 0;
//	for (i = 0; i < 10; i++)
//	{
//		arr[i] = i;
//	}
//	for (i = 0; i < 10; i++)
//	{   
//        #ifdef __DEBUG__
//		printf("%d ", arr[i]);
//        #endif __DEBUG__
//	}
//	return 0;
//}

//
//int main()
//{
//	int arr[10] = { 0 };
//	int i = 0;
//	for (i = 0; i < 10; i++)
//	{
//		arr[i] = i;
//	}
//	for (i = 0; i < 10; i++)
//	{
//#if 1//若后面的常量表达式为真，则printf执行，
//		printf("%d ", arr[i]);
//#endif 
//	}
//	return 0;
//}

//#define __DEBUG__ 1
//int main()
//{
//	int arr[10] = { 0 };
//	int i = 0;
//	for (i = 0; i < 10; i++)
//	{
//		arr[i] = i;
//	}
//	for (i = 0; i < 10; i++)
//	{
//#if __DEBUG__
//		printf("%d ", arr[i]);
//#endif
//	}
//	return 0;
//}

//int main()
//{
//#if  2==1
//	printf("相等\n");
//#elif 1>1
//	printf("大于\n");
//#else
//	printf("小于\n");
//#endif
//	return 0;
//}
 
//#define DEBUG
//int main()
//{
//	int arr[10] = { 0 };
//	int i = 0;
//	for (i = 0; i < 10; i++)
//	{
//		arr[i] = i;
//	}
//	for (i = 0; i < 10; i++)
//	{
//#if defined DEBUG
//		printf("%d ", arr[i]);
//#endif 
//	}
//	return 0;
//}

//#define DEBUG 1
//int main()
//{
//	int arr[10] = { 0 };
//	int i = 0;
//	for (i = 0; i < 10; i++)
//	{
//		arr[i] = i;
//	}
//	for (i = 0; i < 10; i++)
//	{
//#if defined (DEBUG)
//		printf("%d ", arr[i]);
//#endif 
//	}
//	return 0;
//}

////#define DEBUG
//int main()
//{
//	int arr[10] = { 0 };
//	int i = 0;
//	for (i = 0; i < 10; i++)
//	{
//		arr[i] = i;
//	}
//	for (i = 0; i < 10; i++)
//	{
//#if !defined DEBUG//只有未定义才会输出
//		printf("%d ", arr[i]);
//#endif 
//	}
//	return 0;
//}
#define DE
#define WC
int main()
{
#if defined DEBUG
        #if defined ZX
	           printf("zx");
        #endif
        #if defined XZ
	          printf("xz");
        #endif
#elif defined DE
        #if defined WC
              printf("WC");
        #endif
        #if defined CW
             printf("CW");
        #endif
#endif
		return 0;
}