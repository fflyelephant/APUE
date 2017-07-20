#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
jmp_buf jmp;
int globalval;

static void fun2(void)
{	

	longjmp(jmp, 1);

}

static void fun1(int auto_val, int regi_val, int vola_val, int stat_val)
{
	printf("in fun1:\n");
	printf("\tglobal=%d, auto=%d, register:%d, volatile:%d, static:%d\n", \
			  globalval, auto_val, regi_val, vola_val, stat_val);
	fun2();
}

int main(int argc, char const *argv[])
{
	int          autoval;
	register int regival;
	volatile int volaval;
	static   int statval;
	/* main 中初始化 */
	globalval = 1; autoval = 2; regival = 3; volaval = 4; statval = 5;

	if(setjmp(jmp) != 0)
	{
		/* 从其它函数中回调后的值 */
		printf("longjmp from fun2!\n");
		printf("\tglobal=%d, auto=%d, register:%d, volatile:%d, static:%d\n", \
			  globalval, autoval, regival, volaval, statval);
		return 0;
	}

	globalval = 91; autoval = 92; regival = 93; volaval = 94; statval = 95;
	fun1(autoval, regival, volaval, statval);

	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch7_ProcessEnvironment$ ./a.out 
	in fun1:
	        global=91, auto=92, register:93, volatile:94, static:95
	longjmp from fun2!
	        global=91, auto=92, register:93, volatile:94, static:95

注:我的测试结果和书上不一样...  书上建议如果要使用setjmp函数，将函数中的auto变量赋予volatile属性。
*/
