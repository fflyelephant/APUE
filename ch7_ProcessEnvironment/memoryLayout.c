#include <stdio.h>

int global_init = 1; //全局变量、初始化、数据段(data)
int global_uninit;//全局变量、未初始化、未初始化段(bss)
static int static_global_init = 1;//全局变量、静态的、初始化、数据段(data)
static int static_global_uninit;//全局变量、静态的、未初始化、未初始化段(bss)
const  int const_global = 1;//全局常量、初始化、文本段(text)
const  int const_global_uninit;//全局常量、未初始化、文本段(text) *一般不定义这种变量，它的值始终为0且不可改变*
/* 
   全局变量都是保存在静态内存中(这也是为什么不初始化默认是0的原因)，data、bss、text、stack都含有静态内存，
   heap没有静态内存
*/

int main(int agrc, char *argv[])
{
	int local_init = 1;//局部变量、初始化、栈(stack)
	int local_uninit;//局部变量、未初始化、栈(stack)
	static int static_local_init = 1;//全局变量、静态存储区域、初始化、数据段(data)
	static int static_local_uninit;//全局变量、静态存储区域、未初始化、数据段(bss)
	const int const_local_init = 1;//局部常量、栈
	const int const_local_uninit;//局部常量、栈
	char *p = malloc(1024);//p指向的内存、堆
	return 0;
}

