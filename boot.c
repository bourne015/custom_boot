#include "common.h"
#include "setup.h"

static struct tag *params;

static int strlen(char *p)
{
	int len = 0;

	while (p[len])
		len++;
	return len;
}

static void strcpy(char dest[], char src[])
{
	int i = 0, j = 0;

	while ((dest[i++] = src[j++]));
}

static void setup_start_tag(void)
{
        params = (struct tag *)PARAMS_ADDR;

        params->hdr.tag = ATAG_CORE;
        params->hdr.size = tag_size(tag_core);

        params->u.core.flags = 0;
        params->u.core.pagesize = 0;
        params->u.core.rootdev = 0;

        params = tag_next(params);
}

static void setup_memory_tags(void)
{
        int i;

        for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
                params->hdr.tag = ATAG_MEM;
                params->hdr.size = tag_size(tag_mem32);

                params->u.mem.start = RAM_BASE;
                params->u.mem.size = RAM_SIZE;

                params = tag_next(params);
        }
}

static void setup_commandline_tag(char *commandline)
{
        char *p;

        if (!commandline)
                return;

        /* eat leading white space */
        for (p = commandline; *p == ' '; p++);

        /* skip non-existent command lines so the kernel will still
         * use its default command line.
         */
        if (*p == '\0')
                return;

        params->hdr.tag = ATAG_CMDLINE;
        params->hdr.size = (sizeof(struct tag_header) + strlen(p)+1+4)>>2;

        strcpy(params->u.cmdline.cmdline, p);

        params = tag_next(params);
}

static void 
setup_initrd_tag(unsigned long initrd_start, unsigned long initrd_end)
{
        /* an ATAG_INITRD node tells the kernel where the compressed
         * ramdisk can be found. ATAG_RDIMG is a better name, actually.
         */
        params->hdr.tag = ATAG_INITRD2;
        params->hdr.size = tag_size(tag_initrd);

        params->u.initrd.start = initrd_start;
        params->u.initrd.size = initrd_end - initrd_start;

        params = tag_next(params);
}

static void setup_end_tag(void)
{
        params->hdr.tag = ATAG_NONE;
        params->hdr.size = 0;
}

int do_bootm_linux(void)
{
	char *commandline = CMDLINE;
	void (*kernel_entry)(int zero, int arch, unsigned int params);
	volatile unsigned int *p = (volatile unsigned int *)LOAD_ADDR;

	/*copy kernel to ram*/
	uart_init();
	puts("copying kernel to ram......");
	nand_read(0x100000, LOAD_ADDR, LOAD_SIZE);
	puts("done\n\r");

	putmem(*p);

	/*params*/
	puts("\n\rsetting params......");
	setup_start_tag();
	setup_memory_tags();
	setup_commandline_tag(commandline);

//	setup_initrd_tag();
	setup_end_tag();
	puts("done\n\r");

	/*boot*/
	kernel_entry = (void (*)(int, int, unsigned int))(LOAD_ADDR+64);
	puts("Starting kernel...\n\r");
	kernel_entry(0, MACH_TYPE_SMDK6410, PARAMS_ADDR);
	
	puts("oh no, something is wrong!\n\r");
	return 1;
}
