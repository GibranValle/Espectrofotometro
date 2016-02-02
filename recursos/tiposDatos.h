/* 																				||								Range							||
||            Type				|| Size		|| Alignment||    Representation	||           Minimum 			||           Maximum 			||
--------------------------------------------------------------------------------------------------------------------------------------------------
|| char							|| 8 bits	|| 8		|| ASCII				|| 0 or -128 					|| 255 or 127 					||
|| unsigned char				|| 8 bits	|| 8		|| Binary				|| 0							|| 255							||
|| bool	(C99)					|| 8 bits	|| 8		|| Binary				|| 0 (false)					|| 1 (true)						||
|| _Bool	 (C99)				|| 8 bits	|| 8		|| Binary				|| 0 (false)					|| 1 (true)						||
|| bool (C++)					|| 8 bits	|| 8		|| Binary				|| 0 (false)					|| 1 (true)						||
|| short, signed short			|| 16 bits	|| 16		|| 2s complement		|| -32 768						|| 	32 767						||
|| unsigned short				|| 16 bits	|| 16		|| Binary				|| 0							|| 65 535						||
|| int, signed int				|| 16 bits	|| 16		|| 2s complement		|| -32 768						|| 32 767						||
|| unsigned int					|| 16 bits	|| 16		|| Binary				|| 0							|| 65 535						||
|| long, signed long			|| 32 bits	|| 16		|| 2s complement		|| -2 147 483 648				|| 2 147 483 647				||
|| unsigned long				|| 32 bits	|| 16		|| Binary				|| 0							|| 4 294 967 295				||
|| long long, signed long long	|| 64 bits	|| 16		|| 2s complement		|| -9 223 372 036 854 775 808	|| 9 223 372 036 854 775 807	||
|| unsigned long long			|| 64 bits	|| 16		|| Binary				|| 0							|| 18 446 744 073 709 551 615	||
|| enum							|| 	varies	|| 16		|| 2s complement		|| varies						|| varies						||
|| float						|| 32 bits	|| 16		|| IEEE 32-bit			|| 1.175 494e-38				|| 	3.40 282 346e+38			||
|| double						|| 64 bits	|| 16		|| IEEE 64-bit			|| 2.22 507 385e-308			|| 	1.79 769 313e+308			||
|| long double					|| 64 bits	|| 16		|| IEEE 64-bit			|| 2.22 507 385e-308[f]			|| 	1.79 769 313e+308			||
--------------------------------------------------------------------------------------------------------------------------------------------------
     typedef   signed char   int8_t;
    typedef unsigned char  uint8_t;
    typedef          short  int16_t;
    typedef unsigned short uint16_t;
    typedef          int    int32_t;
    typedef unsigned int   uint32_t;
    typedef          long long  int64_t;
    typedef unsigned long long uint64_t;

 */
