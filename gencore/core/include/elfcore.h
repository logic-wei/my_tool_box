/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _UAPI_LINUX_ELFCORE_H
#define _UAPI_LINUX_ELFCORE_H

#include <elf.h>

struct compat_timeval {
    uint32_t tv_sec;
    uint32_t tv_usec;
};

struct elf_siginfo
{
    int32_t	si_signo;			/* signal number */
    int32_t	si_code;			/* extra code */
    int32_t	si_errno;			/* errno */
};

/*
 * Definitions to generate Intel SVR4-like core files.
 * These mostly have the same names as the SVR4 types with "elf_"
 * tacked on the front to prevent clashes with linux definitions,
 * and the typedef forms have been avoided.  This is mostly like
 * the SVR4 structure, but more Linuxy, with things that Linux does
 * not support and which gdb doesn't really use excluded.
 * Fields present but not used are marked with "XXX".
 */
struct elf_prstatus
{
	struct elf_siginfo pr_info;	/* Info associated with signal */
    int16_t	pr_cursig;		/* Current signal */
    uint32_t pr_sigpend;	/* Set of pending signals */
    uint32_t pr_sighold;	/* Set of held signals */
    uint32_t	pr_pid;
    uint32_t	pr_ppid;
    uint32_t	pr_pgrp;
    uint32_t	pr_sid;
    struct compat_timeval pr_utime;	/* User time */
    struct compat_timeval pr_stime;	/* System time */
    struct compat_timeval pr_cutime;	/* Cumulative user time */
    struct compat_timeval pr_cstime;	/* Cumulative system time */
    struct {
        uint32_t uregs[18];
    } pr_reg;	/* GP registers */
    int32_t pr_fpvalid;		/* True if math co-processor being used.  */
};

#define ELF_PRARGSZ	(80)	/* Number of chars for args */

struct elf_prpsinfo
{
	char	pr_state;	/* numeric process state */
	char	pr_sname;	/* char for pr_state */
	char	pr_zomb;	/* zombie */
	char	pr_nice;	/* nice val */
	unsigned long pr_flag;	/* flags */
    uint32_t	pr_uid;
    uint32_t	pr_gid;
    int	pr_pid, pr_ppid, pr_pgrp, pr_sid;
	/* Lots missing */
	char	pr_fname[16];	/* filename of executable */
	char	pr_psargs[ELF_PRARGSZ];	/* initial part of arg list */
};


#endif /* _UAPI_LINUX_ELFCORE_H */
