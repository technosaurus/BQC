#ifndef BQC_H
#define BQC_H

#if 1 // this is where to add/fix architecture ports

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

#ifndef NULL
#define NULL ((void *)0)
#endif

#if __STDC_VERSION__ >= 201103L
#define noreturn _Noreturn
#else
#define noreturn __attribute__((noreturn))
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

#define REGS0 "r"(r0)
#define REGS1 REGS0,"r"(r1)
#define REGS2 REGS1,"r"(r2)
#define REGS3 REGS2,"r"(r3)
#define REGS4 REGS3,"r"(r4)
#define REGS5 REGS4,"r"(r5)
#define REGS6 REGS5,"r"(r6)
#define REGS7 REGS6,"r"(r7)

#ifndef RET
	#define RET "=r"(ret)
#endif

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


#if 1 //typedefs use the kernel's builtin types for our typedefs
typedef unsigned char byte, u8, __u8,  uint8, uint8_t;
typedef unsigned short u16, __u16, word, uid16_t, gid16_t, uint16, old_uid_t, old_gid_t, sa_family_t, uint16_t;
typedef unsigned int u32, __u32, dword, ucs4_t, mode_t, daddr_t, uid_t, uid32_t, gid32_t, gid_t, uint32, uint32_t;
typedef unsigned long long u64, __u64, uint64, uint64_t;

typedef signed char int8, int8_t, s8, __s8;
typedef signed short int16_t, s16, __s16;
typedef signed int bool, clockid_t, int32_t,  ipc_pid_t, key_t, mqd_t, pid_t, s32, __s32, timer_t;
typedef signed long long  loff_t, int64_t, s64, __s64;

typedef char * caddr_t;
typedef unsigned long ino_t, old_dev_t, size_t, ulong_t;
typedef long  clock_t, long_t, off_t, ptrdiff_t, ssize_t, suseconds_t, time_t;

typedef struct {
 int val[2];
}fsid_t;

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


typedef struct {
 unsigned long fds_bits[1024 / (8 * sizeof(long))];
} fd_set;

struct sockaddr_in {
  sa_family_t sin_family;
  u16 sin_port;
  struct in_addr sin_addr;
  u8 __pad[16 - sizeof(short int) - sizeof(unsigned short int) - sizeof(struct in_addr)];
};

struct sockaddr_in6 {
 unsigned short int sin6_family;
 u16 sin6_port;
 u32 sin6_flowinfo;
 struct in6_addr sin6_addr;
 u32 sin6_scope_id;
};

struct sockaddr_storage {
 sa_family_t ss_family;
 char __data[128 - sizeof(unsigned short)];
} __attribute__ ((aligned((__alignof__ (struct sockaddr *)))));

typedef union address {
    struct sockaddr { sa_family_t sa_family; char sa_data[14]; } sa;
    struct sockaddr_in sa_in;
    struct sockaddr_in6 sa_in6;
//    struct sockaddr_storage sa_stor;
} address_t;
#endif

enum{ //this defines _ALL_ common enums in one spot
	AF_UNSPEC    = 0,
	AF_UNIX	     = 1,
	AF_INET	     = 2,
	AF_AX25	     = 3,
	AF_IPX       = 4,
	AF_APPLETALK = 5,
	AF_NETROM    = 6,
	AF_BRIDGE    = 7,
	AF_AAL5      = 8,
	AF_X25       = 9,
	AF_INET6     = 10,
	AF_MAX       = 12,

	IPPROTO_IP = 0,
	IPPROTO_ICMP = 1,
	IPPROTO_IGMP = 2,
	IPPROTO_IPIP = 4,
	IPPROTO_TCP = 6,
	IPPROTO_EGP = 8,
	IPPROTO_PUP = 12,
	IPPROTO_UDP = 17,
	IPPROTO_IDP = 22,
	IPPROTO_TP = 29,
	IPPROTO_DCCP = 33,
	IPPROTO_IPV6 = 41,
	IPPROTO_RSVP = 46,
	IPPROTO_GRE = 47,
	IPPROTO_ESP = 50,
	IPPROTO_AH = 51,
	IPPROTO_MTP = 92,
	IPPROTO_BEETPH = 94,
	IPPROTO_ENCAP = 98,
	IPPROTO_PIM = 103,
	IPPROTO_COMP = 108,
	IPPROTO_SCTP = 132,
	IPPROTO_UDPLITE = 136,
	IPPROTO_RAW = 255,
	IPPROTO_MAX =256,

	PF_UNSPEC    = AF_UNSPEC,
	PF_UNIX      = AF_UNIX,
	PF_INET      = AF_INET,
	PF_AX25      = AF_AX25,
	PF_IPX       = AF_IPX,
	PF_APPLETALK = AF_APPLETALK,
	PF_NETROM    = AF_NETROM,
	PF_BRIDGE    = AF_BRIDGE,
	PF_AAL5      = AF_AAL5,
	PF_X25       = AF_X25,
	PF_INET6     = AF_INET6,
	PF_MAX       = AF_MAX,

	SOCK_STREAM    = 1,
	SOCK_DGRAM     = 2,
	SOCK_RAW       = 3,
	SOCK_RDM       = 4,
	SOCK_SEQPACKET = 5,
	
};

enum { ENOERR,EPERM,ENOENT,ESRCH,EINTR,EIO,ENXIO,E2BIG,ENOEXEC,EBADF,
	ECHILD,EAGAIN,ENOMEM,EACCES,EFAULT,ENOTBLK,EBUSY,EEXIST,EXDEV,ENODEV,
	ENOTDIR,EISDIR,EINVAL,ENFILE,EMFILE,ENOTTY,ETXTBSY,EFBIG,ENOSPC,ESPIPE,
	EROFS,EMLINK,EPIPE,EDOM,ERANGE,EDEADLK,ENAMETOOLONG,ENOLCK,ENOSYS,ENOTEMPTY,
	ELOOP,EWOULDBLOCK,ENOMSG,EIDRM,ECHRNG,EL2NSYNC,EL3HLT,EL3RST,ELNRNG,EUNATCH,
	ENOCSI,EL2HLT,EBADE,EBADR,EXFULL,ENOANO,EBADRQC,EBADSLT,EDEADLOCK,EBFONT,
	ENOSTR,ENODATA,ETIME,ENOSR,ENONET,ENOPKG,EREMOTE,ENOLINK,EADV,ESRMNT,ECOMM,
	EPROTO,EMULTIHOP,EDOTDOT,EBADMSG,EOVERFLOW,ENOTUNIQ,EBADFD,EREMCHG,ELIBACC,
	ELIBBAD,ELIBSCN,ELIBMAX,ELIBEXEC,EILSEQ,ERESTART,ESTRPIPE,EUSERS,ENOTSOCK,
	EDESTADDRREQ,EMSGSIZE,EPROTOTYPE,ENOPROTOOPT,EPROTONOSUPPORT,
	ESOCKTNOSUPPORT,EOPNOTSUPP,EPFNOSUPPORT,EAFNOSUPPORT,EADDRINUSE,
	EADDRNOTAVAIL,ENETDOWN,ENETUNREACH,ENETRESET,ECONNABORTED,ECONNRESET,
	ENOBUFS,EISCONN,ENOTCONN,ESHUTDOWN,ETOOMANYREFS,ETIMEDOUT,ECONNREFUSED,
	EHOSTDOWN,EHOSTUNREACH,EALREADY,EINPROGRESS,ESTALE,EUCLEAN,ENOTNAM,ENAVAIL,
	EISNAM,EREMOTEIO,EDQUOT,ENOMEDIUM,EMEDIUMTYPE,ECANCELED,ENOKEY,EKEYEXPIRED,
	EKEYREVOKED,EKEYREJECTED,EOWNERDEAD,ENOTRECOVERABLE,ERFKILL,EHWPOISON,ELAST
};

#ifndef NO_GLOBAL_VARS
long errno; //technically should be int, but syscalls return long - avoids casts
char **environ;
#endif

#if 1 //syscall inline functions
//TODO change these to macros for architectures that require immediate values as
// arguments to the syscall.  This will require changese to how each syscall is
// defined as well ... this will also allow usage of proper function prototypes
//#define <name>(...) syscall{NARGS}(type, name, type, arg, ...)

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

//exit gets its own code because there is no return
static noreturn void exit(int a1){
	SETREGISTER(r0,CALL_NUMBER,__NR_exit);
	SETREGISTER(r1,ARG1,a1);
	__asm__ __volatile__(SYSCALL::"r"(r0),"r"(r1):CLOB);
	__builtin_unreachable(); //or for(;;); to shut up gcc
}

#endif

#if 1 //builtins section
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
 * iswprint, iswpunct, iswspace, iswupper, iswxdigit, towlower, towupper
 **/

#ifdef __clang__
	#define HAS(...) __has_builtin(__VA_ARGS__)
#elif defined __GNUC__ //assume gcc ... (where the list came from)
	#define HAS(...) 1
#else
	#define HAS(...) 0
#endif

#if HAS(__builtin_abort)
	#define abort __builtin_abort
#endif
#if HAS(__builtin_abs)
	#define abs __builtin_abs
#endif
#if HAS(__builtin_acos)
	#define acos __builtin_acos
#endif
#if HAS(__builtin_acosf)
	#define acosf __builtin_acosf
#endif
#if HAS(__builtin_acosl)
	#define acosl __builtin_acosl
#endif
#if HAS(__builtin_add_overflow)
	#define add_overflow __builtin_add_overflow
#endif
#if HAS(__builtin_alloca)
	#define alloca __builtin_alloca
#endif
#if HAS(__builtin_asin)
	#define asin __builtin_asin
#endif
#if HAS(__builtin_asinf)
	#define asinf __builtin_asinf
#endif
#if HAS(__builtin_asinl)
	#define asinl __builtin_asinl
#endif
#if HAS(__builtin_assume_aligned)
	#define assume_aligned __builtin_assume_aligned
#endif
#if HAS(__builtin_atan)
	#define atan __builtin_atan
#endif
#if HAS(__builtin_atan2)
	#define atan2 __builtin_atan2
#endif
#if HAS(__builtin_atan2f)
	#define atan2f __builtin_atan2f
#endif
#if HAS(__builtin_atan2l)
	#define atan2l __builtin_atan2l
#endif
#if HAS(__builtin_atanf)
	#define atanf __builtin_atanf
#endif
#if HAS(__builtin_atanl)
	#define atanl __builtin_atanl
#endif
#if HAS(__builtin_bcmp)
	#define bcmp __builtin_bcmp
#endif
#if HAS(__builtin___bnd_chk_ptr_bounds)
	#define __bnd_chk_ptr_bounds __builtin___bnd_chk_ptr_bounds
#endif
#if HAS(__builtin___bnd_chk_ptr_lbounds)
	#define __bnd_chk_ptr_lbounds __builtin___bnd_chk_ptr_lbounds
#endif
#if HAS(__builtin___bnd_chk_ptr_ubounds)
	#define __bnd_chk_ptr_ubounds __builtin___bnd_chk_ptr_ubounds
#endif
#if HAS(__builtin___bnd_copy_ptr_bounds)
	#define __bnd_copy_ptr_bounds __builtin___bnd_copy_ptr_bounds
#endif
#if HAS(__builtin___bnd_get_ptr_lbound)
	#define __bnd_get_ptr_lbound __builtin___bnd_get_ptr_lbound
#endif
#if HAS(__builtin___bnd_get_ptr_ubound)
	#define __bnd_get_ptr_ubound __builtin___bnd_get_ptr_ubound
#endif
#if HAS(__builtin___bnd_init_ptr_bounds)
	#define __bnd_init_ptr_bounds __builtin___bnd_init_ptr_bounds
#endif
#if HAS(__builtin___bnd_narrow_ptr_bounds)
	#define __bnd_narrow_ptr_bounds __builtin___bnd_narrow_ptr_bounds
#endif
#if HAS(__builtin___bnd_null_ptr_bounds)
	#define __bnd_null_ptr_bounds __builtin___bnd_null_ptr_bounds
#endif
#if HAS(__builtin___bnd_set_ptr_bounds)
	#define __bnd_set_ptr_bounds __builtin___bnd_set_ptr_bounds
#endif
#if HAS(__builtin___bnd_store_ptr_bounds)
	#define __bnd_store_ptr_bounds __builtin___bnd_store_ptr_bounds
#endif
#if HAS(__builtin_bswap16)
	#define bswap16 __builtin_bswap16
#endif
#if HAS(__builtin_bswap32)
	#define bswap32 __builtin_bswap32
	#define __bswapd bswap32
#endif
#if HAS(__builtin_bswap64)
	#define bswap64 __builtin_bswap64
	#define __bswapq bswap64
	#define _bswap64 bswap64
#endif
#if HAS(__builtin_bzero)
	#define bzero __builtin_bzero
#endif
#if HAS(__builtin_calloc)
	#define calloc __builtin_calloc
#endif
#if HAS(__builtin_call_with_static_chain)
	#define call_with_static_chain __builtin_call_with_static_chain
#endif
#if HAS(__builtin_ceil)
	#define ceil __builtin_ceil
#endif
#if HAS(__builtin_ceilf)
	#define ceilf __builtin_ceilf
#endif
#if HAS(__builtin_ceill)
	#define ceill __builtin_ceill
#endif
#if HAS(__builtin_choose_expr)
	#define choose_expr __builtin_choose_expr
#endif
#if HAS(__builtin___clear_cache)
	#define __clear_cache __builtin___clear_cache
#endif
#if HAS(__builtin_clrsb)
	#define clrsb __builtin_clrsb
#endif
#if HAS(__builtin_clrsbl)
	#define clrsbl __builtin_clrsbl
#endif
#if HAS(__builtin_clrsbll)
	#define clrsbll __builtin_clrsbll
#endif
#if HAS(__builtin_clz)
	#define clz __builtin_clz
#endif
#if HAS(__builtin_clzl)
	#define clzl __builtin_clzl
#endif
#if HAS(__builtin_clzll)
	#define clzll __builtin_clzll
#endif
#if HAS(__builtin_complex)
	#define complex __builtin_complex
#endif
#if HAS(__builtin_constant_p)
	#define constant_p __builtin_constant_p
#endif
#if HAS(__builtin_cos)
	#define cos __builtin_cos
#endif
#if HAS(__builtin_cosf)
	#define cosf __builtin_cosf
#endif
#if HAS(__builtin_cosh)
	#define cosh __builtin_cosh
#endif
#if HAS(__builtin_coshf)
	#define coshf __builtin_coshf
#endif
#if HAS(__builtin_coshl)
	#define coshl __builtin_coshl
#endif
#if HAS(__builtin_cosl)
	#define cosl __builtin_cosl
#endif
#if HAS(__builtin_ctz)
	#define ctz __builtin_ctz
	#define __bsfd ctz
#endif
#if HAS(__builtin_ctzl)
	#define ctzl __builtin_ctzl
#endif
#if HAS(__builtin_ctzll)
	#define ctzll __builtin_ctzll
	#define __bsfq ctzll
#endif
#if HAS(__builtin_dcgettext)
	#define dcgettext __builtin_dcgettext
#endif
#if HAS(__builtin_dgettext)
	#define dgettext __builtin_dgettext
#endif
#if HAS(__builtin_drem)
	#define drem __builtin_drem
#endif
#if HAS(__builtin_dremf)
	#define dremf __builtin_dremf
#endif
#if HAS(__builtin_dreml)
	#define dreml __builtin_dreml
#endif
#if HAS(__builtin_exit) && 0
	#define exit __builtin_exit
#endif
#if HAS(__builtin__exit)
	#define _exit __builtin__exit
#endif
#if HAS(__builtin_exp)
	#define exp __builtin_exp
#endif
#if HAS(__builtin_exp10)
	#define exp10 __builtin_exp10
#endif
#if HAS(__builtin_exp10f)
	#define exp10f __builtin_exp10f
#endif
#if HAS(__builtin_exp10l)
	#define exp10l __builtin_exp10l
#endif
#if HAS(__builtin_expect)
	#define expect __builtin_expect
#endif
#if HAS(__builtin_expf)
	#define expf __builtin_expf
#endif
#if HAS(__builtin_expl)
	#define expl __builtin_expl
#endif
#if HAS(__builtin_extract_return_addr)
	#define extract_return_addr __builtin_extract_return_addr
#endif
#if HAS(__builtin_fabs)
	#define fabs __builtin_fabs
#endif
#if HAS(__builtin_fabsf)
	#define fabsf __builtin_fabsf
#endif
#if HAS(__builtin_fabsl)
	#define fabsl __builtin_fabsl
#endif
#if HAS(__builtin_ffs)
	#define ffs __builtin_ffs
#endif
#if HAS(__builtin_ffsl)
	#define ffsl __builtin_ffsl
#endif
#if HAS(__builtin_ffsll)
	#define ffsll __builtin_ffsll
#endif
#if HAS(__builtin_floor)
	#define floor __builtin_floor
#endif
#if HAS(__builtin_floorf)
	#define floorf __builtin_floorf
#endif
#if HAS(__builtin_floorl)
	#define floorl __builtin_floorl
#endif
#if HAS(__builtin_fmod)
	#define fmod __builtin_fmod
#endif
#if HAS(__builtin_fmodf)
	#define fmodf __builtin_fmodf
#endif
#if HAS(__builtin_fmodl)
	#define fmodl __builtin_fmodl
#endif
#if HAS(__builtin_fpclassify)
	#define fpclassify __builtin_fpclassify
#endif
#if HAS(__builtin_fprintf)
	#define fprintf __builtin_fprintf
#endif
#if HAS(__builtin___fprintf_chk)
	#define __fprintf_chk __builtin___fprintf_chk
#endif
#if HAS(__builtin_fprintf_unlocked)
	#define fprintf_unlocked __builtin_fprintf_unlocked
#endif
#if HAS(__builtin_fputs)
	#define fputs __builtin_fputs
#endif
#if HAS(__builtin_fputs_unlocked)
	#define fputs_unlocked __builtin_fputs_unlocked
#endif
#if HAS(__builtin_frame_address)
	#define frame_address __builtin_frame_address
#endif
#if HAS(__builtin_frexp)
	#define frexp __builtin_frexp
#endif
#if HAS(__builtin_frexpf)
	#define frexpf __builtin_frexpf
#endif
#if HAS(__builtin_frexpl)
	#define frexpl __builtin_frexpl
#endif
#if HAS(__builtin_frob_return_address)
	#define frob_return_address __builtin_frob_return_address
#endif
#if HAS(__builtin_fscanf)
	#define fscanf __builtin_fscanf
#endif
#if HAS(__builtin_gamma)
	#define gamma __builtin_gamma
#endif
#if HAS(__builtin_gammaf)
	#define gammaf __builtin_gammaf
#endif
#if HAS(__builtin_gammaf_r)
	#define gammaf_r __builtin_gammaf_r
#endif
#if HAS(__builtin_gammal)
	#define gammal __builtin_gammal
#endif
#if HAS(__builtin_gammal_r)
	#define gammal_r __builtin_gammal_r
#endif
#if HAS(__builtin_gamma_r)
	#define gamma_r __builtin_gamma_r
#endif
#if HAS(__builtin_gettext)
	#define gettext __builtin_gettext
#endif
#if HAS(__builtin_huge_val)
	#define huge_val __builtin_huge_val
#endif
#if HAS(__builtin_huge_valf)
	#define huge_valf __builtin_huge_valf
#endif
#if HAS(__builtin_huge_vall)
	#define huge_vall __builtin_huge_vall
#endif
#if HAS(__builtin_index)
	#define index __builtin_index
#endif
#if HAS(__builtin_inf)
	#define inf __builtin_inf
#endif
#if HAS(__builtin_infd128)
	#define infd128 __builtin_infd128
#endif
#if HAS(__builtin_infd32)
	#define infd32 __builtin_infd32
#endif
#if HAS(__builtin_infd64)
	#define infd64 __builtin_infd64
#endif
#if HAS(__builtin_inff)
	#define inff __builtin_inff
#endif
#if HAS(__builtin_infl)
	#define infl __builtin_infl
#endif
#if HAS(__builtin_isalnum)
	#define isalnum __builtin_isalnum
#endif
#if HAS(__builtin_isalpha)
	#define isalpha __builtin_isalpha
#endif
#if HAS(__builtin_isascii)
	#define isascii __builtin_isascii
#endif
#if HAS(__builtin_iscntrl)
	#define iscntrl __builtin_iscntrl
#endif
#if HAS(__builtin_isdigit)
	#define isdigit __builtin_isdigit
#endif
#if HAS(__builtin_isfinite)
	#define isfinite __builtin_isfinite
#endif
#if HAS(__builtin_isgraph)
	#define isgraph __builtin_isgraph
#endif
#if HAS(__builtin_isgreater)
	#define isgreater __builtin_isgreater
#endif
#if HAS(__builtin_isgreaterequal)
	#define isgreaterequal __builtin_isgreaterequal
#endif
#if HAS(__builtin_isinf)
	#define isinf __builtin_isinf
#endif
#if HAS(__builtin_isinf_sign)
	#define isinf_sign __builtin_isinf_sign
#endif
#if HAS(__builtin_isless)
	#define isless __builtin_isless
#endif
#if HAS(__builtin_islessequal)
	#define islessequal __builtin_islessequal
#endif
#if HAS(__builtin_islessgreater)
	#define islessgreater __builtin_islessgreater
#endif
#if HAS(__builtin_islower)
	#define islower __builtin_islower
#endif
#if HAS(__builtin_isnan)
	#define isnan __builtin_isnan
#endif
#if HAS(__builtin_isnormal)
	#define isnormal __builtin_isnormal
#endif
#if HAS(__builtin_isprint)
	#define isprint __builtin_isprint
#endif
#if HAS(__builtin_ispunct)
	#define ispunct __builtin_ispunct
#endif
#if HAS(__builtin_isspace)
	#define isspace __builtin_isspace
#endif
#if HAS(__builtin_isunordered)
	#define isunordered __builtin_isunordered
#endif
#if HAS(__builtin_isupper)
	#define isupper __builtin_isupper
#endif
#if HAS(__builtin_isxdigit)
	#define isxdigit __builtin_isxdigit
#endif
#if HAS(__builtin_j0)
	#define j0 __builtin_j0
#endif
#if HAS(__builtin_j0f)
	#define j0f __builtin_j0f
#endif
#if HAS(__builtin_j0l)
	#define j0l __builtin_j0l
#endif
#if HAS(__builtin_j1)
	#define j1 __builtin_j1
#endif
#if HAS(__builtin_j1f)
	#define j1f __builtin_j1f
#endif
#if HAS(__builtin_j1l)
	#define j1l __builtin_j1l
#endif
#if HAS(__builtin_jn)
	#define jn __builtin_jn
#endif
#if HAS(__builtin_jnf)
	#define jnf __builtin_jnf
#endif
#if HAS(__builtin_jnl)
	#define jnl __builtin_jnl
#endif
#if HAS(__builtin_labs)
	#define labs __builtin_labs
#endif
#if HAS(__builtin_ldexp)
	#define ldexp __builtin_ldexp
#endif
#if HAS(__builtin_ldexpf)
	#define ldexpf __builtin_ldexpf
#endif
#if HAS(__builtin_ldexpl)
	#define ldexpl __builtin_ldexpl
#endif
#if HAS(__builtin_lgammaf_r)
	#define lgammaf_r __builtin_lgammaf_r
#endif
#if HAS(__builtin_lgammal_r)
	#define lgammal_r __builtin_lgammal_r
#endif
#if HAS(__builtin_lgamma_r)
	#define lgamma_r __builtin_lgamma_r
#endif
#if HAS(__builtin_log)
	#define log __builtin_log
#endif
#if HAS(__builtin_log10)
	#define log10 __builtin_log10
#endif
#if HAS(__builtin_log10f)
	#define log10f __builtin_log10f
#endif
#if HAS(__builtin_log10l)
	#define log10l __builtin_log10l
#endif
#if HAS(__builtin_logf)
	#define logf __builtin_logf
#endif
#if HAS(__builtin_logl)
	#define logl __builtin_logl
#endif
#if HAS(__builtin_malloc)
	#define malloc __builtin_malloc
#endif
#if HAS(__builtin_memchr)
	#define memchr __builtin_memchr
#endif
#if HAS(__builtin_memcmp)
	#define memcmp __builtin_memcmp
#endif
#if HAS(__builtin_memcpy)
	#define memcpy __builtin_memcpy
#endif
#if HAS(__builtin___memcpy_chk)
	#define __memcpy_chk __builtin___memcpy_chk
#endif
#if HAS(__builtin_mempcpy)
	#define mempcpy __builtin_mempcpy
#endif
#if HAS(__builtin_memset)
	#define memset __builtin_memset
#endif
#if HAS(__builtin_modf)
	#define modf __builtin_modf
#endif
#if HAS(__builtin_modfl)
	#define modfl __builtin_modfl
#endif
#if HAS(__builtin_mul_overflow)
	#define mul_overflow __builtin_mul_overflow
#endif
#if HAS(__builtin_nan)
	#define nan __builtin_nan
#endif
#if HAS(__builtin_nand128)
	#define nand128 __builtin_nand128
#endif
#if HAS(__builtin_nand32)
	#define nand32 __builtin_nand32
#endif
#if HAS(__builtin_nand64)
	#define nand64 __builtin_nand64
#endif
#if HAS(__builtin_nanf)
	#define nanf __builtin_nanf
#endif
#if HAS(__builtin_nanl)
	#define nanl __builtin_nanl
#endif
#if HAS(__builtin_nans)
	#define nans __builtin_nans
#endif
#if HAS(__builtin_nansf)
	#define nansf __builtin_nansf
#endif
#if HAS(__builtin_nansl)
	#define nansl __builtin_nansl
#endif
#if HAS(__builtin_object_size)
	#define object_size __builtin_object_size
#endif
#if HAS(__builtin_offsetof)
	#define offsetof(...)  __builtin_offsetof (__VA_ARGS__)
#endif
#if HAS(__builtin_parity)
	#define parity __builtin_parity
#endif
#if HAS(__builtin_parityl)
	#define parityl __builtin_parityl
#endif
#if HAS(__builtin_parityll)
	#define parityll __builtin_parityll
#endif
#if HAS(__builtin_popcount)
	#define popcount __builtin_popcount
	#define __popcntd popcount
#endif
#if HAS(__builtin_popcountl)
	#define popcountl __builtin_popcountl
#endif
#if HAS(__builtin_popcountll)
	#define popcountll __builtin_popcountll
	#define __popcntq __builtin_popcountll
	#define _popcnt64 __popcntq
#endif
#if HAS(__builtin_pow)
	#define pow __builtin_pow
#endif
#if HAS(__builtin_pow10)
	#define pow10 __builtin_pow10
#endif
#if HAS(__builtin_pow10f)
	#define pow10f __builtin_pow10f
#endif
#if HAS(__builtin_pow10l)
	#define pow10l __builtin_pow10l
#endif
#if HAS(__builtin_powf)
	#define powf __builtin_powf
#endif
#if HAS(__builtin_powi)
	#define powi __builtin_powi
#endif
#if HAS(__builtin_powif)
	#define powif __builtin_powif
#endif
#if HAS(__builtin_powil)
	#define powil __builtin_powil
#endif
#if HAS(__builtin_powl)
	#define powl __builtin_powl
#endif
#if HAS(__builtin_prefetch)
	#define prefetch __builtin_prefetch
#endif
#if HAS(__builtin_printf)
	#define printf __builtin_printf
#endif
#if HAS(__builtin___printf_chk)
	#define __printf_chk __builtin___printf_chk
#endif
#if HAS(__builtin_printf_unlocked)
	#define printf_unlocked __builtin_printf_unlocked
#endif
#if HAS(__builtin_putchar)
	#define putchar __builtin_putchar
#endif
#if HAS(__builtin_puts)
	#define puts __builtin_puts
#endif
#if HAS(__builtin_return_address)
	#define return_address __builtin_return_address
#endif
#if HAS(__builtin_rindex)
	#define rindex __builtin_rindex
#endif
#if HAS(__builtin_saddll_overflow)
	#define saddll_overflow __builtin_saddll_overflow
#endif
#if HAS(__builtin_saddl_overflow)
	#define saddl_overflow __builtin_saddl_overflow
#endif
#if HAS(__builtin_sadd_overflow)
	#define sadd_overflow __builtin_sadd_overflow
#endif
#if HAS(__builtin_scalb)
	#define scalb __builtin_scalb
#endif
#if HAS(__builtin_scalbf)
	#define scalbf __builtin_scalbf
#endif
#if HAS(__builtin_scalbl)
	#define scalbl __builtin_scalbl
#endif
#if HAS(__builtin_scanf)
	#define scanf __builtin_scanf
#endif
#if HAS(__builtin_signbit)
	#define signbit __builtin_signbit
#endif
#if HAS(__builtin_signbitd128)
	#define signbitd128 __builtin_signbitd128
#endif
#if HAS(__builtin_signbitd32)
	#define signbitd32 __builtin_signbitd32
#endif
#if HAS(__builtin_signbitd64)
	#define signbitd64 __builtin_signbitd64
#endif
#if HAS(__builtin_signbitf)
	#define signbitf __builtin_signbitf
#endif
#if HAS(__builtin_signbitl)
	#define signbitl __builtin_signbitl
#endif
#if HAS(__builtin_significand)
	#define significand __builtin_significand
#endif
#if HAS(__builtin_significandf)
	#define significandf __builtin_significandf
#endif
#if HAS(__builtin_significandl)
	#define significandl __builtin_significandl
#endif
#if HAS(__builtin_sin)
	#define sin __builtin_sin
#endif
#if HAS(__builtin_sincos)
	#define sincos __builtin_sincos
#endif
#if HAS(__builtin_sincosf)
	#define sincosf __builtin_sincosf
#endif
#if HAS(__builtin_sincosl)
	#define sincosl __builtin_sincosl
#endif
#if HAS(__builtin_sinf)
	#define sinf __builtin_sinf
#endif
#if HAS(__builtin_sinh)
	#define sinh __builtin_sinh
#endif
#if HAS(__builtin_sinhf)
	#define sinhf __builtin_sinhf
#endif
#if HAS(__builtin_sinhl)
	#define sinhl __builtin_sinhl
#endif
#if HAS(__builtin_sinl)
	#define sinl __builtin_sinl
#endif
#if HAS(__builtin_smulll_overflow)
	#define smulll_overflow __builtin_smulll_overflow
#endif
#if HAS(__builtin_smull_overflow)
	#define smull_overflow __builtin_smull_overflow
#endif
#if HAS(__builtin_smul_overflow)
	#define smul_overflow __builtin_smul_overflow
#endif
#if HAS(__builtin_snprintf)
	#define snprintf __builtin_snprintf
#endif
#if HAS(__builtin___snprintf_chk)
	#define __snprintf_chk __builtin___snprintf_chk
#endif
#if HAS(__builtin_sprintf)
	#define sprintf __builtin_sprintf
#endif
#if HAS(__builtin___sprintf_chk)
	#define __sprintf_chk __builtin___sprintf_chk
#endif
#if HAS(__builtin_sqrt)
	#define sqrt __builtin_sqrt
#endif
#if HAS(__builtin_sqrtf)
	#define sqrtf __builtin_sqrtf
#endif
#if HAS(__builtin_sqrtl)
	#define sqrtl __builtin_sqrtl
#endif
#if HAS(__builtin_sscanf)
	#define sscanf __builtin_sscanf
#endif
#if HAS(__builtin_ssubll_overflow)
	#define ssubll_overflow __builtin_ssubll_overflow
#endif
#if HAS(__builtin_ssubl_overflow)
	#define ssubl_overflow __builtin_ssubl_overflow
#endif
#if HAS(__builtin_ssub_overflow)
	#define ssub_overflow __builtin_ssub_overflow
#endif
#if HAS(__builtin_stpcpy)
	#define stpcpy __builtin_stpcpy
#endif
#if HAS(__builtin_stpncpy)
	#define stpncpy __builtin_stpncpy
#endif
#if HAS(__builtin_strcasecmp)
	#define strcasecmp __builtin_strcasecmp
#endif
#if HAS(__builtin_strcat)
	#define strcat __builtin_strcat
#endif
#if HAS(__builtin_strchr)
	#define strchr __builtin_strchr
#endif
#if HAS(__builtin_strcmp)
	#define strcmp __builtin_strcmp
#endif
#if HAS(__builtin_strcpy)
	#define strcpy __builtin_strcpy
#endif
#if HAS(__builtin_strcspn)
	#define strcspn __builtin_strcspn
#endif
#if HAS(__builtin_strdup)
	#define strdup __builtin_strdup
#endif
#if HAS(__builtin_strfmon)
	#define strfmon __builtin_strfmon
#endif
#if HAS(__builtin_strlen)
	#define strlen __builtin_strlen
#endif
#if HAS(__builtin_strncasecmp)
	#define strncasecmp __builtin_strncasecmp
#endif
#if HAS(__builtin_strncat)
	#define strncat __builtin_strncat
#endif
#if HAS(__builtin_strncmp)
	#define strncmp __builtin_strncmp
#endif
#if HAS(__builtin_strncpy)
	#define strncpy __builtin_strncpy
#endif
#if HAS(__builtin_strndup)
	#define strndup __builtin_strndup
#endif
#if HAS(__builtin_strpbrk)
	#define strpbrk __builtin_strpbrk
#endif
#if HAS(__builtin_strrchr)
	#define strrchr __builtin_strrchr
#endif
#if HAS(__builtin_strspn)
	#define strspn __builtin_strspn
#endif
#if HAS(__builtin_strstr)
	#define strstr __builtin_strstr
#endif
#if HAS(__builtin_sub_overflow)
	#define sub_overflow __builtin_sub_overflow
#endif
#if HAS(__builtin_tan)
	#define tan __builtin_tan
#endif
#if HAS(__builtin_tanf)
	#define tanf __builtin_tanf
#endif
#if HAS(__builtin_tanh)
	#define tanh __builtin_tanh
#endif
#if HAS(__builtin_tanhf)
	#define tanhf __builtin_tanhf
#endif
#if HAS(__builtin_tanhl)
	#define tanhl __builtin_tanhl
#endif
#if HAS(__builtin_tanl)
	#define tanl __builtin_tanl
#endif
#if HAS(__builtin_toascii)
	#define toascii __builtin_toascii
#endif
#if HAS(__builtin_tolower)
	#define tolower __builtin_tolower
#endif
#if HAS(__builtin_toupper)
	#define toupper __builtin_toupper
#endif
#if HAS(__builtin_trap)
	#define trap __builtin_trap
#endif
#if HAS(__builtin_types_compatible_p)
	#define types_compatible_p __builtin_types_compatible_p
#endif
#if HAS(__builtin_uaddll_overflow)
	#define uaddll_overflow __builtin_uaddll_overflow
#endif
#if HAS(__builtin_uaddl_overflow)
	#define uaddl_overflow __builtin_uaddl_overflow
#endif
#if HAS(__builtin_uadd_overflow)
	#define uadd_overflow __builtin_uadd_overflow
#endif
#if HAS(__builtin_umulll_overflow)
	#define umulll_overflow __builtin_umulll_overflow
#endif
#if HAS(__builtin_umull_overflow)
	#define umull_overflow __builtin_umull_overflow
#endif
#if HAS(__builtin_umul_overflow)
	#define umul_overflow __builtin_umul_overflow
#endif
#if HAS(__builtin_unreachable)
	#define unreachable __builtin_unreachable
#endif
#if HAS(__builtin_usubll_overflow)
	#define usubll_overflow __builtin_usubll_overflow
#endif
#if HAS(__builtin_usubl_overflow)
	#define usubl_overflow __builtin_usubl_overflow
#endif
#if HAS(__builtin_usub_overflow)
	#define usub_overflow __builtin_usub_overflow
#endif
#if HAS(__builtin_vfprintf)
	#define vfprintf __builtin_vfprintf
#endif
#if HAS(__builtin___vfprintf_chk)
	#define __vfprintf_chk __builtin___vfprintf_chk
#endif
#if HAS(__builtin_vprintf)
	#define vprintf __builtin_vprintf
#endif
#if HAS(__builtin___vprintf_chk)
	#define __vprintf_chk __builtin___vprintf_chk
#endif
#if HAS(__builtin___vsnprintf_chk)
	#define __vsnprintf_chk __builtin___vsnprintf_chk
#endif
#if HAS(__builtin_vsprintf)
	#define vsprintf __builtin_vsprintf
#endif
#if HAS(__builtin___vsprintf_chk)
	#define __vsprintf_chk __builtin___vsprintf_chk
#endif
#if HAS(__builtin_y0)
	#define y0 __builtin_y0
#endif
#if HAS(__builtin_y0f)
	#define y0f __builtin_y0f
#endif
#if HAS(__builtin_y0l)
	#define y0l __builtin_y0l
#endif
#if HAS(__builtin_y1)
	#define y1 __builtin_y1
#endif
#if HAS(__builtin_y1f)
	#define y1f __builtin_y1f
#endif
#if HAS(__builtin_y1l)
	#define y1l __builtin_y1l
#endif
#if HAS(__builtin_yn)
	#define yn __builtin_yn
#endif
#if HAS(__builtin_ynf)
	#define ynf __builtin_ynf
#endif
#if HAS(__builtin_ynl)
	#define ynl __builtin_ynl
#endif

//builtin atomic operations
#if HAS(__atomic_add_fetch)
	#define add_fetch __atomic_add_fetch
#endif
#if HAS(__atomic_always_lock_free)
	#define always_lock_free __atomic_always_lock_free
#endif
#if HAS(__atomic_and_fetch)
	#define and_fetch __atomic_and_fetch
#endif
#if HAS(__atomic_clear)
	#define clear __atomic_clear
#endif
#if HAS(__atomic_compare_exchange)
	#define compare_exchange __atomic_compare_exchange
#endif
#if HAS(__atomic_compare_exchange_n)
	#define compare_exchange_n __atomic_compare_exchange_n
#endif
#if HAS(__atomic_exchange)
	#define exchange __atomic_exchange
#endif
#if HAS(__atomic_exchange_n)
	#define exchange_n __atomic_exchange_n
#endif
#if HAS(__atomic_fetch_add)
	#define fetch_add __atomic_fetch_add
#endif
#if HAS(__atomic_fetch_and)
	#define fetch_and __atomic_fetch_and
#endif
#if HAS(__atomic_fetch_nand)
	#define fetch_nand __atomic_fetch_nand
#endif
#if HAS(__atomic_fetch_or)
	#define fetch_or __atomic_fetch_or
#endif
#if HAS(__atomic_fetch_sub)
	#define fetch_sub __atomic_fetch_sub
#endif
#if HAS(__atomic_fetch_xor)
	#define fetch_xor __atomic_fetch_xor
#endif
#if HAS(__atomic_is_lock_free)
	#define is_lock_free __atomic_is_lock_free
#endif
#if HAS(__atomic_load)
	#define load __atomic_load
#endif
#if HAS(__atomic_load_n)
	#define load_n __atomic_load_n
#endif
#if HAS(__atomic_nand_fetch)
	#define nand_fetch __atomic_nand_fetch
#endif
#if HAS(__atomic_or_fetch)
	#define or_fetch __atomic_or_fetch
#endif
#if HAS(__atomic_signal_fence)
	#define signal_fence __atomic_signal_fence
#endif
#if HAS(__atomic_store)
	#define store __atomic_store
#endif
#if HAS(__atomic_store_n)
	#define store_n __atomic_store_n
#endif
#if HAS(__atomic_sub_fetch)
	#define sub_fetch __atomic_sub_fetch
#endif
#if HAS(__atomic_test_and_set)
	#define test_and_set __atomic_test_and_set
#endif
#if HAS(__atomic_thread_fence)
	#define thread_fence __atomic_thread_fence
#endif
#if HAS(__atomic_xor_fetch)
	#define xor_fetch __atomic_xor_fetch
#endif

//cilk plus builtins
#if HAS(__sec_implicit_index)
	#define implicit_index __sec_implicit_index
#endif
#if HAS(__sec_reduce)
	#define reduce __sec_reduce
#endif
#if HAS(__sec_reduce_add)
	#define reduce_add __sec_reduce_add
#endif
#if HAS(__sec_reduce_all_nonzero)
	#define reduce_all_nonzero __sec_reduce_all_nonzero
#endif
#if HAS(__sec_reduce_all_zero)
	#define reduce_all_zero __sec_reduce_all_zero
#endif
#if HAS(__sec_reduce_any_nonzero)
	#define reduce_any_nonzero __sec_reduce_any_nonzero
#endif
#if HAS(__sec_reduce_any_zero)
	#define reduce_any_zero __sec_reduce_any_zero
#endif
#if HAS(__sec_reduce_max)
	#define reduce_max __sec_reduce_max
#endif
#if HAS(__sec_reduce_max_ind)
	#define reduce_max_ind __sec_reduce_max_ind
#endif
#if HAS(__sec_reduce_min)
	#define reduce_min __sec_reduce_min
#endif
#if HAS(__sec_reduce_min_ind)
	#define reduce_min_ind __sec_reduce_min_ind
#endif
#if HAS(__sec_reduce_mul)
	#define reduce_mul __sec_reduce_mul
#endif
#if HAS(__sec_reduce_mutating)
	#define reduce_mutating __sec_reduce_mutating
#endif

#endif

//architecture dependent builtins
#if defined __i386__ || defined __x86_64__

	#if HAS(__builtin_ia32_pause)
		#define pause __builtin_ia32_pause
		#define __pause __builtin_ia32_pause
	#endif

	#if HAS(__builtin_cpu_init)
		#define cpu_init __builtin_cpu_init
	#endif

	#if HAS(__builtin_cpu_is)
		#define cpu_is __builtin_cpu_is
	#endif

	#if HAS(__builtin_cpu_supports)
		#define cpu_supports __builtin_cpu_supports
	#endif

//not documented
	#if HAS(__builtin_ia32_bsrsi)
		#define bsrsi __builtin_ia32_bsrsi
		#define __bsrd bsrsi
	#endif

//not documented
	#if HAS(__builtin_ia32_rdpmc)
		#define rdpmc __builtin_ia32_rdpmc
		#define __rdpmc __builtin_ia32_rdpmc
	#endif

//not documented
	#if HAS(__builtin_ia32_rdtsc)
		#define rdtsc __builtin_ia32_rdtsc
		#define __rdtsc __builtin_ia32_rdtsc
	#endif

//not documented
	#if HAS(__builtin_ia32_rdtscp)
		#define rdtscp __builtin_ia32_rdtscp
		#define __rdtscp __builtin_ia32_rdtscp
	#endif

//not documented
	#if HAS(__builtin_ia32_rolqi)
		#define rolb __builtin_ia32_rolqi
		#define __rolb __builtin_ia32_rolqi
	#endif

//not documented
	#if HAS(__builtin_ia32_rolhi)
		#define rolw __builtin_ia32_rolhi
		#define __rolw __builtin_ia32_rolhi
	#endif

//not documented
	#if HAS(__builtin_ia32_rorqi)
		#define rorb __builtin_ia32_rorqi
		#define __rorb __builtin_ia32_rorqi
	#endif

//not documented
	#if HAS(__builtin_ia32_rorhi)
		#define rorw __builtin_ia32_rorhi
		#define __rorw __builtin_ia32_rorhi
	#endif


static __inline unsigned int __rold(unsigned int i,int j){return (i>>(32-j))|(i<<j);}
static __inline unsigned int __rord(unsigned int i,int j){return (i<<(32-j))|(i>>j);}

static __inline unsigned long long __rolq(unsigned long long i,long long j){return (i>>(64-j))|(i<<j);}
static __inline unsigned long long __rorq(unsigned long long i,long long j){return (i<<(64-j))|(i>>j);}

//probably goes outside of arch
//#define _lrotl(a,b)		__rolq((a), (b)) ... __rold((a), (b))
//#define _lrotr(a,b)		__rorq((a), (b)) ... __rord((a), (b))

#define _bit_scan_forward(a)	__bsfd(a)
#define _bit_scan_reverse(a)	__bsrd(a)
#define _bswap(a)		__bswapd(a)
#define _popcnt32(a)	__popcntd(a)
#define _rdpmc(a)		__rdpmc(a)
#define _rdtsc()		__rdtsc()
#define _rdtscp(a)		__rdtscp(a)
#define _rotwl(a,b)		__rolw((a), (b))
#define _rotwr(a,b)		__rorw((a), (b))
#define _rotl(a,b)		__rold((a), (b))
#define _rotr(a,b)		__rord((a), (b))


#ifdef __MMX__
typedef int __m64 __attribute__ ((__vector_size__ (8), __may_alias__));
typedef int __v2si __attribute__ ((__vector_size__ (8)));
typedef short __v4hi __attribute__ ((__vector_size__ (8)));
typedef char __v8qi __attribute__ ((__vector_size__ (8)));
typedef long long __v1di __attribute__ ((__vector_size__ (8)));
typedef float __v2sf __attribute__ ((__vector_size__ (8)));

//not documented
	#if HAS(__builtin_ia32_emms)
		#define emms __builtin_ia32_emms
		#define _mm_empty __builtin_ia32_emms
		#define __mm_empty __builtin_ia32_emms
	#endif

//not documented
	#if HAS(__builtin_ia32_vec_init_v2si)
		#define vec_init_v2si __builtin_ia32_vec_init_v2si
		#define _mm_cvtsi32_si64(i) ((__m64)vec_init_v2si((i),0))
		#define _m_from_int _mm_cvtsi32_si64
		#define _mm_set_pi32(...) ((__m64)vec_init_v2si(__VA_ARGS__))
		#define _mm_setr_pi32(i,j) _mm_set_pi32((j),(i))
		static inline __m64 _mm_set1_pi32(int i){return _mm_set_pi32(i,i);}
	#endif

//not documented
	#if HAS(__builtin_ia32_vec_init_v4hi)
		#define vec_init_v4hi __builtin_ia32_vec_init_v4hi
		#define _mm_set_pi16(...) ((__m64)vec_init_v4hi(__VA_ARGS__))
		#define _mm_setr_pi16(i,j) _mm_set_pi16((j),(i))
		static inline __m64 _mm_set1_pi16(int i){return _mm_set_pi16(i,i,i,i);}
	#endif

//not documented
	#if HAS(__builtin_ia32_vec_init_v8qi)
		#define vec_init_v8qi __builtin_ia32_vec_init_v8qi
		#define _mm_set_pi8(...) ((__m64)vec_init_v8qi(__VA_ARGS__))
		#define _mm_setr_pi8(i,j) _mm_set_pi8((j),(i))
		static inline __m64 _mm_set1_pi8(int i){return _mm_set_pi8(i,i,i,i,i,i,i,i);}
	#endif

//not documented
	#if HAS(__builtin_ia32_vec_ext_v2si)
		#define _mm_cvtsi64_si32(i) __builtin_ia32_vec_ext_v2si((__v2si)(i),0)
		#define _m_to_int _mm_cvtsi64_si32
	#endif

//not documented
	#if HAS(__builtin_ia32_pmaddwd)
		#define pmaddwd __builtin_ia32_pmaddwd
		#define _mm_madd_pi16(i,j) ((__m64)pmaddwd((__v4hi)(i),(__v4hi)(j)))
		#define _m_pmaddwd _mm_madd_pi16
	#endif
	
	#define _mm_setzero_si64 ((__m64)0)

#ifdef __x86_64__
	#define _m_from_int64(i) ((__m64)(i))
	#define _mm_from_int64(i) ((__m64)(i))
	#define _mm_cvtsi64x_si64(i) ((__m64)(i))
	#define _mm_set_pi64x(i) ((__m64)(i))
	#define _m_to_int64(i) ((long long)(i))
	#define _mm_cvtm64_si64(i) ((long long)(i))
	#define _mm_cvtsi64_si64x(i) ((long long)(i))
#endif

	#if HAS(__builtin_ia32_packssdw)
		#define packssdw __builtin_ia32_packssdw
		#define _mm_packs_pi32(i,j) ((__m64)packssdw((__v2si)(i),(__v2si)(j)))
		#define _m_packssdw _mm_packs_pi32
	#endif

	#if HAS(__builtin_ia32_packsswb)
		#define packsswb __builtin_ia32_packsswb
		#define _mm_packs_pi16(i,j) ((__m64)packsswb((__v4hi)(i),(__v4hi)(j)))
		#define _m_packsswb _mm_packs_pi16
	#endif

	#if HAS(__builtin_ia32_packuswb)
		#define packuswb __builtin_ia32_packuswb
		#define _mm_packs_pu16(i,j) ((__m64)packuswb((__v4hi)(i),(__v4hi)(j)))
		#define _m_packuswb _mm_packs_pu16
	#endif

	#if HAS(__builtin_ia32_paddb)
		#define paddb __builtin_ia32_paddb
		#define _mm_add_pi8(i,j) ((__m64)paddb((__v8qi)(i),(__v8qi)(j)))
		#define _m_paddb _mm_add_pi8
	#endif

	#if HAS(__builtin_ia32_paddd)
		#define paddd __builtin_ia32_paddd
		#define _mm_add_pi32(i,j) ((__m64)paddd((__v2si)(i),(__v2si)(j)))
		#define _m_paddd _mm_add_pi32
	#endif

	#if HAS(__builtin_ia32_paddsb)
		#define paddsb __builtin_ia32_paddsb
		#define _mm_adds_pi8(i,j) ((__m64)paddsb((__v8qi)(i),(__v8qi)(j)))
		#define _m_paddsb _mm_adds_pi8
	#endif

	#if HAS(__builtin_ia32_paddsw)
		#define paddsw __builtin_ia32_paddsw
		#define _mm_adds_pi16(i,j) ((__m64)paddsw((__v4hi)(i),(__v4hi)(j)))
		#define _m_paddsw _mm_adds_pi16
	#endif

	#if HAS(__builtin_ia32_paddusb)
		#define paddusb __builtin_ia32_paddusb
		#define _mm_adds_pu8(i,j) ((__m64)paddusb((__v8qi)(i),(__v8qi)(j)))
		#define _m_paddusb _mm_adds_pu8
	#endif

	#if HAS(__builtin_ia32_paddusw)
		#define paddusw __builtin_ia32_paddusw
		#define _mm_adds_pu16(i,j) ((__m64)paddusw((__v4hi)(i),(__v4hi)(j)))
		#define _m_paddusw _mm_adds_pu16
	#endif

	#if HAS(__builtin_ia32_paddw)
		#define paddw __builtin_ia32_paddw
		#define _mm_add_pi16(i,j) ((__m64)paddw((__v4hi)(i),(__v4hi)(j)))
		#define _m_paddw _mm_add_pi16
	#endif

	#if HAS(__builtin_ia32_pand)
		#define pand __builtin_ia32_pand
		#define _mm_and_si64 pand
		#define _m_pand pand
	#endif

	#if HAS(__builtin_ia32_pandn)
		#define pandn __builtin_ia32_pandn
		#define _mm_andnot_si64 pandn
		#define _m_pandn pandn
	#endif

	#if HAS(__builtin_ia32_pcmpeqb)
		#define pcmpeqb __builtin_ia32_pcmpeqb
		#define _mm_cmpeq_pi8(i,j) ((__m64)pcmpeqb((__v8qi)(i),(__v8qi)(j)))
		#define _m_pcmpeqb _mm_cmpeq_pi8
	#endif

	#if HAS(__builtin_ia32_pcmpeqd)
		#define pcmpeqd __builtin_ia32_pcmpeqd
		#define _mm_cmpeq_pi32(i,j) ((__m64)pcmpeqd((__v2si)(i),(__v2si)(j)))
		#define _m_pcmpeqd _mm_cmpeq_pi32
	#endif

	#if HAS(__builtin_ia32_pcmpeqw)
		#define pcmpeqw __builtin_ia32_pcmpeqw
		#define _mm_cmpeq_pi16(i,j) ((__m64)pcmpeqw((__v4hi)(i),(__v4hi)(j)))
		#define _m_pcmpeqw _mm_cmpeq_pi16
	#endif

	#if HAS(__builtin_ia32_pcmpgtb)
		#define pcmpgtb __builtin_ia32_pcmpgtb
		#define _mm_cmpgt_pi8(i,j) ((__m64)pcmpgtb((__v8qi)(i),(__v8qi)(j)))
		#define _m_pcmpgtb _mm_cmpgt_pi8
	#endif

	#if HAS(__builtin_ia32_pcmpgtd)
		#define pcmpgtd __builtin_ia32_pcmpgtd
		#define _mm_cmpgt_pi32(i,j) ((__m64)pcmpgtd((__v2si)(i),(__v2si)(j)))
		#define _m_pcmpgtd _mm_cmpgt_pi32
	#endif

	#if HAS(__builtin_ia32_pcmpgtw)
		#define pcmpgtw __builtin_ia32_pcmpgtw
		#define _mm_cmpgt_pi16(i,j) ((__m64)pcmpgtw((__v4hi)(i),(__v4hi)(j)))
		#define _m_pcmpgtw _mm_cmpgt_pi16
	#endif

	#if HAS(__builtin_ia32_pmulhw)
		#define pmulhw __builtin_ia32_pmulhw
		#define _mm_mulhi_pi16(i,j) ((__m64)pmulhw((__v4hi)(i),(__v4hi)(j)))
		#define _m_pmulhw _mm_mulhi_pi16
	#endif

	#if HAS(__builtin_ia32_pmullw)
		#define pmullw __builtin_ia32_pmullw
		#define _mm_mullo_pi16(i,j) ((__m64)pmullw((__v4hi)(i),(__v4hi)(j)))
		#define _m_pmullw _mm_mullo_pi16
	#endif

	#if HAS(__builtin_ia32_por)
		#define por __builtin_ia32_por
		#define _mm_or_si64 por
		#define _m_por por
	#endif

	#if HAS(__builtin_ia32_pslld)
		#define pslld __builtin_ia32_pslld
		#define _mm_sll_pi32(i,j) ((__m64)pslld((__v2si)(i),(__v2si)(j)))
		#define _m_pslld _mm_sll_pi32
	#endif

	#if HAS(__builtin_ia32_pslldi)
		#define pslldi __builtin_ia32_pslldi
		#define _mm_slli_pi32(i,j) ((__m64)pslldi((__v2si)(i),(j)))
		#define _m_pslldi _mm_slli_pi32
	#endif

	#if HAS(__builtin_ia32_psllq)
		#define psllq __builtin_ia32_psllq
		#define _mm_sll_si64(i,j) ((__m64)psllq((__v1di)(i),(__v1di)(j)))
		#define _m_psllq _mm_sll_si64
	#endif

	#if HAS(__builtin_ia32_psllqi)
		#define psllqi __builtin_ia32_psllqi
		#define _mm_slli_si64(i,j) ((__m64)psllqi((__v1di)(i),(j)))
		#define _m_psllqi _mm_slli_si64
	#endif

	#if HAS(__builtin_ia32_psllw)
		#define psllw __builtin_ia32_psllw
		#define _mm_sll_pi16(i,j) ((__m64)psllw((__v4hi)(i),(__v4hi)(j)))
		#define _m_psllw _mm_sll_pi16
	#endif

	#if HAS(__builtin_ia32_psllwi)
		#define psllwi __builtin_ia32_psllwi
		#define _mm_slli_pi16(i,j) ((__m64)psllwi((__v4hi)(i),(j)))
		#define _m_psllwi _mm_slli_pi16
	#endif

	#if HAS(__builtin_ia32_psrad)
		#define psrad __builtin_ia32_psrad
		#define _mm_sra_pi32(i,j) ((__m64)psrad((__v2si)(i),(__v2si)(j)))
		#define _m_psrad _mm_sra_pi32
	#endif

	#if HAS(__builtin_ia32_psradi)
		#define psradi __builtin_ia32_psradi
		#define _mm_srai_pi32(i,j) ((__m64)psradi((__v2si)(i),(j)))
		#define _m_psradi _mm_srai_pi32
	#endif

	#if HAS(__builtin_ia32_psraw)
		#define psraw __builtin_ia32_psraw
		#define _mm_sra_pi16(i,j) ((__m64)psraw((__v4hi)(i),(__v4hi)(j))) 
		#define _m_psraw _mm_sra_pi16
	#endif

	#if HAS(__builtin_ia32_psrawi)
		#define psrawi __builtin_ia32_psrawi
		#define _mm_srai_pi16(i,j) ((__m64)psrawi((__v4hi)(i),(j)))
		#define _m_psrawi _mm_srai_pi16
	#endif

	#if HAS(__builtin_ia32_psrld)
		#define psrld __builtin_ia32_psrld
		#define _mm_srl_pi32(i,j) ((__m64)psrld((__v2si)(i),(__v2si)(j)))
		#define _m_psrld _mm_srl_pi32
	#endif

	#if HAS(__builtin_ia32_psrldi)
		#define psrldi __builtin_ia32_psrldi
		#define _mm_srli_pi32(i,j) ((__m64)psrldi((__v2si)(i),(j)))
		#define _m_psrldi _mm_srli_pi32
	#endif

	#if HAS(__builtin_ia32_psrlq)
		#define psrlq __builtin_ia32_psrlq
		#define _mm_srl_si64(i,j) ((__m64)psrlq((__v1di)(i),(__v1di)(j)))
		#define _m_psrlq _mm_srl_si64
	#endif

	#if HAS(__builtin_ia32_psrlqi)
		#define psrlqi __builtin_ia32_psrlqi
		#define _mm_srli_si64(i,j) ((__m64)psrlqi((__v1di)(i),(j)))
		#define _m_psrlqi _mm_srli_si64
	#endif

	#if HAS(__builtin_ia32_psrlw)
		#define psrlw __builtin_ia32_psrlw
		#define _mm_srl_pi16(i,j) ((__m64)psrlw((__v4hi)(i),(__v4hi)(j)))
		#define _m_psrlw _mm_srl_pi16
	#endif

	#if HAS(__builtin_ia32_psrlwi)
		#define psrlwi __builtin_ia32_psrlwi
		#define _mm_srli_pi16(i,j) ((__m64)psrlwi((__v4hi)(i),(j)))
		#define _m_psrlwi _mm_srli_pi16
	#endif

	#if HAS(__builtin_ia32_psubb)
		#define psubb __builtin_ia32_psubb
		#define _mm_sub_pi8(i,j) ((__m64)psubb((__v8qi)(i),(__v8qi)(j)))
		#define _m_psubb _mm_sub_pi8
	#endif

	#if HAS(__builtin_ia32_psubd)
		#define psubd __builtin_ia32_psubd
		#define _mm_sub_pi32(i,j) ((__m64)psubd((__v2si)(i),(__v2si)(j)))
		#define _m_psubd _mm_sub_pi32
	#endif

	#if HAS(__builtin_ia32_psubsb)
		#define psubsb __builtin_ia32_psubsb
		#define _mm_subs_pi8(i,j) ((__m64)psubsb((__v8qi)(i),(__v8qi)(j)))
		#define _m_psubsb _mm_subs_pi8
	#endif

	#if HAS(__builtin_ia32_psubsw)
		#define psubsw __builtin_ia32_psubsw
		#define _mm_subs_pi16(i,j) ((__m64)psubsw((__v4hi)(i),(__v4hi)(j)))
		#define _m_psubsw _mm_subs_pi16
	#endif

	#if HAS(__builtin_ia32_psubusb)
		#define psubusb __builtin_ia32_psubusb
		#define _mm_subs_pu8(i,j) ((__m64)psubusb((__v8qi)(i),(__v8qi)(j)))
		#define _m_psubusb _mm_subs_pu8
	#endif

	#if HAS(__builtin_ia32_psubusw)
		#define psubusw __builtin_ia32_psubusw
		#define _mm_subs_pu16(i,j) ((__m64)psubusw((__v4hi)(i),(__v4hi)(j)))
		#define _m_psubusw _mm_subs_pu16
	#endif

	#if HAS(__builtin_ia32_psubw)
		#define psubw __builtin_ia32_psubw
		#define _mm_sub_pi16(i,j) ((__m64)psubw((__v4hi)(i),(__v4hi)(j)))
		#define _m_psubw _mm_sub_pi16
	#endif

	#if HAS(__builtin_ia32_punpckhbw)
		#define punpckhbw __builtin_ia32_punpckhbw
		#define _mm_unpackhi_pi8(i,j) ((__m64)punpckhbw((__v8qi)(i),(__v8qi)(j)))
		#define _m_punpckhbw _mm_unpackhi_pi8
	#endif

	#if HAS(__builtin_ia32_punpckhdq)
		#define punpckhdq __builtin_ia32_punpckhdq
		#define _mm_unpackhi_pi32 (i,j) ((__m64)punpckhdq((__v2si)(i),(__v2si)(j)))
		#define _m_punpckhdq _mm_unpackhi_pi32
	#endif

	#if HAS(__builtin_ia32_punpckhwd)
		#define punpckhwd __builtin_ia32_punpckhwd
		#define _mm_unpackhi_pi16(i,j) ((__m64)punpckhwd((__v4hi)(i),(__v4hi)(j)))
		#define _m_punpckhwd _mm_unpackhi_pi16
	#endif

	#if HAS(__builtin_ia32_punpcklbw)
		#define punpcklbw __builtin_ia32_punpcklbw
		#define _mm_unpacklo_pi8(i,j) ((__m64)punpcklbw((__v8qi)(i),(__v8qi)(j)))
		#define _m_punpcklbw _mm_unpacklo_pi8
	#endif

	#if HAS(__builtin_ia32_punpckldq)
		#define punpckldq __builtin_ia32_punpckldq
		#define _mm_unpacklo_pi32(i,j) ((__m64)punpckldq((__v2si)(i),(__v2si)(j)))
		#define _m_punpckldq _mm_unpacklo_pi32
	#endif

	#if HAS(__builtin_ia32_punpcklwd)
		#define punpcklwd __builtin_ia32_punpcklwd
		#define _mm_unpacklo_pi16(i,j) ((__m64)punpcklwd((__v4hi)(i),(__v4hi)(j)))
		#define _m_punpcklwd _mm_unpacklo_pi16
	#endif

	#if HAS(__builtin_ia32_pxor)
		#define pxor __builtin_ia32_pxor
		#define _mm_xor_si64 pxor
		#define _m_pxor pxor
	#endif

#endif

#if defined __SSE__ || defined __3dNOW_A__

	#if HAS(__builtin_ia32_maskmovq)
		#ifdef maskmovq
			#undef maskmovq
		#endif
		#define maskmovq __builtin_ia32_maskmovq
	#endif

	#if HAS(__builtin_ia32_movntq)
		#ifdef movntq
			#undef movntq
		#endif
		#define movntq __builtin_ia32_movntq
	#endif

	#if HAS(__builtin_ia32_pavgb)
		#ifdef pavgb
			#undef pavgb
		#endif
		#define pavgb __builtin_ia32_pavgb
	#endif

	#if HAS(__builtin_ia32_pavgw)
		#ifdef pavgw
			#undef pavgw
		#endif
		#define pavgw __builtin_ia32_pavgw
	#endif

	#if HAS(__builtin_ia32_pmaxsw)
		#ifdef pmaxsw
			#undef pmaxsw
		#endif
		#define pmaxsw __builtin_ia32_pmaxsw
	#endif

	#if HAS(__builtin_ia32_pmaxub)
		#ifdef pmaxub
			#undef pmaxub
		#endif
		#define pmaxub __builtin_ia32_pmaxub
	#endif

	#if HAS(__builtin_ia32_pminsw)
		#ifdef pminsw
			#undef pminsw
		#endif
		#define pminsw __builtin_ia32_pminsw
	#endif

	#if HAS(__builtin_ia32_pminub)
		#ifdef pminub
			#undef pminub
		#endif
		#define pminub __builtin_ia32_pminub
	#endif

	#if HAS(__builtin_ia32_pmovmskb)
		#ifdef pmovmskb
			#undef pmovmskb
		#endif
		#define pmovmskb __builtin_ia32_pmovmskb
	#endif

	#if HAS(__builtin_ia32_pmulhuw)
		#ifdef pmulhuw
			#undef pmulhuw
		#endif
		#define pmulhuw __builtin_ia32_pmulhuw
	#endif

	#if HAS(__builtin_ia32_psadbw)
		#ifdef psadbw
			#undef psadbw
		#endif
		#define psadbw __builtin_ia32_psadbw
	#endif

	#if HAS(__builtin_ia32_sfence)
		#ifdef sfence
			#undef sfence
		#endif
		#define sfence __builtin_ia32_sfence
	#endif


#endif

#if defined __SSE__

	#if HAS(__builtin_ia32_addps)
		#ifdef addps
			#undef addps
		#endif
		#define addps __builtin_ia32_addps
	#endif

	#if HAS(__builtin_ia32_addss)
		#ifdef addss
			#undef addss
		#endif
		#define addss __builtin_ia32_addss
	#endif

	#if HAS(__builtin_ia32_andnps)
		#ifdef andnps
			#undef andnps
		#endif
		#define andnps __builtin_ia32_andnps
	#endif

	#if HAS(__builtin_ia32_andps)
		#ifdef andps
			#undef andps
		#endif
		#define andps __builtin_ia32_andps
	#endif

	#if HAS(__builtin_ia32_cmpeqps)
		#ifdef cmpeqps
			#undef cmpeqps
		#endif
		#define cmpeqps __builtin_ia32_cmpeqps
	#endif

	#if HAS(__builtin_ia32_cmpeqss)
		#ifdef cmpeqss
			#undef cmpeqss
		#endif
		#define cmpeqss __builtin_ia32_cmpeqss
	#endif

	#if HAS(__builtin_ia32_cmpgeps)
		#ifdef cmpgeps
			#undef cmpgeps
		#endif
		#define cmpgeps __builtin_ia32_cmpgeps
	#endif

	#if HAS(__builtin_ia32_cmpgtps)
		#ifdef cmpgtps
			#undef cmpgtps
		#endif
		#define cmpgtps __builtin_ia32_cmpgtps
	#endif

	#if HAS(__builtin_ia32_cmpleps)
		#ifdef cmpleps
			#undef cmpleps
		#endif
		#define cmpleps __builtin_ia32_cmpleps
	#endif

	#if HAS(__builtin_ia32_cmpless)
		#ifdef cmpless
			#undef cmpless
		#endif
		#define cmpless __builtin_ia32_cmpless
	#endif

	#if HAS(__builtin_ia32_cmpltps)
		#ifdef cmpltps
			#undef cmpltps
		#endif
		#define cmpltps __builtin_ia32_cmpltps
	#endif

	#if HAS(__builtin_ia32_cmpltss)
		#ifdef cmpltss
			#undef cmpltss
		#endif
		#define cmpltss __builtin_ia32_cmpltss
	#endif

	#if HAS(__builtin_ia32_cmpneqps)
		#ifdef cmpneqps
			#undef cmpneqps
		#endif
		#define cmpneqps __builtin_ia32_cmpneqps
	#endif

	#if HAS(__builtin_ia32_cmpneqss)
		#ifdef cmpneqss
			#undef cmpneqss
		#endif
		#define cmpneqss __builtin_ia32_cmpneqss
	#endif

	#if HAS(__builtin_ia32_cmpngeps)
		#ifdef cmpngeps
			#undef cmpngeps
		#endif
		#define cmpngeps __builtin_ia32_cmpngeps
	#endif

	#if HAS(__builtin_ia32_cmpngtps)
		#ifdef cmpngtps
			#undef cmpngtps
		#endif
		#define cmpngtps __builtin_ia32_cmpngtps
	#endif

	#if HAS(__builtin_ia32_cmpnleps)
		#ifdef cmpnleps
			#undef cmpnleps
		#endif
		#define cmpnleps __builtin_ia32_cmpnleps
	#endif

	#if HAS(__builtin_ia32_cmpnless)
		#ifdef cmpnless
			#undef cmpnless
		#endif
		#define cmpnless __builtin_ia32_cmpnless
	#endif

	#if HAS(__builtin_ia32_cmpnltps)
		#ifdef cmpnltps
			#undef cmpnltps
		#endif
		#define cmpnltps __builtin_ia32_cmpnltps
	#endif

	#if HAS(__builtin_ia32_cmpnltss)
		#ifdef cmpnltss
			#undef cmpnltss
		#endif
		#define cmpnltss __builtin_ia32_cmpnltss
	#endif

	#if HAS(__builtin_ia32_cmpordps)
		#ifdef cmpordps
			#undef cmpordps
		#endif
		#define cmpordps __builtin_ia32_cmpordps
	#endif

	#if HAS(__builtin_ia32_cmpordss)
		#ifdef cmpordss
			#undef cmpordss
		#endif
		#define cmpordss __builtin_ia32_cmpordss
	#endif

	#if HAS(__builtin_ia32_cmpunordps)
		#ifdef cmpunordps
			#undef cmpunordps
		#endif
		#define cmpunordps __builtin_ia32_cmpunordps
	#endif

	#if HAS(__builtin_ia32_cmpunordss)
		#ifdef cmpunordss
			#undef cmpunordss
		#endif
		#define cmpunordss __builtin_ia32_cmpunordss
	#endif

	#if HAS(__builtin_ia32_comieq)
		#ifdef comieq
			#undef comieq
		#endif
		#define comieq __builtin_ia32_comieq
	#endif

	#if HAS(__builtin_ia32_comige)
		#ifdef comige
			#undef comige
		#endif
		#define comige __builtin_ia32_comige
	#endif

	#if HAS(__builtin_ia32_comigt)
		#ifdef comigt
			#undef comigt
		#endif
		#define comigt __builtin_ia32_comigt
	#endif

	#if HAS(__builtin_ia32_comile)
		#ifdef comile
			#undef comile
		#endif
		#define comile __builtin_ia32_comile
	#endif

	#if HAS(__builtin_ia32_comilt)
		#ifdef comilt
			#undef comilt
		#endif
		#define comilt __builtin_ia32_comilt
	#endif

	#if HAS(__builtin_ia32_comineq)
		#ifdef comineq
			#undef comineq
		#endif
		#define comineq __builtin_ia32_comineq
	#endif

	#if HAS(__builtin_ia32_cvtpi2ps)
		#ifdef cvtpi2ps
			#undef cvtpi2ps
		#endif
		#define cvtpi2ps __builtin_ia32_cvtpi2ps
	#endif

	#if HAS(__builtin_ia32_cvtps2pi)
		#ifdef cvtps2pi
			#undef cvtps2pi
		#endif
		#define cvtps2pi __builtin_ia32_cvtps2pi
	#endif

	#if HAS(__builtin_ia32_cvtsi2ss)
		#ifdef cvtsi2ss
			#undef cvtsi2ss
		#endif
		#define cvtsi2ss __builtin_ia32_cvtsi2ss
	#endif

	#if HAS(__builtin_ia32_cvtss2si)
		#ifdef cvtss2si
			#undef cvtss2si
		#endif
		#define cvtss2si __builtin_ia32_cvtss2si
	#endif

	#if HAS(__builtin_ia32_cvttps2pi)
		#ifdef cvttps2pi
			#undef cvttps2pi
		#endif
		#define cvttps2pi __builtin_ia32_cvttps2pi
	#endif

	#if HAS(__builtin_ia32_cvttss2si)
		#ifdef cvttss2si
			#undef cvttss2si
		#endif
		#define cvttss2si __builtin_ia32_cvttss2si
	#endif

	#if HAS(__builtin_ia32_divps)
		#ifdef divps
			#undef divps
		#endif
		#define divps __builtin_ia32_divps
	#endif

	#if HAS(__builtin_ia32_divss)
		#ifdef divss
			#undef divss
		#endif
		#define divss __builtin_ia32_divss
	#endif

	#if HAS(__builtin_ia32_loadhps)
		#ifdef loadhps
			#undef loadhps
		#endif
		#define loadhps __builtin_ia32_loadhps
	#endif

	#if HAS(__builtin_ia32_loadlps)
		#ifdef loadlps
			#undef loadlps
		#endif
		#define loadlps __builtin_ia32_loadlps
	#endif

	#if HAS(__builtin_ia32_loadss)
		#ifdef loadss
			#undef loadss
		#endif
		#define loadss __builtin_ia32_loadss
	#endif

	#if HAS(__builtin_ia32_loadups)
		#ifdef loadups
			#undef loadups
		#endif
		#define loadups __builtin_ia32_loadups
	#endif

	#if HAS(__builtin_ia32_maxps)
		#ifdef maxps
			#undef maxps
		#endif
		#define maxps __builtin_ia32_maxps
	#endif

	#if HAS(__builtin_ia32_maxss)
		#ifdef maxss
			#undef maxss
		#endif
		#define maxss __builtin_ia32_maxss
	#endif

	#if HAS(__builtin_ia32_minps)
		#ifdef minps
			#undef minps
		#endif
		#define minps __builtin_ia32_minps
	#endif

	#if HAS(__builtin_ia32_minss)
		#ifdef minss
			#undef minss
		#endif
		#define minss __builtin_ia32_minss
	#endif

	#if HAS(__builtin_ia32_movhlps)
		#ifdef movhlps
			#undef movhlps
		#endif
		#define movhlps __builtin_ia32_movhlps
	#endif

	#if HAS(__builtin_ia32_movlhps)
		#ifdef movlhps
			#undef movlhps
		#endif
		#define movlhps __builtin_ia32_movlhps
	#endif

	#if HAS(__builtin_ia32_movmskps)
		#ifdef movmskps
			#undef movmskps
		#endif
		#define movmskps __builtin_ia32_movmskps
	#endif

	#if HAS(__builtin_ia32_movntps)
		#ifdef movntps
			#undef movntps
		#endif
		#define movntps __builtin_ia32_movntps
	#endif

	#if HAS(__builtin_ia32_movss)
		#ifdef movss
			#undef movss
		#endif
		#define movss __builtin_ia32_movss
	#endif

	#if HAS(__builtin_ia32_mulps)
		#ifdef mulps
			#undef mulps
		#endif
		#define mulps __builtin_ia32_mulps
	#endif

	#if HAS(__builtin_ia32_mulss)
		#ifdef mulss
			#undef mulss
		#endif
		#define mulss __builtin_ia32_mulss
	#endif

	#if HAS(__builtin_ia32_orps)
		#ifdef orps
			#undef orps
		#endif
		#define orps __builtin_ia32_orps
	#endif

	#if HAS(__builtin_ia32_rcpps)
		#ifdef rcpps
			#undef rcpps
		#endif
		#define rcpps __builtin_ia32_rcpps
	#endif

	#if HAS(__builtin_ia32_rcpss)
		#ifdef rcpss
			#undef rcpss
		#endif
		#define rcpss __builtin_ia32_rcpss
	#endif

	#if HAS(__builtin_ia32_rsqrtps)
		#ifdef rsqrtps
			#undef rsqrtps
		#endif
		#define rsqrtps __builtin_ia32_rsqrtps
	#endif

	#if HAS(__builtin_ia32_rsqrtss)
		#ifdef rsqrtss
			#undef rsqrtss
		#endif
		#define rsqrtss __builtin_ia32_rsqrtss
	#endif

	#if HAS(__builtin_ia32_shufps)
		#ifdef shufps
			#undef shufps
		#endif
		#define shufps __builtin_ia32_shufps
	#endif

	#if HAS(__builtin_ia32_sqrtps)
		#ifdef sqrtps
			#undef sqrtps
		#endif
		#define sqrtps __builtin_ia32_sqrtps
	#endif

	#if HAS(__builtin_ia32_sqrtss)
		#ifdef sqrtss
			#undef sqrtss
		#endif
		#define sqrtss __builtin_ia32_sqrtss
	#endif

	#if HAS(__builtin_ia32_storehps)
		#ifdef storehps
			#undef storehps
		#endif
		#define storehps __builtin_ia32_storehps
	#endif

	#if HAS(__builtin_ia32_storelps)
		#ifdef storelps
			#undef storelps
		#endif
		#define storelps __builtin_ia32_storelps
	#endif

	#if HAS(__builtin_ia32_storeups)
		#ifdef storeups
			#undef storeups
		#endif
		#define storeups __builtin_ia32_storeups
	#endif

	#if HAS(__builtin_ia32_subps)
		#ifdef subps
			#undef subps
		#endif
		#define subps __builtin_ia32_subps
	#endif

	#if HAS(__builtin_ia32_subss)
		#ifdef subss
			#undef subss
		#endif
		#define subss __builtin_ia32_subss
	#endif

	#if HAS(__builtin_ia32_ucomieq)
		#ifdef ucomieq
			#undef ucomieq
		#endif
		#define ucomieq __builtin_ia32_ucomieq
	#endif

	#if HAS(__builtin_ia32_ucomige)
		#ifdef ucomige
			#undef ucomige
		#endif
		#define ucomige __builtin_ia32_ucomige
	#endif

	#if HAS(__builtin_ia32_ucomigt)
		#ifdef ucomigt
			#undef ucomigt
		#endif
		#define ucomigt __builtin_ia32_ucomigt
	#endif

	#if HAS(__builtin_ia32_ucomile)
		#ifdef ucomile
			#undef ucomile
		#endif
		#define ucomile __builtin_ia32_ucomile
	#endif

	#if HAS(__builtin_ia32_ucomilt)
		#ifdef ucomilt
			#undef ucomilt
		#endif
		#define ucomilt __builtin_ia32_ucomilt
	#endif

	#if HAS(__builtin_ia32_ucomineq)
		#ifdef ucomineq
			#undef ucomineq
		#endif
		#define ucomineq __builtin_ia32_ucomineq
	#endif

	#if HAS(__builtin_ia32_unpckhps)
		#ifdef unpckhps
			#undef unpckhps
		#endif
		#define unpckhps __builtin_ia32_unpckhps
	#endif

	#if HAS(__builtin_ia32_unpcklps)
		#ifdef unpcklps
			#undef unpcklps
		#endif
		#define unpcklps __builtin_ia32_unpcklps
	#endif

	#if HAS(__builtin_ia32_xorps)
		#ifdef xorps
			#undef xorps
		#endif
		#define xorps __builtin_ia32_xorps
	#endif

#endif

#if defined __SSE2__

	#if HAS(__builtin_ia32_addpd)
		#ifdef addpd
			#undef addpd
		#endif
		#define addpd __builtin_ia32_addpd
	#endif

	#if HAS(__builtin_ia32_addsd)
		#ifdef addsd
			#undef addsd
		#endif
		#define addsd __builtin_ia32_addsd
	#endif

	#if HAS(__builtin_ia32_andnpd)
		#ifdef andnpd
			#undef andnpd
		#endif
		#define andnpd __builtin_ia32_andnpd
	#endif

	#if HAS(__builtin_ia32_andpd)
		#ifdef andpd
			#undef andpd
		#endif
		#define andpd __builtin_ia32_andpd
	#endif

	#if HAS(__builtin_ia32_clflush)
		#ifdef clflush
			#undef clflush
		#endif
		#define clflush __builtin_ia32_clflush
	#endif

	#if HAS(__builtin_ia32_cmpeqpd)
		#ifdef cmpeqpd
			#undef cmpeqpd
		#endif
		#define cmpeqpd __builtin_ia32_cmpeqpd
	#endif

	#if HAS(__builtin_ia32_cmpeqsd)
		#ifdef cmpeqsd
			#undef cmpeqsd
		#endif
		#define cmpeqsd __builtin_ia32_cmpeqsd
	#endif

	#if HAS(__builtin_ia32_cmpgepd)
		#ifdef cmpgepd
			#undef cmpgepd
		#endif
		#define cmpgepd __builtin_ia32_cmpgepd
	#endif

	#if HAS(__builtin_ia32_cmpgtpd)
		#ifdef cmpgtpd
			#undef cmpgtpd
		#endif
		#define cmpgtpd __builtin_ia32_cmpgtpd
	#endif

	#if HAS(__builtin_ia32_cmplepd)
		#ifdef cmplepd
			#undef cmplepd
		#endif
		#define cmplepd __builtin_ia32_cmplepd
	#endif

	#if HAS(__builtin_ia32_cmplesd)
		#ifdef cmplesd
			#undef cmplesd
		#endif
		#define cmplesd __builtin_ia32_cmplesd
	#endif

	#if HAS(__builtin_ia32_cmpltpd)
		#ifdef cmpltpd
			#undef cmpltpd
		#endif
		#define cmpltpd __builtin_ia32_cmpltpd
	#endif

	#if HAS(__builtin_ia32_cmpltsd)
		#ifdef cmpltsd
			#undef cmpltsd
		#endif
		#define cmpltsd __builtin_ia32_cmpltsd
	#endif

	#if HAS(__builtin_ia32_cmpneqpd)
		#ifdef cmpneqpd
			#undef cmpneqpd
		#endif
		#define cmpneqpd __builtin_ia32_cmpneqpd
	#endif

	#if HAS(__builtin_ia32_cmpneqsd)
		#ifdef cmpneqsd
			#undef cmpneqsd
		#endif
		#define cmpneqsd __builtin_ia32_cmpneqsd
	#endif

	#if HAS(__builtin_ia32_cmpngepd)
		#ifdef cmpngepd
			#undef cmpngepd
		#endif
		#define cmpngepd __builtin_ia32_cmpngepd
	#endif

	#if HAS(__builtin_ia32_cmpngtpd)
		#ifdef cmpngtpd
			#undef cmpngtpd
		#endif
		#define cmpngtpd __builtin_ia32_cmpngtpd
	#endif

	#if HAS(__builtin_ia32_cmpnlepd)
		#ifdef cmpnlepd
			#undef cmpnlepd
		#endif
		#define cmpnlepd __builtin_ia32_cmpnlepd
	#endif

	#if HAS(__builtin_ia32_cmpnlesd)
		#ifdef cmpnlesd
			#undef cmpnlesd
		#endif
		#define cmpnlesd __builtin_ia32_cmpnlesd
	#endif

	#if HAS(__builtin_ia32_cmpnltpd)
		#ifdef cmpnltpd
			#undef cmpnltpd
		#endif
		#define cmpnltpd __builtin_ia32_cmpnltpd
	#endif

	#if HAS(__builtin_ia32_cmpnltsd)
		#ifdef cmpnltsd
			#undef cmpnltsd
		#endif
		#define cmpnltsd __builtin_ia32_cmpnltsd
	#endif

	#if HAS(__builtin_ia32_cmpordpd)
		#ifdef cmpordpd
			#undef cmpordpd
		#endif
		#define cmpordpd __builtin_ia32_cmpordpd
	#endif

	#if HAS(__builtin_ia32_cmpordsd)
		#ifdef cmpordsd
			#undef cmpordsd
		#endif
		#define cmpordsd __builtin_ia32_cmpordsd
	#endif

	#if HAS(__builtin_ia32_cmpunordpd)
		#ifdef cmpunordpd
			#undef cmpunordpd
		#endif
		#define cmpunordpd __builtin_ia32_cmpunordpd
	#endif

	#if HAS(__builtin_ia32_cmpunordsd)
		#ifdef cmpunordsd
			#undef cmpunordsd
		#endif
		#define cmpunordsd __builtin_ia32_cmpunordsd
	#endif

	#if HAS(__builtin_ia32_comisdeq)
		#ifdef comisdeq
			#undef comisdeq
		#endif
		#define comisdeq __builtin_ia32_comisdeq
	#endif

	#if HAS(__builtin_ia32_comisdge)
		#ifdef comisdge
			#undef comisdge
		#endif
		#define comisdge __builtin_ia32_comisdge
	#endif

	#if HAS(__builtin_ia32_comisdgt)
		#ifdef comisdgt
			#undef comisdgt
		#endif
		#define comisdgt __builtin_ia32_comisdgt
	#endif

	#if HAS(__builtin_ia32_comisdle)
		#ifdef comisdle
			#undef comisdle
		#endif
		#define comisdle __builtin_ia32_comisdle
	#endif

	#if HAS(__builtin_ia32_comisdlt)
		#ifdef comisdlt
			#undef comisdlt
		#endif
		#define comisdlt __builtin_ia32_comisdlt
	#endif

	#if HAS(__builtin_ia32_comisdneq)
		#ifdef comisdneq
			#undef comisdneq
		#endif
		#define comisdneq __builtin_ia32_comisdneq
	#endif

	#if HAS(__builtin_ia32_cvtdq2pd)
		#ifdef cvtdq2pd
			#undef cvtdq2pd
		#endif
		#define cvtdq2pd __builtin_ia32_cvtdq2pd
	#endif

	#if HAS(__builtin_ia32_cvtdq2ps)
		#ifdef cvtdq2ps
			#undef cvtdq2ps
		#endif
		#define cvtdq2ps __builtin_ia32_cvtdq2ps
	#endif

	#if HAS(__builtin_ia32_cvtpd2dq)
		#ifdef cvtpd2dq
			#undef cvtpd2dq
		#endif
		#define cvtpd2dq __builtin_ia32_cvtpd2dq
	#endif

	#if HAS(__builtin_ia32_cvtpd2pi)
		#ifdef cvtpd2pi
			#undef cvtpd2pi
		#endif
		#define cvtpd2pi __builtin_ia32_cvtpd2pi
	#endif

	#if HAS(__builtin_ia32_cvtpd2ps)
		#ifdef cvtpd2ps
			#undef cvtpd2ps
		#endif
		#define cvtpd2ps __builtin_ia32_cvtpd2ps
	#endif

	#if HAS(__builtin_ia32_cvtpi2pd)
		#ifdef cvtpi2pd
			#undef cvtpi2pd
		#endif
		#define cvtpi2pd __builtin_ia32_cvtpi2pd
	#endif

	#if HAS(__builtin_ia32_cvtps2dq)
		#ifdef cvtps2dq
			#undef cvtps2dq
		#endif
		#define cvtps2dq __builtin_ia32_cvtps2dq
	#endif

	#if HAS(__builtin_ia32_cvtps2pd)
		#ifdef cvtps2pd
			#undef cvtps2pd
		#endif
		#define cvtps2pd __builtin_ia32_cvtps2pd
	#endif

	#if HAS(__builtin_ia32_cvtsd2si)
		#ifdef cvtsd2si
			#undef cvtsd2si
		#endif
		#define cvtsd2si __builtin_ia32_cvtsd2si
	#endif

	#if HAS(__builtin_ia32_cvtsd2ss)
		#ifdef cvtsd2ss
			#undef cvtsd2ss
		#endif
		#define cvtsd2ss __builtin_ia32_cvtsd2ss
	#endif

	#if HAS(__builtin_ia32_cvtsi2sd)
		#ifdef cvtsi2sd
			#undef cvtsi2sd
		#endif
		#define cvtsi2sd __builtin_ia32_cvtsi2sd
	#endif

	#if HAS(__builtin_ia32_cvtsi642sd)
		#ifdef cvtsi642sd
			#undef cvtsi642sd
		#endif
		#define cvtsi642sd __builtin_ia32_cvtsi642sd
	#endif

	#if HAS(__builtin_ia32_cvtss2sd)
		#ifdef cvtss2sd
			#undef cvtss2sd
		#endif
		#define cvtss2sd __builtin_ia32_cvtss2sd
	#endif

	#if HAS(__builtin_ia32_cvttpd2dq)
		#ifdef cvttpd2dq
			#undef cvttpd2dq
		#endif
		#define cvttpd2dq __builtin_ia32_cvttpd2dq
	#endif

	#if HAS(__builtin_ia32_cvttpd2pi)
		#ifdef cvttpd2pi
			#undef cvttpd2pi
		#endif
		#define cvttpd2pi __builtin_ia32_cvttpd2pi
	#endif

	#if HAS(__builtin_ia32_cvttps2dq)
		#ifdef cvttps2dq
			#undef cvttps2dq
		#endif
		#define cvttps2dq __builtin_ia32_cvttps2dq
	#endif

	#if HAS(__builtin_ia32_cvttsd2si)
		#ifdef cvttsd2si
			#undef cvttsd2si
		#endif
		#define cvttsd2si __builtin_ia32_cvttsd2si
	#endif

	#if HAS(__builtin_ia32_divpd)
		#ifdef divpd
			#undef divpd
		#endif
		#define divpd __builtin_ia32_divpd
	#endif

	#if HAS(__builtin_ia32_divsd)
		#ifdef divsd
			#undef divsd
		#endif
		#define divsd __builtin_ia32_divsd
	#endif

	#if HAS(__builtin_ia32_lfence)
		#ifdef lfence
			#undef lfence
		#endif
		#define lfence __builtin_ia32_lfence
	#endif

	#if HAS(__builtin_ia32_loaddqu)
		#ifdef loaddqu
			#undef loaddqu
		#endif
		#define loaddqu __builtin_ia32_loaddqu
	#endif

	#if HAS(__builtin_ia32_loadhpd)
		#ifdef loadhpd
			#undef loadhpd
		#endif
		#define loadhpd __builtin_ia32_loadhpd
	#endif

	#if HAS(__builtin_ia32_loadlpd)
		#ifdef loadlpd
			#undef loadlpd
		#endif
		#define loadlpd __builtin_ia32_loadlpd
	#endif

	#if HAS(__builtin_ia32_loadupd)
		#ifdef loadupd
			#undef loadupd
		#endif
		#define loadupd __builtin_ia32_loadupd
	#endif

	#if HAS(__builtin_ia32_cvtsd2si64)
		#ifdef cvtsd2si64
			#undef cvtsd2si64
		#endif
		#define cvtsd2si64 __builtin_ia32_cvtsd2si64
	#endif

	#if HAS(__builtin_ia32_cvttsd2si64)
		#ifdef cvttsd2si64
			#undef cvttsd2si64
		#endif
		#define cvttsd2si64 __builtin_ia32_cvttsd2si64
	#endif

	#if HAS(__builtin_ia32_maskmovdqu)
		#ifdef maskmovdqu
			#undef maskmovdqu
		#endif
		#define maskmovdqu __builtin_ia32_maskmovdqu
	#endif

	#if HAS(__builtin_ia32_maxpd)
		#ifdef maxpd
			#undef maxpd
		#endif
		#define maxpd __builtin_ia32_maxpd
	#endif

	#if HAS(__builtin_ia32_maxsd)
		#ifdef maxsd
			#undef maxsd
		#endif
		#define maxsd __builtin_ia32_maxsd
	#endif

	#if HAS(__builtin_ia32_mfence)
		#ifdef mfence
			#undef mfence
		#endif
		#define mfence __builtin_ia32_mfence
	#endif

	#if HAS(__builtin_ia32_minpd)
		#ifdef minpd
			#undef minpd
		#endif
		#define minpd __builtin_ia32_minpd
	#endif

	#if HAS(__builtin_ia32_minsd)
		#ifdef minsd
			#undef minsd
		#endif
		#define minsd __builtin_ia32_minsd
	#endif

	#if HAS(__builtin_ia32_movmskpd)
		#ifdef movmskpd
			#undef movmskpd
		#endif
		#define movmskpd __builtin_ia32_movmskpd
	#endif

	#if HAS(__builtin_ia32_movntdq)
		#ifdef movntdq
			#undef movntdq
		#endif
		#define movntdq __builtin_ia32_movntdq
	#endif

	#if HAS(__builtin_ia32_movnti)
		#ifdef movnti
			#undef movnti
		#endif
		#define movnti __builtin_ia32_movnti
	#endif

	#if HAS(__builtin_ia32_movnti64)
		#ifdef movnti64
			#undef movnti64
		#endif
		#define movnti64 __builtin_ia32_movnti64
	#endif

	#if HAS(__builtin_ia32_movntpd)
		#ifdef movntpd
			#undef movntpd
		#endif
		#define movntpd __builtin_ia32_movntpd
	#endif

	#if HAS(__builtin_ia32_movq128)
		#ifdef movq128
			#undef movq128
		#endif
		#define movq128 __builtin_ia32_movq128
	#endif

	#if HAS(__builtin_ia32_movsd)
		#ifdef movsd
			#undef movsd
		#endif
		#define movsd __builtin_ia32_movsd
	#endif

	#if HAS(__builtin_ia32_mulpd)
		#ifdef mulpd
			#undef mulpd
		#endif
		#define mulpd __builtin_ia32_mulpd
	#endif

	#if HAS(__builtin_ia32_mulsd)
		#ifdef mulsd
			#undef mulsd
		#endif
		#define mulsd __builtin_ia32_mulsd
	#endif

	#if HAS(__builtin_ia32_orpd)
		#ifdef orpd
			#undef orpd
		#endif
		#define orpd __builtin_ia32_orpd
	#endif

	#if HAS(__builtin_ia32_packssdw128)
		#ifdef packssdw128
			#undef packssdw128
		#endif
		#define packssdw128 __builtin_ia32_packssdw128
	#endif

	#if HAS(__builtin_ia32_packsswb128)
		#ifdef packsswb128
			#undef packsswb128
		#endif
		#define packsswb128 __builtin_ia32_packsswb128
	#endif

	#if HAS(__builtin_ia32_packuswb128)
		#ifdef packuswb128
			#undef packuswb128
		#endif
		#define packuswb128 __builtin_ia32_packuswb128
	#endif

	#if HAS(__builtin_ia32_paddb128)
		#ifdef paddb128
			#undef paddb128
		#endif
		#define paddb128 __builtin_ia32_paddb128
	#endif

	#if HAS(__builtin_ia32_paddd128)
		#ifdef paddd128
			#undef paddd128
		#endif
		#define paddd128 __builtin_ia32_paddd128
	#endif

	#if HAS(__builtin_ia32_paddq)
		#define paddq __builtin_ia32_paddq
		#define _mm_add_si64(i,j) ((__m64)paddq((__v1di)i,(__v1di)(j)))
	#endif

	#if HAS(__builtin_ia32_paddq128)
		#ifdef paddq128
			#undef paddq128
		#endif
		#define paddq128 __builtin_ia32_paddq128
	#endif

	#if HAS(__builtin_ia32_paddw128)
		#ifdef paddw128
			#undef paddw128
		#endif
		#define paddw128 __builtin_ia32_paddw128
	#endif

	#if HAS(__builtin_ia32_pand128)
		#ifdef pand128
			#undef pand128
		#endif
		#define pand128 __builtin_ia32_pand128
	#endif

	#if HAS(__builtin_ia32_pandn128)
		#ifdef pandn128
			#undef pandn128
		#endif
		#define pandn128 __builtin_ia32_pandn128
	#endif

	#if HAS(__builtin_ia32_pavgb128)
		#ifdef pavgb128
			#undef pavgb128
		#endif
		#define pavgb128 __builtin_ia32_pavgb128
	#endif

	#if HAS(__builtin_ia32_pavgw128)
		#ifdef pavgw128
			#undef pavgw128
		#endif
		#define pavgw128 __builtin_ia32_pavgw128
	#endif

	#if HAS(__builtin_ia32_pcmpeqb128)
		#ifdef pcmpeqb128
			#undef pcmpeqb128
		#endif
		#define pcmpeqb128 __builtin_ia32_pcmpeqb128
	#endif

	#if HAS(__builtin_ia32_pcmpeqd128)
		#ifdef pcmpeqd128
			#undef pcmpeqd128
		#endif
		#define pcmpeqd128 __builtin_ia32_pcmpeqd128
	#endif

	#if HAS(__builtin_ia32_pcmpeqw128)
		#ifdef pcmpeqw128
			#undef pcmpeqw128
		#endif
		#define pcmpeqw128 __builtin_ia32_pcmpeqw128
	#endif

	#if HAS(__builtin_ia32_pcmpgtb128)
		#ifdef pcmpgtb128
			#undef pcmpgtb128
		#endif
		#define pcmpgtb128 __builtin_ia32_pcmpgtb128
	#endif

	#if HAS(__builtin_ia32_pcmpgtd128)
		#ifdef pcmpgtd128
			#undef pcmpgtd128
		#endif
		#define pcmpgtd128 __builtin_ia32_pcmpgtd128
	#endif

	#if HAS(__builtin_ia32_pcmpgtw128)
		#ifdef pcmpgtw128
			#undef pcmpgtw128
		#endif
		#define pcmpgtw128 __builtin_ia32_pcmpgtw128
	#endif

	#if HAS(__builtin_ia32_pmaddwd128)
		#ifdef pmaddwd128
			#undef pmaddwd128
		#endif
		#define pmaddwd128 __builtin_ia32_pmaddwd128
	#endif

	#if HAS(__builtin_ia32_pmaxsw128)
		#ifdef pmaxsw128
			#undef pmaxsw128
		#endif
		#define pmaxsw128 __builtin_ia32_pmaxsw128
	#endif

	#if HAS(__builtin_ia32_pmaxub128)
		#ifdef pmaxub128
			#undef pmaxub128
		#endif
		#define pmaxub128 __builtin_ia32_pmaxub128
	#endif

	#if HAS(__builtin_ia32_pminsw128)
		#ifdef pminsw128
			#undef pminsw128
		#endif
		#define pminsw128 __builtin_ia32_pminsw128
	#endif

	#if HAS(__builtin_ia32_pminub128)
		#ifdef pminub128
			#undef pminub128
		#endif
		#define pminub128 __builtin_ia32_pminub128
	#endif

	#if HAS(__builtin_ia32_pmovmskb128)
		#ifdef pmovmskb128
			#undef pmovmskb128
		#endif
		#define pmovmskb128 __builtin_ia32_pmovmskb128
	#endif

	#if HAS(__builtin_ia32_pmulhuw128)
		#ifdef pmulhuw128
			#undef pmulhuw128
		#endif
		#define pmulhuw128 __builtin_ia32_pmulhuw128
	#endif

	#if HAS(__builtin_ia32_pmulhw128)
		#ifdef pmulhw128
			#undef pmulhw128
		#endif
		#define pmulhw128 __builtin_ia32_pmulhw128
	#endif

	#if HAS(__builtin_ia32_pmullw128)
		#ifdef pmullw128
			#undef pmullw128
		#endif
		#define pmullw128 __builtin_ia32_pmullw128
	#endif

	#if HAS(__builtin_ia32_pmuludq)
		#ifdef pmuludq
			#undef pmuludq
		#endif
		#define pmuludq __builtin_ia32_pmuludq
	#endif

	#if HAS(__builtin_ia32_pmuludq128)
		#ifdef pmuludq128
			#undef pmuludq128
		#endif
		#define pmuludq128 __builtin_ia32_pmuludq128
	#endif

	#if HAS(__builtin_ia32_por128)
		#ifdef por128
			#undef por128
		#endif
		#define por128 __builtin_ia32_por128
	#endif

	#if HAS(__builtin_ia32_psadbw128)
		#ifdef psadbw128
			#undef psadbw128
		#endif
		#define psadbw128 __builtin_ia32_psadbw128
	#endif

	#if HAS(__builtin_ia32_pshufd)
		#ifdef pshufd
			#undef pshufd
		#endif
		#define pshufd __builtin_ia32_pshufd
	#endif

	#if HAS(__builtin_ia32_pshufhw)
		#ifdef pshufhw
			#undef pshufhw
		#endif
		#define pshufhw __builtin_ia32_pshufhw
	#endif

	#if HAS(__builtin_ia32_pshuflw)
		#ifdef pshuflw
			#undef pshuflw
		#endif
		#define pshuflw __builtin_ia32_pshuflw
	#endif

	#if HAS(__builtin_ia32_pslld128)
		#ifdef pslld128
			#undef pslld128
		#endif
		#define pslld128 __builtin_ia32_pslld128
	#endif

	#if HAS(__builtin_ia32_pslldi128)
		#ifdef pslldi128
			#undef pslldi128
		#endif
		#define pslldi128 __builtin_ia32_pslldi128
	#endif

	#if HAS(__builtin_ia32_pslldqi128)
		#ifdef pslldqi128
			#undef pslldqi128
		#endif
		#define pslldqi128 __builtin_ia32_pslldqi128
	#endif

	#if HAS(__builtin_ia32_psllq128)
		#ifdef psllq128
			#undef psllq128
		#endif
		#define psllq128 __builtin_ia32_psllq128
	#endif

	#if HAS(__builtin_ia32_psllqi128)
		#ifdef psllqi128
			#undef psllqi128
		#endif
		#define psllqi128 __builtin_ia32_psllqi128
	#endif

	#if HAS(__builtin_ia32_psllw128)
		#ifdef psllw128
			#undef psllw128
		#endif
		#define psllw128 __builtin_ia32_psllw128
	#endif

	#if HAS(__builtin_ia32_psllwi128)
		#ifdef psllwi128
			#undef psllwi128
		#endif
		#define psllwi128 __builtin_ia32_psllwi128
	#endif

	#if HAS(__builtin_ia32_psrad128)
		#ifdef psrad128
			#undef psrad128
		#endif
		#define psrad128 __builtin_ia32_psrad128
	#endif

	#if HAS(__builtin_ia32_psradi128)
		#ifdef psradi128
			#undef psradi128
		#endif
		#define psradi128 __builtin_ia32_psradi128
	#endif

	#if HAS(__builtin_ia32_psraw128)
		#ifdef psraw128
			#undef psraw128
		#endif
		#define psraw128 __builtin_ia32_psraw128
	#endif

	#if HAS(__builtin_ia32_psrawi128)
		#ifdef psrawi128
			#undef psrawi128
		#endif
		#define psrawi128 __builtin_ia32_psrawi128
	#endif

	#if HAS(__builtin_ia32_psrld128)
		#ifdef psrld128
			#undef psrld128
		#endif
		#define psrld128 __builtin_ia32_psrld128
	#endif

	#if HAS(__builtin_ia32_psrldi128)
		#ifdef psrldi128
			#undef psrldi128
		#endif
		#define psrldi128 __builtin_ia32_psrldi128
	#endif

	#if HAS(__builtin_ia32_psrldqi128)
		#ifdef psrldqi128
			#undef psrldqi128
		#endif
		#define psrldqi128 __builtin_ia32_psrldqi128
	#endif

	#if HAS(__builtin_ia32_psrlq128)
		#ifdef psrlq128
			#undef psrlq128
		#endif
		#define psrlq128 __builtin_ia32_psrlq128
	#endif

	#if HAS(__builtin_ia32_psrlqi128)
		#ifdef psrlqi128
			#undef psrlqi128
		#endif
		#define psrlqi128 __builtin_ia32_psrlqi128
	#endif

	#if HAS(__builtin_ia32_psrlw128)
		#ifdef psrlw128
			#undef psrlw128
		#endif
		#define psrlw128 __builtin_ia32_psrlw128
	#endif

	#if HAS(__builtin_ia32_psrlwi128)
		#ifdef psrlwi128
			#undef psrlwi128
		#endif
		#define psrlwi128 __builtin_ia32_psrlwi128
	#endif

	#if HAS(__builtin_ia32_psubb128)
		#ifdef psubb128
			#undef psubb128
		#endif
		#define psubb128 __builtin_ia32_psubb128
	#endif

	#if HAS(__builtin_ia32_psubd128)
		#ifdef psubd128
			#undef psubd128
		#endif
		#define psubd128 __builtin_ia32_psubd128
	#endif

	#if HAS(__builtin_ia32_psubq)
		#define psubq __builtin_ia32_psubq
		#define _mm_sub_si64(i,j) ((__m64)psubq((__v1di)(i),(__v1di)(j)))
	#endif

	#if HAS(__builtin_ia32_psubq128)
		#ifdef psubq128
			#undef psubq128
		#endif
		#define psubq128 __builtin_ia32_psubq128
	#endif

	#if HAS(__builtin_ia32_psubw128)
		#ifdef psubw128
			#undef psubw128
		#endif
		#define psubw128 __builtin_ia32_psubw128
	#endif

	#if HAS(__builtin_ia32_punpckhbw128)
		#ifdef punpckhbw128
			#undef punpckhbw128
		#endif
		#define punpckhbw128 __builtin_ia32_punpckhbw128
	#endif

	#if HAS(__builtin_ia32_punpckhdq128)
		#ifdef punpckhdq128
			#undef punpckhdq128
		#endif
		#define punpckhdq128 __builtin_ia32_punpckhdq128
	#endif

	#if HAS(__builtin_ia32_punpckhqdq128)
		#ifdef punpckhqdq128
			#undef punpckhqdq128
		#endif
		#define punpckhqdq128 __builtin_ia32_punpckhqdq128
	#endif

	#if HAS(__builtin_ia32_punpckhwd128)
		#ifdef punpckhwd128
			#undef punpckhwd128
		#endif
		#define punpckhwd128 __builtin_ia32_punpckhwd128
	#endif

	#if HAS(__builtin_ia32_punpcklbw128)
		#ifdef punpcklbw128
			#undef punpcklbw128
		#endif
		#define punpcklbw128 __builtin_ia32_punpcklbw128
	#endif

	#if HAS(__builtin_ia32_punpckldq128)
		#ifdef punpckldq128
			#undef punpckldq128
		#endif
		#define punpckldq128 __builtin_ia32_punpckldq128
	#endif

	#if HAS(__builtin_ia32_punpcklqdq128)
		#ifdef punpcklqdq128
			#undef punpcklqdq128
		#endif
		#define punpcklqdq128 __builtin_ia32_punpcklqdq128
	#endif

	#if HAS(__builtin_ia32_punpcklwd128)
		#ifdef punpcklwd128
			#undef punpcklwd128
		#endif
		#define punpcklwd128 __builtin_ia32_punpcklwd128
	#endif

	#if HAS(__builtin_ia32_pxor128)
		#ifdef pxor128
			#undef pxor128
		#endif
		#define pxor128 __builtin_ia32_pxor128
	#endif

	#if HAS(__builtin_ia32_shufpd)
		#ifdef shufpd
			#undef shufpd
		#endif
		#define shufpd __builtin_ia32_shufpd
	#endif

	#if HAS(__builtin_ia32_sqrtpd)
		#ifdef sqrtpd
			#undef sqrtpd
		#endif
		#define sqrtpd __builtin_ia32_sqrtpd
	#endif

	#if HAS(__builtin_ia32_sqrtsd)
		#ifdef sqrtsd
			#undef sqrtsd
		#endif
		#define sqrtsd __builtin_ia32_sqrtsd
	#endif

	#if HAS(__builtin_ia32_storedqu)
		#ifdef storedqu
			#undef storedqu
		#endif
		#define storedqu __builtin_ia32_storedqu
	#endif

	#if HAS(__builtin_ia32_storeupd)
		#ifdef storeupd
			#undef storeupd
		#endif
		#define storeupd __builtin_ia32_storeupd
	#endif

	#if HAS(__builtin_ia32_subpd)
		#ifdef subpd
			#undef subpd
		#endif
		#define subpd __builtin_ia32_subpd
	#endif

	#if HAS(__builtin_ia32_subsd)
		#ifdef subsd
			#undef subsd
		#endif
		#define subsd __builtin_ia32_subsd
	#endif

	#if HAS(__builtin_ia32_ucomisdeq)
		#ifdef ucomisdeq
			#undef ucomisdeq
		#endif
		#define ucomisdeq __builtin_ia32_ucomisdeq
	#endif

	#if HAS(__builtin_ia32_ucomisdge)
		#ifdef ucomisdge
			#undef ucomisdge
		#endif
		#define ucomisdge __builtin_ia32_ucomisdge
	#endif

	#if HAS(__builtin_ia32_ucomisdgt)
		#ifdef ucomisdgt
			#undef ucomisdgt
		#endif
		#define ucomisdgt __builtin_ia32_ucomisdgt
	#endif

	#if HAS(__builtin_ia32_ucomisdle)
		#ifdef ucomisdle
			#undef ucomisdle
		#endif
		#define ucomisdle __builtin_ia32_ucomisdle
	#endif

	#if HAS(__builtin_ia32_ucomisdlt)
		#ifdef ucomisdlt
			#undef ucomisdlt
		#endif
		#define ucomisdlt __builtin_ia32_ucomisdlt
	#endif

	#if HAS(__builtin_ia32_ucomisdneq)
		#ifdef ucomisdneq
			#undef ucomisdneq
		#endif
		#define ucomisdneq __builtin_ia32_ucomisdneq
	#endif

	#if HAS(__builtin_ia32_unpckhpd)
		#ifdef unpckhpd
			#undef unpckhpd
		#endif
		#define unpckhpd __builtin_ia32_unpckhpd
	#endif

	#if HAS(__builtin_ia32_unpcklpd)
		#ifdef unpcklpd
			#undef unpcklpd
		#endif
		#define unpcklpd __builtin_ia32_unpcklpd
	#endif

	#if HAS(__builtin_ia32_xorpd)
		#ifdef xorpd
			#undef xorpd
		#endif
		#define xorpd __builtin_ia32_xorpd
	#endif

#endif

#if defined __x86_64__

	#if HAS(__builtin_ia32_bsrdi)
		#define bsrdi __builtin_ia32_bsrdi
		#define __bsrq bsrdi
	#endif

	#if HAS(__builtin_copysignq)
		#define copysignq __builtin_copysignq
	#endif

	#if HAS(__builtin_fabsq)
		#define fabsq __builtin_fabsq
	#endif

	#if HAS(__builtin_infq)
		#define infq __builtin_infq
	#endif

	#if HAS(__builtin_huge_valq)
		#define huge_valq __builtin_huge_valq
	#endif


#endif

#if defined __SSE3__

	#if HAS(__builtin_ia32_addsubpd)
		#ifdef addsubpd
			#undef addsubpd
		#endif
		#define addsubpd __builtin_ia32_addsubpd
	#endif

	#if HAS(__builtin_ia32_addsubps)
		#ifdef addsubps
			#undef addsubps
		#endif
		#define addsubps __builtin_ia32_addsubps
	#endif

	#if HAS(__builtin_ia32_haddpd)
		#ifdef haddpd
			#undef haddpd
		#endif
		#define haddpd __builtin_ia32_haddpd
	#endif

	#if HAS(__builtin_ia32_haddps)
		#ifdef haddps
			#undef haddps
		#endif
		#define haddps __builtin_ia32_haddps
	#endif

	#if HAS(__builtin_ia32_hsubpd)
		#ifdef hsubpd
			#undef hsubpd
		#endif
		#define hsubpd __builtin_ia32_hsubpd
	#endif

	#if HAS(__builtin_ia32_hsubps)
		#ifdef hsubps
			#undef hsubps
		#endif
		#define hsubps __builtin_ia32_hsubps
	#endif

	#if HAS(__builtin_ia32_lddqu)
		#ifdef lddqu
			#undef lddqu
		#endif
		#define lddqu __builtin_ia32_lddqu
	#endif

	#if HAS(__builtin_ia32_monitor)
		#ifdef monitor
			#undef monitor
		#endif
		#define monitor __builtin_ia32_monitor
	#endif

	#if HAS(__builtin_ia32_movshdup)
		#ifdef movshdup
			#undef movshdup
		#endif
		#define movshdup __builtin_ia32_movshdup
	#endif

	#if HAS(__builtin_ia32_movsldup)
		#ifdef movsldup
			#undef movsldup
		#endif
		#define movsldup __builtin_ia32_movsldup
	#endif

	#if HAS(__builtin_ia32_mwait)
		#ifdef mwait
			#undef mwait
		#endif
		#define mwait __builtin_ia32_mwait
	#endif

#endif

#if defined __SSSE3__

	#if HAS(__builtin_ia32_pabsb)
		#ifdef pabsb
			#undef pabsb
		#endif
		#define pabsb __builtin_ia32_pabsb
	#endif

	#if HAS(__builtin_ia32_pabsb128)
		#ifdef pabsb128
			#undef pabsb128
		#endif
		#define pabsb128 __builtin_ia32_pabsb128
	#endif

	#if HAS(__builtin_ia32_pabsd)
		#ifdef pabsd
			#undef pabsd
		#endif
		#define pabsd __builtin_ia32_pabsd
	#endif

	#if HAS(__builtin_ia32_pabsd128)
		#ifdef pabsd128
			#undef pabsd128
		#endif
		#define pabsd128 __builtin_ia32_pabsd128
	#endif

	#if HAS(__builtin_ia32_pabsw)
		#ifdef pabsw
			#undef pabsw
		#endif
		#define pabsw __builtin_ia32_pabsw
	#endif

	#if HAS(__builtin_ia32_pabsw128)
		#ifdef pabsw128
			#undef pabsw128
		#endif
		#define pabsw128 __builtin_ia32_pabsw128
	#endif

	#if HAS(__builtin_ia32_palignr)
		#ifdef palignr
			#undef palignr
		#endif
		#define palignr __builtin_ia32_palignr
	#endif

	#if HAS(__builtin_ia32_palignr128)
		#ifdef palignr128
			#undef palignr128
		#endif
		#define palignr128 __builtin_ia32_palignr128
	#endif

	#if HAS(__builtin_ia32_phaddd)
		#ifdef phaddd
			#undef phaddd
		#endif
		#define phaddd __builtin_ia32_phaddd
	#endif

	#if HAS(__builtin_ia32_phaddd128)
		#ifdef phaddd128
			#undef phaddd128
		#endif
		#define phaddd128 __builtin_ia32_phaddd128
	#endif

	#if HAS(__builtin_ia32_phaddsw)
		#ifdef phaddsw
			#undef phaddsw
		#endif
		#define phaddsw __builtin_ia32_phaddsw
	#endif

	#if HAS(__builtin_ia32_phaddsw128)
		#ifdef phaddsw128
			#undef phaddsw128
		#endif
		#define phaddsw128 __builtin_ia32_phaddsw128
	#endif

	#if HAS(__builtin_ia32_phaddw)
		#ifdef phaddw
			#undef phaddw
		#endif
		#define phaddw __builtin_ia32_phaddw
	#endif

	#if HAS(__builtin_ia32_phaddw128)
		#ifdef phaddw128
			#undef phaddw128
		#endif
		#define phaddw128 __builtin_ia32_phaddw128
	#endif

	#if HAS(__builtin_ia32_phsubd)
		#ifdef phsubd
			#undef phsubd
		#endif
		#define phsubd __builtin_ia32_phsubd
	#endif

	#if HAS(__builtin_ia32_phsubd128)
		#ifdef phsubd128
			#undef phsubd128
		#endif
		#define phsubd128 __builtin_ia32_phsubd128
	#endif

	#if HAS(__builtin_ia32_phsubsw)
		#ifdef phsubsw
			#undef phsubsw
		#endif
		#define phsubsw __builtin_ia32_phsubsw
	#endif

	#if HAS(__builtin_ia32_phsubsw128)
		#ifdef phsubsw128
			#undef phsubsw128
		#endif
		#define phsubsw128 __builtin_ia32_phsubsw128
	#endif

	#if HAS(__builtin_ia32_phsubw)
		#ifdef phsubw
			#undef phsubw
		#endif
		#define phsubw __builtin_ia32_phsubw
	#endif

	#if HAS(__builtin_ia32_phsubw128)
		#ifdef phsubw128
			#undef phsubw128
		#endif
		#define phsubw128 __builtin_ia32_phsubw128
	#endif

	#if HAS(__builtin_ia32_pmaddubsw)
		#ifdef pmaddubsw
			#undef pmaddubsw
		#endif
		#define pmaddubsw __builtin_ia32_pmaddubsw
	#endif

	#if HAS(__builtin_ia32_pmaddubsw128)
		#ifdef pmaddubsw128
			#undef pmaddubsw128
		#endif
		#define pmaddubsw128 __builtin_ia32_pmaddubsw128
	#endif

	#if HAS(__builtin_ia32_pmulhrsw)
		#ifdef pmulhrsw
			#undef pmulhrsw
		#endif
		#define pmulhrsw __builtin_ia32_pmulhrsw
	#endif

	#if HAS(__builtin_ia32_pmulhrsw128)
		#ifdef pmulhrsw128
			#undef pmulhrsw128
		#endif
		#define pmulhrsw128 __builtin_ia32_pmulhrsw128
	#endif

	#if HAS(__builtin_ia32_pshufb)
		#ifdef pshufb
			#undef pshufb
		#endif
		#define pshufb __builtin_ia32_pshufb
	#endif

	#if HAS(__builtin_ia32_pshufb128)
		#ifdef pshufb128
			#undef pshufb128
		#endif
		#define pshufb128 __builtin_ia32_pshufb128
	#endif

	#if HAS(__builtin_ia32_psignb)
		#ifdef psignb
			#undef psignb
		#endif
		#define psignb __builtin_ia32_psignb
	#endif

	#if HAS(__builtin_ia32_psignb128)
		#ifdef psignb128
			#undef psignb128
		#endif
		#define psignb128 __builtin_ia32_psignb128
	#endif

	#if HAS(__builtin_ia32_psignd)
		#ifdef psignd
			#undef psignd
		#endif
		#define psignd __builtin_ia32_psignd
	#endif

	#if HAS(__builtin_ia32_psignd128)
		#ifdef psignd128
			#undef psignd128
		#endif
		#define psignd128 __builtin_ia32_psignd128
	#endif

	#if HAS(__builtin_ia32_psignw)
		#ifdef psignw
			#undef psignw
		#endif
		#define psignw __builtin_ia32_psignw
	#endif

	#if HAS(__builtin_ia32_psignw128)
		#ifdef psignw128
			#undef psignw128
		#endif
		#define psignw128 __builtin_ia32_psignw128
	#endif

#endif

#if defined __SSE4_1__

	#if HAS(__builtin_ia32_blendpd)
		#ifdef blendpd
			#undef blendpd
		#endif
		#define blendpd __builtin_ia32_blendpd
	#endif

	#if HAS(__builtin_ia32_blendps)
		#ifdef blendps
			#undef blendps
		#endif
		#define blendps __builtin_ia32_blendps
	#endif

	#if HAS(__builtin_ia32_blendvpd)
		#ifdef blendvpd
			#undef blendvpd
		#endif
		#define blendvpd __builtin_ia32_blendvpd
	#endif

	#if HAS(__builtin_ia32_blendvps)
		#ifdef blendvps
			#undef blendvps
		#endif
		#define blendvps __builtin_ia32_blendvps
	#endif

	#if HAS(__builtin_ia32_dppd)
		#ifdef dppd
			#undef dppd
		#endif
		#define dppd __builtin_ia32_dppd
	#endif

	#if HAS(__builtin_ia32_dpps)
		#ifdef dpps
			#undef dpps
		#endif
		#define dpps __builtin_ia32_dpps
	#endif

	#if HAS(__builtin_ia32_insertps128)
		#ifdef insertps128
			#undef insertps128
		#endif
		#define insertps128 __builtin_ia32_insertps128
	#endif

	#if HAS(__builtin_ia32_movntdqa)
		#ifdef movntdqa
			#undef movntdqa
		#endif
		#define movntdqa __builtin_ia32_movntdqa
	#endif

	#if HAS(__builtin_ia32_mpsadbw128)
		#ifdef mpsadbw128
			#undef mpsadbw128
		#endif
		#define mpsadbw128 __builtin_ia32_mpsadbw128
	#endif

	#if HAS(__builtin_ia32_packusdw128)
		#ifdef packusdw128
			#undef packusdw128
		#endif
		#define packusdw128 __builtin_ia32_packusdw128
	#endif

	#if HAS(__builtin_ia32_pblendvb128)
		#ifdef pblendvb128
			#undef pblendvb128
		#endif
		#define pblendvb128 __builtin_ia32_pblendvb128
	#endif

	#if HAS(__builtin_ia32_pblendw128)
		#ifdef pblendw128
			#undef pblendw128
		#endif
		#define pblendw128 __builtin_ia32_pblendw128
	#endif

	#if HAS(__builtin_ia32_pcmpeqq)
		#ifdef pcmpeqq
			#undef pcmpeqq
		#endif
		#define pcmpeqq __builtin_ia32_pcmpeqq
	#endif

	#if HAS(__builtin_ia32_phminposuw128)
		#ifdef phminposuw128
			#undef phminposuw128
		#endif
		#define phminposuw128 __builtin_ia32_phminposuw128
	#endif

	#if HAS(__builtin_ia32_pmaxsb128)
		#ifdef pmaxsb128
			#undef pmaxsb128
		#endif
		#define pmaxsb128 __builtin_ia32_pmaxsb128
	#endif

	#if HAS(__builtin_ia32_pmaxsd128)
		#ifdef pmaxsd128
			#undef pmaxsd128
		#endif
		#define pmaxsd128 __builtin_ia32_pmaxsd128
	#endif

	#if HAS(__builtin_ia32_pmaxud128)
		#ifdef pmaxud128
			#undef pmaxud128
		#endif
		#define pmaxud128 __builtin_ia32_pmaxud128
	#endif

	#if HAS(__builtin_ia32_pmaxuw128)
		#ifdef pmaxuw128
			#undef pmaxuw128
		#endif
		#define pmaxuw128 __builtin_ia32_pmaxuw128
	#endif

	#if HAS(__builtin_ia32_pminsb128)
		#ifdef pminsb128
			#undef pminsb128
		#endif
		#define pminsb128 __builtin_ia32_pminsb128
	#endif

	#if HAS(__builtin_ia32_pminsd128)
		#ifdef pminsd128
			#undef pminsd128
		#endif
		#define pminsd128 __builtin_ia32_pminsd128
	#endif

	#if HAS(__builtin_ia32_pminud128)
		#ifdef pminud128
			#undef pminud128
		#endif
		#define pminud128 __builtin_ia32_pminud128
	#endif

	#if HAS(__builtin_ia32_pminuw128)
		#ifdef pminuw128
			#undef pminuw128
		#endif
		#define pminuw128 __builtin_ia32_pminuw128
	#endif

	#if HAS(__builtin_ia32_pmovsxbd128)
		#ifdef pmovsxbd128
			#undef pmovsxbd128
		#endif
		#define pmovsxbd128 __builtin_ia32_pmovsxbd128
	#endif

	#if HAS(__builtin_ia32_pmovsxbq128)
		#ifdef pmovsxbq128
			#undef pmovsxbq128
		#endif
		#define pmovsxbq128 __builtin_ia32_pmovsxbq128
	#endif

	#if HAS(__builtin_ia32_pmovsxbw128)
		#ifdef pmovsxbw128
			#undef pmovsxbw128
		#endif
		#define pmovsxbw128 __builtin_ia32_pmovsxbw128
	#endif

	#if HAS(__builtin_ia32_pmovsxdq128)
		#ifdef pmovsxdq128
			#undef pmovsxdq128
		#endif
		#define pmovsxdq128 __builtin_ia32_pmovsxdq128
	#endif

	#if HAS(__builtin_ia32_pmovsxwd128)
		#ifdef pmovsxwd128
			#undef pmovsxwd128
		#endif
		#define pmovsxwd128 __builtin_ia32_pmovsxwd128
	#endif

	#if HAS(__builtin_ia32_pmovsxwq128)
		#ifdef pmovsxwq128
			#undef pmovsxwq128
		#endif
		#define pmovsxwq128 __builtin_ia32_pmovsxwq128
	#endif

	#if HAS(__builtin_ia32_pmovzxbd128)
		#ifdef pmovzxbd128
			#undef pmovzxbd128
		#endif
		#define pmovzxbd128 __builtin_ia32_pmovzxbd128
	#endif

	#if HAS(__builtin_ia32_pmovzxbq128)
		#ifdef pmovzxbq128
			#undef pmovzxbq128
		#endif
		#define pmovzxbq128 __builtin_ia32_pmovzxbq128
	#endif

	#if HAS(__builtin_ia32_pmovzxbw128)
		#ifdef pmovzxbw128
			#undef pmovzxbw128
		#endif
		#define pmovzxbw128 __builtin_ia32_pmovzxbw128
	#endif

	#if HAS(__builtin_ia32_pmovzxdq128)
		#ifdef pmovzxdq128
			#undef pmovzxdq128
		#endif
		#define pmovzxdq128 __builtin_ia32_pmovzxdq128
	#endif

	#if HAS(__builtin_ia32_pmovzxwd128)
		#ifdef pmovzxwd128
			#undef pmovzxwd128
		#endif
		#define pmovzxwd128 __builtin_ia32_pmovzxwd128
	#endif

	#if HAS(__builtin_ia32_pmovzxwq128)
		#ifdef pmovzxwq128
			#undef pmovzxwq128
		#endif
		#define pmovzxwq128 __builtin_ia32_pmovzxwq128
	#endif

	#if HAS(__builtin_ia32_pmuldq128)
		#ifdef pmuldq128
			#undef pmuldq128
		#endif
		#define pmuldq128 __builtin_ia32_pmuldq128
	#endif

	#if HAS(__builtin_ia32_pmulld128)
		#ifdef pmulld128
			#undef pmulld128
		#endif
		#define pmulld128 __builtin_ia32_pmulld128
	#endif

	#if HAS(__builtin_ia32_ptestc128)
		#ifdef ptestc128
			#undef ptestc128
		#endif
		#define ptestc128 __builtin_ia32_ptestc128
	#endif

	#if HAS(__builtin_ia32_ptestnzc128)
		#ifdef ptestnzc128
			#undef ptestnzc128
		#endif
		#define ptestnzc128 __builtin_ia32_ptestnzc128
	#endif

	#if HAS(__builtin_ia32_ptestz128)
		#ifdef ptestz128
			#undef ptestz128
		#endif
		#define ptestz128 __builtin_ia32_ptestz128
	#endif

	#if HAS(__builtin_ia32_roundpd)
		#ifdef roundpd
			#undef roundpd
		#endif
		#define roundpd __builtin_ia32_roundpd
	#endif

	#if HAS(__builtin_ia32_roundps)
		#ifdef roundps
			#undef roundps
		#endif
		#define roundps __builtin_ia32_roundps
	#endif

	#if HAS(__builtin_ia32_roundsd)
		#ifdef roundsd
			#undef roundsd
		#endif
		#define roundsd __builtin_ia32_roundsd
	#endif

	#if HAS(__builtin_ia32_roundss)
		#ifdef roundss
			#undef roundss
		#endif
		#define roundss __builtin_ia32_roundss
	#endif

//these have names different than machine instruction - both provided

	#if HAS(__builtin_ia32_vec_ext_v2di)
		#ifdef vec_ext_v2di
			#undef vec_ext_v2di
		#endif
		#define vec_ext_v2di __builtin_ia32_vec_ext_v2di
		#ifdef pextrq
			#undef pextrq
		#endif
		#define pextrq __builtin_ia32_vec_ext_v2di
	#endif

	#if HAS(__builtin_ia32_vec_ext_v16qi)
		#ifdef vec_ext_v16qi
			#undef vec_ext_v16qi
		#endif
		#define vec_ext_v16qi __builtin_ia32_vec_ext_v16qi
		#ifdef pextrb
			#undef pextrb
		#endif
		#define pextrb __builtin_ia32_vec_ext_v16qi

	#endif

	#if HAS(__builtin_ia32_vec_ext_v4sf)
		#ifdef vec_ext_v4sf
			#undef vec_ext_v4sf
		#endif
		#define vec_ext_v4sf __builtin_ia32_vec_ext_v4sf
		#ifdef extractps
			#undef extractps
		#endif
		#define extractps __builtin_ia32_vec_ext_v4sf
	#endif

	#if HAS(__builtin_ia32_vec_ext_v4si)
		#ifdef vec_ext_v4si
			#undef vec_ext_v4si
		#endif
		#define vec_ext_v4si __builtin_ia32_vec_ext_v4si
		#ifdef pextrd
			#undef pextrd
		#endif
		#define pextrd __builtin_ia32_vec_ext_v4si
	#endif

	#if HAS(__builtin_ia32_vec_set_v16qi)
		#ifdef vec_set_v16qi
			#undef vec_set_v16qi
		#endif
		#define vec_set_v16qi __builtin_ia32_vec_set_v16qi
		#ifdef pinsrb
			#undef pinsrb
		#endif
		#define pinsrb __builtin_ia32_vec_set_v16qi
	#endif

	#if HAS(__builtin_ia32_vec_set_v2di)
		#ifdef vec_set_v2di
			#undef vec_set_v2di
		#endif
		#define vec_set_v2di __builtin_ia32_vec_set_v2di
		#ifdef pinsrq
			#undef pinsrq
		#endif
		#define pinsrq __builtin_ia32_vec_set_v2di
	#endif

	#if HAS(__builtin_ia32_vec_set_v4sf)
		#ifdef vec_set_v4sf
			#undef vec_set_v4sf
		#endif
		#define vec_set_v4sf __builtin_ia32_vec_set_v4sf
		#ifdef insertps
			#undef insertps
		#endif
		#define insertps __builtin_ia32_vec_set_v4sf
	#endif

	#if HAS(__builtin_ia32_vec_set_v4si)
		#ifdef vec_set_v4si
			#undef vec_set_v4si
		#endif
		#define vec_set_v4si __builtin_ia32_vec_set_v4si
		#ifdef pinsrd
			#undef pinsrd
		#endif
		#define pinsrd __builtin_ia32_vec_set_v4si
	#endif


#endif

#if defined __SSE4_2__

	#if HAS(__builtin_ia32_crc32di)
		#define crc32di __builtin_ia32_crc32di
		#define crc32q __builtin_ia32_crc32di
		#define __crc32q __builtin_ia32_crc32di
	#endif

	#if HAS(__builtin_ia32_crc32hi)
		#define crc32hi __builtin_ia32_crc32hi
		#define crc32w __builtin_ia32_crc32hi
		#define __crc32w crc32w
	#endif

	#if HAS(__builtin_ia32_crc32qi)
		#define crc32qi __builtin_ia32_crc32qi
		#define crc32b __builtin_ia32_crc32qi
		#define __crc32b crc32b
	#endif

	#if HAS(__builtin_ia32_crc32si)
		#define crc32si __builtin_ia32_crc32si
		#define crc32l __builtin_ia32_crc32si
		#define __crc32d crc32si
	#endif

	#if HAS(__builtin_ia32_pcmpestri128)
		#ifdef pcmpestri128
			#undef pcmpestri128
		#endif
		#define pcmpestri128 __builtin_ia32_pcmpestri128
	#endif

	#if HAS(__builtin_ia32_pcmpestria128)
		#ifdef pcmpestria128
			#undef pcmpestria128
		#endif
		#define pcmpestria128 __builtin_ia32_pcmpestria128
	#endif

	#if HAS(__builtin_ia32_pcmpestric128)
		#ifdef pcmpestric128
			#undef pcmpestric128
		#endif
		#define pcmpestric128 __builtin_ia32_pcmpestric128
	#endif

	#if HAS(__builtin_ia32_pcmpestrio128)
		#ifdef pcmpestrio128
			#undef pcmpestrio128
		#endif
		#define pcmpestrio128 __builtin_ia32_pcmpestrio128
	#endif

	#if HAS(__builtin_ia32_pcmpestris128)
		#ifdef pcmpestris128
			#undef pcmpestris128
		#endif
		#define pcmpestris128 __builtin_ia32_pcmpestris128
	#endif

	#if HAS(__builtin_ia32_pcmpestriz128)
		#ifdef pcmpestriz128
			#undef pcmpestriz128
		#endif
		#define pcmpestriz128 __builtin_ia32_pcmpestriz128
	#endif

	#if HAS(__builtin_ia32_pcmpestrm128)
		#ifdef pcmpestrm128
			#undef pcmpestrm128
		#endif
		#define pcmpestrm128 __builtin_ia32_pcmpestrm128
	#endif

	#if HAS(__builtin_ia32_pcmpgtq)
		#ifdef pcmpgtq
			#undef pcmpgtq
		#endif
		#define pcmpgtq __builtin_ia32_pcmpgtq
	#endif

	#if HAS(__builtin_ia32_pcmpistri128)
		#ifdef pcmpistri128
			#undef pcmpistri128
		#endif
		#define pcmpistri128 __builtin_ia32_pcmpistri128
	#endif

	#if HAS(__builtin_ia32_pcmpistria128)
		#ifdef pcmpistria128
			#undef pcmpistria128
		#endif
		#define pcmpistria128 __builtin_ia32_pcmpistria128
	#endif

	#if HAS(__builtin_ia32_pcmpistric128)
		#ifdef pcmpistric128
			#undef pcmpistric128
		#endif
		#define pcmpistric128 __builtin_ia32_pcmpistric128
	#endif

	#if HAS(__builtin_ia32_pcmpistrio128)
		#ifdef pcmpistrio128
			#undef pcmpistrio128
		#endif
		#define pcmpistrio128 __builtin_ia32_pcmpistrio128
	#endif

	#if HAS(__builtin_ia32_pcmpistris128)
		#ifdef pcmpistris128
			#undef pcmpistris128
		#endif
		#define pcmpistris128 __builtin_ia32_pcmpistris128
	#endif

	#if HAS(__builtin_ia32_pcmpistriz128)
		#ifdef pcmpistriz128
			#undef pcmpistriz128
		#endif
		#define pcmpistriz128 __builtin_ia32_pcmpistriz128
	#endif

	#if HAS(__builtin_ia32_pcmpistrm128)
		#ifdef pcmpistrm128
			#undef pcmpistrm128
		#endif
		#define pcmpistrm128 __builtin_ia32_pcmpistrm128
	#endif

#endif

#if defined __SSE4A__
//#include <ammintrin.h> //must include compiler includes path

	#if HAS(__builtin_ia32_extrq)
		#ifdef extrq
			#undef extrq
		#endif
		#define extrq __builtin_ia32_extrq
	#endif

	#if HAS(__builtin_ia32_extrqi)
		#ifdef extrqi
			#undef extrqi
		#endif
		#define extrqi __builtin_ia32_extrqi
	#endif

	#if HAS(__builtin_ia32_insertq)
		#ifdef insertq
			#undef insertq
		#endif
		#define insertq __builtin_ia32_insertq
	#endif

	#if HAS(__builtin_ia32_insertqi)
		#ifdef insertqi
			#undef insertqi
		#endif
		#define insertqi __builtin_ia32_insertqi
	#endif

	#if HAS(__builtin_ia32_movntsd)
		#ifdef movntsd
			#undef movntsd
		#endif
		#define movntsd __builtin_ia32_movntsd
	#endif

	#if HAS(__builtin_ia32_movntss)
		#ifdef movntss
			#undef movntss
		#endif
		#define movntss __builtin_ia32_movntss
	#endif

#endif

#if defined __AVX__

	#if HAS(__builtin_ia32_addpd256)
		#ifdef addpd256
			#undef addpd256
		#endif
		#define addpd256 __builtin_ia32_addpd256
	#endif

	#if HAS(__builtin_ia32_addps256)
		#ifdef addps256
			#undef addps256
		#endif
		#define addps256 __builtin_ia32_addps256
	#endif

	#if HAS(__builtin_ia32_addsubpd256)
		#ifdef addsubpd256
			#undef addsubpd256
		#endif
		#define addsubpd256 __builtin_ia32_addsubpd256
	#endif

	#if HAS(__builtin_ia32_addsubps256)
		#ifdef addsubps256
			#undef addsubps256
		#endif
		#define addsubps256 __builtin_ia32_addsubps256
	#endif

	#if HAS(__builtin_ia32_andnpd256)
		#ifdef andnpd256
			#undef andnpd256
		#endif
		#define andnpd256 __builtin_ia32_andnpd256
	#endif

	#if HAS(__builtin_ia32_andnps256)
		#ifdef andnps256
			#undef andnps256
		#endif
		#define andnps256 __builtin_ia32_andnps256
	#endif

	#if HAS(__builtin_ia32_andpd256)
		#ifdef andpd256
			#undef andpd256
		#endif
		#define andpd256 __builtin_ia32_andpd256
	#endif

	#if HAS(__builtin_ia32_andps256)
		#ifdef andps256
			#undef andps256
		#endif
		#define andps256 __builtin_ia32_andps256
	#endif

	#if HAS(__builtin_ia32_blendpd256)
		#ifdef blendpd256
			#undef blendpd256
		#endif
		#define blendpd256 __builtin_ia32_blendpd256
	#endif

	#if HAS(__builtin_ia32_blendps256)
		#ifdef blendps256
			#undef blendps256
		#endif
		#define blendps256 __builtin_ia32_blendps256
	#endif

	#if HAS(__builtin_ia32_blendvpd256)
		#ifdef blendvpd256
			#undef blendvpd256
		#endif
		#define blendvpd256 __builtin_ia32_blendvpd256
	#endif

	#if HAS(__builtin_ia32_blendvps256)
		#ifdef blendvps256
			#undef blendvps256
		#endif
		#define blendvps256 __builtin_ia32_blendvps256
	#endif

	#if HAS(__builtin_ia32_cmppd)
		#ifdef cmppd
			#undef cmppd
		#endif
		#define cmppd __builtin_ia32_cmppd
	#endif

	#if HAS(__builtin_ia32_cmppd256)
		#ifdef cmppd256
			#undef cmppd256
		#endif
		#define cmppd256 __builtin_ia32_cmppd256
	#endif

	#if HAS(__builtin_ia32_cmpps)
		#ifdef cmpps
			#undef cmpps
		#endif
		#define cmpps __builtin_ia32_cmpps
	#endif

	#if HAS(__builtin_ia32_cmpps256)
		#ifdef cmpps256
			#undef cmpps256
		#endif
		#define cmpps256 __builtin_ia32_cmpps256
	#endif

	#if HAS(__builtin_ia32_cmpsd)
		#ifdef cmpsd
			#undef cmpsd
		#endif
		#define cmpsd __builtin_ia32_cmpsd
	#endif

	#if HAS(__builtin_ia32_cmpss)
		#ifdef cmpss
			#undef cmpss
		#endif
		#define cmpss __builtin_ia32_cmpss
	#endif

	#if HAS(__builtin_ia32_cvtdq2pd256)
		#ifdef cvtdq2pd256
			#undef cvtdq2pd256
		#endif
		#define cvtdq2pd256 __builtin_ia32_cvtdq2pd256
	#endif

	#if HAS(__builtin_ia32_cvtdq2ps256)
		#ifdef cvtdq2ps256
			#undef cvtdq2ps256
		#endif
		#define cvtdq2ps256 __builtin_ia32_cvtdq2ps256
	#endif

	#if HAS(__builtin_ia32_cvtpd2dq256)
		#ifdef cvtpd2dq256
			#undef cvtpd2dq256
		#endif
		#define cvtpd2dq256 __builtin_ia32_cvtpd2dq256
	#endif

	#if HAS(__builtin_ia32_cvtpd2ps256)
		#ifdef cvtpd2ps256
			#undef cvtpd2ps256
		#endif
		#define cvtpd2ps256 __builtin_ia32_cvtpd2ps256
	#endif

	#if HAS(__builtin_ia32_cvtps2dq256)
		#ifdef cvtps2dq256
			#undef cvtps2dq256
		#endif
		#define cvtps2dq256 __builtin_ia32_cvtps2dq256
	#endif

	#if HAS(__builtin_ia32_cvtps2pd256)
		#ifdef cvtps2pd256
			#undef cvtps2pd256
		#endif
		#define cvtps2pd256 __builtin_ia32_cvtps2pd256
	#endif

	#if HAS(__builtin_ia32_cvttpd2dq256)
		#ifdef cvttpd2dq256
			#undef cvttpd2dq256
		#endif
		#define cvttpd2dq256 __builtin_ia32_cvttpd2dq256
	#endif

	#if HAS(__builtin_ia32_cvttps2dq256)
		#ifdef cvttps2dq256
			#undef cvttps2dq256
		#endif
		#define cvttps2dq256 __builtin_ia32_cvttps2dq256
	#endif

	#if HAS(__builtin_ia32_divpd256)
		#ifdef divpd256
			#undef divpd256
		#endif
		#define divpd256 __builtin_ia32_divpd256
	#endif

	#if HAS(__builtin_ia32_divps256)
		#ifdef divps256
			#undef divps256
		#endif
		#define divps256 __builtin_ia32_divps256
	#endif

	#if HAS(__builtin_ia32_dpps256)
		#ifdef dpps256
			#undef dpps256
		#endif
		#define dpps256 __builtin_ia32_dpps256
	#endif

	#if HAS(__builtin_ia32_haddpd256)
		#ifdef haddpd256
			#undef haddpd256
		#endif
		#define haddpd256 __builtin_ia32_haddpd256
	#endif

	#if HAS(__builtin_ia32_haddps256)
		#ifdef haddps256
			#undef haddps256
		#endif
		#define haddps256 __builtin_ia32_haddps256
	#endif

	#if HAS(__builtin_ia32_hsubpd256)
		#ifdef hsubpd256
			#undef hsubpd256
		#endif
		#define hsubpd256 __builtin_ia32_hsubpd256
	#endif

	#if HAS(__builtin_ia32_hsubps256)
		#ifdef hsubps256
			#undef hsubps256
		#endif
		#define hsubps256 __builtin_ia32_hsubps256
	#endif

	#if HAS(__builtin_ia32_lddqu256)
		#ifdef lddqu256
			#undef lddqu256
		#endif
		#define lddqu256 __builtin_ia32_lddqu256
	#endif

	#if HAS(__builtin_ia32_loaddqu256)
		#ifdef loaddqu256
			#undef loaddqu256
		#endif
		#define loaddqu256 __builtin_ia32_loaddqu256
	#endif

	#if HAS(__builtin_ia32_loadupd256)
		#ifdef loadupd256
			#undef loadupd256
		#endif
		#define loadupd256 __builtin_ia32_loadupd256
	#endif

	#if HAS(__builtin_ia32_loadups256)
		#ifdef loadups256
			#undef loadups256
		#endif
		#define loadups256 __builtin_ia32_loadups256
	#endif

	#if HAS(__builtin_ia32_maskloadpd)
		#ifdef maskloadpd
			#undef maskloadpd
		#endif
		#define maskloadpd __builtin_ia32_maskloadpd
	#endif

	#if HAS(__builtin_ia32_maskloadpd256)
		#ifdef maskloadpd256
			#undef maskloadpd256
		#endif
		#define maskloadpd256 __builtin_ia32_maskloadpd256
	#endif

	#if HAS(__builtin_ia32_maskloadps)
		#ifdef maskloadps
			#undef maskloadps
		#endif
		#define maskloadps __builtin_ia32_maskloadps
	#endif

	#if HAS(__builtin_ia32_maskloadps256)
		#ifdef maskloadps256
			#undef maskloadps256
		#endif
		#define maskloadps256 __builtin_ia32_maskloadps256
	#endif

	#if HAS(__builtin_ia32_maskstorepd)
		#ifdef maskstorepd
			#undef maskstorepd
		#endif
		#define maskstorepd __builtin_ia32_maskstorepd
	#endif

	#if HAS(__builtin_ia32_maskstorepd256)
		#ifdef maskstorepd256
			#undef maskstorepd256
		#endif
		#define maskstorepd256 __builtin_ia32_maskstorepd256
	#endif

	#if HAS(__builtin_ia32_maskstoreps)
		#ifdef maskstoreps
			#undef maskstoreps
		#endif
		#define maskstoreps __builtin_ia32_maskstoreps
	#endif

	#if HAS(__builtin_ia32_maskstoreps256)
		#ifdef maskstoreps256
			#undef maskstoreps256
		#endif
		#define maskstoreps256 __builtin_ia32_maskstoreps256
	#endif

	#if HAS(__builtin_ia32_maxpd256)
		#ifdef maxpd256
			#undef maxpd256
		#endif
		#define maxpd256 __builtin_ia32_maxpd256
	#endif

	#if HAS(__builtin_ia32_maxps256)
		#ifdef maxps256
			#undef maxps256
		#endif
		#define maxps256 __builtin_ia32_maxps256
	#endif

	#if HAS(__builtin_ia32_minpd256)
		#ifdef minpd256
			#undef minpd256
		#endif
		#define minpd256 __builtin_ia32_minpd256
	#endif

	#if HAS(__builtin_ia32_minps256)
		#ifdef minps256
			#undef minps256
		#endif
		#define minps256 __builtin_ia32_minps256
	#endif

	#if HAS(__builtin_ia32_movddup256)
		#ifdef movddup256
			#undef movddup256
		#endif
		#define movddup256 __builtin_ia32_movddup256
	#endif

	#if HAS(__builtin_ia32_movmskpd256)
		#ifdef movmskpd256
			#undef movmskpd256
		#endif
		#define movmskpd256 __builtin_ia32_movmskpd256
	#endif

	#if HAS(__builtin_ia32_movmskps256)
		#ifdef movmskps256
			#undef movmskps256
		#endif
		#define movmskps256 __builtin_ia32_movmskps256
	#endif

	#if HAS(__builtin_ia32_movshdup256)
		#ifdef movshdup256
			#undef movshdup256
		#endif
		#define movshdup256 __builtin_ia32_movshdup256
	#endif

	#if HAS(__builtin_ia32_movsldup256)
		#ifdef movsldup256
			#undef movsldup256
		#endif
		#define movsldup256 __builtin_ia32_movsldup256
	#endif

	#if HAS(__builtin_ia32_mulpd256)
		#ifdef mulpd256
			#undef mulpd256
		#endif
		#define mulpd256 __builtin_ia32_mulpd256
	#endif

	#if HAS(__builtin_ia32_mulps256)
		#ifdef mulps256
			#undef mulps256
		#endif
		#define mulps256 __builtin_ia32_mulps256
	#endif

	#if HAS(__builtin_ia32_orpd256)
		#ifdef orpd256
			#undef orpd256
		#endif
		#define orpd256 __builtin_ia32_orpd256
	#endif

	#if HAS(__builtin_ia32_orps256)
		#ifdef orps256
			#undef orps256
		#endif
		#define orps256 __builtin_ia32_orps256
	#endif

	#if HAS(__builtin_ia32_pd256_pd)
		#ifdef pd256_pd
			#undef pd256_pd
		#endif
		#define pd256_pd __builtin_ia32_pd256_pd
	#endif

	#if HAS(__builtin_ia32_pd_pd256)
		#ifdef pd_pd256
			#undef pd_pd256
		#endif
		#define pd_pd256 __builtin_ia32_pd_pd256
	#endif

	#if HAS(__builtin_ia32_ps256_ps)
		#ifdef ps256_ps
			#undef ps256_ps
		#endif
		#define ps256_ps __builtin_ia32_ps256_ps
	#endif

	#if HAS(__builtin_ia32_ps_ps256)
		#ifdef ps_ps256
			#undef ps_ps256
		#endif
		#define ps_ps256 __builtin_ia32_ps_ps256
	#endif

	#if HAS(__builtin_ia32_ptestc256)
		#ifdef ptestc256
			#undef ptestc256
		#endif
		#define ptestc256 __builtin_ia32_ptestc256
	#endif

	#if HAS(__builtin_ia32_ptestnzc256)
		#ifdef ptestnzc256
			#undef ptestnzc256
		#endif
		#define ptestnzc256 __builtin_ia32_ptestnzc256
	#endif

	#if HAS(__builtin_ia32_ptestz256)
		#ifdef ptestz256
			#undef ptestz256
		#endif
		#define ptestz256 __builtin_ia32_ptestz256
	#endif

	#if HAS(__builtin_ia32_rcpps256)
		#ifdef rcpps256
			#undef rcpps256
		#endif
		#define rcpps256 __builtin_ia32_rcpps256
	#endif

	#if HAS(__builtin_ia32_roundpd256)
		#ifdef roundpd256
			#undef roundpd256
		#endif
		#define roundpd256 __builtin_ia32_roundpd256
	#endif

	#if HAS(__builtin_ia32_roundps256)
		#ifdef roundps256
			#undef roundps256
		#endif
		#define roundps256 __builtin_ia32_roundps256
	#endif

	#if HAS(__builtin_ia32_rsqrtps256)
		#ifdef rsqrtps256
			#undef rsqrtps256
		#endif
		#define rsqrtps256 __builtin_ia32_rsqrtps256
	#endif

	#if HAS(__builtin_ia32_rsqrtps_nr256)
		#ifdef rsqrtps_nr256
			#undef rsqrtps_nr256
		#endif
		#define rsqrtps_nr256 __builtin_ia32_rsqrtps_nr256
	#endif

	#if HAS(__builtin_ia32_shufpd256)
		#ifdef shufpd256
			#undef shufpd256
		#endif
		#define shufpd256 __builtin_ia32_shufpd256
	#endif

	#if HAS(__builtin_ia32_shufps256)
		#ifdef shufps256
			#undef shufps256
		#endif
		#define shufps256 __builtin_ia32_shufps256
	#endif

	#if HAS(__builtin_ia32_si256_si)
		#ifdef si256_si
			#undef si256_si
		#endif
		#define si256_si __builtin_ia32_si256_si
	#endif

	#if HAS(__builtin_ia32_si_si256)
		#ifdef si_si256
			#undef si_si256
		#endif
		#define si_si256 __builtin_ia32_si_si256
	#endif

	#if HAS(__builtin_ia32_sqrtpd256)
		#ifdef sqrtpd256
			#undef sqrtpd256
		#endif
		#define sqrtpd256 __builtin_ia32_sqrtpd256
	#endif

	#if HAS(__builtin_ia32_sqrtps256)
		#ifdef sqrtps256
			#undef sqrtps256
		#endif
		#define sqrtps256 __builtin_ia32_sqrtps256
	#endif

	#if HAS(__builtin_ia32_sqrtps_nr256)
		#ifdef sqrtps_nr256
			#undef sqrtps_nr256
		#endif
		#define sqrtps_nr256 __builtin_ia32_sqrtps_nr256
	#endif

	#if HAS(__builtin_ia32_storedqu256)
		#ifdef storedqu256
			#undef storedqu256
		#endif
		#define storedqu256 __builtin_ia32_storedqu256
	#endif

	#if HAS(__builtin_ia32_storeupd256)
		#ifdef storeupd256
			#undef storeupd256
		#endif
		#define storeupd256 __builtin_ia32_storeupd256
	#endif

	#if HAS(__builtin_ia32_storeups256)
		#ifdef storeups256
			#undef storeups256
		#endif
		#define storeups256 __builtin_ia32_storeups256
	#endif

	#if HAS(__builtin_ia32_subpd256)
		#ifdef subpd256
			#undef subpd256
		#endif
		#define subpd256 __builtin_ia32_subpd256
	#endif

	#if HAS(__builtin_ia32_subps256)
		#ifdef subps256
			#undef subps256
		#endif
		#define subps256 __builtin_ia32_subps256
	#endif

	#if HAS(__builtin_ia32_unpckhpd256)
		#ifdef unpckhpd256
			#undef unpckhpd256
		#endif
		#define unpckhpd256 __builtin_ia32_unpckhpd256
	#endif

	#if HAS(__builtin_ia32_unpckhps256)
		#ifdef unpckhps256
			#undef unpckhps256
		#endif
		#define unpckhps256 __builtin_ia32_unpckhps256
	#endif

	#if HAS(__builtin_ia32_unpcklpd256)
		#ifdef unpcklpd256
			#undef unpcklpd256
		#endif
		#define unpcklpd256 __builtin_ia32_unpcklpd256
	#endif

	#if HAS(__builtin_ia32_unpcklps256)
		#ifdef unpcklps256
			#undef unpcklps256
		#endif
		#define unpcklps256 __builtin_ia32_unpcklps256
	#endif

	#if HAS(__builtin_ia32_vbroadcastf128_pd256)
		#ifdef vbroadcastf128_pd256
			#undef vbroadcastf128_pd256
		#endif
		#define vbroadcastf128_pd256 __builtin_ia32_vbroadcastf128_pd256
	#endif

	#if HAS(__builtin_ia32_vbroadcastf128_ps256)
		#ifdef vbroadcastf128_ps256
			#undef vbroadcastf128_ps256
		#endif
		#define vbroadcastf128_ps256 __builtin_ia32_vbroadcastf128_ps256
	#endif

	#if HAS(__builtin_ia32_vbroadcastsd256)
		#ifdef vbroadcastsd256
			#undef vbroadcastsd256
		#endif
		#define vbroadcastsd256 __builtin_ia32_vbroadcastsd256
	#endif

	#if HAS(__builtin_ia32_vbroadcastss)
		#ifdef vbroadcastss
			#undef vbroadcastss
		#endif
		#define vbroadcastss __builtin_ia32_vbroadcastss
	#endif

	#if HAS(__builtin_ia32_vbroadcastss256)
		#ifdef vbroadcastss256
			#undef vbroadcastss256
		#endif
		#define vbroadcastss256 __builtin_ia32_vbroadcastss256
	#endif

	#if HAS(__builtin_ia32_vextractf128_pd256)
		#ifdef vextractf128_pd256
			#undef vextractf128_pd256
		#endif
		#define vextractf128_pd256 __builtin_ia32_vextractf128_pd256
	#endif

	#if HAS(__builtin_ia32_vextractf128_ps256)
		#ifdef vextractf128_ps256
			#undef vextractf128_ps256
		#endif
		#define vextractf128_ps256 __builtin_ia32_vextractf128_ps256
	#endif

	#if HAS(__builtin_ia32_vextractf128_si256)
		#ifdef vextractf128_si256
			#undef vextractf128_si256
		#endif
		#define vextractf128_si256 __builtin_ia32_vextractf128_si256
	#endif

	#if HAS(__builtin_ia32_vinsertf128_pd256)
		#ifdef vinsertf128_pd256
			#undef vinsertf128_pd256
		#endif
		#define vinsertf128_pd256 __builtin_ia32_vinsertf128_pd256
	#endif

	#if HAS(__builtin_ia32_vinsertf128_ps256)
		#ifdef vinsertf128_ps256
			#undef vinsertf128_ps256
		#endif
		#define vinsertf128_ps256 __builtin_ia32_vinsertf128_ps256
	#endif

	#if HAS(__builtin_ia32_vinsertf128_si256)
		#ifdef vinsertf128_si256
			#undef vinsertf128_si256
		#endif
		#define vinsertf128_si256 __builtin_ia32_vinsertf128_si256
	#endif

	#if HAS(__builtin_ia32_vperm2f128_pd256)
		#ifdef vperm2f128_pd256
			#undef vperm2f128_pd256
		#endif
		#define vperm2f128_pd256 __builtin_ia32_vperm2f128_pd256
	#endif

	#if HAS(__builtin_ia32_vperm2f128_ps256)
		#ifdef vperm2f128_ps256
			#undef vperm2f128_ps256
		#endif
		#define vperm2f128_ps256 __builtin_ia32_vperm2f128_ps256
	#endif

	#if HAS(__builtin_ia32_vperm2f128_si256)
		#ifdef vperm2f128_si256
			#undef vperm2f128_si256
		#endif
		#define vperm2f128_si256 __builtin_ia32_vperm2f128_si256
	#endif

	#if HAS(__builtin_ia32_vpermil2pd)
		#ifdef vpermil2pd
			#undef vpermil2pd
		#endif
		#define vpermil2pd __builtin_ia32_vpermil2pd
	#endif

	#if HAS(__builtin_ia32_vpermil2pd256)
		#ifdef vpermil2pd256
			#undef vpermil2pd256
		#endif
		#define vpermil2pd256 __builtin_ia32_vpermil2pd256
	#endif

	#if HAS(__builtin_ia32_vpermil2ps)
		#ifdef vpermil2ps
			#undef vpermil2ps
		#endif
		#define vpermil2ps __builtin_ia32_vpermil2ps
	#endif

	#if HAS(__builtin_ia32_vpermil2ps256)
		#ifdef vpermil2ps256
			#undef vpermil2ps256
		#endif
		#define vpermil2ps256 __builtin_ia32_vpermil2ps256
	#endif

	#if HAS(__builtin_ia32_vpermilpd)
		#ifdef vpermilpd
			#undef vpermilpd
		#endif
		#define vpermilpd __builtin_ia32_vpermilpd
	#endif

	#if HAS(__builtin_ia32_vpermilpd256)
		#ifdef vpermilpd256
			#undef vpermilpd256
		#endif
		#define vpermilpd256 __builtin_ia32_vpermilpd256
	#endif

	#if HAS(__builtin_ia32_vpermilps)
		#ifdef vpermilps
			#undef vpermilps
		#endif
		#define vpermilps __builtin_ia32_vpermilps
	#endif

	#if HAS(__builtin_ia32_vpermilps256)
		#ifdef vpermilps256
			#undef vpermilps256
		#endif
		#define vpermilps256 __builtin_ia32_vpermilps256
	#endif

	#if HAS(__builtin_ia32_vpermilvarpd)
		#ifdef vpermilvarpd
			#undef vpermilvarpd
		#endif
		#define vpermilvarpd __builtin_ia32_vpermilvarpd
	#endif

	#if HAS(__builtin_ia32_vpermilvarpd256)
		#ifdef vpermilvarpd256
			#undef vpermilvarpd256
		#endif
		#define vpermilvarpd256 __builtin_ia32_vpermilvarpd256
	#endif

	#if HAS(__builtin_ia32_vpermilvarps)
		#ifdef vpermilvarps
			#undef vpermilvarps
		#endif
		#define vpermilvarps __builtin_ia32_vpermilvarps
	#endif

	#if HAS(__builtin_ia32_vpermilvarps256)
		#ifdef vpermilvarps256
			#undef vpermilvarps256
		#endif
		#define vpermilvarps256 __builtin_ia32_vpermilvarps256
	#endif

	#if HAS(__builtin_ia32_vtestcpd)
		#ifdef vtestcpd
			#undef vtestcpd
		#endif
		#define vtestcpd __builtin_ia32_vtestcpd
	#endif

	#if HAS(__builtin_ia32_vtestcpd256)
		#ifdef vtestcpd256
			#undef vtestcpd256
		#endif
		#define vtestcpd256 __builtin_ia32_vtestcpd256
	#endif

	#if HAS(__builtin_ia32_vtestcps)
		#ifdef vtestcps
			#undef vtestcps
		#endif
		#define vtestcps __builtin_ia32_vtestcps
	#endif

	#if HAS(__builtin_ia32_vtestcps256)
		#ifdef vtestcps256
			#undef vtestcps256
		#endif
		#define vtestcps256 __builtin_ia32_vtestcps256
	#endif

	#if HAS(__builtin_ia32_vtestnzcpd)
		#ifdef vtestnzcpd
			#undef vtestnzcpd
		#endif
		#define vtestnzcpd __builtin_ia32_vtestnzcpd
	#endif

	#if HAS(__builtin_ia32_vtestnzcpd256)
		#ifdef vtestnzcpd256
			#undef vtestnzcpd256
		#endif
		#define vtestnzcpd256 __builtin_ia32_vtestnzcpd256
	#endif

	#if HAS(__builtin_ia32_vtestnzcps)
		#ifdef vtestnzcps
			#undef vtestnzcps
		#endif
		#define vtestnzcps __builtin_ia32_vtestnzcps
	#endif

	#if HAS(__builtin_ia32_vtestnzcps256)
		#ifdef vtestnzcps256
			#undef vtestnzcps256
		#endif
		#define vtestnzcps256 __builtin_ia32_vtestnzcps256
	#endif

	#if HAS(__builtin_ia32_vtestzpd)
		#ifdef vtestzpd
			#undef vtestzpd
		#endif
		#define vtestzpd __builtin_ia32_vtestzpd
	#endif

	#if HAS(__builtin_ia32_vtestzpd256)
		#ifdef vtestzpd256
			#undef vtestzpd256
		#endif
		#define vtestzpd256 __builtin_ia32_vtestzpd256
	#endif

	#if HAS(__builtin_ia32_vtestzps)
		#ifdef vtestzps
			#undef vtestzps
		#endif
		#define vtestzps __builtin_ia32_vtestzps
	#endif

	#if HAS(__builtin_ia32_vtestzps256)
		#ifdef vtestzps256
			#undef vtestzps256
		#endif
		#define vtestzps256 __builtin_ia32_vtestzps256
	#endif

	#if HAS(__builtin_ia32_vzeroall)
		#ifdef vzeroall
			#undef vzeroall
		#endif
		#define vzeroall __builtin_ia32_vzeroall
	#endif

	#if HAS(__builtin_ia32_vzeroupper)
		#ifdef vzeroupper
			#undef vzeroupper
		#endif
		#define vzeroupper __builtin_ia32_vzeroupper
	#endif

	#if HAS(__builtin_ia32_xorpd256)
		#ifdef xorpd256
			#undef xorpd256
		#endif
		#define xorpd256 __builtin_ia32_xorpd256
	#endif

	#if HAS(__builtin_ia32_xorps256)
		#ifdef xorps256
			#undef xorps256
		#endif
		#define xorps256 __builtin_ia32_xorps256
	#endif

#endif

#if defined __AVX2__

	#if HAS(__builtin_ia32_andnotsi256)
		#ifdef andnotsi256
			#undef andnotsi256
		#endif
		#define andnotsi256 __builtin_ia32_andnotsi256
	#endif

	#if HAS(__builtin_ia32_andsi256)
		#ifdef andsi256
			#undef andsi256
		#endif
		#define andsi256 __builtin_ia32_andsi256
	#endif

	#if HAS(__builtin_ia32_extract128i256)
		#ifdef extract128i256
			#undef extract128i256
		#endif
		#define extract128i256 __builtin_ia32_extract128i256
	#endif

	#if HAS(__builtin_ia32_gatherdiv2df)
		#ifdef gatherdiv2df
			#undef gatherdiv2df
		#endif
		#define gatherdiv2df __builtin_ia32_gatherdiv2df
	#endif

	#if HAS(__builtin_ia32_gatherdiv2di)
		#ifdef gatherdiv2di
			#undef gatherdiv2di
		#endif
		#define gatherdiv2di __builtin_ia32_gatherdiv2di
	#endif

	#if HAS(__builtin_ia32_gatherdiv4df)
		#ifdef gatherdiv4df
			#undef gatherdiv4df
		#endif
		#define gatherdiv4df __builtin_ia32_gatherdiv4df
	#endif

	#if HAS(__builtin_ia32_gatherdiv4di)
		#ifdef gatherdiv4di
			#undef gatherdiv4di
		#endif
		#define gatherdiv4di __builtin_ia32_gatherdiv4di
	#endif

	#if HAS(__builtin_ia32_gatherdiv4sf)
		#ifdef gatherdiv4sf
			#undef gatherdiv4sf
		#endif
		#define gatherdiv4sf __builtin_ia32_gatherdiv4sf
	#endif

	#if HAS(__builtin_ia32_gatherdiv4sf256)
		#ifdef gatherdiv4sf256
			#undef gatherdiv4sf256
		#endif
		#define gatherdiv4sf256 __builtin_ia32_gatherdiv4sf256
	#endif

	#if HAS(__builtin_ia32_gatherdiv4si)
		#ifdef gatherdiv4si
			#undef gatherdiv4si
		#endif
		#define gatherdiv4si __builtin_ia32_gatherdiv4si
	#endif

	#if HAS(__builtin_ia32_gatherdiv4si256)
		#ifdef gatherdiv4si256
			#undef gatherdiv4si256
		#endif
		#define gatherdiv4si256 __builtin_ia32_gatherdiv4si256
	#endif

	#if HAS(__builtin_ia32_gathersiv2df)
		#ifdef gathersiv2df
			#undef gathersiv2df
		#endif
		#define gathersiv2df __builtin_ia32_gathersiv2df
	#endif

	#if HAS(__builtin_ia32_gathersiv2di)
		#ifdef gathersiv2di
			#undef gathersiv2di
		#endif
		#define gathersiv2di __builtin_ia32_gathersiv2di
	#endif

	#if HAS(__builtin_ia32_gathersiv4df)
		#ifdef gathersiv4df
			#undef gathersiv4df
		#endif
		#define gathersiv4df __builtin_ia32_gathersiv4df
	#endif

	#if HAS(__builtin_ia32_gathersiv4di)
		#ifdef gathersiv4di
			#undef gathersiv4di
		#endif
		#define gathersiv4di __builtin_ia32_gathersiv4di
	#endif

	#if HAS(__builtin_ia32_gathersiv4sf)
		#ifdef gathersiv4sf
			#undef gathersiv4sf
		#endif
		#define gathersiv4sf __builtin_ia32_gathersiv4sf
	#endif

	#if HAS(__builtin_ia32_gathersiv4si)
		#ifdef gathersiv4si
			#undef gathersiv4si
		#endif
		#define gathersiv4si __builtin_ia32_gathersiv4si
	#endif

	#if HAS(__builtin_ia32_gathersiv8sf)
		#ifdef gathersiv8sf
			#undef gathersiv8sf
		#endif
		#define gathersiv8sf __builtin_ia32_gathersiv8sf
	#endif

	#if HAS(__builtin_ia32_gathersiv8si)
		#ifdef gathersiv8si
			#undef gathersiv8si
		#endif
		#define gathersiv8si __builtin_ia32_gathersiv8si
	#endif

	#if HAS(__builtin_ia32_insert128i256)
		#ifdef insert128i256
			#undef insert128i256
		#endif
		#define insert128i256 __builtin_ia32_insert128i256
	#endif

	#if HAS(__builtin_ia32_maskloadd)
		#ifdef maskloadd
			#undef maskloadd
		#endif
		#define maskloadd __builtin_ia32_maskloadd
	#endif

	#if HAS(__builtin_ia32_maskloadd256)
		#ifdef maskloadd256
			#undef maskloadd256
		#endif
		#define maskloadd256 __builtin_ia32_maskloadd256
	#endif

	#if HAS(__builtin_ia32_maskloadq)
		#ifdef maskloadq
			#undef maskloadq
		#endif
		#define maskloadq __builtin_ia32_maskloadq
	#endif

	#if HAS(__builtin_ia32_maskloadq256)
		#ifdef maskloadq256
			#undef maskloadq256
		#endif
		#define maskloadq256 __builtin_ia32_maskloadq256
	#endif

	#if HAS(__builtin_ia32_maskstored)
		#ifdef maskstored
			#undef maskstored
		#endif
		#define maskstored __builtin_ia32_maskstored
	#endif

	#if HAS(__builtin_ia32_maskstored256)
		#ifdef maskstored256
			#undef maskstored256
		#endif
		#define maskstored256 __builtin_ia32_maskstored256
	#endif

	#if HAS(__builtin_ia32_maskstoreq)
		#ifdef maskstoreq
			#undef maskstoreq
		#endif
		#define maskstoreq __builtin_ia32_maskstoreq
	#endif

	#if HAS(__builtin_ia32_maskstoreq256)
		#ifdef maskstoreq256
			#undef maskstoreq256
		#endif
		#define maskstoreq256 __builtin_ia32_maskstoreq256
	#endif

	#if HAS(__builtin_ia32_movntdqa256)
		#ifdef movntdqa256
			#undef movntdqa256
		#endif
		#define movntdqa256 __builtin_ia32_movntdqa256
	#endif

	#if HAS(__builtin_ia32_mpsadbw256)
		#ifdef mpsadbw256
			#undef mpsadbw256
		#endif
		#define mpsadbw256 __builtin_ia32_mpsadbw256
	#endif

	#if HAS(__builtin_ia32_pabsb256)
		#ifdef pabsb256
			#undef pabsb256
		#endif
		#define pabsb256 __builtin_ia32_pabsb256
	#endif

	#if HAS(__builtin_ia32_pabsd256)
		#ifdef pabsd256
			#undef pabsd256
		#endif
		#define pabsd256 __builtin_ia32_pabsd256
	#endif

	#if HAS(__builtin_ia32_pabsw256)
		#ifdef pabsw256
			#undef pabsw256
		#endif
		#define pabsw256 __builtin_ia32_pabsw256
	#endif

	#if HAS(__builtin_ia32_packssdw256)
		#ifdef packssdw256
			#undef packssdw256
		#endif
		#define packssdw256 __builtin_ia32_packssdw256
	#endif

	#if HAS(__builtin_ia32_packsswb256)
		#ifdef packsswb256
			#undef packsswb256
		#endif
		#define packsswb256 __builtin_ia32_packsswb256
	#endif

	#if HAS(__builtin_ia32_packusdw256)
		#ifdef packusdw256
			#undef packusdw256
		#endif
		#define packusdw256 __builtin_ia32_packusdw256
	#endif

	#if HAS(__builtin_ia32_packuswb256)
		#ifdef packuswb256
			#undef packuswb256
		#endif
		#define packuswb256 __builtin_ia32_packuswb256
	#endif

	#if HAS(__builtin_ia32_paddb256)
		#ifdef paddb256
			#undef paddb256
		#endif
		#define paddb256 __builtin_ia32_paddb256
	#endif

	#if HAS(__builtin_ia32_paddd256)
		#ifdef paddd256
			#undef paddd256
		#endif
		#define paddd256 __builtin_ia32_paddd256
	#endif

	#if HAS(__builtin_ia32_paddq256)
		#ifdef paddq256
			#undef paddq256
		#endif
		#define paddq256 __builtin_ia32_paddq256
	#endif

	#if HAS(__builtin_ia32_paddsb256)
		#ifdef paddsb256
			#undef paddsb256
		#endif
		#define paddsb256 __builtin_ia32_paddsb256
	#endif

	#if HAS(__builtin_ia32_paddsw256)
		#ifdef paddsw256
			#undef paddsw256
		#endif
		#define paddsw256 __builtin_ia32_paddsw256
	#endif

	#if HAS(__builtin_ia32_paddusb256)
		#ifdef paddusb256
			#undef paddusb256
		#endif
		#define paddusb256 __builtin_ia32_paddusb256
	#endif

	#if HAS(__builtin_ia32_paddusw256)
		#ifdef paddusw256
			#undef paddusw256
		#endif
		#define paddusw256 __builtin_ia32_paddusw256
	#endif

	#if HAS(__builtin_ia32_paddw256)
		#ifdef paddw256
			#undef paddw256
		#endif
		#define paddw256 __builtin_ia32_paddw256
	#endif

	#if HAS(__builtin_ia32_palignr256)
		#ifdef palignr256
			#undef palignr256
		#endif
		#define palignr256 __builtin_ia32_palignr256
	#endif

	#if HAS(__builtin_ia32_pavgb256)
		#ifdef pavgb256
			#undef pavgb256
		#endif
		#define pavgb256 __builtin_ia32_pavgb256
	#endif

	#if HAS(__builtin_ia32_pavgw256)
		#ifdef pavgw256
			#undef pavgw256
		#endif
		#define pavgw256 __builtin_ia32_pavgw256
	#endif

	#if HAS(__builtin_ia32_pblendd128)
		#ifdef pblendd128
			#undef pblendd128
		#endif
		#define pblendd128 __builtin_ia32_pblendd128
	#endif

	#if HAS(__builtin_ia32_pblendd256)
		#ifdef pblendd256
			#undef pblendd256
		#endif
		#define pblendd256 __builtin_ia32_pblendd256
	#endif

	#if HAS(__builtin_ia32_pblendvb256)
		#ifdef pblendvb256
			#undef pblendvb256
		#endif
		#define pblendvb256 __builtin_ia32_pblendvb256
	#endif

	#if HAS(__builtin_ia32_pblendw256)
		#ifdef pblendw256
			#undef pblendw256
		#endif
		#define pblendw256 __builtin_ia32_pblendw256
	#endif

	#if HAS(__builtin_ia32_pbroadcastb128)
		#ifdef pbroadcastb128
			#undef pbroadcastb128
		#endif
		#define pbroadcastb128 __builtin_ia32_pbroadcastb128
	#endif

	#if HAS(__builtin_ia32_pbroadcastb256)
		#ifdef pbroadcastb256
			#undef pbroadcastb256
		#endif
		#define pbroadcastb256 __builtin_ia32_pbroadcastb256
	#endif

	#if HAS(__builtin_ia32_pbroadcastd128)
		#ifdef pbroadcastd128
			#undef pbroadcastd128
		#endif
		#define pbroadcastd128 __builtin_ia32_pbroadcastd128
	#endif

	#if HAS(__builtin_ia32_pbroadcastd256)
		#ifdef pbroadcastd256
			#undef pbroadcastd256
		#endif
		#define pbroadcastd256 __builtin_ia32_pbroadcastd256
	#endif

	#if HAS(__builtin_ia32_pbroadcastq128)
		#ifdef pbroadcastq128
			#undef pbroadcastq128
		#endif
		#define pbroadcastq128 __builtin_ia32_pbroadcastq128
	#endif

	#if HAS(__builtin_ia32_pbroadcastq256)
		#ifdef pbroadcastq256
			#undef pbroadcastq256
		#endif
		#define pbroadcastq256 __builtin_ia32_pbroadcastq256
	#endif

	#if HAS(__builtin_ia32_pbroadcastw128)
		#ifdef pbroadcastw128
			#undef pbroadcastw128
		#endif
		#define pbroadcastw128 __builtin_ia32_pbroadcastw128
	#endif

	#if HAS(__builtin_ia32_pbroadcastw256)
		#ifdef pbroadcastw256
			#undef pbroadcastw256
		#endif
		#define pbroadcastw256 __builtin_ia32_pbroadcastw256
	#endif

	#if HAS(__builtin_ia32_pcmpeqb256)
		#ifdef pcmpeqb256
			#undef pcmpeqb256
		#endif
		#define pcmpeqb256 __builtin_ia32_pcmpeqb256
	#endif

	#if HAS(__builtin_ia32_pcmpeqd256)
		#ifdef pcmpeqd256
			#undef pcmpeqd256
		#endif
		#define pcmpeqd256 __builtin_ia32_pcmpeqd256
	#endif

	#if HAS(__builtin_ia32_pcmpeqq256)
		#ifdef pcmpeqq256
			#undef pcmpeqq256
		#endif
		#define pcmpeqq256 __builtin_ia32_pcmpeqq256
	#endif

	#if HAS(__builtin_ia32_pcmpeqw256)
		#ifdef pcmpeqw256
			#undef pcmpeqw256
		#endif
		#define pcmpeqw256 __builtin_ia32_pcmpeqw256
	#endif

	#if HAS(__builtin_ia32_pcmpgtb256)
		#ifdef pcmpgtb256
			#undef pcmpgtb256
		#endif
		#define pcmpgtb256 __builtin_ia32_pcmpgtb256
	#endif

	#if HAS(__builtin_ia32_pcmpgtd256)
		#ifdef pcmpgtd256
			#undef pcmpgtd256
		#endif
		#define pcmpgtd256 __builtin_ia32_pcmpgtd256
	#endif

	#if HAS(__builtin_ia32_pcmpgtq256)
		#ifdef pcmpgtq256
			#undef pcmpgtq256
		#endif
		#define pcmpgtq256 __builtin_ia32_pcmpgtq256
	#endif

	#if HAS(__builtin_ia32_pcmpgtw256)
		#ifdef pcmpgtw256
			#undef pcmpgtw256
		#endif
		#define pcmpgtw256 __builtin_ia32_pcmpgtw256
	#endif

	#if HAS(__builtin_ia32_permdf256)
		#ifdef permdf256
			#undef permdf256
		#endif
		#define permdf256 __builtin_ia32_permdf256
	#endif

	#if HAS(__builtin_ia32_permdi256)
		#ifdef permdi256
			#undef permdi256
		#endif
		#define permdi256 __builtin_ia32_permdi256
	#endif

	#if HAS(__builtin_ia32_permti256)
		#ifdef permti256
			#undef permti256
		#endif
		#define permti256 __builtin_ia32_permti256
	#endif

	#if HAS(__builtin_ia32_permvarsf256)
		#ifdef permvarsf256
			#undef permvarsf256
		#endif
		#define permvarsf256 __builtin_ia32_permvarsf256
	#endif

	#if HAS(__builtin_ia32_permvarsi256)
		#ifdef permvarsi256
			#undef permvarsi256
		#endif
		#define permvarsi256 __builtin_ia32_permvarsi256
	#endif

	#if HAS(__builtin_ia32_phaddd256)
		#ifdef phaddd256
			#undef phaddd256
		#endif
		#define phaddd256 __builtin_ia32_phaddd256
	#endif

	#if HAS(__builtin_ia32_phaddsw256)
		#ifdef phaddsw256
			#undef phaddsw256
		#endif
		#define phaddsw256 __builtin_ia32_phaddsw256
	#endif

	#if HAS(__builtin_ia32_phaddw256)
		#ifdef phaddw256
			#undef phaddw256
		#endif
		#define phaddw256 __builtin_ia32_phaddw256
	#endif

	#if HAS(__builtin_ia32_phsubd256)
		#ifdef phsubd256
			#undef phsubd256
		#endif
		#define phsubd256 __builtin_ia32_phsubd256
	#endif

	#if HAS(__builtin_ia32_phsubsw256)
		#ifdef phsubsw256
			#undef phsubsw256
		#endif
		#define phsubsw256 __builtin_ia32_phsubsw256
	#endif

	#if HAS(__builtin_ia32_phsubw256)
		#ifdef phsubw256
			#undef phsubw256
		#endif
		#define phsubw256 __builtin_ia32_phsubw256
	#endif

	#if HAS(__builtin_ia32_pmaddubsw256)
		#ifdef pmaddubsw256
			#undef pmaddubsw256
		#endif
		#define pmaddubsw256 __builtin_ia32_pmaddubsw256
	#endif

	#if HAS(__builtin_ia32_pmaddwd256)
		#ifdef pmaddwd256
			#undef pmaddwd256
		#endif
		#define pmaddwd256 __builtin_ia32_pmaddwd256
	#endif

	#if HAS(__builtin_ia32_pmaxsb256)
		#ifdef pmaxsb256
			#undef pmaxsb256
		#endif
		#define pmaxsb256 __builtin_ia32_pmaxsb256
	#endif

	#if HAS(__builtin_ia32_pmaxsd256)
		#ifdef pmaxsd256
			#undef pmaxsd256
		#endif
		#define pmaxsd256 __builtin_ia32_pmaxsd256
	#endif

	#if HAS(__builtin_ia32_pmaxsw256)
		#ifdef pmaxsw256
			#undef pmaxsw256
		#endif
		#define pmaxsw256 __builtin_ia32_pmaxsw256
	#endif

	#if HAS(__builtin_ia32_pmaxub256)
		#ifdef pmaxub256
			#undef pmaxub256
		#endif
		#define pmaxub256 __builtin_ia32_pmaxub256
	#endif

	#if HAS(__builtin_ia32_pmaxud256)
		#ifdef pmaxud256
			#undef pmaxud256
		#endif
		#define pmaxud256 __builtin_ia32_pmaxud256
	#endif

	#if HAS(__builtin_ia32_pmaxuw256)
		#ifdef pmaxuw256
			#undef pmaxuw256
		#endif
		#define pmaxuw256 __builtin_ia32_pmaxuw256
	#endif

	#if HAS(__builtin_ia32_pminsb256)
		#ifdef pminsb256
			#undef pminsb256
		#endif
		#define pminsb256 __builtin_ia32_pminsb256
	#endif

	#if HAS(__builtin_ia32_pminsd256)
		#ifdef pminsd256
			#undef pminsd256
		#endif
		#define pminsd256 __builtin_ia32_pminsd256
	#endif

	#if HAS(__builtin_ia32_pminsw256)
		#ifdef pminsw256
			#undef pminsw256
		#endif
		#define pminsw256 __builtin_ia32_pminsw256
	#endif

	#if HAS(__builtin_ia32_pminub256)
		#ifdef pminub256
			#undef pminub256
		#endif
		#define pminub256 __builtin_ia32_pminub256
	#endif

	#if HAS(__builtin_ia32_pminud256)
		#ifdef pminud256
			#undef pminud256
		#endif
		#define pminud256 __builtin_ia32_pminud256
	#endif

	#if HAS(__builtin_ia32_pminuw256)
		#ifdef pminuw256
			#undef pminuw256
		#endif
		#define pminuw256 __builtin_ia32_pminuw256
	#endif

	#if HAS(__builtin_ia32_pmovmskb256)
		#ifdef pmovmskb256
			#undef pmovmskb256
		#endif
		#define pmovmskb256 __builtin_ia32_pmovmskb256
	#endif

	#if HAS(__builtin_ia32_pmovsxbd256)
		#ifdef pmovsxbd256
			#undef pmovsxbd256
		#endif
		#define pmovsxbd256 __builtin_ia32_pmovsxbd256
	#endif

	#if HAS(__builtin_ia32_pmovsxbq256)
		#ifdef pmovsxbq256
			#undef pmovsxbq256
		#endif
		#define pmovsxbq256 __builtin_ia32_pmovsxbq256
	#endif

	#if HAS(__builtin_ia32_pmovsxbw256)
		#ifdef pmovsxbw256
			#undef pmovsxbw256
		#endif
		#define pmovsxbw256 __builtin_ia32_pmovsxbw256
	#endif

	#if HAS(__builtin_ia32_pmovsxdq256)
		#ifdef pmovsxdq256
			#undef pmovsxdq256
		#endif
		#define pmovsxdq256 __builtin_ia32_pmovsxdq256
	#endif

	#if HAS(__builtin_ia32_pmovsxwd256)
		#ifdef pmovsxwd256
			#undef pmovsxwd256
		#endif
		#define pmovsxwd256 __builtin_ia32_pmovsxwd256
	#endif

	#if HAS(__builtin_ia32_pmovsxwq256)
		#ifdef pmovsxwq256
			#undef pmovsxwq256
		#endif
		#define pmovsxwq256 __builtin_ia32_pmovsxwq256
	#endif

	#if HAS(__builtin_ia32_pmovzxbd256)
		#ifdef pmovzxbd256
			#undef pmovzxbd256
		#endif
		#define pmovzxbd256 __builtin_ia32_pmovzxbd256
	#endif

	#if HAS(__builtin_ia32_pmovzxbq256)
		#ifdef pmovzxbq256
			#undef pmovzxbq256
		#endif
		#define pmovzxbq256 __builtin_ia32_pmovzxbq256
	#endif

	#if HAS(__builtin_ia32_pmovzxbw256)
		#ifdef pmovzxbw256
			#undef pmovzxbw256
		#endif
		#define pmovzxbw256 __builtin_ia32_pmovzxbw256
	#endif

	#if HAS(__builtin_ia32_pmovzxdq256)
		#ifdef pmovzxdq256
			#undef pmovzxdq256
		#endif
		#define pmovzxdq256 __builtin_ia32_pmovzxdq256
	#endif

	#if HAS(__builtin_ia32_pmovzxwd256)
		#ifdef pmovzxwd256
			#undef pmovzxwd256
		#endif
		#define pmovzxwd256 __builtin_ia32_pmovzxwd256
	#endif

	#if HAS(__builtin_ia32_pmovzxwq256)
		#ifdef pmovzxwq256
			#undef pmovzxwq256
		#endif
		#define pmovzxwq256 __builtin_ia32_pmovzxwq256
	#endif

	#if HAS(__builtin_ia32_pmuldq256)
		#ifdef pmuldq256
			#undef pmuldq256
		#endif
		#define pmuldq256 __builtin_ia32_pmuldq256
	#endif

	#if HAS(__builtin_ia32_pmulhrsw256)
		#ifdef pmulhrsw256
			#undef pmulhrsw256
		#endif
		#define pmulhrsw256 __builtin_ia32_pmulhrsw256
	#endif

	#if HAS(__builtin_ia32_pmulhuw256)
		#ifdef pmulhuw256
			#undef pmulhuw256
		#endif
		#define pmulhuw256 __builtin_ia32_pmulhuw256
	#endif

	#if HAS(__builtin_ia32_pmulhw256)
		#ifdef pmulhw256
			#undef pmulhw256
		#endif
		#define pmulhw256 __builtin_ia32_pmulhw256
	#endif

	#if HAS(__builtin_ia32_pmulld256)
		#ifdef pmulld256
			#undef pmulld256
		#endif
		#define pmulld256 __builtin_ia32_pmulld256
	#endif

	#if HAS(__builtin_ia32_pmullw256)
		#ifdef pmullw256
			#undef pmullw256
		#endif
		#define pmullw256 __builtin_ia32_pmullw256
	#endif

	#if HAS(__builtin_ia32_pmuludq256)
		#ifdef pmuludq256
			#undef pmuludq256
		#endif
		#define pmuludq256 __builtin_ia32_pmuludq256
	#endif

	#if HAS(__builtin_ia32_por256)
		#ifdef por256
			#undef por256
		#endif
		#define por256 __builtin_ia32_por256
	#endif

	#if HAS(__builtin_ia32_psadbw256)
		#ifdef psadbw256
			#undef psadbw256
		#endif
		#define psadbw256 __builtin_ia32_psadbw256
	#endif

	#if HAS(__builtin_ia32_pshufb256)
		#ifdef pshufb256
			#undef pshufb256
		#endif
		#define pshufb256 __builtin_ia32_pshufb256
	#endif

	#if HAS(__builtin_ia32_pshufd256)
		#ifdef pshufd256
			#undef pshufd256
		#endif
		#define pshufd256 __builtin_ia32_pshufd256
	#endif

	#if HAS(__builtin_ia32_pshufhw256)
		#ifdef pshufhw256
			#undef pshufhw256
		#endif
		#define pshufhw256 __builtin_ia32_pshufhw256
	#endif

	#if HAS(__builtin_ia32_pshuflw256)
		#ifdef pshuflw256
			#undef pshuflw256
		#endif
		#define pshuflw256 __builtin_ia32_pshuflw256
	#endif

	#if HAS(__builtin_ia32_psignb256)
		#ifdef psignb256
			#undef psignb256
		#endif
		#define psignb256 __builtin_ia32_psignb256
	#endif

	#if HAS(__builtin_ia32_psignd256)
		#ifdef psignd256
			#undef psignd256
		#endif
		#define psignd256 __builtin_ia32_psignd256
	#endif

	#if HAS(__builtin_ia32_psignw256)
		#ifdef psignw256
			#undef psignw256
		#endif
		#define psignw256 __builtin_ia32_psignw256
	#endif

	#if HAS(__builtin_ia32_pslld256(v8si,v4si))
		#ifdef pslld256(v8si,v4si)
			#undef pslld256(v8si,v4si)
		#endif
		#define pslld256(v8si,v4si) __builtin_ia32_pslld256(v8si,v4si)
	#endif

	#if HAS(__builtin_ia32_pslldi256)
		#ifdef pslldi256
			#undef pslldi256
		#endif
		#define pslldi256 __builtin_ia32_pslldi256
	#endif

	#if HAS(__builtin_ia32_pslldqi256)
		#ifdef pslldqi256
			#undef pslldqi256
		#endif
		#define pslldqi256 __builtin_ia32_pslldqi256
	#endif

	#if HAS(__builtin_ia32_psllq256(v4di,v2di))
		#ifdef psllq256(v4di,v2di)
			#undef psllq256(v4di,v2di)
		#endif
		#define psllq256(v4di,v2di) __builtin_ia32_psllq256(v4di,v2di)
	#endif

	#if HAS(__builtin_ia32_psllqi256)
		#ifdef psllqi256
			#undef psllqi256
		#endif
		#define psllqi256 __builtin_ia32_psllqi256
	#endif

	#if HAS(__builtin_ia32_psllv2di)
		#ifdef psllv2di
			#undef psllv2di
		#endif
		#define psllv2di __builtin_ia32_psllv2di
	#endif

	#if HAS(__builtin_ia32_psllv4di)
		#ifdef psllv4di
			#undef psllv4di
		#endif
		#define psllv4di __builtin_ia32_psllv4di
	#endif

	#if HAS(__builtin_ia32_psllv4si)
		#ifdef psllv4si
			#undef psllv4si
		#endif
		#define psllv4si __builtin_ia32_psllv4si
	#endif

	#if HAS(__builtin_ia32_psllv8si)
		#ifdef psllv8si
			#undef psllv8si
		#endif
		#define psllv8si __builtin_ia32_psllv8si
	#endif

	#if HAS(__builtin_ia32_psllw256(v16hi,v8hi))
		#ifdef psllw256(v16hi,v8hi)
			#undef psllw256(v16hi,v8hi)
		#endif
		#define psllw256(v16hi,v8hi) __builtin_ia32_psllw256(v16hi,v8hi)
	#endif

	#if HAS(__builtin_ia32_psllwi256)
		#ifdef psllwi256
			#undef psllwi256
		#endif
		#define psllwi256 __builtin_ia32_psllwi256
	#endif

	#if HAS(__builtin_ia32_psrad256)
		#ifdef psrad256
			#undef psrad256
		#endif
		#define psrad256 __builtin_ia32_psrad256
	#endif

	#if HAS(__builtin_ia32_psradi256)
		#ifdef psradi256
			#undef psradi256
		#endif
		#define psradi256 __builtin_ia32_psradi256
	#endif

	#if HAS(__builtin_ia32_psrav4si)
		#ifdef psrav4si
			#undef psrav4si
		#endif
		#define psrav4si __builtin_ia32_psrav4si
	#endif

	#if HAS(__builtin_ia32_psrav8si)
		#ifdef psrav8si
			#undef psrav8si
		#endif
		#define psrav8si __builtin_ia32_psrav8si
	#endif

	#if HAS(__builtin_ia32_psraw256)
		#ifdef psraw256
			#undef psraw256
		#endif
		#define psraw256 __builtin_ia32_psraw256
	#endif

	#if HAS(__builtin_ia32_psrawi256)
		#ifdef psrawi256
			#undef psrawi256
		#endif
		#define psrawi256 __builtin_ia32_psrawi256
	#endif

	#if HAS(__builtin_ia32_psrld256)
		#ifdef psrld256
			#undef psrld256
		#endif
		#define psrld256 __builtin_ia32_psrld256
	#endif

	#if HAS(__builtin_ia32_psrldi256)
		#ifdef psrldi256
			#undef psrldi256
		#endif
		#define psrldi256 __builtin_ia32_psrldi256
	#endif

	#if HAS(__builtin_ia32_psrldqi256)
		#ifdef psrldqi256
			#undef psrldqi256
		#endif
		#define psrldqi256 __builtin_ia32_psrldqi256
	#endif

	#if HAS(__builtin_ia32_psrlq256(v4di,v2di))
		#ifdef psrlq256(v4di,v2di)
			#undef psrlq256(v4di,v2di)
		#endif
		#define psrlq256(v4di,v2di) __builtin_ia32_psrlq256(v4di,v2di)
	#endif

	#if HAS(__builtin_ia32_psrlqi256)
		#ifdef psrlqi256
			#undef psrlqi256
		#endif
		#define psrlqi256 __builtin_ia32_psrlqi256
	#endif

	#if HAS(__builtin_ia32_psrlv2di)
		#ifdef psrlv2di
			#undef psrlv2di
		#endif
		#define psrlv2di __builtin_ia32_psrlv2di
	#endif

	#if HAS(__builtin_ia32_psrlv4di)
		#ifdef psrlv4di
			#undef psrlv4di
		#endif
		#define psrlv4di __builtin_ia32_psrlv4di
	#endif

	#if HAS(__builtin_ia32_psrlv4si)
		#ifdef psrlv4si
			#undef psrlv4si
		#endif
		#define psrlv4si __builtin_ia32_psrlv4si
	#endif

	#if HAS(__builtin_ia32_psrlv8si)
		#ifdef psrlv8si
			#undef psrlv8si
		#endif
		#define psrlv8si __builtin_ia32_psrlv8si
	#endif

	#if HAS(__builtin_ia32_psrlw256)
		#ifdef psrlw256
			#undef psrlw256
		#endif
		#define psrlw256 __builtin_ia32_psrlw256
	#endif

	#if HAS(__builtin_ia32_psrlwi256)
		#ifdef psrlwi256
			#undef psrlwi256
		#endif
		#define psrlwi256 __builtin_ia32_psrlwi256
	#endif

	#if HAS(__builtin_ia32_psubb256)
		#ifdef psubb256
			#undef psubb256
		#endif
		#define psubb256 __builtin_ia32_psubb256
	#endif

	#if HAS(__builtin_ia32_psubd256)
		#ifdef psubd256
			#undef psubd256
		#endif
		#define psubd256 __builtin_ia32_psubd256
	#endif

	#if HAS(__builtin_ia32_psubq256)
		#ifdef psubq256
			#undef psubq256
		#endif
		#define psubq256 __builtin_ia32_psubq256
	#endif

	#if HAS(__builtin_ia32_psubsb256)
		#ifdef psubsb256
			#undef psubsb256
		#endif
		#define psubsb256 __builtin_ia32_psubsb256
	#endif

	#if HAS(__builtin_ia32_psubsw256)
		#ifdef psubsw256
			#undef psubsw256
		#endif
		#define psubsw256 __builtin_ia32_psubsw256
	#endif

	#if HAS(__builtin_ia32_psubusb256)
		#ifdef psubusb256
			#undef psubusb256
		#endif
		#define psubusb256 __builtin_ia32_psubusb256
	#endif

	#if HAS(__builtin_ia32_psubusw256)
		#ifdef psubusw256
			#undef psubusw256
		#endif
		#define psubusw256 __builtin_ia32_psubusw256
	#endif

	#if HAS(__builtin_ia32_psubw256)
		#ifdef psubw256
			#undef psubw256
		#endif
		#define psubw256 __builtin_ia32_psubw256
	#endif

	#if HAS(__builtin_ia32_punpckhbw256)
		#ifdef punpckhbw256
			#undef punpckhbw256
		#endif
		#define punpckhbw256 __builtin_ia32_punpckhbw256
	#endif

	#if HAS(__builtin_ia32_punpckhdq256)
		#ifdef punpckhdq256
			#undef punpckhdq256
		#endif
		#define punpckhdq256 __builtin_ia32_punpckhdq256
	#endif

	#if HAS(__builtin_ia32_punpckhqdq256)
		#ifdef punpckhqdq256
			#undef punpckhqdq256
		#endif
		#define punpckhqdq256 __builtin_ia32_punpckhqdq256
	#endif

	#if HAS(__builtin_ia32_punpckhwd256)
		#ifdef punpckhwd256
			#undef punpckhwd256
		#endif
		#define punpckhwd256 __builtin_ia32_punpckhwd256
	#endif

	#if HAS(__builtin_ia32_punpcklbw256)
		#ifdef punpcklbw256
			#undef punpcklbw256
		#endif
		#define punpcklbw256 __builtin_ia32_punpcklbw256
	#endif

	#if HAS(__builtin_ia32_punpckldq256)
		#ifdef punpckldq256
			#undef punpckldq256
		#endif
		#define punpckldq256 __builtin_ia32_punpckldq256
	#endif

	#if HAS(__builtin_ia32_punpcklqdq256)
		#ifdef punpcklqdq256
			#undef punpcklqdq256
		#endif
		#define punpcklqdq256 __builtin_ia32_punpcklqdq256
	#endif

	#if HAS(__builtin_ia32_punpcklwd256)
		#ifdef punpcklwd256
			#undef punpcklwd256
		#endif
		#define punpcklwd256 __builtin_ia32_punpcklwd256
	#endif

	#if HAS(__builtin_ia32_pxor256)
		#ifdef pxor256
			#undef pxor256
		#endif
		#define pxor256 __builtin_ia32_pxor256
	#endif

	#if HAS(__builtin_ia32_vbroadcastsd_pd256)
		#ifdef vbroadcastsd_pd256
			#undef vbroadcastsd_pd256
		#endif
		#define vbroadcastsd_pd256 __builtin_ia32_vbroadcastsd_pd256
	#endif

	#if HAS(__builtin_ia32_vbroadcastsi256)
		#ifdef vbroadcastsi256
			#undef vbroadcastsi256
		#endif
		#define vbroadcastsi256 __builtin_ia32_vbroadcastsi256
	#endif

	#if HAS(__builtin_ia32_vbroadcastss_ps)
		#ifdef vbroadcastss_ps
			#undef vbroadcastss_ps
		#endif
		#define vbroadcastss_ps __builtin_ia32_vbroadcastss_ps
	#endif

	#if HAS(__builtin_ia32_vbroadcastss_ps256)
		#ifdef vbroadcastss_ps256
			#undef vbroadcastss_ps256
		#endif
		#define vbroadcastss_ps256 __builtin_ia32_vbroadcastss_ps256
	#endif

#endif

#if defined __AES__

	#if HAS(__builtin_ia32_aesdec128)
		#ifdef aesdec128
			#undef aesdec128
		#endif
		#define aesdec128 __builtin_ia32_aesdec128
	#endif

	#if HAS(__builtin_ia32_aesdeclast128)
		#ifdef aesdeclast128
			#undef aesdeclast128
		#endif
		#define aesdeclast128 __builtin_ia32_aesdeclast128
	#endif

	#if HAS(__builtin_ia32_aesenc128)
		#ifdef aesenc128
			#undef aesenc128
		#endif
		#define aesenc128 __builtin_ia32_aesenc128
	#endif

	#if HAS(__builtin_ia32_aesenclast128)
		#ifdef aesenclast128
			#undef aesenclast128
		#endif
		#define aesenclast128 __builtin_ia32_aesenclast128
	#endif

	#if HAS(__builtin_ia32_aesimc128)
		#ifdef aesimc128
			#undef aesimc128
		#endif
		#define aesimc128 __builtin_ia32_aesimc128
	#endif

	#if HAS(__builtin_ia32_aeskeygenassist128)
		#ifdef aeskeygenassist128
			#undef aeskeygenassist128
		#endif
		#define aeskeygenassist128 __builtin_ia32_aeskeygenassist128
	#endif

#endif

#if defined __PCLMUL__

	#if HAS(__builtin_ia32_pclmulqdq128)
		#ifdef pclmulqdq128
			#undef pclmulqdq128
		#endif
		#define pclmulqdq128 __builtin_ia32_pclmulqdq128
		#ifdef pclmulqdq
			#undef pclmulqdq
		#endif
		#define pclmulqdq __builtin_ia32_pclmulqdq128
	#endif

#endif

#if defined __FSGSBASE__

	#if HAS(__builtin_ia32_rdfsbase32)
		#ifdef rdfsbase32
			#undef rdfsbase32
		#endif
		#define rdfsbase32 __builtin_ia32_rdfsbase32
	#endif

	#if HAS(__builtin_ia32_rdfsbase64)
		#ifdef rdfsbase64
			#undef rdfsbase64
		#endif
		#define rdfsbase64 __builtin_ia32_rdfsbase64
	#endif

	#if HAS(__builtin_ia32_rdgsbase32)
		#ifdef rdgsbase32
			#undef rdgsbase32
		#endif
		#define rdgsbase32 __builtin_ia32_rdgsbase32
	#endif

	#if HAS(__builtin_ia32_rdgsbase64)
		#ifdef rdgsbase64
			#undef rdgsbase64
		#endif
		#define rdgsbase64 __builtin_ia32_rdgsbase64
	#endif

	#if HAS(_writefsbase_u32)
		#ifdef writefsbase_u32
			#undef writefsbase_u32
		#endif
		#define writefsbase_u32 _writefsbase_u32
	#endif

	#if HAS(_writefsbase_u64)
		#ifdef writefsbase_u64
			#undef writefsbase_u64
		#endif
		#define writefsbase_u64 _writefsbase_u64
	#endif

	#if HAS(_writegsbase_u32)
		#ifdef writegsbase_u32
			#undef writegsbase_u32
		#endif
		#define writegsbase_u32 _writegsbase_u32
	#endif

	#if HAS(_writegsbase_u64)
		#ifdef writegsbase_u64
			#undef writegsbase_u64
		#endif
		#define writegsbase_u64 _writegsbase_u64
	#endif

#endif

#if defined __RDRND__

	#if HAS(__builtin_ia32_rdrand16_step)
		#ifdef rdrand16_step
			#undef rdrand16_step
		#endif
		#define rdrand16_step __builtin_ia32_rdrand16_step
	#endif

	#if HAS(__builtin_ia32_rdrand32_step)
		#ifdef rdrand32_step
			#undef rdrand32_step
		#endif
		#define rdrand32_step __builtin_ia32_rdrand32_step
	#endif

	#if HAS(__builtin_ia32_rdrand64_step)
		#ifdef rdrand64_step
			#undef rdrand64_step
		#endif
		#define rdrand64_step __builtin_ia32_rdrand64_step
	#endif

#endif

#if defined __XOP__

	#if HAS(__builtin_ia32_vfrczpd)
		#ifdef vfrczpd
			#undef vfrczpd
		#endif
		#define vfrczpd __builtin_ia32_vfrczpd
	#endif

	#if HAS(__builtin_ia32_vfrczpd256)
		#ifdef vfrczpd256
			#undef vfrczpd256
		#endif
		#define vfrczpd256 __builtin_ia32_vfrczpd256
	#endif

	#if HAS(__builtin_ia32_vfrczps)
		#ifdef vfrczps
			#undef vfrczps
		#endif
		#define vfrczps __builtin_ia32_vfrczps
	#endif

	#if HAS(__builtin_ia32_vfrczps256)
		#ifdef vfrczps256
			#undef vfrczps256
		#endif
		#define vfrczps256 __builtin_ia32_vfrczps256
	#endif

	#if HAS(__builtin_ia32_vfrczsd)
		#ifdef vfrczsd
			#undef vfrczsd
		#endif
		#define vfrczsd __builtin_ia32_vfrczsd
	#endif

	#if HAS(__builtin_ia32_vfrczss)
		#ifdef vfrczss
			#undef vfrczss
		#endif
		#define vfrczss __builtin_ia32_vfrczss
	#endif

	#if HAS(__builtin_ia32_vpcmov)
		#ifdef vpcmov
			#undef vpcmov
		#endif
		#define vpcmov __builtin_ia32_vpcmov
	#endif

	#if HAS(__builtin_ia32_vpcmov_v16hi256)
		#ifdef vpcmov_v16hi256
			#undef vpcmov_v16hi256
		#endif
		#define vpcmov_v16hi256 __builtin_ia32_vpcmov_v16hi256
	#endif

	#if HAS(__builtin_ia32_vpcmov_v16qi)
		#ifdef vpcmov_v16qi
			#undef vpcmov_v16qi
		#endif
		#define vpcmov_v16qi __builtin_ia32_vpcmov_v16qi
	#endif

	#if HAS(__builtin_ia32_vpcmov_v2df)
		#ifdef vpcmov_v2df
			#undef vpcmov_v2df
		#endif
		#define vpcmov_v2df __builtin_ia32_vpcmov_v2df
	#endif

	#if HAS(__builtin_ia32_vpcmov_v2di)
		#ifdef vpcmov_v2di
			#undef vpcmov_v2di
		#endif
		#define vpcmov_v2di __builtin_ia32_vpcmov_v2di
	#endif

	#if HAS(__builtin_ia32_vpcmov_v32qi256)
		#ifdef vpcmov_v32qi256
			#undef vpcmov_v32qi256
		#endif
		#define vpcmov_v32qi256 __builtin_ia32_vpcmov_v32qi256
	#endif

	#if HAS(__builtin_ia32_vpcmov_v4df256)
		#ifdef vpcmov_v4df256
			#undef vpcmov_v4df256
		#endif
		#define vpcmov_v4df256 __builtin_ia32_vpcmov_v4df256
	#endif

	#if HAS(__builtin_ia32_vpcmov_v4di256)
		#ifdef vpcmov_v4di256
			#undef vpcmov_v4di256
		#endif
		#define vpcmov_v4di256 __builtin_ia32_vpcmov_v4di256
	#endif

	#if HAS(__builtin_ia32_vpcmov_v4sf)
		#ifdef vpcmov_v4sf
			#undef vpcmov_v4sf
		#endif
		#define vpcmov_v4sf __builtin_ia32_vpcmov_v4sf
	#endif

	#if HAS(__builtin_ia32_vpcmov_v4si)
		#ifdef vpcmov_v4si
			#undef vpcmov_v4si
		#endif
		#define vpcmov_v4si __builtin_ia32_vpcmov_v4si
	#endif

	#if HAS(__builtin_ia32_vpcmov_v8hi)
		#ifdef vpcmov_v8hi
			#undef vpcmov_v8hi
		#endif
		#define vpcmov_v8hi __builtin_ia32_vpcmov_v8hi
	#endif

	#if HAS(__builtin_ia32_vpcmov_v8sf256)
		#ifdef vpcmov_v8sf256
			#undef vpcmov_v8sf256
		#endif
		#define vpcmov_v8sf256 __builtin_ia32_vpcmov_v8sf256
	#endif

	#if HAS(__builtin_ia32_vpcmov_v8si256)
		#ifdef vpcmov_v8si256
			#undef vpcmov_v8si256
		#endif
		#define vpcmov_v8si256 __builtin_ia32_vpcmov_v8si256
	#endif

	#if HAS(__builtin_ia32_vpcomeqb)
		#ifdef vpcomeqb
			#undef vpcomeqb
		#endif
		#define vpcomeqb __builtin_ia32_vpcomeqb
	#endif

	#if HAS(__builtin_ia32_vpcomeqd)
		#ifdef vpcomeqd
			#undef vpcomeqd
		#endif
		#define vpcomeqd __builtin_ia32_vpcomeqd
	#endif

	#if HAS(__builtin_ia32_vpcomeqq)
		#ifdef vpcomeqq
			#undef vpcomeqq
		#endif
		#define vpcomeqq __builtin_ia32_vpcomeqq
	#endif

	#if HAS(__builtin_ia32_vpcomequb)
		#ifdef vpcomequb
			#undef vpcomequb
		#endif
		#define vpcomequb __builtin_ia32_vpcomequb
	#endif

	#if HAS(__builtin_ia32_vpcomequd)
		#ifdef vpcomequd
			#undef vpcomequd
		#endif
		#define vpcomequd __builtin_ia32_vpcomequd
	#endif

	#if HAS(__builtin_ia32_vpcomequq)
		#ifdef vpcomequq
			#undef vpcomequq
		#endif
		#define vpcomequq __builtin_ia32_vpcomequq
	#endif

	#if HAS(__builtin_ia32_vpcomequw)
		#ifdef vpcomequw
			#undef vpcomequw
		#endif
		#define vpcomequw __builtin_ia32_vpcomequw
	#endif

	#if HAS(__builtin_ia32_vpcomeqw)
		#ifdef vpcomeqw
			#undef vpcomeqw
		#endif
		#define vpcomeqw __builtin_ia32_vpcomeqw
	#endif

	#if HAS(__builtin_ia32_vpcomeqw)
		#ifdef vpcomeqw
			#undef vpcomeqw
		#endif
		#define vpcomeqw __builtin_ia32_vpcomeqw
	#endif

	#if HAS(__builtin_ia32_vpcomfalseb)
		#ifdef vpcomfalseb
			#undef vpcomfalseb
		#endif
		#define vpcomfalseb __builtin_ia32_vpcomfalseb
	#endif

	#if HAS(__builtin_ia32_vpcomfalsed)
		#ifdef vpcomfalsed
			#undef vpcomfalsed
		#endif
		#define vpcomfalsed __builtin_ia32_vpcomfalsed
	#endif

	#if HAS(__builtin_ia32_vpcomfalseq)
		#ifdef vpcomfalseq
			#undef vpcomfalseq
		#endif
		#define vpcomfalseq __builtin_ia32_vpcomfalseq
	#endif

	#if HAS(__builtin_ia32_vpcomfalseub)
		#ifdef vpcomfalseub
			#undef vpcomfalseub
		#endif
		#define vpcomfalseub __builtin_ia32_vpcomfalseub
	#endif

	#if HAS(__builtin_ia32_vpcomfalseud)
		#ifdef vpcomfalseud
			#undef vpcomfalseud
		#endif
		#define vpcomfalseud __builtin_ia32_vpcomfalseud
	#endif

	#if HAS(__builtin_ia32_vpcomfalseuq)
		#ifdef vpcomfalseuq
			#undef vpcomfalseuq
		#endif
		#define vpcomfalseuq __builtin_ia32_vpcomfalseuq
	#endif

	#if HAS(__builtin_ia32_vpcomfalseuw)
		#ifdef vpcomfalseuw
			#undef vpcomfalseuw
		#endif
		#define vpcomfalseuw __builtin_ia32_vpcomfalseuw
	#endif

	#if HAS(__builtin_ia32_vpcomfalsew)
		#ifdef vpcomfalsew
			#undef vpcomfalsew
		#endif
		#define vpcomfalsew __builtin_ia32_vpcomfalsew
	#endif

	#if HAS(__builtin_ia32_vpcomgeb)
		#ifdef vpcomgeb
			#undef vpcomgeb
		#endif
		#define vpcomgeb __builtin_ia32_vpcomgeb
	#endif

	#if HAS(__builtin_ia32_vpcomged)
		#ifdef vpcomged
			#undef vpcomged
		#endif
		#define vpcomged __builtin_ia32_vpcomged
	#endif

	#if HAS(__builtin_ia32_vpcomgeq)
		#ifdef vpcomgeq
			#undef vpcomgeq
		#endif
		#define vpcomgeq __builtin_ia32_vpcomgeq
	#endif

	#if HAS(__builtin_ia32_vpcomgeub)
		#ifdef vpcomgeub
			#undef vpcomgeub
		#endif
		#define vpcomgeub __builtin_ia32_vpcomgeub
	#endif

	#if HAS(__builtin_ia32_vpcomgeud)
		#ifdef vpcomgeud
			#undef vpcomgeud
		#endif
		#define vpcomgeud __builtin_ia32_vpcomgeud
	#endif

	#if HAS(__builtin_ia32_vpcomgeuq)
		#ifdef vpcomgeuq
			#undef vpcomgeuq
		#endif
		#define vpcomgeuq __builtin_ia32_vpcomgeuq
	#endif

	#if HAS(__builtin_ia32_vpcomgeuw)
		#ifdef vpcomgeuw
			#undef vpcomgeuw
		#endif
		#define vpcomgeuw __builtin_ia32_vpcomgeuw
	#endif

	#if HAS(__builtin_ia32_vpcomgew)
		#ifdef vpcomgew
			#undef vpcomgew
		#endif
		#define vpcomgew __builtin_ia32_vpcomgew
	#endif

	#if HAS(__builtin_ia32_vpcomgtb)
		#ifdef vpcomgtb
			#undef vpcomgtb
		#endif
		#define vpcomgtb __builtin_ia32_vpcomgtb
	#endif

	#if HAS(__builtin_ia32_vpcomgtd)
		#ifdef vpcomgtd
			#undef vpcomgtd
		#endif
		#define vpcomgtd __builtin_ia32_vpcomgtd
	#endif

	#if HAS(__builtin_ia32_vpcomgtq)
		#ifdef vpcomgtq
			#undef vpcomgtq
		#endif
		#define vpcomgtq __builtin_ia32_vpcomgtq
	#endif

	#if HAS(__builtin_ia32_vpcomgtub)
		#ifdef vpcomgtub
			#undef vpcomgtub
		#endif
		#define vpcomgtub __builtin_ia32_vpcomgtub
	#endif

	#if HAS(__builtin_ia32_vpcomgtud)
		#ifdef vpcomgtud
			#undef vpcomgtud
		#endif
		#define vpcomgtud __builtin_ia32_vpcomgtud
	#endif

	#if HAS(__builtin_ia32_vpcomgtuq)
		#ifdef vpcomgtuq
			#undef vpcomgtuq
		#endif
		#define vpcomgtuq __builtin_ia32_vpcomgtuq
	#endif

	#if HAS(__builtin_ia32_vpcomgtuw)
		#ifdef vpcomgtuw
			#undef vpcomgtuw
		#endif
		#define vpcomgtuw __builtin_ia32_vpcomgtuw
	#endif

	#if HAS(__builtin_ia32_vpcomgtw)
		#ifdef vpcomgtw
			#undef vpcomgtw
		#endif
		#define vpcomgtw __builtin_ia32_vpcomgtw
	#endif

	#if HAS(__builtin_ia32_vpcomleb)
		#ifdef vpcomleb
			#undef vpcomleb
		#endif
		#define vpcomleb __builtin_ia32_vpcomleb
	#endif

	#if HAS(__builtin_ia32_vpcomled)
		#ifdef vpcomled
			#undef vpcomled
		#endif
		#define vpcomled __builtin_ia32_vpcomled
	#endif

	#if HAS(__builtin_ia32_vpcomleq)
		#ifdef vpcomleq
			#undef vpcomleq
		#endif
		#define vpcomleq __builtin_ia32_vpcomleq
	#endif

	#if HAS(__builtin_ia32_vpcomleub)
		#ifdef vpcomleub
			#undef vpcomleub
		#endif
		#define vpcomleub __builtin_ia32_vpcomleub
	#endif

	#if HAS(__builtin_ia32_vpcomleud)
		#ifdef vpcomleud
			#undef vpcomleud
		#endif
		#define vpcomleud __builtin_ia32_vpcomleud
	#endif

	#if HAS(__builtin_ia32_vpcomleuq)
		#ifdef vpcomleuq
			#undef vpcomleuq
		#endif
		#define vpcomleuq __builtin_ia32_vpcomleuq
	#endif

	#if HAS(__builtin_ia32_vpcomleuw)
		#ifdef vpcomleuw
			#undef vpcomleuw
		#endif
		#define vpcomleuw __builtin_ia32_vpcomleuw
	#endif

	#if HAS(__builtin_ia32_vpcomlew)
		#ifdef vpcomlew
			#undef vpcomlew
		#endif
		#define vpcomlew __builtin_ia32_vpcomlew
	#endif

	#if HAS(__builtin_ia32_vpcomltb)
		#ifdef vpcomltb
			#undef vpcomltb
		#endif
		#define vpcomltb __builtin_ia32_vpcomltb
	#endif

	#if HAS(__builtin_ia32_vpcomltd)
		#ifdef vpcomltd
			#undef vpcomltd
		#endif
		#define vpcomltd __builtin_ia32_vpcomltd
	#endif

	#if HAS(__builtin_ia32_vpcomltq)
		#ifdef vpcomltq
			#undef vpcomltq
		#endif
		#define vpcomltq __builtin_ia32_vpcomltq
	#endif

	#if HAS(__builtin_ia32_vpcomltub)
		#ifdef vpcomltub
			#undef vpcomltub
		#endif
		#define vpcomltub __builtin_ia32_vpcomltub
	#endif

	#if HAS(__builtin_ia32_vpcomltud)
		#ifdef vpcomltud
			#undef vpcomltud
		#endif
		#define vpcomltud __builtin_ia32_vpcomltud
	#endif

	#if HAS(__builtin_ia32_vpcomltuq)
		#ifdef vpcomltuq
			#undef vpcomltuq
		#endif
		#define vpcomltuq __builtin_ia32_vpcomltuq
	#endif

	#if HAS(__builtin_ia32_vpcomltuw)
		#ifdef vpcomltuw
			#undef vpcomltuw
		#endif
		#define vpcomltuw __builtin_ia32_vpcomltuw
	#endif

	#if HAS(__builtin_ia32_vpcomltw)
		#ifdef vpcomltw
			#undef vpcomltw
		#endif
		#define vpcomltw __builtin_ia32_vpcomltw
	#endif

	#if HAS(__builtin_ia32_vpcomneb)
		#ifdef vpcomneb
			#undef vpcomneb
		#endif
		#define vpcomneb __builtin_ia32_vpcomneb
	#endif

	#if HAS(__builtin_ia32_vpcomned)
		#ifdef vpcomned
			#undef vpcomned
		#endif
		#define vpcomned __builtin_ia32_vpcomned
	#endif

	#if HAS(__builtin_ia32_vpcomneq)
		#ifdef vpcomneq
			#undef vpcomneq
		#endif
		#define vpcomneq __builtin_ia32_vpcomneq
	#endif

	#if HAS(__builtin_ia32_vpcomneub)
		#ifdef vpcomneub
			#undef vpcomneub
		#endif
		#define vpcomneub __builtin_ia32_vpcomneub
	#endif

	#if HAS(__builtin_ia32_vpcomneud)
		#ifdef vpcomneud
			#undef vpcomneud
		#endif
		#define vpcomneud __builtin_ia32_vpcomneud
	#endif

	#if HAS(__builtin_ia32_vpcomneuq)
		#ifdef vpcomneuq
			#undef vpcomneuq
		#endif
		#define vpcomneuq __builtin_ia32_vpcomneuq
	#endif

	#if HAS(__builtin_ia32_vpcomneuw)
		#ifdef vpcomneuw
			#undef vpcomneuw
		#endif
		#define vpcomneuw __builtin_ia32_vpcomneuw
	#endif

	#if HAS(__builtin_ia32_vpcomnew)
		#ifdef vpcomnew
			#undef vpcomnew
		#endif
		#define vpcomnew __builtin_ia32_vpcomnew
	#endif

	#if HAS(__builtin_ia32_vpcomtrueb)
		#ifdef vpcomtrueb
			#undef vpcomtrueb
		#endif
		#define vpcomtrueb __builtin_ia32_vpcomtrueb
	#endif

	#if HAS(__builtin_ia32_vpcomtrued)
		#ifdef vpcomtrued
			#undef vpcomtrued
		#endif
		#define vpcomtrued __builtin_ia32_vpcomtrued
	#endif

	#if HAS(__builtin_ia32_vpcomtrueq)
		#ifdef vpcomtrueq
			#undef vpcomtrueq
		#endif
		#define vpcomtrueq __builtin_ia32_vpcomtrueq
	#endif

	#if HAS(__builtin_ia32_vpcomtrueub)
		#ifdef vpcomtrueub
			#undef vpcomtrueub
		#endif
		#define vpcomtrueub __builtin_ia32_vpcomtrueub
	#endif

	#if HAS(__builtin_ia32_vpcomtrueud)
		#ifdef vpcomtrueud
			#undef vpcomtrueud
		#endif
		#define vpcomtrueud __builtin_ia32_vpcomtrueud
	#endif

	#if HAS(__builtin_ia32_vpcomtrueuq)
		#ifdef vpcomtrueuq
			#undef vpcomtrueuq
		#endif
		#define vpcomtrueuq __builtin_ia32_vpcomtrueuq
	#endif

	#if HAS(__builtin_ia32_vpcomtrueuw)
		#ifdef vpcomtrueuw
			#undef vpcomtrueuw
		#endif
		#define vpcomtrueuw __builtin_ia32_vpcomtrueuw
	#endif

	#if HAS(__builtin_ia32_vpcomtruew)
		#ifdef vpcomtruew
			#undef vpcomtruew
		#endif
		#define vpcomtruew __builtin_ia32_vpcomtruew
	#endif

	#if HAS(__builtin_ia32_vphaddbd)
		#ifdef vphaddbd
			#undef vphaddbd
		#endif
		#define vphaddbd __builtin_ia32_vphaddbd
	#endif

	#if HAS(__builtin_ia32_vphaddbq)
		#ifdef vphaddbq
			#undef vphaddbq
		#endif
		#define vphaddbq __builtin_ia32_vphaddbq
	#endif

	#if HAS(__builtin_ia32_vphaddbw)
		#ifdef vphaddbw
			#undef vphaddbw
		#endif
		#define vphaddbw __builtin_ia32_vphaddbw
	#endif

	#if HAS(__builtin_ia32_vphadddq)
		#ifdef vphadddq
			#undef vphadddq
		#endif
		#define vphadddq __builtin_ia32_vphadddq
	#endif

	#if HAS(__builtin_ia32_vphaddubd)
		#ifdef vphaddubd
			#undef vphaddubd
		#endif
		#define vphaddubd __builtin_ia32_vphaddubd
	#endif

	#if HAS(__builtin_ia32_vphaddubq)
		#ifdef vphaddubq
			#undef vphaddubq
		#endif
		#define vphaddubq __builtin_ia32_vphaddubq
	#endif

	#if HAS(__builtin_ia32_vphaddubw)
		#ifdef vphaddubw
			#undef vphaddubw
		#endif
		#define vphaddubw __builtin_ia32_vphaddubw
	#endif

	#if HAS(__builtin_ia32_vphaddudq)
		#ifdef vphaddudq
			#undef vphaddudq
		#endif
		#define vphaddudq __builtin_ia32_vphaddudq
	#endif

	#if HAS(__builtin_ia32_vphadduwd)
		#ifdef vphadduwd
			#undef vphadduwd
		#endif
		#define vphadduwd __builtin_ia32_vphadduwd
	#endif

	#if HAS(__builtin_ia32_vphadduwq)
		#ifdef vphadduwq
			#undef vphadduwq
		#endif
		#define vphadduwq __builtin_ia32_vphadduwq
	#endif

	#if HAS(__builtin_ia32_vphaddwd)
		#ifdef vphaddwd
			#undef vphaddwd
		#endif
		#define vphaddwd __builtin_ia32_vphaddwd
	#endif

	#if HAS(__builtin_ia32_vphaddwq)
		#ifdef vphaddwq
			#undef vphaddwq
		#endif
		#define vphaddwq __builtin_ia32_vphaddwq
	#endif

	#if HAS(__builtin_ia32_vphsubbw)
		#ifdef vphsubbw
			#undef vphsubbw
		#endif
		#define vphsubbw __builtin_ia32_vphsubbw
	#endif

	#if HAS(__builtin_ia32_vphsubdq)
		#ifdef vphsubdq
			#undef vphsubdq
		#endif
		#define vphsubdq __builtin_ia32_vphsubdq
	#endif

	#if HAS(__builtin_ia32_vphsubwd)
		#ifdef vphsubwd
			#undef vphsubwd
		#endif
		#define vphsubwd __builtin_ia32_vphsubwd
	#endif

	#if HAS(__builtin_ia32_vpmacsdd)
		#ifdef vpmacsdd
			#undef vpmacsdd
		#endif
		#define vpmacsdd __builtin_ia32_vpmacsdd
	#endif

	#if HAS(__builtin_ia32_vpmacsdqh)
		#ifdef vpmacsdqh
			#undef vpmacsdqh
		#endif
		#define vpmacsdqh __builtin_ia32_vpmacsdqh
	#endif

	#if HAS(__builtin_ia32_vpmacsdql)
		#ifdef vpmacsdql
			#undef vpmacsdql
		#endif
		#define vpmacsdql __builtin_ia32_vpmacsdql
	#endif

	#if HAS(__builtin_ia32_vpmacssdd)
		#ifdef vpmacssdd
			#undef vpmacssdd
		#endif
		#define vpmacssdd __builtin_ia32_vpmacssdd
	#endif

	#if HAS(__builtin_ia32_vpmacssdqh)
		#ifdef vpmacssdqh
			#undef vpmacssdqh
		#endif
		#define vpmacssdqh __builtin_ia32_vpmacssdqh
	#endif

	#if HAS(__builtin_ia32_vpmacssdql)
		#ifdef vpmacssdql
			#undef vpmacssdql
		#endif
		#define vpmacssdql __builtin_ia32_vpmacssdql
	#endif

	#if HAS(__builtin_ia32_vpmacsswd)
		#ifdef vpmacsswd
			#undef vpmacsswd
		#endif
		#define vpmacsswd __builtin_ia32_vpmacsswd
	#endif

	#if HAS(__builtin_ia32_vpmacssww)
		#ifdef vpmacssww
			#undef vpmacssww
		#endif
		#define vpmacssww __builtin_ia32_vpmacssww
	#endif

	#if HAS(__builtin_ia32_vpmacswd)
		#ifdef vpmacswd
			#undef vpmacswd
		#endif
		#define vpmacswd __builtin_ia32_vpmacswd
	#endif

	#if HAS(__builtin_ia32_vpmacsww)
		#ifdef vpmacsww
			#undef vpmacsww
		#endif
		#define vpmacsww __builtin_ia32_vpmacsww
	#endif

	#if HAS(__builtin_ia32_vpmadcsswd)
		#ifdef vpmadcsswd
			#undef vpmadcsswd
		#endif
		#define vpmadcsswd __builtin_ia32_vpmadcsswd
	#endif

	#if HAS(__builtin_ia32_vpmadcswd)
		#ifdef vpmadcswd
			#undef vpmadcswd
		#endif
		#define vpmadcswd __builtin_ia32_vpmadcswd
	#endif

	#if HAS(__builtin_ia32_vpperm)
		#ifdef vpperm
			#undef vpperm
		#endif
		#define vpperm __builtin_ia32_vpperm
	#endif

	#if HAS(__builtin_ia32_vprotb)
		#ifdef vprotb
			#undef vprotb
		#endif
		#define vprotb __builtin_ia32_vprotb
	#endif

	#if HAS(__builtin_ia32_vprotd)
		#ifdef vprotd
			#undef vprotd
		#endif
		#define vprotd __builtin_ia32_vprotd
	#endif

	#if HAS(__builtin_ia32_vprotq)
		#ifdef vprotq
			#undef vprotq
		#endif
		#define vprotq __builtin_ia32_vprotq
	#endif

	#if HAS(__builtin_ia32_vprotw)
		#ifdef vprotw
			#undef vprotw
		#endif
		#define vprotw __builtin_ia32_vprotw
	#endif

	#if HAS(__builtin_ia32_vpshab)
		#ifdef vpshab
			#undef vpshab
		#endif
		#define vpshab __builtin_ia32_vpshab
	#endif

	#if HAS(__builtin_ia32_vpshad)
		#ifdef vpshad
			#undef vpshad
		#endif
		#define vpshad __builtin_ia32_vpshad
	#endif

	#if HAS(__builtin_ia32_vpshaq)
		#ifdef vpshaq
			#undef vpshaq
		#endif
		#define vpshaq __builtin_ia32_vpshaq
	#endif

	#if HAS(__builtin_ia32_vpshaw)
		#ifdef vpshaw
			#undef vpshaw
		#endif
		#define vpshaw __builtin_ia32_vpshaw
	#endif

	#if HAS(__builtin_ia32_vpshlb)
		#ifdef vpshlb
			#undef vpshlb
		#endif
		#define vpshlb __builtin_ia32_vpshlb
	#endif

	#if HAS(__builtin_ia32_vpshld)
		#ifdef vpshld
			#undef vpshld
		#endif
		#define vpshld __builtin_ia32_vpshld
	#endif

	#if HAS(__builtin_ia32_vpshlq)
		#ifdef vpshlq
			#undef vpshlq
		#endif
		#define vpshlq __builtin_ia32_vpshlq
	#endif

	#if HAS(__builtin_ia32_vpshlw)
		#ifdef vpshlw
			#undef vpshlw
		#endif
		#define vpshlw __builtin_ia32_vpshlw
	#endif

#endif

#if defined __FMA4__

	#if HAS(__builtin_ia32_)
		#ifdef 
			#undef 
		#endif
		#define  __builtin_ia32_
	#endif

	#if HAS(__builtin_ia32_vfmaddpd)
		#ifdef vfmaddpd
			#undef vfmaddpd
		#endif
		#define vfmaddpd __builtin_ia32_vfmaddpd
	#endif

	#if HAS(__builtin_ia32_vfmaddpd256)
		#ifdef vfmaddpd256
			#undef vfmaddpd256
		#endif
		#define vfmaddpd256 __builtin_ia32_vfmaddpd256
	#endif

	#if HAS(__builtin_ia32_vfmaddps)
		#ifdef vfmaddps
			#undef vfmaddps
		#endif
		#define vfmaddps __builtin_ia32_vfmaddps
	#endif

	#if HAS(__builtin_ia32_vfmaddps256)
		#ifdef vfmaddps256
			#undef vfmaddps256
		#endif
		#define vfmaddps256 __builtin_ia32_vfmaddps256
	#endif

	#if HAS(__builtin_ia32_vfmaddsd)
		#ifdef vfmaddsd
			#undef vfmaddsd
		#endif
		#define vfmaddsd __builtin_ia32_vfmaddsd
	#endif

	#if HAS(__builtin_ia32_vfmaddss)
		#ifdef vfmaddss
			#undef vfmaddss
		#endif
		#define vfmaddss __builtin_ia32_vfmaddss
	#endif

	#if HAS(__builtin_ia32_vfmaddsubpd)
		#ifdef vfmaddsubpd
			#undef vfmaddsubpd
		#endif
		#define vfmaddsubpd __builtin_ia32_vfmaddsubpd
	#endif

	#if HAS(__builtin_ia32_vfmaddsubpd256)
		#ifdef vfmaddsubpd256
			#undef vfmaddsubpd256
		#endif
		#define vfmaddsubpd256 __builtin_ia32_vfmaddsubpd256
	#endif

	#if HAS(__builtin_ia32_vfmaddsubps)
		#ifdef vfmaddsubps
			#undef vfmaddsubps
		#endif
		#define vfmaddsubps __builtin_ia32_vfmaddsubps
	#endif

	#if HAS(__builtin_ia32_vfmaddsubps256)
		#ifdef vfmaddsubps256
			#undef vfmaddsubps256
		#endif
		#define vfmaddsubps256 __builtin_ia32_vfmaddsubps256
	#endif

	#if HAS(__builtin_ia32_vfmsubaddpd)
		#ifdef vfmsubaddpd
			#undef vfmsubaddpd
		#endif
		#define vfmsubaddpd __builtin_ia32_vfmsubaddpd
	#endif

	#if HAS(__builtin_ia32_vfmsubaddpd256)
		#ifdef vfmsubaddpd256
			#undef vfmsubaddpd256
		#endif
		#define vfmsubaddpd256 __builtin_ia32_vfmsubaddpd256
	#endif

	#if HAS(__builtin_ia32_vfmsubaddps)
		#ifdef vfmsubaddps
			#undef vfmsubaddps
		#endif
		#define vfmsubaddps __builtin_ia32_vfmsubaddps
	#endif

	#if HAS(__builtin_ia32_vfmsubaddps256)
		#ifdef vfmsubaddps256
			#undef vfmsubaddps256
		#endif
		#define vfmsubaddps256 __builtin_ia32_vfmsubaddps256
	#endif

	#if HAS(__builtin_ia32_vfmsubpd)
		#ifdef vfmsubpd
			#undef vfmsubpd
		#endif
		#define vfmsubpd __builtin_ia32_vfmsubpd
	#endif

	#if HAS(__builtin_ia32_vfmsubpd256)
		#ifdef vfmsubpd256
			#undef vfmsubpd256
		#endif
		#define vfmsubpd256 __builtin_ia32_vfmsubpd256
	#endif

	#if HAS(__builtin_ia32_vfmsubps)
		#ifdef vfmsubps
			#undef vfmsubps
		#endif
		#define vfmsubps __builtin_ia32_vfmsubps
	#endif

	#if HAS(__builtin_ia32_vfmsubps256)
		#ifdef vfmsubps256
			#undef vfmsubps256
		#endif
		#define vfmsubps256 __builtin_ia32_vfmsubps256
	#endif

	#if HAS(__builtin_ia32_vfmsubsd)
		#ifdef vfmsubsd
			#undef vfmsubsd
		#endif
		#define vfmsubsd __builtin_ia32_vfmsubsd
	#endif

	#if HAS(__builtin_ia32_vfmsubss)
		#ifdef vfmsubss
			#undef vfmsubss
		#endif
		#define vfmsubss __builtin_ia32_vfmsubss
	#endif

	#if HAS(__builtin_ia32_vfnmaddpd)
		#ifdef vfnmaddpd
			#undef vfnmaddpd
		#endif
		#define vfnmaddpd __builtin_ia32_vfnmaddpd
	#endif

	#if HAS(__builtin_ia32_vfnmaddpd256)
		#ifdef vfnmaddpd256
			#undef vfnmaddpd256
		#endif
		#define vfnmaddpd256 __builtin_ia32_vfnmaddpd256
	#endif

	#if HAS(__builtin_ia32_vfnmaddps)
		#ifdef vfnmaddps
			#undef vfnmaddps
		#endif
		#define vfnmaddps __builtin_ia32_vfnmaddps
	#endif

	#if HAS(__builtin_ia32_vfnmaddps256)
		#ifdef vfnmaddps256
			#undef vfnmaddps256
		#endif
		#define vfnmaddps256 __builtin_ia32_vfnmaddps256
	#endif

	#if HAS(__builtin_ia32_vfnmaddsd)
		#ifdef vfnmaddsd
			#undef vfnmaddsd
		#endif
		#define vfnmaddsd __builtin_ia32_vfnmaddsd
	#endif

	#if HAS(__builtin_ia32_vfnmaddss)
		#ifdef vfnmaddss
			#undef vfnmaddss
		#endif
		#define vfnmaddss __builtin_ia32_vfnmaddss
	#endif

	#if HAS(__builtin_ia32_vfnmsubpd)
		#ifdef vfnmsubpd
			#undef vfnmsubpd
		#endif
		#define vfnmsubpd __builtin_ia32_vfnmsubpd
	#endif

	#if HAS(__builtin_ia32_vfnmsubpd256)
		#ifdef vfnmsubpd256
			#undef vfnmsubpd256
		#endif
		#define vfnmsubpd256 __builtin_ia32_vfnmsubpd256
	#endif

	#if HAS(__builtin_ia32_vfnmsubps)
		#ifdef vfnmsubps
			#undef vfnmsubps
		#endif
		#define vfnmsubps __builtin_ia32_vfnmsubps
	#endif

	#if HAS(__builtin_ia32_vfnmsubps256)
		#ifdef vfnmsubps256
			#undef vfnmsubps256
		#endif
		#define vfnmsubps256 __builtin_ia32_vfnmsubps256
	#endif

	#if HAS(__builtin_ia32_vfnmsubsd)
		#ifdef vfnmsubsd
			#undef vfnmsubsd
		#endif
		#define vfnmsubsd __builtin_ia32_vfnmsubsd
	#endif

	#if HAS(__builtin_ia32_vfnmsubss)
		#ifdef vfnmsubss
			#undef vfnmsubss
		#endif
		#define vfnmsubss __builtin_ia32_vfnmsubss
	#endif

#endif

#if defined __LWP__

	#if HAS(__builtin_ia32_llwpcb16)
		#ifdef llwpcb16
			#undef llwpcb16
		#endif
		#define llwpcb16 __builtin_ia32_llwpcb16
	#endif

	#if HAS(__builtin_ia32_llwpcb16)
		#ifdef llwpcb16
			#undef llwpcb16
		#endif
		#define llwpcb16 __builtin_ia32_llwpcb16
	#endif

	#if HAS(__builtin_ia32_llwpcb32)
		#ifdef llwpcb32
			#undef llwpcb32
		#endif
		#define llwpcb32 __builtin_ia32_llwpcb32
	#endif

	#if HAS(__builtin_ia32_llwpcb32)
		#ifdef llwpcb32
			#undef llwpcb32
		#endif
		#define llwpcb32 __builtin_ia32_llwpcb32
	#endif

	#if HAS(__builtin_ia32_llwpcb64)
		#ifdef llwpcb64
			#undef llwpcb64
		#endif
		#define llwpcb64 __builtin_ia32_llwpcb64
	#endif

	#if HAS(__builtin_ia32_llwpcb64)
		#ifdef llwpcb64
			#undef llwpcb64
		#endif
		#define llwpcb64 __builtin_ia32_llwpcb64
	#endif

	#if HAS(__builtin_ia32_lwpins16)
		#ifdef lwpins16
			#undef lwpins16
		#endif
		#define lwpins16 __builtin_ia32_lwpins16
	#endif

	#if HAS(__builtin_ia32_lwpins32)
		#ifdef lwpins32
			#undef lwpins32
		#endif
		#define lwpins32 __builtin_ia32_lwpins32
	#endif

	#if HAS(__builtin_ia32_lwpins64)
		#ifdef lwpins64
			#undef lwpins64
		#endif
		#define lwpins64 __builtin_ia32_lwpins64
	#endif

	#if HAS(__builtin_ia32_lwpval16)
		#ifdef lwpval16
			#undef lwpval16
		#endif
		#define lwpval16 __builtin_ia32_lwpval16
	#endif

	#if HAS(__builtin_ia32_lwpval32)
		#ifdef lwpval32
			#undef lwpval32
		#endif
		#define lwpval32 __builtin_ia32_lwpval32
	#endif

	#if HAS(__builtin_ia32_lwpval64)
		#ifdef lwpval64
			#undef lwpval64
		#endif
		#define lwpval64 __builtin_ia32_lwpval64
	#endif

#endif

#if defined __BMI__

	#if HAS(__builtin_ia32_bextr_u32)
		#ifdef bextr_u32
			#undef bextr_u32
		#endif
		#define bextr_u32 __builtin_ia32_bextr_u32
	#endif

	#if HAS(__builtin_ia32_bextr_u64)
		#ifdef bextr_u64
			#undef bextr_u64
		#endif
		#define bextr_u64 __builtin_ia32_bextr_u64
	#endif

#endif

#if defined __BMI2__

	#if HAS(__builtin_ia32__bzhi_u32)
		#ifdef _bzhi_u32
			#undef _bzhi_u32
		#endif
		#define _bzhi_u32 __builtin_ia32__bzhi_u32
	#endif

	#if HAS(__builtin_ia32__bzhi_u64)
		#ifdef _bzhi_u64
			#undef _bzhi_u64
		#endif
		#define _bzhi_u64 __builtin_ia32__bzhi_u64
	#endif

	#if HAS(__builtin_ia32__pdep_u32)
		#ifdef _pdep_u32
			#undef _pdep_u32
		#endif
		#define _pdep_u32 __builtin_ia32__pdep_u32
	#endif

	#if HAS(__builtin_ia32__pdep_u64)
		#ifdef _pdep_u64
			#undef _pdep_u64
		#endif
		#define _pdep_u64 __builtin_ia32__pdep_u64
	#endif

	#if HAS(__builtin_ia32__pext_u32)
		#ifdef _pext_u32
			#undef _pext_u32
		#endif
		#define _pext_u32 __builtin_ia32__pext_u32
	#endif

	#if HAS(__builtin_ia32__pext_u64)
		#ifdef _pext_u64
			#undef _pext_u64
		#endif
		#define _pext_u64 __builtin_ia32__pext_u64
	#endif

#endif

#if defined __LZCNT__

	#if HAS(__builtin_ia32_lzcnt_16)
		#ifdef lzcnt_16
			#undef lzcnt_16
		#endif
		#define lzcnt_16 __builtin_ia32_lzcnt_16
	#endif

	#if HAS(__builtin_ia32_lzcnt_u32)
		#ifdef lzcnt_u32
			#undef lzcnt_u32
		#endif
		#define lzcnt_u32 __builtin_ia32_lzcnt_u32
	#endif

	#if HAS(__builtin_ia32_lzcnt_u64)
		#ifdef lzcnt_u64
			#undef lzcnt_u64
		#endif
		#define lzcnt_u64 __builtin_ia32_lzcnt_u64
	#endif

#endif

#if defined __FXSR__

	#if HAS(__builtin_ia32_fxrstor)
		#ifdef fxrstor
			#undef fxrstor
		#endif
		#define fxrstor __builtin_ia32_fxrstor
	#endif

	#if HAS(__builtin_ia32_fxrstor64)
		#ifdef fxrstor64
			#undef fxrstor64
		#endif
		#define fxrstor64 __builtin_ia32_fxrstor64
	#endif

	#if HAS(__builtin_ia32_fxsave)
		#ifdef fxsave
			#undef fxsave
		#endif
		#define fxsave __builtin_ia32_fxsave
	#endif

	#if HAS(__builtin_ia32_fxsave64)
		#ifdef fxsave64
			#undef fxsave64
		#endif
		#define fxsave64 __builtin_ia32_fxsave64
	#endif

#endif

#if defined __XSAVE__

	#if HAS(__builtin_ia32_xrstor)
		#ifdef xrstor
			#undef xrstor
		#endif
		#define xrstor __builtin_ia32_xrstor
	#endif

	#if HAS(__builtin_ia32_xrstor64)
		#ifdef xrstor64
			#undef xrstor64
		#endif
		#define xrstor64 __builtin_ia32_xrstor64
	#endif

	#if HAS(__builtin_ia32_xsave)
		#ifdef xsave
			#undef xsave
		#endif
		#define xsave __builtin_ia32_xsave
	#endif

	#if HAS(__builtin_ia32_xsave64)
		#ifdef xsave64
			#undef xsave64
		#endif
		#define xsave64 __builtin_ia32_xsave64
	#endif

#endif

#if defined __XSAVEOPT__

	#if HAS(__builtin_ia32_xsaveopt)
		#ifdef xsaveopt
			#undef xsaveopt
		#endif
		#define xsaveopt __builtin_ia32_xsaveopt
	#endif

	#if HAS(__builtin_ia32_xsaveopt64)
		#ifdef xsaveopt64
			#undef xsaveopt64
		#endif
		#define xsaveopt64 __builtin_ia32_xsaveopt64
	#endif

#endif

#if defined __TBM__

	#if HAS(__builtin_ia32_bextri_u32)
		#ifdef bextri_u32
			#undef bextri_u32
		#endif
		#define bextri_u32 __builtin_ia32_bextri_u32
	#endif

	#if HAS(__builtin_ia32_bextri_u64)
		#ifdef bextri_u64
			#undef bextri_u64
		#endif
		#define bextri_u64 __builtin_ia32_bextri_u64
	#endif

#endif

#if defined __3dNOW__

	#if HAS(__builtin_ia32_femms)
		#ifdef femms
			#undef femms
		#endif
		#define femms __builtin_ia32_femms
	#endif

	#if HAS(__builtin_ia32_pavgusb)
		#ifdef pavgusb
			#undef pavgusb
		#endif
		#define pavgusb __builtin_ia32_pavgusb
	#endif

	#if HAS(__builtin_ia32_pf2id)
		#ifdef pf2id
			#undef pf2id
		#endif
		#define pf2id __builtin_ia32_pf2id
	#endif

	#if HAS(__builtin_ia32_pfacc)
		#ifdef pfacc
			#undef pfacc
		#endif
		#define pfacc __builtin_ia32_pfacc
	#endif

	#if HAS(__builtin_ia32_pfadd)
		#ifdef pfadd
			#undef pfadd
		#endif
		#define pfadd __builtin_ia32_pfadd
	#endif

	#if HAS(__builtin_ia32_pfcmpeq)
		#ifdef pfcmpeq
			#undef pfcmpeq
		#endif
		#define pfcmpeq __builtin_ia32_pfcmpeq
	#endif

	#if HAS(__builtin_ia32_pfcmpge)
		#ifdef pfcmpge
			#undef pfcmpge
		#endif
		#define pfcmpge __builtin_ia32_pfcmpge
	#endif

	#if HAS(__builtin_ia32_pfcmpgt)
		#ifdef pfcmpgt
			#undef pfcmpgt
		#endif
		#define pfcmpgt __builtin_ia32_pfcmpgt
	#endif

	#if HAS(__builtin_ia32_pfmax)
		#ifdef pfmax
			#undef pfmax
		#endif
		#define pfmax __builtin_ia32_pfmax
	#endif

	#if HAS(__builtin_ia32_pfmin)
		#ifdef pfmin
			#undef pfmin
		#endif
		#define pfmin __builtin_ia32_pfmin
	#endif

	#if HAS(__builtin_ia32_pfmul)
		#ifdef pfmul
			#undef pfmul
		#endif
		#define pfmul __builtin_ia32_pfmul
	#endif

	#if HAS(__builtin_ia32_pfrcp)
		#ifdef pfrcp
			#undef pfrcp
		#endif
		#define pfrcp __builtin_ia32_pfrcp
	#endif

	#if HAS(__builtin_ia32_pfrcpit1)
		#ifdef pfrcpit1
			#undef pfrcpit1
		#endif
		#define pfrcpit1 __builtin_ia32_pfrcpit1
	#endif

	#if HAS(__builtin_ia32_pfrcpit2)
		#ifdef pfrcpit2
			#undef pfrcpit2
		#endif
		#define pfrcpit2 __builtin_ia32_pfrcpit2
	#endif

	#if HAS(__builtin_ia32_pfrsqrt)
		#ifdef pfrsqrt
			#undef pfrsqrt
		#endif
		#define pfrsqrt __builtin_ia32_pfrsqrt
	#endif

	#if HAS(__builtin_ia32_pfsub)
		#ifdef pfsub
			#undef pfsub
		#endif
		#define pfsub __builtin_ia32_pfsub
	#endif

	#if HAS(__builtin_ia32_pfsubr)
		#ifdef pfsubr
			#undef pfsubr
		#endif
		#define pfsubr __builtin_ia32_pfsubr
	#endif

	#if HAS(__builtin_ia32_pi2fd)
		#ifdef pi2fd
			#undef pi2fd
		#endif
		#define pi2fd __builtin_ia32_pi2fd
	#endif

	#if HAS(__builtin_ia32_pmulhrw)
		#ifdef pmulhrw
			#undef pmulhrw
		#endif
		#define pmulhrw __builtin_ia32_pmulhrw
	#endif

#endif

#if defined __3dNOW_A__

	#if HAS(__builtin_ia32_pf2iw)
		#ifdef pf2iw
			#undef pf2iw
		#endif
		#define pf2iw __builtin_ia32_pf2iw
	#endif

	#if HAS(__builtin_ia32_pfnacc)
		#ifdef pfnacc
			#undef pfnacc
		#endif
		#define pfnacc __builtin_ia32_pfnacc
	#endif

	#if HAS(__builtin_ia32_pfpnacc)
		#ifdef pfpnacc
			#undef pfpnacc
		#endif
		#define pfpnacc __builtin_ia32_pfpnacc
	#endif

	#if HAS(__builtin_ia32_pi2fw)
		#ifdef pi2fw
			#undef pi2fw
		#endif
		#define pi2fw __builtin_ia32_pi2fw
	#endif

	#if HAS(__builtin_ia32_pswapdsf)
		#ifdef pswapdsf
			#undef pswapdsf
		#endif
		#define pswapdsf __builtin_ia32_pswapdsf
	#endif

	#if HAS(__builtin_ia32_pswapdsi)
		#ifdef pswapdsi
			#undef pswapdsi
		#endif
		#define pswapdsi __builtin_ia32_pswapdsi
	#endif

#endif

#if defined __RTM__

	#if HAS(__builtin_ia32_xabort)
		#ifdef xabort
			#undef xabort
		#endif
		#define xabort __builtin_ia32_xabort
	#endif

	#if HAS(__builtin_ia32_xbegin)
		#ifdef xbegin
			#undef xbegin
		#endif
		#define xbegin __builtin_ia32_xbegin
	#endif

	#if HAS(__builtin_ia32_xend)
		#ifdef xend
			#undef xend
		#endif
		#define xend __builtin_ia32_xend
	#endif

	#if HAS(__builtin_ia32_xtest)
		#ifdef xtest
			#undef xtest
		#endif
		#define xtest __builtin_ia32_xtest
	#endif

#endif

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
 * int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
 **/
#ifdef __NR_accept
	#define accept(...) syscall(__NR_accept,__VA_ARGS__)
#else
	#define accept(...) _socketcall(SYS_ACCEPT,__VA_ARGS__)
#endif

/** accept4() - accept() w/ param for flagging NONBLOCK/CLOEXEC	
 * int accept4(int sockfd,struct sockaddr *addr,socklen_t *addrlen,int flags);
 **/
#ifdef __NR_accept4
	#define accept4(...) syscall(__NR_accept4,__VA_ARGS__)
#else
	#define accept4(...) _socketcall(SYS_ACCEPT4,__VA_ARGS__)
#endif

/** access() - check file permissions
 * int access(const char *pathname, int mode);
 **/
#ifdef __NR_access
	#define access(...) syscall(__NR_access,__VA_ARGS__)
#endif

/** acct() - toggle process accounting
 * int acct(const char *filename);
 **/
#ifdef __NR_acct
	#define acct(...) syscall(__NR_acct,__VA_ARGS__)
#endif

/** add_key() - add/change a key in a kernel keyring
 * key_serial_t add_key(const char *type,const char *description,const void *payload, size_t plen,key_serial_t keyring);
 **/
#ifdef __NR_add_key
	#define add_key(...) syscall(__NR_add_key,__VA_ARGS__)
#endif

/** adjtimex() - tune kernel's clock
 * int adjtimex(struct timex *buf);
 **/
#ifdef __NR_adjtimex
	#define adjtimex(...) syscall(__NR_adjtimex,__VA_ARGS__)
#endif

/** afs_syscall() - not implemented, 
 * returns -1 and ENOSYS
 **/
#ifdef __NR_afs_syscall
#define afs_syscall(...) syscall(__NR_afs_syscall,__VA_ARGS__)
#endif

/** alarm() - send a SIGALRM in `n` seconds
 * unsigned int alarm(unsigned int seconds);
 **/
#ifdef __NR_alarm
	#define alarm(...) syscall(__NR_alarm,__VA_ARGS__)
#endif

/** arch_prctl() - get/set arch-specific thread state
 * int arch_prctl(int code,unsigned long addr); (or *addr for get ops)
 **/
#ifdef __NR_arch_prctl
	#define arch_prctl(...) syscall(__NR_arch_prctl,__VA_ARGS__)
#endif

/** bind() - bind address (name) to a socket
 * int bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen);
 **/
#ifdef __NR_bind
	#define bind(...) syscall(__NR_bind,__VA_ARGS__)
#else
#define bind(...) _socketcall(SYS_BIND,__VA_ARGS__)
#endif

/** 
 * 
 **/
#ifdef __NR_bpf
#define bpf(...) syscall(__NR_bpf,__VA_ARGS__)
#endif

/** 
 * 
 **/
#ifdef __NR_brk
#define brk(...) syscall(__NR_brk,__VA_ARGS__)
#endif

/** 
 * 
 **/
#ifdef __NR_capget
#define capget(...) syscall(__NR_capget,__VA_ARGS__)
#endif

/** 
 * 
 **/
#ifdef __NR_capset
#define capset(...) syscall(__NR_capset,__VA_ARGS__)
#endif

/** 
 * 
 **/
#ifdef __NR_chdir
#define chdir(...) syscall(__NR_chdir,__VA_ARGS__)
#endif

/** 
 * 
 **/
#ifdef __NR_chmod
#define chmod(...) syscall(__NR_chmod,__VA_ARGS__)
#endif

/** 
 * 
 **/

/**
 *
 **/
#ifdef __NR_chown
#define chown(...) syscall(__NR_chown,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_chroot
#define chroot(...) syscall(__NR_chroot,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_clock_adjtime
#define clock_adjtime(...) syscall(__NR_clock_adjtime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_clock_getres
#define clock_getres(...) syscall(__NR_clock_getres,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_clock_gettime
#define clock_gettime(...) syscall(__NR_clock_gettime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_clock_nanosleep
#define clock_nanosleep(...) syscall(__NR_clock_nanosleep,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_clock_settime
#define clock_settime(...) syscall(__NR_clock_settime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_clone
#define clone(...) syscall(__NR_clone,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_close
#define close(...) syscall(__NR_close,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_connect
#define connect(...) syscall(__NR_connect,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_creat
#define creat(...) syscall(__NR_creat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_create_module
#define create_module(...) syscall(__NR_create_module,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_delete_module
#define delete_module(...) syscall(__NR_delete_module,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_dup
#define dup(...) syscall(__NR_dup,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_dup2
#define dup2(...) syscall(__NR_dup2,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_dup3
#define dup3(...) syscall(__NR_dup3,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_create
#define epoll_create(...) syscall(__NR_epoll_create,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_create1
#define epoll_create1(...) syscall(__NR_epoll_create1,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_ctl
#define epoll_ctl(...) syscall(__NR_epoll_ctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_ctl_old
#define epoll_ctl_old(...) syscall(__NR_epoll_ctl_old,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_pwait
#define epoll_pwait(...) syscall(__NR_epoll_pwait,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_wait
#define epoll_wait(...) syscall(__NR_epoll_wait,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_epoll_wait_old
#define epoll_wait_old(...) syscall(__NR_epoll_wait_old,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_eventfd
#define eventfd(...) syscall(__NR_eventfd,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_eventfd2
#define eventfd2(...) syscall(__NR_eventfd2,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_execve
#define execve(...) syscall(__NR_execve,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_execveat
#define execveat(...) syscall(__NR_execveat,__VA_ARGS__)
#endif

//__NR_exit get its own exit() inline/no-return function

/**
 *
 **/
#ifdef __NR_exit_group
#define exit_group(...) syscall(__NR_exit_group,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_faccessat
#define faccessat(...) syscall(__NR_faccessat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fadvise64
#define fadvise64(...) syscall(__NR_fadvise64,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fallocate
#define fallocate(...) syscall(__NR_fallocate,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fanotify_init
#define fanotify_init(...) syscall(__NR_fanotify_init,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fanotify_mark
#define fanotify_mark(...) syscall(__NR_fanotify_mark,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fchdir
#define fchdir(...) syscall(__NR_fchdir,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fchmod
#define fchmod(...) syscall(__NR_fchmod,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fchmodat
#define fchmodat(...) syscall(__NR_fchmodat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fchown
#define fchown(...) syscall(__NR_fchown,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fchownat
#define fchownat(...) syscall(__NR_fchownat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fcntl
#define fcntl(...) syscall(__NR_fcntl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fdatasync
#define fdatasync(...) syscall(__NR_fdatasync,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fgetxattr
#define fgetxattr(...) syscall(__NR_fgetxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_finit_module
#define finit_module(...) syscall(__NR_finit_module,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_finit_module
#define finit_module(...) syscall(__NR_finit_module,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_flistxattr
#define flistxattr(...) syscall(__NR_flistxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_flock
#define flock(...) syscall(__NR_flock,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fork
#define fork(...) syscall(__NR_fork,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fremovexattr
#define fremovexattr(...) syscall(__NR_fremovexattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fsetxattr
#define fsetxattr(...) syscall(__NR_fsetxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fstat
#define fstat(...) syscall(__NR_fstat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fstatfs
#define fstatfs(...) syscall(__NR_fstatfs,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_fsync
#define fsync(...) syscall(__NR_fsync,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ftruncate
#define ftruncate(...) syscall(__NR_ftruncate,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_futex
#define futex(...) syscall(__NR_futex,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_futimesat
#define futimesat(...) syscall(__NR_futimesat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getcpu
#define getcpu(...) syscall(__NR_getcpu,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getcwd
#define getcwd(...) syscall(__NR_getcwd,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getdents
#define getdents(...) syscall(__NR_getdents,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getdents64
#define getdents64(...) syscall(__NR_getdents64,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getegid
#define getegid(...) syscall(__NR_getegid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_geteuid
#define geteuid(...) syscall(__NR_geteuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getgid
#define getgid(...) syscall(__NR_getgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getgroups
#define getgroups(...) syscall(__NR_getgroups,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getitimer
#define getitimer(...) syscall(__NR_getitimer,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_get_kernel_syms
#define get_kernel_syms(...) syscall(__NR_get_kernel_syms,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_get_mempolicy
#define get_mempolicy(...) syscall(__NR_get_mempolicy,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getpeername
#define getpeername(...) syscall(__NR_getpeername,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getpgid
#define getpgid(...) syscall(__NR_getpgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getpgrp
#define getpgrp(...) syscall(__NR_getpgrp,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getpid
#define getpid(...) syscall(__NR_getpid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getpmsg
#define getpmsg(...) syscall(__NR_getpmsg,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getppid
#define getppid(...) syscall(__NR_getppid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getpriority
#define getpriority(...) syscall(__NR_getpriority,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getrandom
#define getrandom(...) syscall(__NR_getrandom,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getresgid
#define getresgid(...) syscall(__NR_getresgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getresuid
#define getresuid(...) syscall(__NR_getresuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getrlimit
#define getrlimit(...) syscall(__NR_getrlimit,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_get_robust_list
#define get_robust_list(...) syscall(__NR_get_robust_list,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getrusage
#define getrusage(...) syscall(__NR_getrusage,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getsid
#define getsid(...) syscall(__NR_getsid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getsockname
#define getsockname(...) syscall(__NR_getsockname,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getsockopt
#define getsockopt(...) syscall(__NR_getsockopt,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_get_thread_area
#define get_thread_area(...) syscall(__NR_get_thread_area,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_gettid
#define gettid(...) syscall(__NR_gettid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_gettimeofday
#define gettimeofday(...) syscall(__NR_gettimeofday,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getuid
#define getuid(...) syscall(__NR_getuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_getxattr
#define getxattr(...) syscall(__NR_getxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_init_module
#define init_module(...) syscall(__NR_init_module,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_inotify_add_watch
#define inotify_add_watch(...) syscall(__NR_inotify_add_watch,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_inotify_init
#define inotify_init(...) syscall(__NR_inotify_init,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_inotify_init1
#define inotify_init1(...) syscall(__NR_inotify_init1,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_inotify_rm_watch
#define inotify_rm_watch(...) syscall(__NR_inotify_rm_watch,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_io_cancel
#define io_cancel(...) syscall(__NR_io_cancel,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ioctl
#define ioctl(...) syscall(__NR_ioctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_io_destroy
#define io_destroy(...) syscall(__NR_io_destroy,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_io_getevents
#define io_getevents(...) syscall(__NR_io_getevents,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ioperm
#define ioperm(...) syscall(__NR_ioperm,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_iopl
#define iopl(...) syscall(__NR_iopl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ioprio_get
#define ioprio_get(...) syscall(__NR_ioprio_get,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ioprio_set
#define ioprio_set(...) syscall(__NR_ioprio_set,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_io_setup
#define io_setup(...) syscall(__NR_io_setup,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_io_submit
#define io_submit(...) syscall(__NR_io_submit,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_kcmp
#define kcmp(...) syscall(__NR_kcmp,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_kexec_file_load
#define kexec_file_load(...) syscall(__NR_kexec_file_load,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_kexec_load
#define kexec_load(...) syscall(__NR_kexec_load,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_keyctl
#define keyctl(...) syscall(__NR_keyctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_kill
#define kill(...) syscall(__NR_kill,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lchown
#define lchown(...) syscall(__NR_lchown,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lgetxattr
#define lgetxattr(...) syscall(__NR_lgetxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_link
#define link(...) syscall(__NR_link,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_linkat
#define linkat(...) syscall(__NR_linkat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_listen
#define listen(...) syscall(__NR_listen,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_listxattr
#define listxattr(...) syscall(__NR_listxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_llistxattr
#define llistxattr(...) syscall(__NR_llistxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lookup_dcookie
#define lookup_dcookie(...) syscall(__NR_lookup_dcookie,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lremovexattr
#define lremovexattr(...) syscall(__NR_lremovexattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lseek
#define lseek(...) syscall(__NR_lseek,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lsetxattr
#define lsetxattr(...) syscall(__NR_lsetxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_lstat
#define lstat(...) syscall(__NR_lstat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_madvise
#define madvise(...) syscall(__NR_madvise,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mbind
#define mbind(...) syscall(__NR_mbind,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_memfd_create
#define memfd_create(...) syscall(__NR_memfd_create,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_migrate_pages
#define migrate_pages(...) syscall(__NR_migrate_pages,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mincore
#define mincore(...) syscall(__NR_mincore,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mkdir
#define mkdir(...) syscall(__NR_mkdir,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mkdirat
#define mkdirat(...) syscall(__NR_mkdirat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mknod
#define mknod(...) syscall(__NR_mknod,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mknodat
#define mknodat(...) syscall(__NR_mknodat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mlock
#define mlock(...) syscall(__NR_mlock,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mlockall
#define mlockall(...) syscall(__NR_mlockall,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mmap
#define mmap(...) syscall(__NR_mmap,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_modify_ldt
#define modify_ldt(...) syscall(__NR_modify_ldt,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mount
#define mount(...) syscall(__NR_mount,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_move_pages
#define move_pages(...) syscall(__NR_move_pages,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mprotect
#define mprotect(...) syscall(__NR_mprotect,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mq_getsetattr
#define mq_getsetattr(...) syscall(__NR_mq_getsetattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mq_notify
#define mq_notify(...) syscall(__NR_mq_notify,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mq_open
#define mq_open(...) syscall(__NR_mq_open,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mq_timedreceive
#define mq_timedreceive(...) syscall(__NR_mq_timedreceive,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mq_timedsend
#define mq_timedsend(...) syscall(__NR_mq_timedsend,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mq_unlink
#define mq_unlink(...) syscall(__NR_mq_unlink,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_mremap
#define mremap(...) syscall(__NR_mremap,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_msgctl
#define msgctl(...) syscall(__NR_msgctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_msgget
#define msgget(...) syscall(__NR_msgget,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_msgrcv
#define msgrcv(...) syscall(__NR_msgrcv,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_msgsnd
#define msgsnd(...) syscall(__NR_msgsnd,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_msync
#define msync(...) syscall(__NR_msync,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_munlock
#define munlock(...) syscall(__NR_munlock,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_munlockall
#define munlockall(...) syscall(__NR_munlockall,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_munmap
#define munmap(...) syscall(__NR_munmap,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_name_to_handle_at
#define name_to_handle_at(...) syscall(__NR_name_to_handle_at,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_nanosleep
#define nanosleep(...) syscall(__NR_nanosleep,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_newfstatat
#define newfstatat(...) syscall(__NR_newfstatat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_nfsservctl
#define nfsservctl(...) syscall(__NR_nfsservctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_open
#define open(...) syscall(__NR_open,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_openat
#define openat(...) syscall(__NR_openat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_open_by_handle_at
#define open_by_handle_at(...) syscall(__NR_open_by_handle_at,__VA_ARGS__)
#endif

/**
 *
 **/
#if defined __NR_pause && !defined pause
#define pause(...) syscall(__NR_pause,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_perf_event_open
#define perf_event_open(...) syscall(__NR_perf_event_open,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_personality
#define personality(...) syscall(__NR_personality,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pipe
#define pipe(...) syscall(__NR_pipe,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pipe2
#define pipe2(...) syscall(__NR_pipe2,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pivot_root
#define pivot_root(...) syscall(__NR_pivot_root,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_poll
#define poll(...) syscall(__NR_poll,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ppoll
#define ppoll(...) syscall(__NR_ppoll,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_prctl
#define prctl(...) syscall(__NR_prctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pread64
#define pread64(...) syscall(__NR_pread64,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_preadv
#define preadv(...) syscall(__NR_preadv,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_prlimit64
#define prlimit64(...) syscall(__NR_prlimit64,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_process_vm_readv
#define process_vm_readv(...) syscall(__NR_process_vm_readv,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_process_vm_writev
#define process_vm_writev(...) syscall(__NR_process_vm_writev,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pselect6
#define pselect6(...) syscall(__NR_pselect6,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ptrace
#define ptrace(...) syscall(__NR_ptrace,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_putpmsg
#define putpmsg(...) syscall(__NR_putpmsg,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pwrite64
#define pwrite64(...) syscall(__NR_pwrite64,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_pwritev
#define pwritev(...) syscall(__NR_pwritev,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_query_module
#define query_module(...) syscall(__NR_query_module,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_quotactl
#define quotactl(...) syscall(__NR_quotactl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_read
#define read(...) syscall(__NR_read,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_readahead
#define readahead(...) syscall(__NR_readahead,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_readlink
#define readlink(...) syscall(__NR_readlink,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_readlinkat
#define readlinkat(...) syscall(__NR_readlinkat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_readv
#define readv(...) syscall(__NR_readv,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_reboot
#define reboot(...) syscall(__NR_reboot,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_recvfrom
#define recvfrom(...) syscall(__NR_recvfrom,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_recvmmsg
#define recvmmsg(...) syscall(__NR_recvmmsg,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_recvmsg
#define recvmsg(...) syscall(__NR_recvmsg,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_remap_file_pages
#define remap_file_pages(...) syscall(__NR_remap_file_pages,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_removexattr
#define removexattr(...) syscall(__NR_removexattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rename
#define rename(...) syscall(__NR_rename,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_renameat
#define renameat(...) syscall(__NR_renameat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_renameat2
#define renameat2(...) syscall(__NR_renameat2,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_request_key
#define request_key(...) syscall(__NR_request_key,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_restart_syscall
#define restart_syscall(...) syscall(__NR_restart_syscall,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rmdir
#define rmdir(...) syscall(__NR_rmdir,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigaction
#define rt_sigaction(...) syscall(__NR_rt_sigaction,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigpending
#define rt_sigpending(...) syscall(__NR_rt_sigpending,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigprocmask
#define rt_sigprocmask(...) syscall(__NR_rt_sigprocmask,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigqueueinfo
#define rt_sigqueueinfo(...) syscall(__NR_rt_sigqueueinfo,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigreturn
#define rt_sigreturn(...) syscall(__NR_rt_sigreturn,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigsuspend
#define rt_sigsuspend(...) syscall(__NR_rt_sigsuspend,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_sigtimedwait
#define rt_sigtimedwait(...) syscall(__NR_rt_sigtimedwait,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_rt_tgsigqueueinfo
#define rt_tgsigqueueinfo(...) syscall(__NR_rt_tgsigqueueinfo,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_getaffinity
#define sched_getaffinity(...) syscall(__NR_sched_getaffinity,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_getattr
#define sched_getattr(...) syscall(__NR_sched_getattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_getparam
#define sched_getparam(...) syscall(__NR_sched_getparam,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_get_priority_max
#define sched_get_priority_max(...) syscall(__NR_sched_get_priority_max,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_get_priority_min
#define sched_get_priority_min(...) syscall(__NR_sched_get_priority_min,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_getscheduler
#define sched_getscheduler(...) syscall(__NR_sched_getscheduler,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_rr_get_interval
#define sched_rr_get_interval(...) syscall(__NR_sched_rr_get_interval,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_setaffinity
#define sched_setaffinity(...) syscall(__NR_sched_setaffinity,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_setattr
#define sched_setattr(...) syscall(__NR_sched_setattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_setparam
#define sched_setparam(...) syscall(__NR_sched_setparam,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_setscheduler
#define sched_setscheduler(...) syscall(__NR_sched_setscheduler,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sched_yield
#define sched_yield(...) syscall(__NR_sched_yield,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_seccomp
#define seccomp(...) syscall(__NR_seccomp,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_security
#define security(...) syscall(__NR_security,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_select
#define select(...) syscall(__NR_select,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_semctl
#define semctl(...) syscall(__NR_semctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_semget
#define semget(...) syscall(__NR_semget,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_semop
#define semop(...) syscall(__NR_semop,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_semtimedop
#define semtimedop(...) syscall(__NR_semtimedop,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sendfile
#define sendfile(...) syscall(__NR_sendfile,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sendmmsg
#define sendmmsg(...) syscall(__NR_sendmmsg,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sendmsg
#define sendmsg(...) syscall(__NR_sendmsg,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sendto
#define sendto(...) syscall(__NR_sendto,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setdomainname
#define setdomainname(...) syscall(__NR_setdomainname,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setfsgid
#define setfsgid(...) syscall(__NR_setfsgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setfsuid
#define setfsuid(...) syscall(__NR_setfsuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setgid
#define setgid(...) syscall(__NR_setgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setgroups
#define setgroups(...) syscall(__NR_setgroups,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sethostname
#define sethostname(...) syscall(__NR_sethostname,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setitimer
#define setitimer(...) syscall(__NR_setitimer,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_set_mempolicy
#define set_mempolicy(...) syscall(__NR_set_mempolicy,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setns
#define setns(...) syscall(__NR_setns,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setpgid
#define setpgid(...) syscall(__NR_setpgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setpriority
#define setpriority(...) syscall(__NR_setpriority,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setregid
#define setregid(...) syscall(__NR_setregid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setresgid
#define setresgid(...) syscall(__NR_setresgid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setresuid
#define setresuid(...) syscall(__NR_setresuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setreuid
#define setreuid(...) syscall(__NR_setreuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setrlimit
#define setrlimit(...) syscall(__NR_setrlimit,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_set_robust_list
#define set_robust_list(...) syscall(__NR_set_robust_list,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setsid
#define setsid(...) syscall(__NR_setsid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setsockopt
#define setsockopt(...) syscall(__NR_setsockopt,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_set_thread_area
#define set_thread_area(...) syscall(__NR_set_thread_area,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_set_tid_address
#define set_tid_address(...) syscall(__NR_set_tid_address,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_settimeofday
#define settimeofday(...) syscall(__NR_settimeofday,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setuid
#define setuid(...) syscall(__NR_setuid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_setxattr
#define setxattr(...) syscall(__NR_setxattr,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_shmat
#define shmat(...) syscall(__NR_shmat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_shmctl
#define shmctl(...) syscall(__NR_shmctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_shmdt
#define shmdt(...) syscall(__NR_shmdt,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_shmget
#define shmget(...) syscall(__NR_shmget,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_shutdown
#define shutdown(...) syscall(__NR_shutdown,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sigaltstack
#define sigaltstack(...) syscall(__NR_sigaltstack,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_signalfd
#define signalfd(...) syscall(__NR_signalfd,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_signalfd4
#define signalfd4(...) syscall(__NR_signalfd4,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_socket
#define socket(...) syscall(__NR_socket,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_socketcall
#define socket(...) syscall(__NR_socketcall,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_socketpair
#define socketpair(...) syscall(__NR_socketpair,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_splice
#define splice(...) syscall(__NR_splice,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_stat
#define stat(...) syscall(__NR_stat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_statfs
#define statfs(...) syscall(__NR_statfs,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_swapoff
#define swapoff(...) syscall(__NR_swapoff,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_swapon
#define swapon(...) syscall(__NR_swapon,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_symlink
#define symlink(...) syscall(__NR_symlink,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_symlinkat
#define symlinkat(...) syscall(__NR_symlinkat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sync
#define sync(...) syscall(__NR_sync,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sync_file_range
#define sync_file_range(...) syscall(__NR_sync_file_range,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_syncfs
#define syncfs(...) syscall(__NR_syncfs,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR__sysctl
#define _sysctl(...) syscall(__NR__sysctl,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sysfs
#define sysfs(...) syscall(__NR_sysfs,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_sysinfo
#define sysinfo(...) syscall(__NR_sysinfo,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_syslog
#define syslog(...) syscall(__NR_syslog,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_tee
#define tee(...) syscall(__NR_tee,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_tgkill
#define tgkill(...) syscall(__NR_tgkill,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_time
#define time(...) syscall(__NR_time,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timer_create
#define timer_create(...) syscall(__NR_timer_create,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timer_delete
#define timer_delete(...) syscall(__NR_timer_delete,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timerfd_create
#define timerfd_create(...) syscall(__NR_timerfd_create,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timerfd_gettime
#define timerfd_gettime(...) syscall(__NR_timerfd_gettime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timerfd_settime
#define timerfd_settime(...) syscall(__NR_timerfd_settime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timer_getoverrun
#define timer_getoverrun(...) syscall(__NR_timer_getoverrun,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timer_gettime
#define timer_gettime(...) syscall(__NR_timer_gettime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_timer_settime
#define timer_settime(...) syscall(__NR_timer_settime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_times
#define times(...) syscall(__NR_times,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_tkill
#define tkill(...) syscall(__NR_tkill,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_truncate
#define truncate(...) syscall(__NR_truncate,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_tuxcall
#define tuxcall(...) syscall(__NR_tuxcall,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_umask
#define umask(...) syscall(__NR_umask,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_umount2
#define umount2(...) syscall(__NR_umount2,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_uname
#define uname(...) syscall(__NR_uname,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_unlink
#define unlink(...) syscall(__NR_unlink,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_unlinkat
#define unlinkat(...) syscall(__NR_unlinkat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_unshare
#define unshare(...) syscall(__NR_unshare,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_uselib
#define uselib(...) syscall(__NR_uselib,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_ustat
#define ustat(...) syscall(__NR_ustat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_utime
#define utime(...) syscall(__NR_utime,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_utimensat
#define utimensat(...) syscall(__NR_utimensat,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_utimes
#define utimes(...) syscall(__NR_utimes,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_vfork
#define vfork(...) syscall(__NR_vfork,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_vhangup
#define vhangup(...) syscall(__NR_vhangup,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_vmsplice
#define vmsplice(...) syscall(__NR_vmsplice,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_vserver
#define vserver(...) syscall(__NR_vserver,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_wait4
#define wait4(...) syscall(__NR_wait4,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_waitid
#define waitid(...) syscall(__NR_waitid,__VA_ARGS__)
#endif

/**
 *
 **/
#ifdef __NR_write
#define write(...) syscall(__NR_write,__VA_ARGS__)
#endif

/**
 *
 **/
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

#if 1 //strings section


#define sys_nerr (ELAST-1)  //BSD_COMPAT
static const char strerrors[]=
"Not an Error\0" \
"Operation not permitted\0" \
"No such file or directory\0" \
"No such process\0" \
"Interrupted system call\0" \
"I/O error\0" \
"No such device or address\0" \
"Argument list too long\0" \
"Exec format error\0" \
"Bad file number\0" \
"No child processes\0" \
"Try again\0" \
"Out of memory\0" \
"Permission denied\0" \
"Bad address\0" \
"Block device required\0" \
"Device or resource busy\0" \
"File exists\0" \
"Cross-device link\0" \
"No such device\0" \
"Not a directory\0" \
"Is a directory\0" \
"Invalid argument\0" \
"File table overflow\0" \
"Too many open files\0" \
"Not a typewriter\0" \
"Text file busy\0" \
"File too large\0" \
"No space left on device\0" \
"Illegal seek\0" \
"Read-only file system\0" \
"Too many links\0" \
"Broken pipe\0" \
"Math argument out of domain of func\0" \
"Math result not representable\0" \
"Resource deadlock would occur\0" \
"File name too long\0" \
"No record locks available\0" \
"Function not implemented\0" \
"Directory not empty\0" \
"Too many symbolic links encountered\0" \
"Operation would block\0" \
"No message of desired type\0" \
"Identifier removed\0" \
"Channel number out of range\0" \
"Level 2 not synchronized\0" \
"Level 3 halted\0" \
"Level 3 reset\0" \
"Link number out of range\0" \
"Protocol driver not attached\0" \
"No CSI structure available\0" \
"Level 2 halted\0" \
"Invalid exchange\0" \
"Invalid request descriptor\0" \
"Exchange full\0" \
"No anode\0" \
"Invalid request code\0" \
"Invalid slot\0" \
"Resource deadlock would occur\0" /*duplicate*/ \
"Bad font file format\0" \
"Device not a stream\0" \
"No data available\0" \
"Timer expired\0" \
"Out of streams resources\0" \
"Machine is not on the network\0" \
"Package not installed\0" \
"Object is remote\0" \
"Link has been severed\0" \
"Advertise error\0" \
"Srmount error\0" \
"Communication error on send\0" \
"Protocol error\0" \
"Multihop attempted\0" \
"RFS specific error\0" \
"Not a data message\0" \
"Value too large for defined data type\0" \
"Name not unique on network\0" \
"File descriptor in bad state\0" \
"Remote address changed\0" \
"Can not access a needed shared library\0" \
"Accessing a corrupted shared library\0" \
".lib section in a.out corrupted\0" \
"Attempting to link in too many shared libraries\0" \
"Cannot exec a shared library directly\0" \
"Illegal byte sequence\0" \
"Interrupted system call should be restarted\0" \
"Streams pipe error\0" \
"Too many users\0" \
"Socket operation on non-socket\0" \
"Destination address required\0" \
"Message too long\0" \
"Protocol wrong type for socket\0" \
"Protocol not available\0" \
"Protocol not supported\0" \
"Socket type not supported\0" \
"Operation not supported on transport endpoint\0" \
"Protocol family not supported\0" \
"Address family not supported by protocol\0" \
"Address already in use\0" \
"Cannot assign requested address\0" \
"Network is down\0" \
"Network is unreachable\0" \
"Network dropped connection because of reset\0" \
"Software caused connection abort\0" \
"Connection reset by peer\0" \
"No buffer space available\0" \
"Transport endpoint is already connected\0" \
"Transport endpoint is not connected\0" \
"Cannot send after transport endpoint shutdown\0" \
"Too many references: cannot splice\0" \
"Connection timed out\0" \
"Connection refused\0" \
"Host is down\0" \
"No route to host\0" \
"Operation already in progress\0" \
"Operation now in progress\0" \
"Stale file handle\0" \
"Structure needs cleaning\0" \
"Not a XENIX named type file\0" \
"No XENIX semaphores available\0" \
"Is a named type file\0" \
"Remote I/O error\0" \
"Quota exceeded\0" \
"No medium found\0" \
"Wrong medium type\0" \
"Operation Canceled\0" \
"Required key not available\0" \
"Key has expired\0" \
"Key has been revoked\0" \
"Key was rejected by service\0" \
"Owner died\0" \
"State not recoverable\0";

static inline const char *getnthstring(const char *s,unsigned long n){while(n--)while(*s++);return s;}
static inline const char *strerror(unsigned long e){return getnthstring(strerrors,(e<ELAST)?e:0);}

static inline int strncmp(const char *s1, const char *s2, size_t len);
static inline char *strstr(const char *haystack, const char *needle);

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



/** HACK alert:
 * linux-elf passes all arguments to _start() on the stack regardless of
 * the system's actuall calling convention, so you can't just set up
 * _start(argc, argv, ...) because you will just get some random data
 * in a register.  I tried using explicit named register variables, to
 * access the stack pointer directly; however optimizations sometimes
 * move the stack pointer in the prolog and break it ... which is also
 * why we can't just set up a variable at the very beginning of the 
 * function and refer to its address.  Which leads me to this hack.
 * Since large structs are always passed on the stack, we can use a
 * fake large struct as the sole parameter.  The compiler doesn't
 * mess with this.
 **/
struct stack {long arr[8];};
int main(); //if we change this to inline mymain(), we can inline main
void noreturn __attribute__ ((visibility ("protected")))
_start(struct stack stack){
	long argc = *(((long*)(&stack))-1);
	char **argv = (char**)&stack;
#ifdef NO_GLOBAL_VARS
	char **environ;
#endif
	environ = argv+argc;
	(void)exit(main(argc,argv,environ) );
	__builtin_unreachable(); //or for(;;); to shut up gcc
}

#endif

