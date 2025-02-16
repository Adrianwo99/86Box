#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>
#include <86box/86box.h>
#include "cpu.h"
#include <86box/machine.h>
#include "x87_timings.h"

x87_timings_t x87_timings;
x87_timings_t x87_concurrency;

const x87_timings_t x87_timings_8087 =
{
        .f2xm1      = (310 + 630) / 2,
        .fabs       = (10 + 17) / 2,
        .fadd       = (70 + 100) / 2,
        .fadd_32    = (90 + 120) / 2,
        .fadd_64    = (95 + 125) / 2,
        .fbld       = (290 + 310) / 2,
        .fbstp      = (520 + 540) / 2,
        .fchs       = (10 + 17) / 2,
        .fclex      = (2 + 8) / 2,
        .fcom       = (40 + 50) / 2,
        .fcom_32    = (60 + 70) / 2,
        .fcom_64    = (65 + 75) / 2,
        .fcos       = 0, /*387+*/
        .fincdecstp = (6 + 12) / 2,
        .fdisi_eni  = (6 + 12) / 2,
        .fdiv       = (193 + 203) / 2,
        .fdiv_32    = (215 + 225) / 2,
        .fdiv_64    = (220 + 230) / 2,
        .ffree      = (9 + 16) / 2,
        .fadd_i16   = (102 + 137) / 2,
        .fadd_i32   = (108 + 143) / 2,
        .fcom_i16   = (72 + 86) / 2,
        .fcom_i32   = (78 + 91) / 2,
        .fdiv_i16   = (224 + 238) / 2,
        .fdiv_i32   = (230 + 243) / 2,
        .fild_16    = (46 + 54) / 2,
        .fild_32    = (50 + 60) / 2,
        .fild_64    = (60 + 68) / 2,
        .fmul_i16   = (124 + 138) / 2,
        .fmul_i32   = (130 + 144) / 2,
        .finit      = (2 + 8) / 2,
        .fist_16    = (80 + 90) / 2,
        .fist_32    = (82 + 92) / 2,
        .fist_64    = (94 + 105) / 2,
        .fld        = (17 + 22) / 2,
        .fld_32     = (38 + 56) / 2,
        .fld_64     = (40 + 60) / 2,
        .fld_80     = (53 + 65) / 2,
        .fld_z1     = (11 + 21) / 2,
        .fld_const  = (15 + 24) / 2,
        .fldcw      = (7 + 14) / 2,
        .fldenv     = (35 + 45) / 2,
        .fmul       = (90 + 145) / 2,
        .fmul_32    = (110 + 125) / 2,
        .fmul_64    = (154 + 168) / 2,
        .fnop       = (10 + 16) / 2,
        .fpatan     = (250 + 800) / 2,
        .fprem      = (15 + 190) / 2,
        .fprem1     = 0, /*387+*/
        .fptan      = (30 + 540) / 2,
        .frndint    = (16 + 50) / 2,
        .frstor     = (197 + 207) / 2,
        .fsave      = (197 + 207) / 2,
        .fscale     = (32 + 38) / 2,
        .fsetpm     = 0, /*287+*/
        .fsin_cos   = 0, /*387+*/
        .fsincos    = 0, /*387+*/
        .fsqrt      = (180 + 186) / 2,
        .fst        = (15 + 22) / 2,
        .fst_32     = (84 + 90) / 2,
        .fst_64     = (96 + 104) / 2,
        .fst_80     = (52 + 58) / 2,
        .fstcw_sw   = (12 + 18) / 2,
        .fstenv     = (40 + 50) / 2,
        .ftst       = (38 + 48) / 2,
        .fucom      = 0, /*387+*/
        .fwait      = 4,
        .fxam       = (12 + 23) / 2,
        .fxch       = (10 + 15) / 2,
        .fxtract    = (27 + 55) / 2,
        .fyl2x      = (900 + 1100) / 2,
        .fyl2xp1    = (700 + 1000) / 2
};

/*Mostly the same as 8087*/
const x87_timings_t x87_timings_287 =
{
        .f2xm1      = (310 + 630) / 2,
        .fabs       = (10 + 17) / 2,
        .fadd       = (70 + 100) / 2,
        .fadd_32    = (90 + 120) / 2,
        .fadd_64    = (95 + 125) / 2,
        .fbld       = (290 + 310) / 2,
        .fbstp      = (520 + 540) / 2,
        .fchs       = (10 + 17) / 2,
        .fclex      = (2 + 8) / 2,
        .fcom       = (40 + 50) / 2,
        .fcom_32    = (60 + 70) / 2,
        .fcom_64    = (65 + 75) / 2,
        .fcos       = 0, /*387+*/
        .fincdecstp = (6 + 12) / 2,
        .fdisi_eni  = 2,
        .fdiv       = (193 + 203) / 2,
        .fdiv_32    = (215 + 225) / 2,
        .fdiv_64    = (220 + 230) / 2,
        .ffree      = (9 + 16) / 2,
        .fadd_i16   = (102 + 137) / 2,
        .fadd_i32   = (108 + 143) / 2,
        .fcom_i16   = (72 + 86) / 2,
        .fcom_i32   = (78 + 91) / 2,
        .fdiv_i16   = (224 + 238) / 2,
        .fdiv_i32   = (230 + 243) / 2,
        .fild_16    = (46 + 54) / 2,
        .fild_32    = (50 + 60) / 2,
        .fild_64    = (60 + 68) / 2,
        .fmul_i16   = (124 + 138) / 2,
        .fmul_i32   = (130 + 144) / 2,
        .finit      = (2 + 8) / 2,
        .fist_16    = (80 + 90) / 2,
        .fist_32    = (82 + 92) / 2,
        .fist_64    = (94 + 105) / 2,
        .fld        = (17 + 22) / 2,
        .fld_32     = (38 + 56) / 2,
        .fld_64     = (40 + 60) / 2,
        .fld_80     = (53 + 65) / 2,
        .fld_z1     = (11 + 21) / 2,
        .fld_const  = (15 + 24) / 2,
        .fldcw      = (7 + 14) / 2,
        .fldenv     = (35 + 45) / 2,
        .fmul       = (90 + 145) / 2,
        .fmul_32    = (110 + 125) / 2,
        .fmul_64    = (154 + 168) / 2,
        .fnop       = (10 + 16) / 2,
        .fpatan     = (250 + 800) / 2,
        .fprem      = (15 + 190) / 2,
        .fprem1     = 0, /*387+*/
        .fptan      = (30 + 540) / 2,
        .frndint    = (16 + 50) / 2,
        .frstor     = (197 + 207) / 2,
        .fsave      = (197 + 207) / 2,
        .fscale     = (32 + 38) / 2,
        .fsetpm     = (2 + 8) / 2, /*287+*/
        .fsin_cos   = 0, /*387+*/
        .fsincos    = 0, /*387+*/
        .fsqrt      = (180 + 186) / 2,
        .fst        = (15 + 22) / 2,
        .fst_32     = (84 + 90) / 2,
        .fst_64     = (96 + 104) / 2,
        .fst_80     = (52 + 58) / 2,
        .fstcw_sw   = (12 + 18) / 2,
        .fstenv     = (40 + 50) / 2,
        .ftst       = (38 + 48) / 2,
        .fucom      = 0, /*387+*/
        .fwait      = 3,
        .fxam       = (12 + 23) / 2,
        .fxch       = (10 + 15) / 2,
        .fxtract    = (27 + 55) / 2,
        .fyl2x      = (900 + 1100) / 2,
        .fyl2xp1    = (700 + 1000) / 2
};

const x87_timings_t x87_timings_387 =
{
        .f2xm1      = (211 + 476) / 2,
        .fabs       = 22,
        .fadd       = (23 + 34) / 2,
        .fadd_32    = (24 + 32) / 2,
        .fadd_64    = (29 + 37) / 2,
        .fbld       = (266 + 275) / 2,
        .fbstp      = (512 + 534) / 2,
        .fchs       = (24 + 25) / 2,
        .fclex      = 11,
        .fcom       = 24,
        .fcom_32    = 26,
        .fcom_64    = 31,
        .fcos       = (122 + 772) / 2,
        .fincdecstp = 22,
        .fdisi_eni  = 2,
        .fdiv       = (88 + 91) / 2,
        .fdiv_32    = 89,
        .fdiv_64    = 94,
        .ffree      = 18,
        .fadd_i16   = (71 + 85) / 2,
        .fadd_i32   = (57 + 72) / 2,
        .fcom_i16   = (71 + 75) / 2,
        .fcom_i32   = (56 + 63) / 2,
        .fdiv_i16   = (136 + 140) / 2,
        .fdiv_i32   = (120 + 127) / 2,
        .fild_16    = (61 + 65) / 2,
        .fild_32    = (45 + 52) / 2,
        .fild_64    = (56 + 67) / 2,
        .fmul_i16   = (76 + 87) / 2,
        .fmul_i32   = (61 + 82) / 2,
        .finit      = 33,
        .fist_16    = (82 + 95) / 2,
        .fist_32    = (79 + 93) / 2,
        .fist_64    = (80 + 97) / 2,
        .fld        = 14,
        .fld_32     = 20,
        .fld_64     = 25,
        .fld_80     = 44,
        .fld_z1     = (20 + 24) / 2,
        .fld_const  = 40,
        .fldcw      = 19,
        .fldenv     = 71,
        .fmul       = (29 + 57) / 2,
        .fmul_32    = (27 + 35) / 2,
        .fmul_64    = (32 + 57) / 2,
        .fnop       = 12,
        .fpatan     = (314 + 487) / 2,
        .fprem      = (74 + 155) / 2,
        .fprem1     = (95 + 185) / 2,
        .fptan      = (191 + 497) / 2,
        .frndint    = (66 + 80) / 2,
        .frstor     = 308,
        .fsave      = 375,
        .fscale     = (67 + 86) / 2,
        .fsetpm     = 12,
        .fsin_cos   = (122 + 771) / 2,
        .fsincos    = (194 + 809) / 2,
        .fsqrt      = (122 + 129) / 2,
        .fst        = 11,
        .fst_32     = 44,
        .fst_64     = 45,
        .fst_80     = 53,
        .fstcw_sw   = 15,
        .fstenv     = 103,
        .ftst       = 28,
        .fucom      = 24,
        .fwait      = 6,
        .fxam       = (30 + 38) / 2,
        .fxch       = 18,
        .fxtract    = (70 + 76) / 2,
        .fyl2x      = (120 + 538) / 2,
        .fyl2xp1    = (257 + 547) / 2
};

const x87_timings_t x87_timings_486 =
{
        .f2xm1      = (140 + 270) / 2,
        .fabs       = 3,
        .fadd       = (8 + 20) / 2,
        .fadd_32    = (8 + 20) / 2,
        .fadd_64    = (8 + 20) / 2,
        .fbld       = (70 + 103) / 2,
        .fbstp      = (172 + 176) / 2,
        .fchs       = 6,
        .fclex      = 7,
        .fcom       = 4,
        .fcom_32    = 4,
        .fcom_64    = 4,
        .fcos       = (257 + 354) / 2,
        .fincdecstp = 3,
        .fdisi_eni  = 3,
        .fdiv       = 73,
        .fdiv_32    = 73,
        .fdiv_64    = 73,
        .ffree      = 3,
        .fadd_i16   = (20 + 35) / 2,
        .fadd_i32   = (19 + 32) / 2,
        .fcom_i16   = (16 + 20) / 2,
        .fcom_i32   = (15 + 17) / 2,
        .fdiv_i16   = (85 + 89) / 2,
        .fdiv_i32   = (84 + 86) / 2,
        .fild_16    = (13 + 16) / 2,
        .fild_32    = (9 + 12) / 2,
        .fild_64    = (10 + 18) / 2,
        .fmul_i16   = (23 + 27) / 2,
        .fmul_i32   = (22 + 24) / 2,
        .finit      = 17,
        .fist_16    = (29 + 34) / 2,
        .fist_32    = (28 + 34) / 2,
        .fist_64    = (29 + 34) / 2,
        .fld        = 4,
        .fld_32     = 3,
        .fld_64     = 3,
        .fld_80     = 6,
        .fld_z1     = 4,
        .fld_const  = 8,
        .fldcw      = 4,
        .fldenv     = 34,
        .fmul       = 16,
        .fmul_32    = 11,
        .fmul_64    = 14,
        .fnop       = 3,
        .fpatan     = (218 + 303) / 2,
        .fprem      = (70 + 138) / 2,
        .fprem1     = (72 + 167) / 2,
        .fptan      = (200 + 273) / 2,
        .frndint    = (21 + 30) / 2,
        .frstor     = 120,
        .fsave      = 143,
        .fscale     = (30 + 32) / 2,
        .fsetpm     = 3,
        .fsin_cos   = (257 + 354) / 2,
        .fsincos    = (292 + 365) / 2,
        .fsqrt      = (83 + 87) / 2,
        .fst        = 3,
        .fst_32     = 7,
        .fst_64     = 8,
        .fst_80     = 6,
        .fstcw_sw   = 3,
        .fstenv     = 56,
        .ftst       = 4,
        .fucom      = 4,
        .fwait      = (1 + 3) / 2,
        .fxam       = 8,
        .fxch       = 4,
        .fxtract    = (16 + 20) / 2,
        .fyl2x      = (196 + 329) / 2,
        .fyl2xp1    = (171 + 326) / 2
};

/* this should be used for FPUs with no concurrency.
some pre-486DX Cyrix FPUs reportedly are like this. */
const x87_timings_t x87_concurrency_none =
{
        .f2xm1      = 0,
        .fabs       = 0,
        .fadd       = 0,
        .fadd_32    = 0,
        .fadd_64    = 0,
        .fbld       = 0,
        .fbstp      = 0,
        .fchs       = 0,
        .fclex      = 0,
        .fcom       = 0,
        .fcom_32    = 0,
        .fcom_64    = 0,
        .fcos       = 0,
        .fincdecstp = 0,
        .fdisi_eni  = 0,
        .fdiv       = 0,
        .fdiv_32    = 0,
        .fdiv_64    = 0,
        .ffree      = 0,
        .fadd_i16   = 0,
        .fadd_i32   = 0,
        .fcom_i16   = 0,
        .fcom_i32   = 0,
        .fdiv_i16   = 0,
        .fdiv_i32   = 0,
        .fild_16    = 0,
        .fild_32    = 0,
        .fild_64    = 0,
        .fmul_i16   = 0,
        .fmul_i32   = 0,
        .finit      = 0,
        .fist_16    = 0,
        .fist_32    = 0,
        .fist_64    = 0,
        .fld        = 0,
        .fld_32     = 0,
        .fld_64     = 0,
        .fld_80     = 0,
        .fld_z1     = 0,
        .fld_const  = 0,
        .fldcw      = 0,
        .fldenv     = 0,
        .fmul       = 0,
        .fmul_32    = 0,
        .fmul_64    = 0,
        .fnop       = 0,
        .fpatan     = 0,
        .fprem      = 0,
        .fprem1     = 0,
        .fptan      = 0,
        .frndint    = 0,
        .frstor     = 0,
        .fsave      = 0,
        .fscale     = 0,
        .fsetpm     = 0,
        .fsin_cos   = 0,
        .fsincos    = 0,
        .fsqrt      = 0,
        .fst        = 0,
        .fst_32     = 0,
        .fst_64     = 0,
        .fst_80     = 0,
        .fstcw_sw   = 0,
        .fstenv     = 0,
        .ftst       = 0,
        .fucom      = 0,
        .fwait      = 0,
        .fxam       = 0,
        .fxch       = 0,
        .fxtract    = 0,
        .fyl2x      = 0,
        .fyl2xp1    = 0,
};

const x87_timings_t x87_concurrency_486 =
{
        .f2xm1      = 2,
        .fabs       = 0,
        .fadd       = 7,
        .fadd_32    = 7,
        .fadd_64    = 7,
        .fbld       = 8,
        .fbstp      = 0,
        .fchs       = 0,
        .fclex      = 0,
        .fcom       = 1,
        .fcom_32    = 1,
        .fcom_64    = 1,
        .fcos       = 2,
        .fincdecstp = 0,
        .fdisi_eni  = 0,
        .fdiv       = 70,
        .fdiv_32    = 70,
        .fdiv_64    = 70,
        .ffree      = 0,
        .fadd_i16   = 7,
        .fadd_i32   = 7,
        .fcom_i16   = 1,
        .fcom_i32   = 1,
        .fdiv_i16   = 70,
        .fdiv_i32   = 70,
        .fild_16    = 4,
        .fild_32    = 4,
        .fild_64    = 8,
        .fmul_i16   = 8,
        .fmul_i32   = 8,
        .finit      = 0,
        .fist_16    = 0,
        .fist_32    = 0,
        .fist_64    = 0,
        .fld        = 0,
        .fld_32     = 0,
        .fld_64     = 0,
        .fld_80     = 0,
        .fld_z1     = 0,
        .fld_const  = 2,
        .fldcw      = 0,
        .fldenv     = 0,
        .fmul       = 13,
        .fmul_32    = 8,
        .fmul_64    = 11,
        .fnop       = 0,
        .fpatan     = 5,
        .fprem      = 2,
        .fprem1     = 6,
        .fptan      = 70,
        .frndint    = 0,
        .frstor     = 0,
        .fsave      = 0,
        .fscale     = 2,
        .fsetpm     = 0,
        .fsin_cos   = 2,
        .fsincos    = 2,
        .fsqrt      = 70,
        .fst        = 0,
        .fst_32     = 0,
        .fst_64     = 0,
        .fst_80     = 0,
        .fstcw_sw   = 0,
        .fstenv     = 0,
        .ftst       = 1,
        .fucom      = 1,
        .fwait      = 0,
        .fxam       = 0,
        .fxch       = 0,
        .fxtract    = 4,
        .fyl2x      = 13,
        .fyl2xp1    = 13,
};