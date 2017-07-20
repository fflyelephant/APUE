#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
#include <unistd.h>
jmp_buf jmp; /* jmp必须是全局变量(因为要实现函数间的跳转) */

static void fun_init(void)
{
	int retjmp;
	retjmp = setjmp(jmp);/* 首次调用立即返回0 */

	printf("%d\n", retjmp);
	switch(retjmp){
		case 0:
			printf("first call setjmp!\n");
			break;
		case 1:
			printf("longjmp from fun1!\n");
			break;
		case 2:
			printf("longjmp from fun2!\n");
			break;
		default:
			printf("unexpected retjmp\n");
	}
}

static void fun1(void)
{

	longjmp(jmp, 1);

}

static void fun2(void)
{

	longjmp(jmp, 2);

}

int main(int argc, char const *argv[])
{
	
	fun_init(); /* 要注意调用顺序，longjmp不能在setjmp之前执行 */
	fun1();	    /* 使用了setjmp函数的函数在函数执行完毕之后不回收栈空间，以备longjmp的访问(实现机制) */
	fun2();

	return 0;
}
/*result:

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch7_ProcessEnvironment$ ./a.out 
	0
	first call setjmp!
	1
	longjmp from fun1!
	2
	longjmp from fun2!

 */