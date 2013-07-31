#define MACH_TYPE_SMDK6410	1626
#define LOAD_ADDR		0x5e000000
#define LOAD_SIZE		0x500000
#define PARAMS_ADDR		(RAM_BASE + 0x100)
#define RAM_BASE		0x50000000
#define RAM_SIZE		(256*1024*1024)
#define CONFIG_NR_DRAM_BANKS    1

#define CMDLINE		"root=/dev/nfs nfsroot=10.11.12.42:/home/fantao/ok6410/myrootfs ip=10.11.12.2:10.11.12.42:10.11.12.254:255.255.255.0::eth0:off console=ttySAC0,115200"
//#define CMDLINE			"console=ttySAC0,115200 root=/dev/mtdblock3 rootfstype=yaffs2 rw init=/linuxrc mem=256M"

