#ifndef BQC_H
#define BQC_H
#ifdef  __GNUC__
#define PASTE3_(x,y,z) x##y##z
#define PASTE3(x,y,z) PASTE3_(x,y,z)
#define GCCVER PASTE3(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#endif
#if 1 //architecture ports

#include <linux/unistd.h>

//to add a new arch:
//
//to get the gcc register syntax (sp, %sp, $sp, :sp) see:
//https://github.com/gcc-mirror/gcc/tree/master/gcc/config/<arch>/<arch>.h
//if you have a working compiler try `echo|gcc -E -dM - |grep REGISTER_PREFIX`
//then prepend its value to "sp"
//to figure out syscalls see
//https://git.kernel.org/cgit/linux/kernel/git/next/linux-next.git/patch/?id=f5738ceed46782aea7663d62cb6398eb05fc4ce0
//note: kernel devs removed _syscall()s without documenting alternatives
//if you are lucky, your architecture may be in the 2.6.18 unistd.h

//set up architecture dependent stuff
//	#define ARCH_DATA	"sp", "syscall","callnum","ret","arg1","arg2","arg3","arg4","arg5","arg6","arg7","memory",...
#ifdef __x86_64__
	#define ARCH_DATA	"rsp","syscall", "rax","rax","rdi","rsi","rdx","r10","r8", "r9", "0", "rcx","r11","memory"
//TODO x32 as subset
	enum {
		__NR_
	};
#elif defined (__i386__)
	#define ARCH_DATA	"esp","int $128","eax","eax","ebx","ecx","edx","esi","edi","ebp","0", "memory"
#elif defined (__aarch64__)
	#define ARCH_DATA	"x31","svc 0",   "x8", "x0", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13","x14", "x15", "x16", "x17", "x18", "memory"
#elif defined (__arm__)
	#define ARCH_DATA	"r13","swi 0x0", "r7", "r0", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "memory"
#elif defined (__alpha__) //* also returns error on $19 */
	//#define ARCH_DATA	"sp","syscall",  "v0", "v0", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "memory"
	/* older way */
	//#define ARCH_DATA	"sp","callsys #  %0 %1 %2 %3 %4 %5 %6 %7 %8", "$0", "$0","$16","$17","$18","$19","$20","$21", "$22","$23","$24","$25","$27","$28","$1","$2","$3","$4","$5","$6","$7","$8","memory"
#elif defined (__alpha__) //stack pointer called "sp", but located @ "r28"
	//#define ARCH_DATA "sp",
#elif defined (__avr32__)
	//#define ARCH_DATA	"__SP_L__","scall",
#elif defined __bfin
	//#define ARCH_DATA	"SP","excpt 0x0","P0", "R0", "R0", "R1", "R2", "R3", "R4", "R5", "R6", /* more clobbers?*/ "memory"
#elif defined (__TMS320C6X__) || defined (_TMS320C6X) //c6x
	//#define ARCH_DATA	"B15",
#elif defined (__CR__) ||  defined (__CR16__) || defined (__CR16C__)
	//#define ARCH_DATA	"B15",
#elif defined __cris__
	//#define ARCH_DATA	"sp","break 13","r9","r10","r10","r11","r12","r13","mof","srp",0,"memory"
#elif defined (__epiphany__)
	//#define ARCH_DATA "sp",
#elif defined (fr30)
	//#define ARCH_DATA "sp",
#elif defined (__frv__) //sp is "gr1"
	//#define ARCH_DATA "sp",
#elif defined (__FT32__)
	//#define ARCH_DATA "$sp",
#elif defined(__H8300SX__) || defined(__H8300S__) || defined(__H8300H__) || defined(__H8300__) //sp is "r7"
	//#define ARCH_DATA "sp",
#elif defined(__hppa__) || defined(__hppa)
	//#define ARCH_DATA	"%usp","ble 0x100(%sr2,%r0)","%r20","%r28","%r26","%r25","%r24","%r23","%r22","%r21","0", ,"r1","r2","r20","r29","r31","memory"
	//#define ARCH_DATA "r30","ble 0x100(%sr2, %r0)","r20","r26","r25","r24","r23","r22","r21","0","0", memory","r1","r2","r20","r29","r31"
#elif defined(__ia64__)	/* has 2 returns */	//sp is "r12"
	//#define ARCH_DATA	"sp","break 0x100000","r15","r10/r8","out0","out1","out2","out3","out4","out5","0", "memory"
#elif defined(__iq2000__)
	//#define ARCH_DATA "%29",
#elif defined(__lm32__)
	//#define ARCH_DATA "sp",
#elif defined(__m32c__)
	//#define ARCH_DATA "sp",
#elif defined(__M32R__) || defined(__m32r__) //sp is "r15"
	//#define ARCH_DATA "sp",
#elif defined(__m68k__)
	//#define ARCH_DATA	"sp","trap  #0","d0","d0","d1","d2","d3","d4","d5", "0", "a0","d0","d1","a0","memory"
	//#define ARCH_DATA	"%sp","trap &0","%d0","%d0","%d1","%d2","%d3","%d4","%d5","0",%a0","%d0","%d1","%a0","memory"
#elif defined(__mcore__) || defined(__MCORE__) //sp is "r0"
	//#define ARCH_DATA "sp",
#elif defined(__MEP__)  || defined(__MeP__)  || defined(mep) //$sp is "$15" also aliased as "sp"
	//#define ARCH_DATA "$sp",
#elif defined(__microblaze__)
	//#define ARCH_DATA	"r1","brki r14, 0x8","r12","r3","r5","r6","r7","r8","r9","r10","0","memory","r4"
#elif defined(__mips64__)
	//#define ARCH_DATA	"$sp","syscall","$v0","$v0","$a0","$a1","$a2","$a3","$a4","$a5","$a6","$a7","$at","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$hi","$lo","memory"
#elif defined(__mips__) || defined (_mips)
	//#define ARCH_DATA	"$sp","syscall","$v0","$v0","$a0","$a1","$a2","$a3","$a4","$a5","$a6", "$a7","$at","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","$t9","$hi","$lo","memory"
#elif defined(__mmix__) || defined (__MMIX__) //sp is "$254" also aliased as ":sp"
	//#define ARCH_DATA "sp",
#elif defined(__MN10300__) || defined (__mn10300__)
	//#define ARCH_DATA "sp",
#elif defined(moxie) || defined (MOXIE)
	//#define ARCH_DATA "$sp",
#elif defined(__MSP430__)
	//#define ARCH_DATA "R1",
#elif defined(__nds32__) //sp is $r31
	//#define ARCH_DATA "$sp",
#elif defined(__nios2_arch__) || defined(nios2) || defined(NIOS2) //sp is r27
	//#define ARCH_DATA "sp",
#elif defined(__nvptx__) //sp is regnum which would correspond to "%hr1" which is list as %outargs
	//#define ARCH_DATA  "%outargs",	
#elif defined(__or1k__) //clobbers unused would-be arg registers
	//#define ARCH_DATA	???,"l.sys 1","r11","r11","r3","r4","r5","r6","r7","r8","0", memory","r12","r13","r15","r17","r19","r21","r23","r25","r27","r29","r31"
#elif defined(pdp11) //sp is r6
	//#define ARCH_DATA  "sp",
#elif defined(__powerpc64__)
	//#define ARCH_DATA	???,"sc","r0","r0","r3","r4","r5","r6","r7","r8","0","memory","cr0","ctr","r8","r9","r10","r11","r12"
#elif defined(__powerpc__)
	//#define ARCH_DATA	???,"sc","r0","r0","r3","r4","r5","r6","r7","r8","0","memory","cr0","ctr","r8","r9","r10","r11","r12"
#elif defined(__RL78__) //sp would be r32, but not aliased
	//#define ARCH_DATA "sp",
#elif defined(__RX__) //sp ~= r0
	//#define ARCH_DATA "sp",
#elif defined(__s390x__)
	//#define ARCH_DATA  "r15","svc 0","r1","r2","r2","r3","r4","r5","r6","r7","0","memory"
#elif defined (__s390__)
	//#define ARCH_DATA	"r15","    svc %b1\n"
#elif defined __sh__
	//#define ARCH_DATA	"r15","trapa #","r3", "r0", "r4", "r5", "r6", "r7", "r0", "r1", "0", "memory"
#elif defined(__sparc64__) || defined(__sparcv9) || defined(__sparcv9__) || defined(__sparc_v9__) || defined(__arch64__) //sp is %o6
	//#define ARCH_DATA	"%sp","t 0x6d", "%g1","%o0","%o0","%o1","%o2","%o3","%o4","%o5", "0", "memory"
#elif defined (__sparc__) || defined (__sparclet__) || defined (__sparclite__) || defined (__sparclite8x__) || defined (__supersparc__) || defined(__sparc_v8__) //sp is %o6
 	//#define ARCH_DATA	"%sp","t 0x10", "%g1","%o0","%o0","%o1","%o2","%o3","%o4","%o5", "0", "memory"
#elif defined (__spu__)
	//#define ARCH_DATA "$sp",
#elif defined (xstormy16) //sp is r15
	//#define ARCH_DATA "sp",	
#elif defined (__tilegx__) || defined (__tilepro__) //sp is r54
	//#define ARCH_DATA "sp",
#elif defined(__v850__)
	//sp is r3
	//#define ARCH_DATA "sp",
#elif defined(__vax__) 	//sp is r14
	//#define ARCH_DATA "sp",
#elif defined(__VISIUM__) //sp is r23
	//#define ARCH_DATA "sp",
#elif defined __xtensa__
	//#define ARCH_DATA	???,syscall,a2,a2??,a6,a3,a4,a5,a8,a9,0,"memory"
#else
	#error architecture not supported by bQc
#endif

//Crap should have been: SP,PRE6-1,SYSCALL,POST1-6,RET2,RET,NUM,ARG1-ARG6,CLOBBERS
//ex. x86
// 	#define ARCH	"esp","","","","","","","int $128","","","","","","", \
//	0,NA,"eax","eax","ebx","ecx","edx","esi","edi","ebp",  "memory"
//ex. arch
//  #define ARCH	"sp","","","","","","","callsys #  %0 %1 %2"," %3"," %4"," %5"," %6"," %7"," %8", \
//  1,"$19","$0", "$0","$16","$17","$18","$19","$20","$21",  "$22","$23","$24","$25","$27","$28","$1","$2","$3","$4","$5","$6","$7","$8","memory"
// The syscall pre and post strings get concatenated together as needed
// This would be for architectures that change the syscall string according to # of args
// arch uses: (((((callsys # %0) %1) %2) %3) %4) %5)
// while other architectures may need additional prefix instructions (mov, etc...)

#endif
#if 1 //constants
#define AF_AAL5     	8
#define AF_APPLETALK	5
#define AF_AX25	    	3
#define AF_BRIDGE   	7
#define AF_INET	    	2
#define AF_INET6    	10
#define AF_IPX      	4
#define AF_MAX      	12
#define AF_NETROM   	6
#define AF_UNIX	    	1
#define AF_UNSPEC   	0
#define AF_X25      	9
#define __ARCH_SI_PREAMBLE_SIZE	(3 * sizeof(int))
#define __ARCH_SIGEV_PREAMBLE_SIZE	(sizeof(int) * 2 + sizeof(sigval_t))
#define ASK_VGA	0xfffd
#define B0	0000000
#define B1000000	0010010
#define B110	0000003
#define B1152000	0010011
#define B115200	0010002
#define B1200	0000011
#define B134	0000004
#define B1500000	0010012
#define B150	0000005
#define B1800	0000012
#define B19200	0000016
#define B2000000	0010013
#define B200	0000006
#define B230400	0010003
#define B2400	0000013
#define B2500000	0010014
#define B3000000	0010015
#define B300	0000007
#define B3500000	0010016
#define B38400	0000017
#define B4000000	0010017
#define B460800	0010004
#define B4800	0000014
#define B50	0000001
#define B500000	0010005
#define B576000	0010006
#define B57600	0010001
#define B600	0000010
#define B75	0000002
#define B921600	0010007
#define B9600	0000015
#define BOTHER	0010000
#define BRKINT	0000002
#define BS0	0000000
#define BS1	0020000
#define BSDLY	0020000
#define BUS_ADRALN	(__SI_FAULT|1)
#define BUS_ADRERR	(__SI_FAULT|2)
#define BUS_MCEERR_AO	(__SI_FAULT|5)
#define BUS_MCEERR_AR	(__SI_FAULT|4)
#define BUS_OBJERR	(__SI_FAULT|3)
#define CAN_USE_HEAP	128
#define CBAUD	0010017
#define CBAUDEX	0010000
#define CIBAUD	 002003600000
#define CLD_CONTINUED	(__SI_CHLD|6)
#define CLD_DUMPED	(__SI_CHLD|3)
#define CLD_EXITED	(__SI_CHLD|1)
#define CLD_KILLED	(__SI_CHLD|2)
#define CLD_STOPPED	(__SI_CHLD|5)
#define CLD_TRAPPED	(__SI_CHLD|4)
#define CLOCAL	0004000
#define CLONE_CHILD_CLEARTID	0x00200000
#define CLONE_CHILD_SETTID	0x01000000
#define CLONE_DETACHED	0x00400000
#define CLONE_FILES	0x00000400
#define CLONE_FS	0x00000200
#define CLONE_IO	0x80000000
#define CLONE_NEWIPC	0x08000000
#define CLONE_NEWNET	0x40000000
#define CLONE_NEWNS	0x00020000
#define CLONE_NEWPID	0x20000000
#define CLONE_NEWUSER	0x10000000
#define CLONE_NEWUTS	0x04000000
#define CLONE_PARENT	0x00008000
#define CLONE_PARENT_SETTID	0x00100000
#define CLONE_PTRACE	0x00002000
#define CLONE_SETTLS	0x00080000
#define CLONE_SIGHAND	0x00000800
#define CLONE_SYSVSEM	0x00040000
#define CLONE_THREAD	0x00010000
#define CLONE_UNTRACED	0x00800000
#define CLONE_VFORK	0x00004000
#define CLONE_VM	0x00000100
#define CMSPAR	 010000000000
#define COMMAND_LINE_SIZE	512
#define CR0	0000000
#define CR1	0001000
#define CR2	0002000
#define CR3	0003000
#define CRDLY	0003000
#define CREAD	0000200
#define CRTSCTS	 020000000000
#define CS5	0000000
#define CS6	0000020
#define CS7	0000040
#define CS8	0000060
#define CSIGNAL	0x000000ff
#define CSIZE	0000060
#define CSTOPB	0000100
#define E2BIG	 7
#define EACCES	13
#define EADDRINUSE	98
#define EADDRNOTAVAIL	99
#define EADV	68
#define EAFNOSUPPORT	97
#define EAGAIN	11
#define EALREADY	114
#define EBADE	52
#define EBADF	 9
#define EBADFD	77
#define EBADMSG	74
#define EBADR	53
#define EBADRQC	56
#define EBADSLT	57
#define EBFONT	59
#define EBUSY	16
#define ECANCELED	125
#define ECHILD	10
#define ECHO	0000010
#define ECHOCTL	0001000
#define ECHOE	0000020
#define ECHOK	0000040
#define ECHOKE	0004000
#define ECHONL	0000100
#define ECHOPRT	0002000
#define ECHRNG	44
#define ECOMM	70
#define ECONNABORTED	103
#define ECONNREFUSED	111
#define ECONNRESET	104
#define EDEADLK	35
#define EDEADLOCK	EDEADLK
#define EDESTADDRREQ	89
#define EDOM	33
#define EDOTDOT	73
#define EDQUOT	122
#define EEXIST	17
#define EFAULT	14
#define EFBIG	27
#define EHOSTDOWN	112
#define EHOSTUNREACH	113
#define EHWPOISON	133
#define EIDRM	43
#define EILSEQ	84
#define EINPROGRESS	115
#define EINTR	 4
#define EINVAL	22
#define EIO	 5
#define EISCONN	106
#define EISDIR	21
#define EISNAM	120
#define EKEYEXPIRED	127
#define EKEYREJECTED	129
#define EKEYREVOKED	128
#define EL2HLT	51
#define EL2NSYNC	45
#define EL3HLT	46
#define EL3RST	47
#define ELAST	134
#define ELIBACC	79
#define ELIBBAD	80
#define ELIBEXEC	83
#define ELIBMAX	82
#define ELIBSCN	81
#define ELNRNG	48
#define ELOOP	40
#define EMEDIUMTYPE	124
#define EMFILE	24
#define EMLINK	31
#define EMSGSIZE	90
#define EMULTIHOP	72
#define ENAMETOOLONG	36
#define ENAVAIL	119
#define ENETDOWN	100
#define ENETRESET	102
#define ENETUNREACH	101
#define ENFILE	23
#define ENOANO	55
#define ENOBUFS	105
#define ENOCSI	50
#define ENODATA	61
#define ENODEV	19
#define ENOENT	 2
#define ENOEXEC	 8
#define ENOKEY	126
#define ENOLCK	37
#define ENOLINK	67
#define ENOMEDIUM	123
#define ENOMEM	12
#define ENOMSG	42
#define ENONET	64
#define ENOPKG	65
#define ENOPROTOOPT	92
#define ENOSPC	28
#define ENOSR	63
#define ENOSTR	60
#define ENOSYS	38
#define ENOTBLK	15
#define ENOTCONN	107
#define ENOTDIR	20
#define ENOTEMPTY	39
#define ENOTNAM	118
#define ENOTRECOVERABLE	131
#define ENOTSOCK	88
#define ENOTSUP	EOPNOTSUPP
#define ENOTTY	25
#define ENOTUNIQ	76
#define ENXIO	 6
#define EOPNOTSUPP	95
#define EOVERFLOW	75
#define EOWNERDEAD	130
#define EPERM	 1
#define EPFNOSUPPORT	96
#define EPIPE	32
#define EPROTO	71
#define EPROTONOSUPPORT	93
#define EPROTOTYPE	91
#define ERANGE	34
#define EREMCHG	78
#define EREMOTE	66
#define EREMOTEIO	121
#define ERESTART	85
#define ERFKILL	132
#define EROFS	30
#define ESHUTDOWN	108
#define ESOCKTNOSUPPORT	94
#define ESPIPE	29
#define ESRCH	 3
#define ESRMNT	69
#define ESTALE	116
#define ESTRPIPE	86
#define ETIME	62
#define ETIMEDOUT	110
#define ETOOMANYREFS	109
#define ETXTBSY	26
#define EUCLEAN	117
#define EUNATCH	49
#define EUSERS	87
#define EWOULDBLOCK	EAGAIN
#define EXDEV	18
#define EXEC_PAGESIZE	4096
#define EXFULL	54
#define EXTA	B19200
#define EXTB	B38400
#define EXTENDED_VGA	0xfffe
#define EXTPROC	0200000
#define FASYNC     	00020000
#define FD_CLOEXEC	1
#define F_DUPFD	0
#define F_EXLCK	4
#define FF0	0000000
#define FF1	0100000
#define FFDLY	0100000
#define F_GETFD	1
#define F_GETFL	3
#define F_GETLK	5
#define F_GETLK64	12
#define F_GETOWN	9
#define F_GETOWNER_UIDS	17
#define F_GETOWN_EX	16
#define F_GETSIG	11
#define FIOASYNC	0x5452
#define FIOCLEX	0x5451
#define FIOGETOWN	0x8903
#define FIONBIO	0x5421
#define FIONCLEX	0x5450
#define FIONREAD	0x541B
#define FIOQSIZE	0x5460
#define FIOSETOWN	0x8901
#define F_LINUX_SPECIFIC_BASE	1024
#define FLUSHO	0010000
#define F_OWNER_PGRP	2
#define F_OWNER_PID	1
#define F_OWNER_TID	0
#define FPE_FLTDIV	(__SI_FAULT|3)
#define FPE_FLTINV	(__SI_FAULT|7)
#define FPE_FLTOVF	(__SI_FAULT|4)
#define FPE_FLTRES	(__SI_FAULT|6)
#define FPE_FLTSUB	(__SI_FAULT|8)
#define FPE_FLTUND	(__SI_FAULT|5)
#define FPE_INTDIV	(__SI_FAULT|1)
#define FPE_INTOVF	(__SI_FAULT|2)
#define F_RDLCK	0
#define F_SETFD	2
#define F_SETFL	4
#define F_SETLK	6
#define F_SETLK64	13
#define F_SETLKW64	14
#define F_SETLKW	7
#define F_SETOWN	8
#define F_SETOWN_EX	15
#define F_SETSIG	10
#define F_SHLCK	8
#define F_UNLCK	2
#define F_WRLCK	1
#define HUPCL	0002000
#define HZ	100
#define IBSHIFT	 16
#define ICANON	0000002
#define ICRNL	0000400
#define IEXTEN	0100000
#define IGNBRK	0000001
#define IGNCR	0000200
#define IGNPAR	0000004
#define ILL_BADSTK	(__SI_FAULT|8)
#define ILL_COPROC	(__SI_FAULT|7)
#define ILL_ILLADR	(__SI_FAULT|3)
#define ILL_ILLOPC	(__SI_FAULT|1)
#define ILL_ILLOPN	(__SI_FAULT|2)
#define ILL_ILLTRP	(__SI_FAULT|4)
#define ILL_PRVOPC	(__SI_FAULT|5)
#define ILL_PRVREG	(__SI_FAULT|6)
#define IMAXBEL	0020000
#define INLCR	0000100
#define INPCK	0000020
#define _IOC_DIRBITS	2
#define _IOC_DIRMASK	((1 << _IOC_DIRBITS)-1)
#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)
#define _IOC_NONE	0U
#define _IOC_NRBITS	8
#define _IOC_NRMASK	((1 << _IOC_NRBITS)-1)
#define _IOC_NRSHIFT	0
#define _IOC_READ	2U
#define _IOC_SIZEBITS	14
#define _IOC_SIZEMASK	((1 << _IOC_SIZEBITS)-1)
#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_TYPEBITS	8
#define _IOC_TYPEMASK	((1 << _IOC_TYPEBITS)-1)
#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_WRITE	1U
#define IPPROTO_AH	51
#define IPPROTO_BEETPH	94
#define IPPROTO_COMP	108
#define IPPROTO_DCCP	33
#define IPPROTO_EGP	8
#define IPPROTO_ENCAP	98
#define IPPROTO_ESP	50
#define IPPROTO_GRE	47
#define IPPROTO_ICMP	1
#define IPPROTO_IDP	22
#define IPPROTO_IGMP	2
#define IPPROTO_IP	0
#define IPPROTO_IPIP	4
#define IPPROTO_IPV6	41
#define IPPROTO_MAX =256
#define IPPROTO_MTP	92
#define IPPROTO_PIM	103
#define IPPROTO_PUP	12
#define IPPROTO_RAW	255
#define IPPROTO_RSVP	46
#define IPPROTO_SCTP	132
#define IPPROTO_TCP	6
#define IPPROTO_TP	29
#define IPPROTO_UDP	17
#define IPPROTO_UDPLITE	136
#define ISIG	0000001
#define ISTRIP	0000040
#define IUCLC	0001000
#define IUTF8	0040000
#define IXANY	0004000
#define IXOFF	0010000
#define IXON	0002000
#define KEEP_SEGMENTS	64
#define LOADED_HIGH	= 1
#define LOCK_EX	2
#define LOCK_MAND	32
#define LOCK_NB	4
#define LOCK_READ	64
#define LOCK_RW	192
#define LOCK_SH	1
#define LOCK_UN	8
#define LOCK_WRITE	128
#define MADV_DODUMP	17
#define MADV_DOFORK	11
#define MADV_DONTDUMP	16
#define MADV_DONTFORK	10
#define MADV_DONTNEED	4
#define MADV_HUGEPAGE	14
#define MADV_HWPOISON	100
#define MADV_MERGEABLE	12
#define MADV_NOHUGEPAGE	15
#define MADV_NORMAL	0
#define MADV_RANDOM	1
#define MADV_REMOVE	9
#define MADV_SEQUENTIAL	2
#define MADV_SOFT_OFFLINE	101
#define MADV_UNMERGEABLE	13
#define MADV_WILLNEED	3
#define MAP_ANONYMOUS	0x20
#define MAP_FILE	0
#define MAP_FIXED	0x10
#define MAP_HUGE_MASK	0x3f
#define MAP_HUGE_SHIFT	26
#define MAP_PRIVATE	0x02
#define MAP_SHARED	0x01
#define MAP_TYPE	0x0f
#ifdef CONFIG_MMAP_ALLOW_UNINITIALIZED
#define MAP_UNINITIALIZED	0x4000000
#else
#define MAP_UNINITIALIZED	0x0
#endif
#define MAXHOSTNAMELEN	64
#define MS_ASYNC	1
#define MS_INVALIDATE	2
#define MS_SYNC	4
#define NCC	8
#define NCCS	32
#define NL0	0000000
#define NL1	0000400
#define NLDLY	0000400
#define NOFLSH	0000200
#define NOGROUP	(-1)
#define NORMAL_VGA	0xffff
#define NSIGBUS	5
#define NSIGCHLD	6
#define NSIGFPE	8
#define NSIGILL	8
#define NSIGPOLL	6
#define NSIGSEGV	2
#define NSIGSYS	1
#define NSIGTRAP	4
#define O_ACCMODE  	00000003
#define O_APPEND   	00002000
#define O_CLOEXEC  	02000000
#define O_CREAT    	00000100
#define OCRNL	0000010
#define O_DIRECT   	00040000
#define O_DIRECTORY	00200000
#define O_DSYNC    	00010000
#define O_EXCL     	00000200
#define OFDEL	0000200
#define OFILL	0000100
#define O_LARGEFILE	00100000
#define OLCUC	0000002
#define O_NDELAY   	00004000
#define ONLCR	0000004
#define ONLRET	0000040
#define O_NOATIME  	01000000
#define ONOCR	0000020
#define O_NOCTTY   	00000400
#define O_NOFOLLOW 	00400000
#define O_NONBLOCK 	00004000
#define O_PATH     	010000000
#define OPOST	0000001
#define O_RDONLY   	00000000
#define O_RDWR     	00000002
#define __O_SYNC   	04000000
#define __O_TMPFILE	020000000
#define O_TMPFILE  	020200000
#define O_TMPFILE_MASK =020200100
#define O_TRUNC    	00001000
#define O_WRONLY   	00000001
#define PAGE_SIZE	4096
#define P_ALL	0
#define PARENB	0000400
#define PARMRK	0000010
#define PARODD	0001000
#define PENDIN	0040000
#define PF_AAL5     	AF_AAL5
#define PF_APPLETALK	AF_APPLETALK
#define PF_AX25     	AF_AX25
#define PF_BRIDGE   	AF_BRIDGE
#define PF_INET6    	AF_INET6
#define PF_INET     	AF_INET
#define PF_IPX      	AF_IPX
#define PF_MAX      	AF_MAX
#define PF_NETROM   	AF_NETROM
#define PF_UNIX     	AF_UNIX
#define PF_UNSPEC   	AF_UNSPEC
#define PF_X25      	AF_X25
#define POLL_BUSY_LOOP	0x8000
#define POLLERR	0x0008
#define POLL_ERR	(__SI_POLL|4)
#define POLLFREE	0x4000
#define POLLHUP	0x0010
#define POLL_HUP	(__SI_POLL|6)
#define POLLIN	0x0001
#define POLL_IN	(__SI_POLL|1)
#define POLLMSG	0x0400
#define POLL_MSG	(__SI_POLL|3)
#define POLLNVAL	0x0020
#define POLLOUT	0x0004
#define POLL_OUT	(__SI_POLL|2)
#define POLLPRI	0x0002
#define POLL_PRI	(__SI_POLL|5)
#define POLLRDBAND	0x0080
#define POLLRDHUP	0x2000
#define POLLRDNORM	0x0040
#define POLLREMOVE	0x1000
#define POLLWRBAND	0x0200
#define POLLWRNORM	0x0100
#define P_PGID	2
#define P_PID	1
#define PROT_EXEC	0x4
#define PROT_GROWSDOWN	0x01000000
#define PROT_GROWSUP	0x02000000
#define PROT_NONE	0x0
#define PROT_READ	0x1
#define PROT_SEM	0x8
#define PROT_WRITE	0x2
#define QUIET_FLAG	32
#define RAMDISK_IMAGE_START_MASK	0x07FF
#define RAMDISK_LOAD_FLAG	0x4000
#define RAMDISK_PROMPT_FLAG	0x8000
#define RLIM_INFINITY	(~0UL)
#define RLIMIT_AS	9
#define RLIMIT_CORE	4
#define RLIMIT_CPU	0
#define RLIMIT_DATA	2
#define RLIMIT_FSIZE	1
#define RLIMIT_LOCKS	10
#define RLIMIT_MEMLOCK	8
#define RLIMIT_MSGQUEUE	12
#define RLIMIT_NICE	13
#define RLIMIT_NOFILE	7
#define RLIMIT_NPROC	6
#define RLIMIT_RSS	5
#define RLIMIT_RTPRIO	14
#define RLIMIT_RTTIME	15
#define RLIMIT_SIGPENDING	11
#define RLIMIT_STACK	3
#define RLIM_NLIMITS	16
#define SCM_TIMESTAMPING	SO_TIMESTAMPING
#define SCM_TIMESTAMPNS	SO_TIMESTAMPNS
#define SCM_TIMESTAMP	SO_TIMESTAMP
#define SCM_WIFI_STATUS	SO_WIFI_STATUS
#define SEGV_ACCERR	(__SI_FAULT|2)
#define SEGV_MAPERR	(__SI_FAULT|1)
#define SETUP_DTB	2
#define SETUP_E820_EXT	1
#define SETUP_NONE	0
#define SETUP_PCI	3
#define SHMLBA	PAGE_SIZE
#define SI_ASYNCIO	-4
#define __SI_CHLD	0
#define SI_DETHREAD	-7
#define __SI_FAULT	0
#define SIGEV_MAX_SIZE	64
#define SIGEV_NONE	1
#define SIGEV_PAD_SIZE	((SIGEV_MAX_SIZE - __ARCH_SIGEV_PREAMBLE_SIZE) / sizeof(int))
#define SIGEV_SIGNAL	0
#define SIGEV_THREAD	2
#define SIGEV_THREAD_ID	4
#define SI_KERNEL	0x80
#define __SI_KILL	0
#define __SI_MESGQ	0
#define SI_MESGQ	-3
#define SI_MAX_SIZE	128
#define SIOCATMARK	0x8905
#define SIOCGPGRP	0x8904
#define SIOCGSTAMP	0x8906 
#define SIOCGSTAMPNS	0x8907
#define SIOCSPGRP	0x8902
#define SI_PAD_SIZE	((SI_MAX_SIZE - __ARCH_SI_PREAMBLE_SIZE) / sizeof(int))
#define __SI_POLL	0
#define SI_QUEUE	-1
#define __SI_RT	0
#define SI_SIGIO	-5
#define __SI_SYS	0
#define __SI_TIMER	0
#define SI_TIMER	-2
#define SI_TKILL	-6
#define SI_USER	0
#define SO_ACCEPTCONN	30
#define SO_ATTACH_FILTER	26
#define SO_BINDTODEVICE	25
#define SO_BROADCAST	6
#define SO_BSDCOMPAT	14
#define SO_BUSY_POLL	46
#define SOCK_DGRAM    	2
#define SOCK_RAW      	3
#define SOCK_RDM      	4
#define SOCK_SEQPACKET	5
#define SOCK_STREAM   	1
#define SO_DEBUG	1
#define SO_DETACH_FILTER	27
#define SO_DOMAIN	39
#define SO_DONTROUTE	5
#define SO_ERROR	4
#define SO_GET_FILTER	SO_ATTACH_FILTER
#define SO_KEEPALIVE	9
#define SO_LINGER	13
#define SO_LOCK_FILTER	44
#define SOL_SOCKET	1
#define SO_MARK	36
#define SO_NO_CHECK	11
#define SO_NOFCS	43
#define SO_OOBINLINE	10
#define SO_PASSCRED	16
#define SO_PASSSEC	34
#define SO_PEEK_OFF	42
#define SO_PEERCRED	17
#define SO_PEERNAME	28
#define SO_PEERSEC	31
#define SO_PRIORITY	12
#define SO_PROTOCOL	38
#define SO_RCVBUF	8
#define SO_RCVBUFFORCE	33
#define SO_RCVLOWAT	18
#define SO_RCVTIMEO	20
#define SO_REUSEADDR	2
#define SO_REUSEPORT	15
#define SO_RXQ_OVFL	40
#define SO_SECURITY_AUTHENTICATION	22
#define SO_SECURITY_ENCRYPTION_NETWORK	24
#define SO_SECURITY_ENCRYPTION_TRANSPORT	23
#define SO_SELECT_ERR_QUEUE	45
#define SO_SNDBUF	7
#define SO_SNDBUFFORCE	32
#define SO_SNDLOWAT	19
#define SO_SNDTIMEO	21
#define SO_TIMESTAMP	29
#define SO_TIMESTAMPING	37
#define SO_TIMESTAMPNS	35
#define SO_TYPE	3
#define SO_WIFI_STATUS	41
#define _STK_LIM_MAX	RLIM_INFINITY
#define SYS_SECCOMP	(__SI_SYS|1)
#define TAB0	0000000
#define TAB1	0004000
#define TAB2	0010000
#define TAB3	0014000
#define TABDLY	0014000
#define TCFLSH	0x540B
#define TCGETA	0x5405
#define TCGETS	0x5401
#define TCGETS2	_IOR('T' 0x2A struct termios2)
#define TCGETX	0x5432
#define TCIFLUSH	0
#define TCIOFF	2
#define TCIOFLUSH	2
#define TCION	3
#define TCOFLUSH	1
#define TCOOFF	0
#define TCOON	1
#define TCSADRAIN	1
#define TCSAFLUSH	2
#define TCSANOW	0
#define TCSBRK	0x5409
#define TCSBRKP	0x5425
#define TCSETA	0x5406
#define TCSETAF	0x5408
#define TCSETAW	0x5407
#define TCSETS	0x5402
#define TCSETS2	_IOW('T' 0x2B struct termios2)
#define TCSETSF	0x5404
#define TCSETSF2	_IOW('T' 0x2D struct termios2)
#define TCSETSW	0x5403
#define TCSETSW2	_IOW('T' 0x2C struct termios2)
#define TCSETX	0x5433
#define TCSETXF	0x5434
#define TCSETXW	0x5435
#define TCXONC	0x540A
#define TIOCCBRK	0x5428 
#define TIOCCONS	0x541D
#define TIOCEXCL	0x540C
#define TIOCGDEV	_IOR('T' 0x32 unsigned int)
#define TIOCGETD	0x5424
#define TIOCGEXCL	_IOR('T' 0x40 int)
#define TIOCGICOUNT	0x545D
#define TIOCGLCKTRMIOS	0x5456
#define TIOCGPGRP	0x540F
#define TIOCGPKT	_IOR('T' 0x38 int)
#define TIOCGPTLCK	_IOR('T' 0x39 int)
#define TIOCGPTN	_IOR('T' 0x30 unsigned int)
#define TIOCGRS485	0x542E
#define TIOCGSERIAL	0x541E
#define TIOCGSID	0x5429 
#define TIOCGSOFTCAR	0x5419
#define TIOCGWINSZ	0x5413
#define TIOCINQ	FIONREAD
#define TIOCLINUX	0x541C
#define TIOCMBIC	0x5417
#define TIOCMBIS	0x5416
#define TIOCM_CAR	0x040
#define TIOCM_CD	TIOCM_CAR
#define TIOCM_CTS	0x020
#define TIOCM_DSR	0x100
#define TIOCM_DTR	0x002
#define TIOCMGET	0x5415
#define TIOCMIWAIT	0x545C
#define TIOCM_LE	0x001
#define TIOCM_LOOP	0x8000
#define TIOCM_OUT1	0x2000
#define TIOCM_OUT2	0x4000
#define TIOCM_RI	TIOCM_RNG
#define TIOCM_RNG	0x080
#define TIOCM_RTS	0x004
#define TIOCMSET	0x5418
#define TIOCM_SR	0x010
#define TIOCM_ST	0x008
#define TIOCNOTTY	0x5422
#define TIOCNXCL	0x540D
#define TIOCOUTQ	0x5411
#define TIOCPKT	0x5420
#define TIOCSBRK	0x5427 
#define TIOCSCTTY	0x540E
#define TIOCSERCONFIG	0x5453
#define TIOCSERGETLSR	0x5459
#define TIOCSERGETMULTI	0x545A
#define TIOCSERGSTRUCT	0x5458
#define TIOCSERGWILD	0x5454
#define TIOCSERSETMULTI	0x545B
#define TIOCSERSWILD	0x5455
#define TIOCSETD	0x5423
#define TIOCSIG	_IOW('T' 0x36 int) 
#define TIOCSLCKTRMIOS	0x5457
#define TIOCSPGRP	0x5410
#define TIOCSPTLCK	_IOW('T' 0x31 int) 
#define TIOCSRS485	0x542F
#define TIOCSSERIAL	0x541F
#define TIOCSSOFTCAR	0x541A
#define TIOCSTI	0x5412
#define TIOCSWINSZ	0x5414
#define TIOCVHANGUP	0x5437
#define TOSTOP	0000400
#define TRAP_BRANCH	(__SI_FAULT|3)
#define TRAP_BRKPT	(__SI_FAULT|1)
#define TRAP_HWBKPT	(__SI_FAULT|4)
#define TRAP_TRACE	(__SI_FAULT|2)
#define VDISCARD	13
#define VEOF	4
#define VEOL	11
#define VEOL2	16
#define VERASE	2
#define VINTR	0
#define VKILL	3
#define VLNEXT	15
#define VMIN	6
#define VQUIT	1
#define VREPRINT	12
#define VSTART	8
#define VSTOP	9
#define VSUSP	10
#define VSWTC	7
#define VT0	0000000
#define VT1	0040000
#define VTDLY	0040000
#define VTIME	5
#define VWERASE	14
#define __WALL	0x40000000
#define __WCLONE	0x80000000
#define WCONTINUED	0x00000008
#define WEXITED	0x00000004
#define WNOHANG	0x00000001
#define __WNOTHREAD	0x20000000
#define WNOWAIT	0x01000000
#define WSTOPPED	WUNTRACED
#define WUNTRACED	0x00000002
#define X86_NR_SUBARCHS	5
#define X86_SUBARCH_CE4100	4
#define X86_SUBARCH_LGUEST	1
#define X86_SUBARCH_MRST	3
#define X86_SUBARCH_PC	0
#define X86_SUBARCH_XEN	2
#define XCASE	0000004
#define XLF_CAN_BE_LOADED_ABOVE_4G	2
#define XLF_EFI_HANDOVER_32	4
#define XLF_EFI_HANDOVER_64	8
#define XLF_KERNEL_64	1
#define XTABS	0014000
#endif
#if 1 //macros
#ifndef NULL
#define NULL ((void *)0)
#endif
#define CHAR_BIT __CHAR_BIT__ //__CHAR_BIT__ is passed by the compiler
#if __BITS_PER_LONG != 64
#define IFBITSPERLONGNEQ64(...) __VA_ARGS__
#define IFBITSPERLONGEQ64(...)
#else
#define IFBITSPERLONGNEQ64(...)
#define IFBITSPERLONGEQ64(...) __VA_ARGS__
#endif
#ifndef  __ARCH_SI_ATTRIBUTES
#define __ARCH_SI_ATTRIBUTES
#endif

#define ROL(x,y) (x<<y)|(x>>((sizeof(x)*CHAR_BIT) -y))
#define ROR(x,y) (x>>y)|(x<<((sizeof(x)*CHAR_BIT) -y))

#define SI_FROMUSER(siptr)	((siptr)->si_code <= 0),
#define SI_FROMKERNEL(siptr)	((siptr)->si_code > 0),

#define _IOC(d,t,n,sz) (((d) << _IOC_DIRSHIFT) | ((t) << _IOC_TYPESHIFT) | \
	((n) << _IOC_NRSHIFT) | ((sz) << _IOC_SIZESHIFT))
#define _IOC_TYPECHECK(t) (sizeof(t))
#define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOWR(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOR_BAD(type,nr,size)	_IOC(_IOC_READ,(type),(nr),sizeof(size))
#define _IOW_BAD(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),sizeof(size))
#define _IOWR_BAD(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))
#define _IOC_DIR(nr)		(((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr)		(((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)		(((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr)		(((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)
#define IOC_IN		(_IOC_WRITE << _IOC_DIRSHIFT)
#define IOC_OUT		(_IOC_READ << _IOC_DIRSHIFT)
#define IOC_INOUT	((_IOC_WRITE|_IOC_READ) << _IOC_DIRSHIFT)
#define IOCSIZE_MASK	(_IOC_SIZEMASK << _IOC_SIZESHIFT)
#define IOCSIZE_SHIFT	(_IOC_SIZESHIFT)

#if __STDC_VERSION__ >= 201103L
#define noreturn _Noreturn
#else
#define noreturn __attribute__((noreturn))
#endif

#endif
#if 1 //syscall macro stuff, TODO - handle multiple returns and extra clobs

#define _P(x, y) x ## y
#define P(x, y) _P(x, y)
#define _H(x,...) x##_(__VA_ARGS__)
#define	STACK_POINTER_(sp,...) sp
#define	STACK_POINTER _H(STACK_POINTER,ARCH_DATA)
#define	SYSCALL_(a,syscall,...) syscall
#define	SYSCALL _H(SYSCALL,ARCH_DATA)
#define	CALL_NUMBER_(a,b,cnum,...) cnum
#define	CALL_NUMBER _H(CALL_NUMBER,ARCH_DATA)
#define	RETURN_REGISTER_(a,b,c,ret,...) ret
#define	RETURN_REGISTER  _H(RETURN_REGISTER,ARCH_DATA)
#define	ARG1_(sp,sysc,cnum,ret,a1,...) a1
#define	ARG1 _H(ARG1,ARCH_DATA)
#define	ARG2_(sp,sysc,cnum,ret,a1,a2,...) a2
#define	ARG2 _H(ARG2,ARCH_DATA)
#define	ARG3_(sp,sysc,cnum,ret,a1,a2,a3,...) a3
#define	ARG3 _H(ARG3,ARCH_DATA)
#define	ARG4_(sp,sysc,cnum,ret,a1,a2,a3,a4,...) a4
#define	ARG4 _H(ARG4,ARCH_DATA)
#define	ARG5_(sp,sysc,cnum,ret,a1,a2,a3,a4,a5,...) a5
#define	ARG5 _H(ARG5,ARCH_DATA)
#define	ARG6_(sp,sysc,cnum,ret,a1,a2,a3,a4,a5,a6,...) a6
#define	ARG6 _H(ARG6,ARCH_DATA)
#define	ARG7_(sp,sysc,cnum,ret,a1,a2,a3,a4,a5,a6,a7,...) a7
#define	ARG7 _H(ARG7,ARCH_DATA)
#define	CLOB_(sp,sysc,cnum,ret,a1,a2,a3,a4,a5,a6,a7,...) __VA_ARGS__
#define	CLOB _H(CLOB,ARCH_DATA)
//TODO add option to clobber unused arguments of some platforms
//ex. CLOB3_(sp,sysc,cnum,ret,a1,a2,a3,...) __VA_ARGS__

#define SETREGISTER(var,reg,val) register long var __asm__(reg) = val
#define SETRETURN_REGISTER register long ret __asm__(RETURN_REGISTER)
#define SETREGISTERS0 SETRETURN_REGISTER;SETREGISTER(r0,CALL_NUMBER,cnum)
#define SETREGISTERS1 SETREGISTERS0;SETREGISTER(r1,ARG1,a1)
#define SETREGISTERS2 SETREGISTERS1;SETREGISTER(r2,ARG2,a2)
#define SETREGISTERS3 SETREGISTERS2;SETREGISTER(r3,ARG3,a3)
#define SETREGISTERS4 SETREGISTERS3;SETREGISTER(r4,ARG4,a4)
#define SETREGISTERS5 SETREGISTERS4;SETREGISTER(r5,ARG5,a5)
#define SETREGISTERS6 SETREGISTERS5;SETREGISTER(r6,ARG6,a6)
#define SETREGISTERS7 SETREGISTERS6;SETREGISTER(r7,ARG7,a7)

#define RET "=r"(ret)
#define REGS0 "r"(r0)
#define REGS1 REGS0,"r"(r1)
#define REGS2 REGS1,"r"(r2)
#define REGS3 REGS2,"r"(r3)
#define REGS4 REGS3,"r"(r4)
#define REGS5 REGS4,"r"(r5)
#define REGS6 REGS5,"r"(r6)
#define REGS7 REGS6,"r"(r7)

#define MKFNS(fn,...) MKFN_N(fn,##__VA_ARGS__,9,8,7,6,5,4,3,2,1,0)(__VA_ARGS__)
#define MKFN_N(fn, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n, ...) fn##n
#define syscall(...) MKFNS(syscall,##__VA_ARGS__)
#define syscall0(n) _syscall0(n)
#define syscall1(n,a) _syscall1(n,(long)(a))
#define syscall2(n,a,b) _syscall2(n,(long)(a),(long)(b))
#define syscall3(n,a,b,c) _syscall3(n,(long)(a),(long)(b),(long)(c))
#define syscall4(n,a,b,c,d) _syscall4(n,(long)(a),(long)(b),(long)(c),(long)(d))
#define syscall5(n,a,b,c,d,e) _syscall5(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e))
#define syscall6(n,a,b,c,d,e,f) _syscall6(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f))
#define syscall7(n,a,b,c,d,e,f,g) _syscall7(n,(long)(a),(long)(b),(long)(c),(long)(d),(long)(e),(long)(f),(long)(g))
#endif 

#if 1 //typedefs 
typedef unsigned char byte, cc_t, __mmask8, u8, __u8,  uint8, uint8_t;
typedef unsigned short gid16_t, __mmask16, old_gid_t, old_uid_t, sa_family_t, u16, __u16, uid16_t, uint16, uint16_t, word;
typedef unsigned int dword, __mmask32, mode_t, daddr_t, gid32_t, gid_t, old_dev_t, speed_t, tcflag_t, u32, __u32, ucs4_t, uid_t, uid32_t, uint, uint32, uint32_t;
typedef unsigned long long __mmask64, u64, __u64, uint64, uint64_t;

typedef signed char int8, int8_t, s8, __s8;
typedef signed short int16_t, s16, __s16;
typedef signed int bool, clockid_t, int32_t,  ipc_pid_t, key_t, mqd_t, pid_t, s32, __s32, timer_t;
typedef signed long long  loff_t, int64_t, s64, __s64;

typedef char * caddr_t;
typedef unsigned long ino_t, size_t, sigset_t, ulong, ulong_t;
typedef long  band_t, clock_t, long_t, off_t, ptrdiff_t, ssize_t, suseconds_t, time_t;

typedef char __v8qi __attribute__((__vector_size__(8)));
typedef char __v16qi __attribute__((__vector_size__(16)));
typedef char __v32qi __attribute__ ((__vector_size__ (32)));
typedef char __v64qi __attribute__ ((__vector_size__ (64)));
typedef signed char __v16qs __attribute__((__vector_size__(16)));
typedef signed char __v32qs __attribute__((__vector_size__(32)));

typedef short __v4hi __attribute__((__vector_size__(8)));
typedef short __v8hi __attribute__((__vector_size__(16)));
typedef short __v16hi __attribute__ ((__vector_size__ (32)));
typedef short __v32hi __attribute__ ((__vector_size__ (64)));

typedef int __v2si __attribute__((__vector_size__(8)));
typedef int __v4si __attribute__((__vector_size__(16)));
typedef int __v8si __attribute__ ((__vector_size__ (32)));
typedef int __v16si __attribute__((__vector_size__(64)));

typedef long long __m64 __attribute__((__vector_size__(8)));
typedef long long __m128i __attribute__((__vector_size__(16)));
typedef long long __m256i __attribute__((__vector_size__(32)));
typedef long long __m512i __attribute__((__vector_size__(64)));
typedef long long __v2di __attribute__ ((__vector_size__ (16)));
typedef long long __v4di __attribute__ ((__vector_size__ (32)));
typedef long long __v8di __attribute__((__vector_size__(64)));

typedef float __v2sf __attribute__((__vector_size__(8)));
typedef float __v4sf __attribute__((__vector_size__(16)));
typedef float __v8sf __attribute__ ((__vector_size__ (32)));
typedef float __v16sf __attribute__((__vector_size__(64)));
typedef float __mm_loadh_pi_v2f32 __attribute__((__vector_size__(8)));
typedef float __m128 __attribute__((__vector_size__(16)));
typedef float __m256 __attribute__ ((__vector_size__ (32)));
typedef float __m512 __attribute__((__vector_size__(64)));

typedef double __m128d __attribute__((__vector_size__(16)));
typedef double __m256d __attribute__((__vector_size__(32)));
typedef double __m512d __attribute__((__vector_size__(64)));
typedef double __v2df __attribute__ ((__vector_size__ (16)));
typedef double __v4df __attribute__ ((__vector_size__ (32)));
typedef double __v8df __attribute__((__vector_size__(64)));

#endif
#if 1 //structs
struct exec{
	u32 a_info;
	u32 a_text;
	u32 a_data;
	u32 a_bss;	
	u32 a_syms;
	u32 a_entry;
	u32 a_trsize;
	u32 a_drsize;
};
#define N_TRSIZE(a) ((a).a_trsize)
#define N_DRSIZE(a) ((a).a_drsize)
#define N_SYMSIZE(a) ((a).a_syms)

typedef struct {
 int val[2];
}fsid_t;

typedef struct {
 ulong fds_bits[1024 / (8 * sizeof(long))];
} fd_set;

struct flock {
	short	l_type;
	short	l_whence;
	off_t	l_start;
	off_t	l_len;
	pid_t	l_pid;
};

struct flock64 {
	short  l_type;
	short  l_whence;
	loff_t l_start;
	loff_t l_len;
	pid_t  l_pid;
};

struct f_owner_ex {
	int	type;
	pid_t	pid;
};

struct in_addr {
 u32 s_addr;
};

struct in6_addr {
	union {
		u8 u6_addr8[16];
		u16 u6_addr16[8];
		u32 u6_addr32[4];
	} in6_u;
};

struct ipc64_perm {
	key_t		key;
	uid32_t	uid;
	gid32_t	gid;
	uid32_t	cuid;
	gid32_t	cgid;
	mode_t		mode;
	u8		__pad1[4 - sizeof(mode_t)];
	u16		seq;
	u16		__pad2;
	ulong		__unused1;
	ulong		__unused2;
};

struct ktermios {
	tcflag_t c_iflag;	
	tcflag_t c_oflag;	
	tcflag_t c_cflag;	
	tcflag_t c_lflag;	
	cc_t c_line;		
	cc_t c_cc[NCCS];	
	speed_t c_ispeed;	
	speed_t c_ospeed;	
};

struct msqid64_ds {
	struct ipc64_perm msg_perm;
	time_t msg_stime;
IFBITSPERLONGNEQ64(ulong	__unused1;)
	time_t msg_rtime;
IFBITSPERLONGNEQ64(ulong	__unused2;)
	time_t msg_ctime;
IFBITSPERLONGNEQ64(ulong	__unused3;)
	ulong msg_cbytes;
	ulong msg_qnum;
	ulong msg_qbytes;
	pid_t msg_lspid;
	pid_t msg_lrpid;
	ulong  __unused4;
	ulong  __unused5;
};

struct pollfd {
	int fd;
	short events;
	short revents;
};

struct semid64_ds {
	struct ipc64_perm sem_perm;
	time_t	sem_otime;
IFBITSPERLONGNEQ64(ulong	__unused1;)
	time_t	sem_ctime;
IFBITSPERLONGNEQ64(ulong	__unused2;)
	ulong	sem_nsems;
	ulong	__unused3;
	ulong	__unused4;
};

struct setup_data {
	u64 next;
	u32 type;
	u32 len;
	u8 data[0];
};

struct setup_header {
	u8 setup_sects;
	u16 root_flags;
	u32 syssize;
	u16 ram_size;
	u16 vid_mode;
	u16 root_dev;
	u16 boot_flag;
	u16 jump;
	u32 header;
	u16 version;
	u32 realmode_swtch;
	u16 start_sys;
	u16 kernel_version;
	u8 type_of_loader;
	u8 loadflags;
	u16 setup_move_size;
	u32 code32_start;
	u32 ramdisk_image;
	u32 ramdisk_size;
	u32 bootsect_kludge;
	u16 heap_end_ptr;
	u8 ext_loader_ver;
	u8 ext_loader_type;
	u32 cmd_line_ptr;
	u32 initrd_addr_max;
	u32 kernel_alignment;
	u8 relocatable_kernel;
	u8 min_alignment;
	u16 xloadflags;
	u32 cmdline_size;
	u32 hardware_subarch;
	u64 hardware_subarch_data;
	u32 payload_offset;
	u32 payload_length;
	u64 setup_data;
	u64 pref_address;
	u32 init_size;
	u32 handover_offset;
} __attribute__((packed));

struct shmid64_ds {
	struct ipc64_perm	shm_perm;
	size_t shm_segsz;
	time_t shm_atime;
IFBITSPERLONGNEQ64(ulong __unused1;)
	time_t 	shm_dtime;
IFBITSPERLONGNEQ64(ulong __unused2;)
	time_t		shm_ctime;
IFBITSPERLONGNEQ64(ulong __unused3;)
	pid_t shm_cpid;
	pid_t shm_lpid;
	ulong shm_nattch;
	ulong __unused4;
	ulong __unused5;
};

struct shminfo64 {
	ulong shmmax;
	ulong shmmin;
	ulong shmmni;
	ulong shmseg;
	ulong shmall;
	ulong __unused1;
	ulong __unused2;
	ulong __unused3;
	ulong __unused4;
};

#if defined(__i386__) || defined(__x86_64__)
struct _fpx_sw_bytes {
	u32 magic1;
	u32 extended_size;
	u64 xstate_bv;
	u32 xstate_size;
	u32 padding[7];
};
#endif
#ifdef __i386__

struct _fpreg {
	ushort significand[4];
	ushort exponent;
};

struct _fpxreg {
	ushort significand[4];
	ushort exponent;
	ushort padding[3];
};

struct _xmmreg {
	ulong element[4];
};

struct _fpstate {
	ulong	cw;
	ulong	sw;
	ulong	tag;
	ulong	ipoff;
	ulong	cssel;
	ulong	dataoff;
	ulong	datasel;
	struct _fpreg	_st[8];
	ushort	status;
	ushort	magic;
	ulong	_fxsr_env[6];
	ulong	mxcsr;
	ulong	reserved;
	struct _fpxreg	_fxsr_st[8];
	struct _xmmreg	_xmm[8];
	ulong	padding1[44];
	union {
		ulong	padding2[12];
		struct _fpx_sw_bytes sw_reserved;
	};
};
struct sigcontext {
	ushort gs, __gsh;
	ushort fs, __fsh;
	ushort es, __esh;
	ushort ds, __dsh;
	ulong edi;
	ulong esi;
	ulong ebp;
	ulong esp;
	ulong ebx;
	ulong edx;
	ulong ecx;
	ulong eax;
	ulong trapno;
	ulong err;
	ulong eip;
	ushort cs, __csh;
	ulong eflags;
	ulong esp_at_signal;
	ushort ss, __ssh;
	struct _fpstate *fpstate;
	ulong oldmask;
	ulong cr2;
};
#elif defined(__x86_64__)

struct _fpstate {
	u16	cwd;
	u16	swd;
	u16	twd;
	u16	fop;
	u64	rip;
	u64	rdp;
	u32	mxcsr;
	u32	mxcsr_mask;
	u32	st_space[32];
	u32	xmm_space[64];
	u32	reserved2[12];
	union {
		u32	reserved3[12];
		struct _fpx_sw_bytes sw_reserved;
	};
};
struct sigcontext {
	u64 r8;
	u64 r9;
	u64 r10;
	u64 r11;
	u64 r12;
	u64 r13;
	u64 r14;
	u64 r15;
	u64 rdi;
	u64 rsi;
	u64 rbp;
	u64 rbx;
	u64 rdx;
	u64 rax;
	u64 rcx;
	u64 rsp;
	u64 rip;
	u64 eflags;
	u16 cs;
	u16 gs;
	u16 fs;
	u16 __pad0;
	u64 err;
	u64 trapno;
	u64 oldmask;
	u64 cr2;
	struct _fpstate *fpstate;
#ifdef __ILP32__
	u32 __fpstate_pad;
#endif
	u64 reserved1[8];
};

#elif defined( __arm__)
struct sigcontext {
	unsigned long trap_no;
	unsigned long error_code;
	unsigned long oldmask;
	unsigned long arm_r0;
	unsigned long arm_r1;
	unsigned long arm_r2;
	unsigned long arm_r3;
	unsigned long arm_r4;
	unsigned long arm_r5;
	unsigned long arm_r6;
	unsigned long arm_r7;
	unsigned long arm_r8;
	unsigned long arm_r9;
	unsigned long arm_r10;
	unsigned long arm_fp;
	unsigned long arm_ip;
	unsigned long arm_sp;
	unsigned long arm_lr;
	unsigned long arm_pc;
	unsigned long arm_cpsr;
	unsigned long fault_address;
};

#endif

typedef union sigval {
	int sival_int;
	void *sival_ptr;
} sigval_t;

typedef struct sigevent {
	sigval_t sigev_value;
	int sigev_signo;
	int sigev_notify;
	union {
		int _pad[SIGEV_PAD_SIZE];
		int _tid;
		struct {
			void (*_function)(sigval_t);
			void *_attribute;
		} _sigev_thread;
	} _sigev_un;
} sigevent_t;

typedef struct siginfo {
	int si_signo;
	int si_errno;
	int si_code;
	union {
		int _pad[SI_PAD_SIZE];
		struct {
			pid_t _pid;
			uid_t _uid;
		} _kill;
		struct {
			timer_t _tid;
			int _overrun;
			char _pad[sizeof(uid_t) - sizeof(int)];
			sigval_t _sigval;
			int _sys_private;
		} _timer;
		struct {
			pid_t _pid;
			uid_t _uid;
			sigval_t _sigval;
		} _rt;
		struct {
			pid_t _pid;
			uid_t _uid;
			int _status;
			clock_t _utime;
			clock_t _stime;
		} _sigchld;
		struct {
			void *_addr;
#ifdef __ARCH_SI_TRAPNO
			int _trapno;
#endif
			short _addr_lsb;
		} _sigfault;
		struct {
			band_t _band;
			int _fd;
		} _sigpoll;
		struct {
			void *_call_addr;
			int _syscall;
			uint _arch;	
		} _sigsys;
	} _sifields;
} __ARCH_SI_ATTRIBUTES siginfo_t;

struct sockaddr_in {
  sa_family_t sin_family;
  u16 sin_port;
  struct in_addr sin_addr;
  u8 __pad[16 - sizeof(sa_family_t) - sizeof(u16) - sizeof(struct in_addr)];
};

struct sockaddr_in6 {
 u16 sin6_family;
 u16 sin6_port;
 u32 sin6_flowinfo;
 struct in6_addr sin6_addr;
 u32 sin6_scope_id;
};

struct sockaddr_storage {
 sa_family_t ss_family;
 char __data[128 - sizeof(unsigned short)];
} __attribute__ ((aligned((__alignof__ (struct sockaddr *)))));

typedef struct sigaltstack {
	void *ss_sp;
	int ss_flags;
	size_t ss_size;
} stack_t;

struct stat {
	unsigned long	st_dev;	
	unsigned long	st_ino;	
	unsigned int	st_mode;
	unsigned int	st_nlink;
	unsigned int	st_uid;	
	unsigned int	st_gid;	
	unsigned long	st_rdev;
	unsigned long	__pad1;
	long		st_size;
	int		st_blksize;
	int		__pad2;
	long		st_blocks;
	long		st_atime;
	unsigned long	st_atime_nsec;
	long		st_mtime;
	unsigned long	st_mtime_nsec;
	long		st_ctime;
	unsigned long	st_ctime_nsec;
	unsigned int	__unused4;
	unsigned int	__unused5;
};

struct stat64 {
	unsigned long long st_dev;
	unsigned long long st_ino;
	unsigned int	st_mode;
	unsigned int	st_nlink;
	unsigned int	st_uid;	
	unsigned int	st_gid;	
	unsigned long long st_rdev;
	unsigned long long __pad1;
	long long	st_size;
	int		st_blksize;
	int		__pad2;
	long long	st_blocks;
	int		st_atime;
	unsigned int	st_atime_nsec;
	int		st_mtime;
	unsigned int	st_mtime_nsec;
	int		st_ctime;
	unsigned int	st_ctime_nsec;
	unsigned int	__unused4;
	unsigned int	__unused5;
};

struct termio {
	u16 c_iflag;	
	u16 c_oflag;	
	u16 c_cflag;	
	u16 c_lflag;	
	u8 c_line;	
	u8 c_cc[NCC];
};

struct termios2 {
	tcflag_t c_iflag;	
	tcflag_t c_oflag;	
	tcflag_t c_cflag;	
	tcflag_t c_lflag;	
	cc_t c_line;		
	cc_t c_cc[NCCS];	
	speed_t c_ispeed;	
	speed_t c_ospeed;	
};

struct timespec{
    time_t tv_sec;
    long tv_nsec;
};

struct ucontext {
	ulong uc_flags;
	struct ucontext *uc_link;
	stack_t uc_stack;
	struct sigcontext uc_mcontext;
	sigset_t uc_sigmask;
};

struct winsize {
	u16 ws_row;
	u16 ws_col;
	u16 ws_xpixel;
	u16 ws_ypixel;
};

typedef union address {
    struct sockaddr { sa_family_t sa_family; char sa_data[14]; } sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
} address_t;

#endif
#ifndef NO_GLOBAL_VARS
long errno; //technically should be int, but syscalls return long - avoids casts
char **environ;
#endif
#if 1 //syscall inline functions
//TODO change these to macros for architectures that require immediate value
// arguments to the syscall.  This will require changes to how each syscall is
// defined as well ... it will also allow usage of proper function prototypes
//#define <name>(...) syscall{NARGS}(type, name, type, arg, ...)
//IIRC bionic does/did something similar

static inline long _syscall0(long cnum){
	SETREGISTERS0;
	__asm__ __volatile__(SYSCALL:RET:REGS0:CLOB);
	return ret;
}
static inline long _syscall1(long cnum,long a1){
	SETREGISTERS1;
	__asm__ __volatile__(SYSCALL:RET:REGS1:CLOB);
	return ret;
}
static inline long _syscall2(long cnum,long a1,long a2){
	SETREGISTERS2;
	__asm__ __volatile__(SYSCALL:RET:REGS2:CLOB);
	return ret;
}
static inline long _syscall3(long cnum,long a1,long a2,long a3){
	SETREGISTERS3;
	__asm__ __volatile__(SYSCALL:RET:REGS3:CLOB);
	return ret;
}
static inline long _syscall4(long cnum,long a1,long a2,long a3,long a4){
	SETREGISTERS4;
	__asm__ __volatile__(SYSCALL:RET:REGS4:CLOB);
	return ret;
}
static inline long _syscall5(long cnum,long a1,long a2,long a3,long a4,long a5){
	SETREGISTERS5;
	__asm__ __volatile__(SYSCALL:RET:REGS5:CLOB);
	return ret;
}
static inline long _syscall6(long cnum,long a1,long a2,long a3,long a4,long a5,long a6){
	SETREGISTERS6;
	__asm__ __volatile__(SYSCALL:RET:REGS6:CLOB);
	return ret;
}
//is this ever actually used???
static inline long _syscall7(long cnum,long a1,long a2,long a3,long a4,long a5,long a6,long a7){
	SETREGISTERS7;
	__asm__ __volatile__(SYSCALL:RET:REGS7:CLOB);
	return ret;
}

#if defined (__GNUC__) && (GCCVER < 450)
#define __builtin_unreachable() for(;;)
#endif

//exit gets its own code because there is no return
static noreturn void exit(int a1){
	SETREGISTER(r0,CALL_NUMBER,__NR_exit);
	SETREGISTER(r1,ARG1,a1);
	__asm__ __volatile__(SYSCALL::"r"(r0),"r"(r1):CLOB);
	__builtin_unreachable(); //or for(;;); to shut up gcc
}

#endif

/** The following will also act _like_ builtins but may have no __builtin_
 * _Exit, acoshf, acoshl, acosh, asinhf, asinhl, asinh, atanhf, atanhl, atanh,
 * cabsf, cabsl, cabs, cacosf, cacoshf, cacoshl, cacosh, cacosl, cacos, cargf,
 * cargl, carg, casinf, casinhf, casinhl, casinh, casinl, casin, catanf,
 * catanhf, catanhl, catanh, catanl, catan, cbrtf, cbrtl, cbrt, ccosf, ccoshf,
 * ccoshl, ccosh, ccosl, ccos, cexpf, cexpl, cexp, cimagf, cimagl, cimag, clogf,
 * clogl, clog, conjf, conjl, conj, copysignf, copysignl, copysign, cpowf,
 * cpowl, cpow, cprojf, cprojl, cproj, crealf, creall, creal, csinf, csinhf,
 * csinhl, csinh, csinl, csin, csqrtf, csqrtl, csqrt, ctanf, ctanhf, ctanhl,
 * ctanh, ctanl, ctan, erfcf, erfcl, erfc, erff, erfl, erf, exp2f, exp2l, exp2,
 * expm1f, expm1l, expm1, fdimf, fdiml, fdim, fmaf, fmal, fmaxf, fmaxl, fmax,
 * fma, fminf, fminl, fmin, hypotf, hypotl, hypot, ilogbf, ilogbl, ilogb,
 * imaxabs, isblank, iswblank, lgammaf, lgammal, lgamma, llabs, llrintf,
 * llrintl, llrint, llroundf, llroundl, llround, log1pf, log1pl, log1p, log2f,
 * log2l, log2, logbf, logbl, logb, lrintf, lrintl, lrint, lroundf, lroundl,
 * lround, nearbyintf, nearbyintl, nearbyint, nextafterf, nextafterl,
 * nextafter, nexttowardf, nexttowardl, nexttoward, remainderf, remainderl,
 * remainder, remquof, remquol, remquo, rintf, rintl, rint, roundf, roundl,
 * round, scalblnf, scalblnl, scalbln, scalbnf, scalbnl, scalbn, snprintf,
 * tgammaf, tgammal, tgamma, truncf, truncl, trunc, vfscanf, vscanf, vsnprintf,
 * vsscanf, iswalnum, iswalpha, iswcntrl, iswdigit, iswgraph, iswlower,
 * iswprint, iswpunct, iswspace, iswupper, iswxdigit, towlower, towupper*/
#ifdef __clang__
	#define HAS(...) __has_builtin(__VA_ARGS__)
#elif  GCCVER > 470 //assume gcc ... (where the list came from)
	#define HAS(...) 1
#else
	#define HAS(...) 0
#endif
#if 1 //setup syscall wrappers for each syscall
/**
TODO document each syscall above its implementation
These are really raw syscall wrappers, they don't even return -1 and set errno
It is currently up to the user to handle a return of -errno.  This makes it
more streamlined for thread usage (via clone) since the inline syscalls can be
wrapped to handle errors directly or for automatic error handling via callbacks
**/
/** accept() - accept a socket connection
 * int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);*/
#ifdef __NR_accept
	#define accept(...) syscall(__NR_accept,__VA_ARGS__)
#else
	#define accept(...) _socketcall(SYS_ACCEPT,__VA_ARGS__)
#endif

/** accept4() - accept() w/ param for flagging NONBLOCK/CLOEXEC	
 * int accept4(int sockfd,struct sockaddr *addr,socklen_t *addrlen,int flags);*/
#ifdef __NR_accept4
	#define accept4(...) syscall(__NR_accept4,__VA_ARGS__)
#else
	#define accept4(...) _socketcall(SYS_ACCEPT4,__VA_ARGS__)
#endif

/** access() - check file permissions
 * int access(const char *pathname, int mode);*/
#ifdef __NR_access
	#define access(...) syscall(__NR_access,__VA_ARGS__)
#endif

/** acct() - toggle process accounting
 * int acct(const char *filename);*/
#ifdef __NR_acct
	#define acct(...) syscall(__NR_acct,__VA_ARGS__)
#endif

/** add_key() - add/change a key in a kernel keyring
 * key_serial_t add_key(const char *type,const char *description,const void *payload, size_t plen,key_serial_t keyring);*/
#ifdef __NR_add_key
	#define add_key(...) syscall(__NR_add_key,__VA_ARGS__)
#endif

/** adjtimex() - tune kernel's clock
 * int adjtimex(struct timex *buf);*/
#ifdef __NR_adjtimex
	#define adjtimex(...) syscall(__NR_adjtimex,__VA_ARGS__)
#endif

/** afs_syscall() - not implemented, 
 * returns -1 and ENOSYS*/
#ifdef __NR_afs_syscall
#define afs_syscall(...) syscall(__NR_afs_syscall,__VA_ARGS__)
#endif

/** alarm() - send a SIGALRM in `n` seconds
 * unsigned int alarm(unsigned int seconds);*/
#ifdef __NR_alarm
	#define alarm(...) syscall(__NR_alarm,__VA_ARGS__)
#endif

/** arch_prctl() - get/set arch-specific thread state
 * int arch_prctl(int code,ulong addr); (or *addr for get ops)*/
#ifdef __NR_arch_prctl
	#define arch_prctl(...) syscall(__NR_arch_prctl,__VA_ARGS__)
#endif

/** bind() - bind address (name) to a socket
 * int bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen);*/
#ifdef __NR_bind
	#define bind(...) syscall(__NR_bind,__VA_ARGS__)
#else
#define bind(...) _socketcall(SYS_BIND,__VA_ARGS__)
#endif

/**
 **/
#ifdef __NR_bpf
#define bpf(...) syscall(__NR_bpf,__VA_ARGS__)
#endif
#ifdef __NR_brk
#define brk(...) syscall(__NR_brk,__VA_ARGS__)
#endif
#ifdef __NR_capget
#define capget(...) syscall(__NR_capget,__VA_ARGS__)
#endif
#ifdef __NR_capset
#define capset(...) syscall(__NR_capset,__VA_ARGS__)
#endif
#ifdef __NR_chdir
#define chdir(...) syscall(__NR_chdir,__VA_ARGS__)
#endif
#ifdef __NR_chmod
#define chmod(...) syscall(__NR_chmod,__VA_ARGS__)
#endif
#ifdef __NR_chown
#define chown(...) syscall(__NR_chown,__VA_ARGS__)
#endif
#ifdef __NR_chroot
#define chroot(...) syscall(__NR_chroot,__VA_ARGS__)
#endif
#ifdef __NR_clock_adjtime
#define clock_adjtime(...) syscall(__NR_clock_adjtime,__VA_ARGS__)
#endif
#ifdef __NR_clock_getres
#define clock_getres(...) syscall(__NR_clock_getres,__VA_ARGS__)
#endif
#ifdef __NR_clock_gettime
#define clock_gettime(...) syscall(__NR_clock_gettime,__VA_ARGS__)
#endif
#ifdef __NR_clock_nanosleep
#define clock_nanosleep(...) syscall(__NR_clock_nanosleep,__VA_ARGS__)
#endif
#ifdef __NR_clock_settime
#define clock_settime(...) syscall(__NR_clock_settime,__VA_ARGS__)
#endif
#ifdef __NR_clone
#define clone(...) syscall(__NR_clone,__VA_ARGS__)
#endif
#ifdef __NR_close
#define close(...) syscall(__NR_close,__VA_ARGS__)
#endif
#ifdef __NR_connect
#define connect(...) syscall(__NR_connect,__VA_ARGS__)
#endif
#ifdef __NR_creat
#define creat(...) syscall(__NR_creat,__VA_ARGS__)
#endif
#ifdef __NR_create_module
#define create_module(...) syscall(__NR_create_module,__VA_ARGS__)
#endif
#ifdef __NR_delete_module
#define delete_module(...) syscall(__NR_delete_module,__VA_ARGS__)
#endif
#ifdef __NR_dup
#define dup(...) syscall(__NR_dup,__VA_ARGS__)
#endif
#ifdef __NR_dup2
#define dup2(...) syscall(__NR_dup2,__VA_ARGS__)
#endif
#ifdef __NR_dup3
#define dup3(...) syscall(__NR_dup3,__VA_ARGS__)
#endif
#ifdef __NR_epoll_create
#define epoll_create(...) syscall(__NR_epoll_create,__VA_ARGS__)
#endif
#ifdef __NR_epoll_create1
#define epoll_create1(...) syscall(__NR_epoll_create1,__VA_ARGS__)
#endif
#ifdef __NR_epoll_ctl
#define epoll_ctl(...) syscall(__NR_epoll_ctl,__VA_ARGS__)
#endif
#ifdef __NR_epoll_ctl_old
#define epoll_ctl_old(...) syscall(__NR_epoll_ctl_old,__VA_ARGS__)
#endif
#ifdef __NR_epoll_pwait
#define epoll_pwait(...) syscall(__NR_epoll_pwait,__VA_ARGS__)
#endif
#ifdef __NR_epoll_wait
#define epoll_wait(...) syscall(__NR_epoll_wait,__VA_ARGS__)
#endif
#ifdef __NR_epoll_wait_old
#define epoll_wait_old(...) syscall(__NR_epoll_wait_old,__VA_ARGS__)
#endif
#ifdef __NR_eventfd
#define eventfd(...) syscall(__NR_eventfd,__VA_ARGS__)
#endif
#ifdef __NR_eventfd2
#define eventfd2(...) syscall(__NR_eventfd2,__VA_ARGS__)
#endif
#ifdef __NR_execve
#define execve(...) syscall(__NR_execve,__VA_ARGS__)
#endif
#ifdef __NR_execveat
#define execveat(...) syscall(__NR_execveat,__VA_ARGS__)
#endif

//__NR_exit get its own exit() inline/no-return function

#ifdef __NR_exit_group
#define exit_group(...) syscall(__NR_exit_group,__VA_ARGS__)
#endif
#ifdef __NR_faccessat
#define faccessat(...) syscall(__NR_faccessat,__VA_ARGS__)
#endif
#ifdef __NR_fadvise64
#define fadvise64(...) syscall(__NR_fadvise64,__VA_ARGS__)
#endif
#ifdef __NR_fallocate
#define fallocate(...) syscall(__NR_fallocate,__VA_ARGS__)
#endif
#ifdef __NR_fanotify_init
#define fanotify_init(...) syscall(__NR_fanotify_init,__VA_ARGS__)
#endif
#ifdef __NR_fanotify_mark
#define fanotify_mark(...) syscall(__NR_fanotify_mark,__VA_ARGS__)
#endif
#ifdef __NR_fchdir
#define fchdir(...) syscall(__NR_fchdir,__VA_ARGS__)
#endif
#ifdef __NR_fchmod
#define fchmod(...) syscall(__NR_fchmod,__VA_ARGS__)
#endif
#ifdef __NR_fchmodat
#define fchmodat(...) syscall(__NR_fchmodat,__VA_ARGS__)
#endif
#ifdef __NR_fchown
#define fchown(...) syscall(__NR_fchown,__VA_ARGS__)
#endif
#ifdef __NR_fchownat
#define fchownat(...) syscall(__NR_fchownat,__VA_ARGS__)
#endif
#ifdef __NR_fcntl
#define fcntl(...) syscall(__NR_fcntl,__VA_ARGS__)
#endif
#ifdef __NR_fdatasync
#define fdatasync(...) syscall(__NR_fdatasync,__VA_ARGS__)
#endif
#ifdef __NR_fgetxattr
#define fgetxattr(...) syscall(__NR_fgetxattr,__VA_ARGS__)
#endif
#ifdef __NR_finit_module
#define finit_module(...) syscall(__NR_finit_module,__VA_ARGS__)
#endif
#ifdef __NR_finit_module
#define finit_module(...) syscall(__NR_finit_module,__VA_ARGS__)
#endif
#ifdef __NR_flistxattr
#define flistxattr(...) syscall(__NR_flistxattr,__VA_ARGS__)
#endif
#ifdef __NR_flock
#define flock(...) syscall(__NR_flock,__VA_ARGS__)
#endif
#ifdef __NR_fork
#define fork(...) syscall(__NR_fork,__VA_ARGS__)
#endif
#ifdef __NR_fremovexattr
#define fremovexattr(...) syscall(__NR_fremovexattr,__VA_ARGS__)
#endif
#ifdef __NR_fsetxattr
#define fsetxattr(...) syscall(__NR_fsetxattr,__VA_ARGS__)
#endif
#ifdef __NR_fstat
#define fstat(...) syscall(__NR_fstat,__VA_ARGS__)
#endif
#ifdef __NR_fstatfs
#define fstatfs(...) syscall(__NR_fstatfs,__VA_ARGS__)
#endif
#ifdef __NR_fsync
#define fsync(...) syscall(__NR_fsync,__VA_ARGS__)
#endif
#ifdef __NR_ftruncate
#define ftruncate(...) syscall(__NR_ftruncate,__VA_ARGS__)
#endif
#ifdef __NR_futex
#define futex(...) syscall(__NR_futex,__VA_ARGS__)
#endif
#ifdef __NR_futimesat
#define futimesat(...) syscall(__NR_futimesat,__VA_ARGS__)
#endif
#ifdef __NR_getcpu
#define getcpu(...) syscall(__NR_getcpu,__VA_ARGS__)
#endif
#ifdef __NR_getcwd
#define getcwd(...) syscall(__NR_getcwd,__VA_ARGS__)
#endif
#ifdef __NR_getdents
#define getdents(...) syscall(__NR_getdents,__VA_ARGS__)
#endif
#ifdef __NR_getdents64
#define getdents64(...) syscall(__NR_getdents64,__VA_ARGS__)
#endif
#ifdef __NR_getegid
#define getegid(...) syscall(__NR_getegid,__VA_ARGS__)
#endif
#ifdef __NR_geteuid
#define geteuid(...) syscall(__NR_geteuid,__VA_ARGS__)
#endif
#ifdef __NR_getgid
#define getgid(...) syscall(__NR_getgid,__VA_ARGS__)
#endif
#ifdef __NR_getgroups
#define getgroups(...) syscall(__NR_getgroups,__VA_ARGS__)
#endif
#ifdef __NR_getitimer
#define getitimer(...) syscall(__NR_getitimer,__VA_ARGS__)
#endif
#ifdef __NR_get_kernel_syms
#define get_kernel_syms(...) syscall(__NR_get_kernel_syms,__VA_ARGS__)
#endif
#ifdef __NR_get_mempolicy
#define get_mempolicy(...) syscall(__NR_get_mempolicy,__VA_ARGS__)
#endif
#ifdef __NR_getpeername
#define getpeername(...) syscall(__NR_getpeername,__VA_ARGS__)
#endif
#ifdef __NR_getpgid
#define getpgid(...) syscall(__NR_getpgid,__VA_ARGS__)
#endif
#ifdef __NR_getpgrp
#define getpgrp(...) syscall(__NR_getpgrp,__VA_ARGS__)
#endif
#ifdef __NR_getpid
#define getpid(...) syscall(__NR_getpid,__VA_ARGS__)
#endif
#ifdef __NR_getpmsg
#define getpmsg(...) syscall(__NR_getpmsg,__VA_ARGS__)
#endif
#ifdef __NR_getppid
#define getppid(...) syscall(__NR_getppid,__VA_ARGS__)
#endif
#ifdef __NR_getpriority
#define getpriority(...) syscall(__NR_getpriority,__VA_ARGS__)
#endif
#ifdef __NR_getrandom
#define getrandom(...) syscall(__NR_getrandom,__VA_ARGS__)
#endif
#ifdef __NR_getresgid
#define getresgid(...) syscall(__NR_getresgid,__VA_ARGS__)
#endif
#ifdef __NR_getresuid
#define getresuid(...) syscall(__NR_getresuid,__VA_ARGS__)
#endif
#ifdef __NR_getrlimit
#define getrlimit(...) syscall(__NR_getrlimit,__VA_ARGS__)
#endif
#ifdef __NR_get_robust_list
#define get_robust_list(...) syscall(__NR_get_robust_list,__VA_ARGS__)
#endif
#ifdef __NR_getrusage
#define getrusage(...) syscall(__NR_getrusage,__VA_ARGS__)
#endif
#ifdef __NR_getsid
#define getsid(...) syscall(__NR_getsid,__VA_ARGS__)
#endif
#ifdef __NR_getsockname
#define getsockname(...) syscall(__NR_getsockname,__VA_ARGS__)
#endif
#ifdef __NR_getsockopt
#define getsockopt(...) syscall(__NR_getsockopt,__VA_ARGS__)
#endif
#ifdef __NR_get_thread_area
#define get_thread_area(...) syscall(__NR_get_thread_area,__VA_ARGS__)
#endif
#ifdef __NR_gettid
#define gettid(...) syscall(__NR_gettid,__VA_ARGS__)
#endif
#ifdef __NR_gettimeofday
#define gettimeofday(...) syscall(__NR_gettimeofday,__VA_ARGS__)
#endif
#ifdef __NR_getuid
#define getuid(...) syscall(__NR_getuid,__VA_ARGS__)
#endif
#ifdef __NR_getxattr
#define getxattr(...) syscall(__NR_getxattr,__VA_ARGS__)
#endif
#ifdef __NR_init_module
#define init_module(...) syscall(__NR_init_module,__VA_ARGS__)
#endif
#ifdef __NR_inotify_add_watch
#define inotify_add_watch(...) syscall(__NR_inotify_add_watch,__VA_ARGS__)
#endif
#ifdef __NR_inotify_init
#define inotify_init(...) syscall(__NR_inotify_init,__VA_ARGS__)
#endif
#ifdef __NR_inotify_init1
#define inotify_init1(...) syscall(__NR_inotify_init1,__VA_ARGS__)
#endif
#ifdef __NR_inotify_rm_watch
#define inotify_rm_watch(...) syscall(__NR_inotify_rm_watch,__VA_ARGS__)
#endif
#ifdef __NR_io_cancel
#define io_cancel(...) syscall(__NR_io_cancel,__VA_ARGS__)
#endif
#ifdef __NR_ioctl
#define ioctl(...) syscall(__NR_ioctl,__VA_ARGS__)
#endif
#ifdef __NR_io_destroy
#define io_destroy(...) syscall(__NR_io_destroy,__VA_ARGS__)
#endif
#ifdef __NR_io_getevents
#define io_getevents(...) syscall(__NR_io_getevents,__VA_ARGS__)
#endif
#ifdef __NR_ioperm
#define ioperm(...) syscall(__NR_ioperm,__VA_ARGS__)
#endif
#ifdef __NR_iopl
#define iopl(...) syscall(__NR_iopl,__VA_ARGS__)
#endif
#ifdef __NR_ioprio_get
#define ioprio_get(...) syscall(__NR_ioprio_get,__VA_ARGS__)
#endif
#ifdef __NR_ioprio_set
#define ioprio_set(...) syscall(__NR_ioprio_set,__VA_ARGS__)
#endif
#ifdef __NR_io_setup
#define io_setup(...) syscall(__NR_io_setup,__VA_ARGS__)
#endif
#ifdef __NR_io_submit
#define io_submit(...) syscall(__NR_io_submit,__VA_ARGS__)
#endif
#ifdef __NR_kcmp
#define kcmp(...) syscall(__NR_kcmp,__VA_ARGS__)
#endif
#ifdef __NR_kexec_file_load
#define kexec_file_load(...) syscall(__NR_kexec_file_load,__VA_ARGS__)
#endif
#ifdef __NR_kexec_load
#define kexec_load(...) syscall(__NR_kexec_load,__VA_ARGS__)
#endif
#ifdef __NR_keyctl
#define keyctl(...) syscall(__NR_keyctl,__VA_ARGS__)
#endif
#ifdef __NR_kill
#define kill(...) syscall(__NR_kill,__VA_ARGS__)
#endif
#ifdef __NR_lchown
#define lchown(...) syscall(__NR_lchown,__VA_ARGS__)
#endif
#ifdef __NR_lgetxattr
#define lgetxattr(...) syscall(__NR_lgetxattr,__VA_ARGS__)
#endif
#ifdef __NR_link
#define link(...) syscall(__NR_link,__VA_ARGS__)
#endif
#ifdef __NR_linkat
#define linkat(...) syscall(__NR_linkat,__VA_ARGS__)
#endif
#ifdef __NR_listen
#define listen(...) syscall(__NR_listen,__VA_ARGS__)
#endif
#ifdef __NR_listxattr
#define listxattr(...) syscall(__NR_listxattr,__VA_ARGS__)
#endif
#ifdef __NR_llistxattr
#define llistxattr(...) syscall(__NR_llistxattr,__VA_ARGS__)
#endif
#ifdef __NR_lookup_dcookie
#define lookup_dcookie(...) syscall(__NR_lookup_dcookie,__VA_ARGS__)
#endif
#ifdef __NR_lremovexattr
#define lremovexattr(...) syscall(__NR_lremovexattr,__VA_ARGS__)
#endif
#ifdef __NR_lseek
#define lseek(...) syscall(__NR_lseek,__VA_ARGS__)
#endif
#ifdef __NR_lsetxattr
#define lsetxattr(...) syscall(__NR_lsetxattr,__VA_ARGS__)
#endif
#ifdef __NR_lstat
#define lstat(...) syscall(__NR_lstat,__VA_ARGS__)
#endif
#ifdef __NR_madvise
#define madvise(...) syscall(__NR_madvise,__VA_ARGS__)
#endif
#ifdef __NR_mbind
#define mbind(...) syscall(__NR_mbind,__VA_ARGS__)
#endif
#ifdef __NR_memfd_create
#define memfd_create(...) syscall(__NR_memfd_create,__VA_ARGS__)
#endif
#ifdef __NR_migrate_pages
#define migrate_pages(...) syscall(__NR_migrate_pages,__VA_ARGS__)
#endif
#ifdef __NR_mincore
#define mincore(...) syscall(__NR_mincore,__VA_ARGS__)
#endif
#ifdef __NR_mkdir
#define mkdir(...) syscall(__NR_mkdir,__VA_ARGS__)
#endif
#ifdef __NR_mkdirat
#define mkdirat(...) syscall(__NR_mkdirat,__VA_ARGS__)
#endif
#ifdef __NR_mknod
#define mknod(...) syscall(__NR_mknod,__VA_ARGS__)
#endif
#ifdef __NR_mknodat
#define mknodat(...) syscall(__NR_mknodat,__VA_ARGS__)
#endif
#ifdef __NR_mlock
#define mlock(...) syscall(__NR_mlock,__VA_ARGS__)
#endif
#ifdef __NR_mlockall
#define mlockall(...) syscall(__NR_mlockall,__VA_ARGS__)
#endif
#ifdef __NR_mmap
#define mmap(...) syscall(__NR_mmap,__VA_ARGS__)
#endif
#ifdef __NR_modify_ldt
#define modify_ldt(...) syscall(__NR_modify_ldt,__VA_ARGS__)
#endif
#ifdef __NR_mount
#define mount(...) syscall(__NR_mount,__VA_ARGS__)
#endif
#ifdef __NR_move_pages
#define move_pages(...) syscall(__NR_move_pages,__VA_ARGS__)
#endif
#ifdef __NR_mprotect
#define mprotect(...) syscall(__NR_mprotect,__VA_ARGS__)
#endif
#ifdef __NR_mq_getsetattr
#define mq_getsetattr(...) syscall(__NR_mq_getsetattr,__VA_ARGS__)
#endif
#ifdef __NR_mq_notify
#define mq_notify(...) syscall(__NR_mq_notify,__VA_ARGS__)
#endif
#ifdef __NR_mq_open
#define mq_open(...) syscall(__NR_mq_open,__VA_ARGS__)
#endif
#ifdef __NR_mq_timedreceive
#define mq_timedreceive(...) syscall(__NR_mq_timedreceive,__VA_ARGS__)
#endif
#ifdef __NR_mq_timedsend
#define mq_timedsend(...) syscall(__NR_mq_timedsend,__VA_ARGS__)
#endif
#ifdef __NR_mq_unlink
#define mq_unlink(...) syscall(__NR_mq_unlink,__VA_ARGS__)
#endif
#ifdef __NR_mremap
#define mremap(...) syscall(__NR_mremap,__VA_ARGS__)
#endif
#ifdef __NR_msgctl
#define msgctl(...) syscall(__NR_msgctl,__VA_ARGS__)
#endif
#ifdef __NR_msgget
#define msgget(...) syscall(__NR_msgget,__VA_ARGS__)
#endif
#ifdef __NR_msgrcv
#define msgrcv(...) syscall(__NR_msgrcv,__VA_ARGS__)
#endif
#ifdef __NR_msgsnd
#define msgsnd(...) syscall(__NR_msgsnd,__VA_ARGS__)
#endif
#ifdef __NR_msync
#define msync(...) syscall(__NR_msync,__VA_ARGS__)
#endif
#ifdef __NR_munlock
#define munlock(...) syscall(__NR_munlock,__VA_ARGS__)
#endif
#ifdef __NR_munlockall
#define munlockall(...) syscall(__NR_munlockall,__VA_ARGS__)
#endif
#ifdef __NR_munmap
#define munmap(...) syscall(__NR_munmap,__VA_ARGS__)
#endif
#ifdef __NR_name_to_handle_at
#define name_to_handle_at(...) syscall(__NR_name_to_handle_at,__VA_ARGS__)
#endif
#ifdef __NR_nanosleep
#define nanosleep(...) syscall(__NR_nanosleep,__VA_ARGS__)
#endif
#ifdef __NR_newfstatat
#define newfstatat(...) syscall(__NR_newfstatat,__VA_ARGS__)
#endif
#ifdef __NR_nfsservctl
#define nfsservctl(...) syscall(__NR_nfsservctl,__VA_ARGS__)
#endif
#ifdef __NR_open
#define open(...) syscall(__NR_open,__VA_ARGS__)
#endif
#ifdef __NR_openat
#define openat(...) syscall(__NR_openat,__VA_ARGS__)
#endif
#ifdef __NR_open_by_handle_at
#define open_by_handle_at(...) syscall(__NR_open_by_handle_at,__VA_ARGS__)
#endif
#if defined __NR_pause && !defined pause
#define pause(...) syscall(__NR_pause,__VA_ARGS__)
#endif
#ifdef __NR_perf_event_open
#define perf_event_open(...) syscall(__NR_perf_event_open,__VA_ARGS__)
#endif
#ifdef __NR_personality
#define personality(...) syscall(__NR_personality,__VA_ARGS__)
#endif
#ifdef __NR_pipe
#define pipe(...) syscall(__NR_pipe,__VA_ARGS__)
#endif
#ifdef __NR_pipe2
#define pipe2(...) syscall(__NR_pipe2,__VA_ARGS__)
#endif
#ifdef __NR_pivot_root
#define pivot_root(...) syscall(__NR_pivot_root,__VA_ARGS__)
#endif
#ifdef __NR_poll
#define poll(...) syscall(__NR_poll,__VA_ARGS__)
#endif
#ifdef __NR_ppoll
#define ppoll(...) syscall(__NR_ppoll,__VA_ARGS__)
#endif
#ifdef __NR_prctl
#define prctl(...) syscall(__NR_prctl,__VA_ARGS__)
#endif
#ifdef __NR_pread64
#define pread64(...) syscall(__NR_pread64,__VA_ARGS__)
#endif
#ifdef __NR_preadv
#define preadv(...) syscall(__NR_preadv,__VA_ARGS__)
#endif
#ifdef __NR_prlimit64
#define prlimit64(...) syscall(__NR_prlimit64,__VA_ARGS__)
#endif
#ifdef __NR_process_vm_readv
#define process_vm_readv(...) syscall(__NR_process_vm_readv,__VA_ARGS__)
#endif
#ifdef __NR_process_vm_writev
#define process_vm_writev(...) syscall(__NR_process_vm_writev,__VA_ARGS__)
#endif
#ifdef __NR_pselect6
#define pselect6(...) syscall(__NR_pselect6,__VA_ARGS__)
#endif
#ifdef __NR_ptrace
#define ptrace(...) syscall(__NR_ptrace,__VA_ARGS__)
#endif
#ifdef __NR_putpmsg
#define putpmsg(...) syscall(__NR_putpmsg,__VA_ARGS__)
#endif
#ifdef __NR_pwrite64
#define pwrite64(...) syscall(__NR_pwrite64,__VA_ARGS__)
#endif
#ifdef __NR_pwritev
#define pwritev(...) syscall(__NR_pwritev,__VA_ARGS__)
#endif
#ifdef __NR_query_module
#define query_module(...) syscall(__NR_query_module,__VA_ARGS__)
#endif
#ifdef __NR_quotactl
#define quotactl(...) syscall(__NR_quotactl,__VA_ARGS__)
#endif
#ifdef __NR_read
#define read(...) syscall(__NR_read,__VA_ARGS__)
#endif
#ifdef __NR_readahead
#define readahead(...) syscall(__NR_readahead,__VA_ARGS__)
#endif
#ifdef __NR_readlink
#define readlink(...) syscall(__NR_readlink,__VA_ARGS__)
#endif
#ifdef __NR_readlinkat
#define readlinkat(...) syscall(__NR_readlinkat,__VA_ARGS__)
#endif
#ifdef __NR_readv
#define readv(...) syscall(__NR_readv,__VA_ARGS__)
#endif
#ifdef __NR_reboot
#define reboot(...) syscall(__NR_reboot,__VA_ARGS__)
#endif
#ifdef __NR_recvfrom
#define recvfrom(...) syscall(__NR_recvfrom,__VA_ARGS__)
#endif
#ifdef __NR_recvmmsg
#define recvmmsg(...) syscall(__NR_recvmmsg,__VA_ARGS__)
#endif
#ifdef __NR_recvmsg
#define recvmsg(...) syscall(__NR_recvmsg,__VA_ARGS__)
#endif
#ifdef __NR_remap_file_pages
#define remap_file_pages(...) syscall(__NR_remap_file_pages,__VA_ARGS__)
#endif
#ifdef __NR_removexattr
#define removexattr(...) syscall(__NR_removexattr,__VA_ARGS__)
#endif
#ifdef __NR_rename
#define rename(...) syscall(__NR_rename,__VA_ARGS__)
#endif
#ifdef __NR_renameat
#define renameat(...) syscall(__NR_renameat,__VA_ARGS__)
#endif
#ifdef __NR_renameat2
#define renameat2(...) syscall(__NR_renameat2,__VA_ARGS__)
#endif
#ifdef __NR_request_key
#define request_key(...) syscall(__NR_request_key,__VA_ARGS__)
#endif
#ifdef __NR_restart_syscall
#define restart_syscall(...) syscall(__NR_restart_syscall,__VA_ARGS__)
#endif
#ifdef __NR_rmdir
#define rmdir(...) syscall(__NR_rmdir,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigaction
#define rt_sigaction(...) syscall(__NR_rt_sigaction,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigpending
#define rt_sigpending(...) syscall(__NR_rt_sigpending,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigprocmask
#define rt_sigprocmask(...) syscall(__NR_rt_sigprocmask,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigqueueinfo
#define rt_sigqueueinfo(...) syscall(__NR_rt_sigqueueinfo,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigreturn
#define rt_sigreturn(...) syscall(__NR_rt_sigreturn,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigsuspend
#define rt_sigsuspend(...) syscall(__NR_rt_sigsuspend,__VA_ARGS__)
#endif
#ifdef __NR_rt_sigtimedwait
#define rt_sigtimedwait(...) syscall(__NR_rt_sigtimedwait,__VA_ARGS__)
#endif
#ifdef __NR_rt_tgsigqueueinfo
#define rt_tgsigqueueinfo(...) syscall(__NR_rt_tgsigqueueinfo,__VA_ARGS__)
#endif
#ifdef __NR_sched_getaffinity
#define sched_getaffinity(...) syscall(__NR_sched_getaffinity,__VA_ARGS__)
#endif
#ifdef __NR_sched_getattr
#define sched_getattr(...) syscall(__NR_sched_getattr,__VA_ARGS__)
#endif
#ifdef __NR_sched_getparam
#define sched_getparam(...) syscall(__NR_sched_getparam,__VA_ARGS__)
#endif
#ifdef __NR_sched_get_priority_max
#define sched_get_priority_max(...) syscall(__NR_sched_get_priority_max,__VA_ARGS__)
#endif
#ifdef __NR_sched_get_priority_min
#define sched_get_priority_min(...) syscall(__NR_sched_get_priority_min,__VA_ARGS__)
#endif
#ifdef __NR_sched_getscheduler
#define sched_getscheduler(...) syscall(__NR_sched_getscheduler,__VA_ARGS__)
#endif
#ifdef __NR_sched_rr_get_interval
#define sched_rr_get_interval(...) syscall(__NR_sched_rr_get_interval,__VA_ARGS__)
#endif
#ifdef __NR_sched_setaffinity
#define sched_setaffinity(...) syscall(__NR_sched_setaffinity,__VA_ARGS__)
#endif
#ifdef __NR_sched_setattr
#define sched_setattr(...) syscall(__NR_sched_setattr,__VA_ARGS__)
#endif
#ifdef __NR_sched_setparam
#define sched_setparam(...) syscall(__NR_sched_setparam,__VA_ARGS__)
#endif
#ifdef __NR_sched_setscheduler
#define sched_setscheduler(...) syscall(__NR_sched_setscheduler,__VA_ARGS__)
#endif
#ifdef __NR_sched_yield
#define sched_yield(...) syscall(__NR_sched_yield,__VA_ARGS__)
#endif
#ifdef __NR_seccomp
#define seccomp(...) syscall(__NR_seccomp,__VA_ARGS__)
#endif
#ifdef __NR_security
#define security(...) syscall(__NR_security,__VA_ARGS__)
#endif
#ifdef __NR_select
#define select(...) syscall(__NR_select,__VA_ARGS__)
#endif
#ifdef __NR_semctl
#define semctl(...) syscall(__NR_semctl,__VA_ARGS__)
#endif
#ifdef __NR_semget
#define semget(...) syscall(__NR_semget,__VA_ARGS__)
#endif
#ifdef __NR_semop
#define semop(...) syscall(__NR_semop,__VA_ARGS__)
#endif
#ifdef __NR_semtimedop
#define semtimedop(...) syscall(__NR_semtimedop,__VA_ARGS__)
#endif
#ifdef __NR_sendfile
#define sendfile(...) syscall(__NR_sendfile,__VA_ARGS__)
#endif
#ifdef __NR_sendmmsg
#define sendmmsg(...) syscall(__NR_sendmmsg,__VA_ARGS__)
#endif
#ifdef __NR_sendmsg
#define sendmsg(...) syscall(__NR_sendmsg,__VA_ARGS__)
#endif
#ifdef __NR_sendto
#define sendto(...) syscall(__NR_sendto,__VA_ARGS__)
#endif
#ifdef __NR_setdomainname
#define setdomainname(...) syscall(__NR_setdomainname,__VA_ARGS__)
#endif
#ifdef __NR_setfsgid
#define setfsgid(...) syscall(__NR_setfsgid,__VA_ARGS__)
#endif
#ifdef __NR_setfsuid
#define setfsuid(...) syscall(__NR_setfsuid,__VA_ARGS__)
#endif
#ifdef __NR_setgid
#define setgid(...) syscall(__NR_setgid,__VA_ARGS__)
#endif
#ifdef __NR_setgroups
#define setgroups(...) syscall(__NR_setgroups,__VA_ARGS__)
#endif
#ifdef __NR_sethostname
#define sethostname(...) syscall(__NR_sethostname,__VA_ARGS__)
#endif
#ifdef __NR_setitimer
#define setitimer(...) syscall(__NR_setitimer,__VA_ARGS__)
#endif
#ifdef __NR_set_mempolicy
#define set_mempolicy(...) syscall(__NR_set_mempolicy,__VA_ARGS__)
#endif
#ifdef __NR_setns
#define setns(...) syscall(__NR_setns,__VA_ARGS__)
#endif
#ifdef __NR_setpgid
#define setpgid(...) syscall(__NR_setpgid,__VA_ARGS__)
#endif
#ifdef __NR_setpriority
#define setpriority(...) syscall(__NR_setpriority,__VA_ARGS__)
#endif
#ifdef __NR_setregid
#define setregid(...) syscall(__NR_setregid,__VA_ARGS__)
#endif
#ifdef __NR_setresgid
#define setresgid(...) syscall(__NR_setresgid,__VA_ARGS__)
#endif
#ifdef __NR_setresuid
#define setresuid(...) syscall(__NR_setresuid,__VA_ARGS__)
#endif
#ifdef __NR_setreuid
#define setreuid(...) syscall(__NR_setreuid,__VA_ARGS__)
#endif
#ifdef __NR_setrlimit
#define setrlimit(...) syscall(__NR_setrlimit,__VA_ARGS__)
#endif
#ifdef __NR_set_robust_list
#define set_robust_list(...) syscall(__NR_set_robust_list,__VA_ARGS__)
#endif
#ifdef __NR_setsid
#define setsid(...) syscall(__NR_setsid,__VA_ARGS__)
#endif
#ifdef __NR_setsockopt
#define setsockopt(...) syscall(__NR_setsockopt,__VA_ARGS__)
#endif
#ifdef __NR_set_thread_area
#define set_thread_area(...) syscall(__NR_set_thread_area,__VA_ARGS__)
#endif
#ifdef __NR_set_tid_address
#define set_tid_address(...) syscall(__NR_set_tid_address,__VA_ARGS__)
#endif
#ifdef __NR_settimeofday
#define settimeofday(...) syscall(__NR_settimeofday,__VA_ARGS__)
#endif
#ifdef __NR_setuid
#define setuid(...) syscall(__NR_setuid,__VA_ARGS__)
#endif
#ifdef __NR_setxattr
#define setxattr(...) syscall(__NR_setxattr,__VA_ARGS__)
#endif
#ifdef __NR_shmat
#define shmat(...) syscall(__NR_shmat,__VA_ARGS__)
#endif
#ifdef __NR_shmctl
#define shmctl(...) syscall(__NR_shmctl,__VA_ARGS__)
#endif
#ifdef __NR_shmdt
#define shmdt(...) syscall(__NR_shmdt,__VA_ARGS__)
#endif
#ifdef __NR_shmget
#define shmget(...) syscall(__NR_shmget,__VA_ARGS__)
#endif
#ifdef __NR_shutdown
#define shutdown(...) syscall(__NR_shutdown,__VA_ARGS__)
#endif
#ifdef __NR_sigaltstack
#define sigaltstack(...) syscall(__NR_sigaltstack,__VA_ARGS__)
#endif
#ifdef __NR_signalfd
#define signalfd(...) syscall(__NR_signalfd,__VA_ARGS__)
#endif
#ifdef __NR_signalfd4
#define signalfd4(...) syscall(__NR_signalfd4,__VA_ARGS__)
#endif
#ifdef __NR_socket
#define socket(...) syscall(__NR_socket,__VA_ARGS__)
#endif
#ifdef __NR_socketcall
#define socket(...) syscall(__NR_socketcall,__VA_ARGS__)
#endif
#ifdef __NR_socketpair
#define socketpair(...) syscall(__NR_socketpair,__VA_ARGS__)
#endif
#ifdef __NR_splice
#define splice(...) syscall(__NR_splice,__VA_ARGS__)
#endif
#ifdef __NR_stat
#define stat(...) syscall(__NR_stat,__VA_ARGS__)
#endif
#ifdef __NR_statfs
#define statfs(...) syscall(__NR_statfs,__VA_ARGS__)
#endif
#ifdef __NR_swapoff
#define swapoff(...) syscall(__NR_swapoff,__VA_ARGS__)
#endif
#ifdef __NR_swapon
#define swapon(...) syscall(__NR_swapon,__VA_ARGS__)
#endif
#ifdef __NR_symlink
#define symlink(...) syscall(__NR_symlink,__VA_ARGS__)
#endif
#ifdef __NR_symlinkat
#define symlinkat(...) syscall(__NR_symlinkat,__VA_ARGS__)
#endif
#ifdef __NR_sync
#define sync(...) syscall(__NR_sync,__VA_ARGS__)
#endif
#ifdef __NR_sync_file_range
#define sync_file_range(...) syscall(__NR_sync_file_range,__VA_ARGS__)
#endif
#ifdef __NR_syncfs
#define syncfs(...) syscall(__NR_syncfs,__VA_ARGS__)
#endif
#ifdef __NR__sysctl
#define _sysctl(...) syscall(__NR__sysctl,__VA_ARGS__)
#endif
#ifdef __NR_sysfs
#define sysfs(...) syscall(__NR_sysfs,__VA_ARGS__)
#endif
#ifdef __NR_sysinfo
#define sysinfo(...) syscall(__NR_sysinfo,__VA_ARGS__)
#endif
#ifdef __NR_syslog
#define syslog(...) syscall(__NR_syslog,__VA_ARGS__)
#endif
#ifdef __NR_tee
#define tee(...) syscall(__NR_tee,__VA_ARGS__)
#endif
#ifdef __NR_tgkill
#define tgkill(...) syscall(__NR_tgkill,__VA_ARGS__)
#endif
#ifdef __NR_time
#define time(...) syscall(__NR_time,__VA_ARGS__)
#endif
#ifdef __NR_timer_create
#define timer_create(...) syscall(__NR_timer_create,__VA_ARGS__)
#endif
#ifdef __NR_timer_delete
#define timer_delete(...) syscall(__NR_timer_delete,__VA_ARGS__)
#endif
#ifdef __NR_timerfd_create
#define timerfd_create(...) syscall(__NR_timerfd_create,__VA_ARGS__)
#endif
#ifdef __NR_timerfd_gettime
#define timerfd_gettime(...) syscall(__NR_timerfd_gettime,__VA_ARGS__)
#endif
#ifdef __NR_timerfd_settime
#define timerfd_settime(...) syscall(__NR_timerfd_settime,__VA_ARGS__)
#endif
#ifdef __NR_timer_getoverrun
#define timer_getoverrun(...) syscall(__NR_timer_getoverrun,__VA_ARGS__)
#endif
#ifdef __NR_timer_gettime
#define timer_gettime(...) syscall(__NR_timer_gettime,__VA_ARGS__)
#endif
#ifdef __NR_timer_settime
#define timer_settime(...) syscall(__NR_timer_settime,__VA_ARGS__)
#endif
#ifdef __NR_times
#define times(...) syscall(__NR_times,__VA_ARGS__)
#endif
#ifdef __NR_tkill
#define tkill(...) syscall(__NR_tkill,__VA_ARGS__)
#endif
#ifdef __NR_truncate
#define truncate(...) syscall(__NR_truncate,__VA_ARGS__)
#endif
#ifdef __NR_tuxcall
#define tuxcall(...) syscall(__NR_tuxcall,__VA_ARGS__)
#endif
#ifdef __NR_umask
#define umask(...) syscall(__NR_umask,__VA_ARGS__)
#endif
#ifdef __NR_umount2
#define umount2(...) syscall(__NR_umount2,__VA_ARGS__)
#endif
#ifdef __NR_uname
#define uname(...) syscall(__NR_uname,__VA_ARGS__)
#endif
#ifdef __NR_unlink
#define unlink(...) syscall(__NR_unlink,__VA_ARGS__)
#endif
#ifdef __NR_unlinkat
#define unlinkat(...) syscall(__NR_unlinkat,__VA_ARGS__)
#endif
#ifdef __NR_unshare
#define unshare(...) syscall(__NR_unshare,__VA_ARGS__)
#endif
#ifdef __NR_uselib
#define uselib(...) syscall(__NR_uselib,__VA_ARGS__)
#endif
#ifdef __NR_ustat
#define ustat(...) syscall(__NR_ustat,__VA_ARGS__)
#endif
#ifdef __NR_utime
#define utime(...) syscall(__NR_utime,__VA_ARGS__)
#endif
#ifdef __NR_utimensat
#define utimensat(...) syscall(__NR_utimensat,__VA_ARGS__)
#endif
#ifdef __NR_utimes
#define utimes(...) syscall(__NR_utimes,__VA_ARGS__)
#endif
#ifdef __NR_vfork
#define vfork(...) syscall(__NR_vfork,__VA_ARGS__)
#endif
#ifdef __NR_vhangup
#define vhangup(...) syscall(__NR_vhangup,__VA_ARGS__)
#endif
#ifdef __NR_vmsplice
#define vmsplice(...) syscall(__NR_vmsplice,__VA_ARGS__)
#endif
#ifdef __NR_vserver
#define vserver(...) syscall(__NR_vserver,__VA_ARGS__)
#endif
#ifdef __NR_wait4
#define wait4(...) syscall(__NR_wait4,__VA_ARGS__)
#endif
#ifdef __NR_waitid
#define waitid(...) syscall(__NR_waitid,__VA_ARGS__)
#endif
#ifdef __NR_write
#define write(...) syscall(__NR_write,__VA_ARGS__)
#endif
#ifdef __NR_writev
#define writev(...) syscall(__NR_writev,__VA_ARGS__)
#endif

#endif

//TODO add the rest of these as builtins
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

#ifndef htons
#define htons(x) bswap16(x)
#endif
#ifndef htonl
	#define htonl(x) bswap32(x)
#endif
#ifndef htonll
	#define htonll(x) bswap64(x)
#endif
#ifndef htobe16
	#define htobe16(x) bswap16(x)
#endif
#ifndef htobe32
	#define htobe32(x) bswap32(x)
#endif
#ifndef htobe64
	#define htobe64(x) bswap64(x)
#endif
#ifndef be16toh
	#define be16toh(x) bswap16(x)
#endif
#ifndef be32toh
	#define be32toh(x) bswap32(x)
#endif
#ifndef be64toh
	#define be64toh(x) bswap64(x)
#endif
#ifndef htole16
	#define htole16(x) (x)
#endif
#ifndef htole32
	#define htole32(x) (x)
#endif
#ifndef htole64
	#define htole64(x) (x)
#endif
#ifndef le16toh
	#define le16toh(x) (x)
#endif
#ifndef le32toh
	#define le32toh(x) (x)
#endif
#ifndef le64toh
	#define le64toh(x) (x)
#endif

#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__

#ifndef htons
	#define htons(x) (x)
#endif
#ifndef htonl
	#define htonl(x) (x)
#endif
#ifndef htonll
	#define htonll(x) (x)
#endif
#ifndef htobe16
	#define htobe16(x) (x)
#endif
#ifndef htobe32
	#define htobe32(x) (x)
#endif
#ifndef htobe64
	#define htobe64(x) (x)
#endif
#ifndef be16toh
	#define be16toh(x) (x)
#endif
#ifndef be32toh
	#define be32toh(x) (x)
#endif
#ifndef be64toh
	#define be64toh(x) (x)
#endif
#ifndef htole16
	#define htole16(x) bswap16(x)
#endif
#ifndef htole32
	#define htole32(x) bswap32(x)
#endif
#ifndef htole64
	#define htole64(x) bswap64(x)
#endif
#ifndef le16toh
	#define le16toh(x) bswap16(x)
#endif
#ifndef le32toh
	#define le32toh(x) bswap32(x)
#endif
#ifndef le64toh
	#define le64toh(x) bswap64(x)
#endif

#elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#error only big and little endian currently supported
#else
#error only big and little endian currently supported
#endif
#if 1 //ctype.h

static inline int isalnum(int c){return ((unsigned)c-'0' < 10)|(((unsigned)c|32)-'a' < 26);}
static inline int isalpha(int c){return (((unsigned)c|32)-'a' < 26);}
static inline int isascii(int c){return (unsigned)c<128;}
static inline int isblank(int c){return (c==' ')|(c=='\t');}
static inline int iscntrl(int c){return ((unsigned)c < 0x20) | (c == 0x7f);}
static inline int isdigit(int c){return (unsigned)c-'0' < 10;}
static inline int isgraph(int c){return (unsigned)c-0x21 < 0x5e;}
static inline int islower(int c){return (unsigned)c-'a' < 26;}
static inline int isprint(int c){return (unsigned)c-0x20 < 0x5f;}
static inline int ispunct(int c){return (isgraph(c) & !isalnum(c));}
static inline int isspace(int c){return ((unsigned)c-'\t' < 5)|(c == ' ');}
static inline int isupper(int c){return (unsigned)c-'A' < 26;}
static inline int isxdigit(int c){return ((unsigned)c-'0' < 10) | (((unsigned)c|32)-'a' < 6);}
static inline int tolower(int c){return c | ((isupper(c))<<5);}
static inline int toupper(int c){return c & 0x5f & (-((unsigned)c-'a'<26));}

#endif
#if 1 //commonly used helpers not in libc

#if HAS(__builtin_ia32_rolqi)
	#define rolb __builtin_ia32_rolqi
#else
	static inline u8 rolb(const u8 x,const u8 y){return ROL(x,y);}
#endif
#if HAS(__builtin_ia32_rolhi)
	#define rolw __builtin_ia32_rolhi
#else
	static inline u16 rolw(const u16 x,const u8 y){return ROL(x,y);}
#endif
#if HAS(__builtin_ia32_rorqi)
	#define rorb __builtin_ia32_rorqi
#else
	static inline u8 rorb(const u8 x,const u8 y){return ROR(x,y);}
#endif
#if HAS(__builtin_ia32_rorhi)
	#define rorw __builtin_ia32_rorhi
#else
	static inline u16 rorw(const u16 x,const u8 y){return ROR(x,y);}
#endif

static inline u16 bswap16(u16 x){return x>>8|x<<8;}
static inline u32 rold(u32 x,u8 y){return ROL(x,y);}
static inline u32 rord(u32 x,u8 y){return ROR(x,y);}
static inline u64 rolq(u64 x,u8 y){return ROL(x,y);}
static inline u64 rorq(u64 x,u8 y){return ROR(x,y);}
#define __rolb rolb
#define __rorb rorb
#define __rolw rolw
#define __rorw rorw
#define __rold rold
#define __rord rord
#define __rolq rolq
#define __rorq rorq

#endif
#if 1 //strings section

#define sys_nerr (ELAST-1)  //BSD_COMPAT
static const char strerrors[]= "PERM\0NOENT\0SRCH\0INTR\0IO\0NXIO\02BIG\0NOEXEC\0BADF\0CHILD\0AGAIN\0NOMEM\0ACCES\0FAULT\0NOTBLK\0BUSY\0EXIST\0XDEV\0NODEV\0NOTDIR\0ISDIR\0INVAL\0NFILE\0MFILE\0NOTTY\0TXTBSY\0FBIG\0NOSPC\0SPIPE\0ROFS\0MLINK\0PIPE\0DOM\0RANGE\0DEADLK\0NAMETOOLONG\0NOLCK\0NOSYS\0NOTEMPTY\0LOOP\0"/*EWOULDBLOCK*/"\0NOMSG\0IDRM\0CHRNG\0L2NSYNC\0L3HLT\0L3RST\0LNRNG\0UNATCH\0NOCSI\0L2HLT\0BADE\0BADR\0XFULL\0NOANO\0BADRQC\0BADSLT\0"/*EDEADLOCK*/"\0BFONT\0NOSTR\0NODATA\0TIME\0NOSR\0NONET\0NOPKG\0REMOTE\0NOLINK\0ADV\0SRMNT\0COMM\0PROTO\0MULTIHOP\0DOTDOT\0BADMSG\0OVERFLOW\0NOTUNIQ\0BADFD\0REMCHG\0LIBACC\0LIBBAD\0LIBSCN\0LIBMAX\0LIBEXEC\0ILSEQ\0RESTART\0STRPIPE\0USERS\0NOTSOCK\0DESTADDRREQ\0MSGSIZE\0PROTOTYPE\0NOPROTOOPT\0PROTONOSUPPORT\0SOCKTNOSUPPORT\0OPNOTSUPP\0"/*ENOTSUP*/"\0PFNOSUPPORT\0AFNOSUPPORT\0ADDRINUSE\0ADDRNOTAVAIL\0NETDOWN\0NETUNREACH\0NETRESET\0CONNABORTED\0CONNRESET\0NOBUFS\0ISCONN\0NOTCONN\0SHUTDOWN\0TOOMANYREFS\0TIMEDOUT\0CONNREFUSED\0HOSTDOWN\0HOSTUNREACH\0ALREADY\0INPROGRESS\0STALE\0UCLEAN\0NOTNAM\0NAVAIL\0ISNAM\0REMOTEIO\0DQUOT\0NOMEDIUM\0MEDIUMTYPE\0CANCELED\0NOKEY\0KEYEXPIRED\0KEYREVOKED\0KEYREJECTED\0OWNERDEAD\0NOTRECOVERABLE\0RFKILL\0HWPOISON";
	
static inline const char *getnthstring(const char *s,size_t n){while(n--)while(*s++);return s;}
static inline const char *strerror(size_t e){return getnthstring(strerrors,(e<ELAST)?e:0);}

static inline int strncmp(const char *s1, const char *s2, size_t len);
static inline char *strstr(const char *haystack, const char *needle);
static inline size_t strlen(const char *s){size_t i=0; while(s[i])++i;return i;}
void *memset(void *sp, int c, size_t n){u8 *s=sp;while (n--)*s++=c;return sp;}
void *memcpy(void *dp, const void *sp, size_t n){u8 *d=dp;const u8 *s=sp;while(n--)*d++=*s++;return dp;}
static inline int strncmp(const char *s1, const char *s2, size_t len){
	while (*s1 && --len && *s1==*s2){++s1;++s2;}
	return *s2-*s1;
}

static inline char *strstr(const char *haystack, const char *needle){
	const char *hp=haystack;
	size_t len=strlen(needle);
	while(*hp){
		if(*hp==*needle && !strncmp(hp,needle,len)) return (char *)hp;
		else hp++;
	}
	return NULL;
}
#endif

/** Note:
 * linux-elf passes all arguments to _start() on the stack regardless of
 * the system's actual calling convention, so you can't just set up
 * _start(argc, argv, ...) because you will just get some random data
 * in a register.  I tried using explicit named register variables, to
 * access the stack pointer directly; however optimizations sometimes
 * move the stack pointer in the prolog and break it ... which is also
 * why we can't just set up a variable at the very beginning of the 
 * function and refer to its address.  Which lead me to a hack using.
 * fake large struct parameters since they always pass via stack. That
 * didn't work for arm, so I am back to using named reg vars, so
 * for now require -Os so -O2/-03 don't break stuff.
 * Todo: check -mpush-args or -mno-accumulate outgoing args*/
static inline int _main();
#ifdef INLINEMAIN
#define main(...) DUMMYFUNC(); static inline int _main(__VA_ARGS_)
#endif
int main(); //if we change this to inline mymain(), we can inline main
void noreturn __attribute__ ((visibility ("protected")))
_start(void){
	register long *sp __asm__( STACK_POINTER );
	long argc = *sp;
	char **argv = (char**)sp+1;
#ifdef NO_GLOBAL_VARS
	char **environ;
#endif
	environ = (char**)sp+1+argc;
#ifdef INLINEMAIN
	(void)exit(_main(argc,argv,environ) );
#else
	(void)exit(main(argc,argv,environ) );
#endif
	__builtin_unreachable(); //or for(;;); to shut up gcc
}

#endif

