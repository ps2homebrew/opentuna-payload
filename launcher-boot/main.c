
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <libcdvd.h>

#include <fcntl.h>
#include <sbv_patches.h>

#include <stdio.h>
#include <debug.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void wipeUserMem(void)
{
	int i;
	for (i = 0x100000; i < GetMemorySize(); i += 64)
	{
		asm volatile(
			"\tsq $0, 0(%0) \n"
			"\tsq $0, 16(%0) \n"
			"\tsq $0, 32(%0) \n"
			"\tsq $0, 48(%0) \n" ::"r"(i));
	}
}

void ResetIOP()
{
	SifInitRpc(0);
	while (!SifIopReset("", 0))
	{
	};
	while (!SifIopSync())
	{
	};
	SifInitRpc(0);
}

void InitPS2()
{
	//init_scr();
	ResetIOP();
	SifInitIopHeap();
	SifLoadFileInit();
	fioInit();
	sbv_patch_disable_prefix_check();
	SifLoadModule("rom0:SIO2MAN", 0, NULL);
	SifLoadModule("rom0:MCMAN", 0, NULL);
	SifLoadModule("rom0:MCSERV", 0, NULL);
}

void LoadElf(char *filename, char *party)
{

	char *args[1];
	t_ExecData exec;
	SifLoadElf(filename, &exec);

	if (exec.epc > 0)
	{
		ResetIOP();

		if (party != 0)
		{
			args[0] = party;
			ExecPS2((void *)exec.epc, (void *)exec.gp, 1, args);
		}
		else
		{
			ExecPS2((void *)exec.epc, (void *)exec.gp, 0, NULL);
		}
	}
}

int file_exists(char filepath[])
{
	int fdn;

	fdn = open(filepath, O_RDONLY);
	if (fdn < 0)
		return 0;
	close(fdn);

	return 1;
}

int main(int argc, char *argv[])
{

	wipeUserMem();

	InitPS2();

	if (file_exists("mc0:/BOOT/BOOT.ELF"))
		LoadElf("mc0:/BOOT/BOOT.ELF", "mc0:/BOOT/");

	if (file_exists("mc1:/BOOT/BOOT.ELF"))
		LoadElf("mc1:/BOOT/BOOT.ELF", "mc1:/BOOT/");

	__asm__ __volatile__(
		"	li $3, 0x04;"
		"	syscall;"
		"	nop;");

	return 0;
}
