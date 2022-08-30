#ifndef D_MK66F18_SVBMP_PORT_H
#define D_MK66F18_SVBMP_PORT_H

#include <inc_stdlib.h>
#include "cmodule_common.h"

#if (defined(CMODULE_USE_APP_SVBMP) && (CMODULE_USE_APP_SVBMP > 0))
#include "ff.h"

//#ifdef ON_WINDOWS
//#include <stdio.h>
//#define FIL FILE
//#else
//#include "ff.h"
//#endif // ON_WINDOWS

typedef FIL svbmp_file_t;

inline int SVBMP_FileWrite(svbmp_file_t* fp, const void* buff, uint32_t btw)
{
#ifdef ON_WINDOWS
    fwrite(buff, 1, btw, fp);
#else
    UINT bw =0;
    f_write(fp,buff,btw,&bw);
    if(bw!=btw){PRINTF("the volume got full\r\n");}
#endif // ON_WINDOWS
    return 0;
}

#endif // ! CMODULE_USE_APP_SVBMP

#endif // ! D_MK66F18_SVBMP_PORT_H
