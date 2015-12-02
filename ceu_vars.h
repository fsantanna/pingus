#ifndef _CEU_VARS_H
#define _CEU_VARS_H

#include <assert.h>

#define ceu_out_assert assert
#define ceu_out_log(m,s) ceu_sys_log(m,s)

#include "_ceu_app.h"
extern tceu_app CEU_APP;
extern int      XXX_FROM_CEU;

#endif
