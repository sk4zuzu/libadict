//
// LIBADICT 0.2 20140815 copyright sk4zuzu@gmail.com 2014
//
// This file is part of LIBADICT.
//
// LIBADICT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LIBADICT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LIBADICT.  If not, see <http://www.gnu.org/licenses/>.
//


#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#include "config.h"
#include "libadict.h"


#ifdef HAVE_DEV_URANDOM
static int fd=-1;
#endif /* #ifdef HAVE_DEV_URANDOM */


void urand_init(void){
#ifdef HAVE_DEV_URANDOM
    if(fd<0){
        fd=open("/dev/urandom",O_RDONLY);

        if(fd<0){
            perror("urand_init().open()");
            exit(1);
        }
    }
#else
    srand(getpid());
#endif /* #ifdef HAVE_DEV_URANDOM */
}


int8_t urand8(void){
#ifdef HAVE_DEV_URANDOM
    int8_t value8;

    int len=read(fd,&value8,sizeof(value8));

    if(len<=0){
        perror("urand().read()");
        exit(1);
    }

    if(len != sizeof(value8)){
        printf("%s\n","len is not equal to sizeof(value8)");
        exit(1);
    }

    return value8;
#else
    return (int8_t)rand();
#endif /* #ifdef HAVE_DEV_URANDOM */
}


int16_t urand16(void){
#ifdef HAVE_DEV_URANDOM
    int16_t value16;

    int len=read(fd,&value16,sizeof(value16));

    if(len<=0){
        perror("urand().read()");
        exit(1);
    }

    if(len != sizeof(value16)){
        printf("%s\n","len is not equal to sizeof(value8)");
        exit(1);
    }

    return value16;
#else
    return (int16_t)rand();
#endif /* #ifdef HAVE_DEV_URANDOM */
}


int32_t urand32(void){
#ifdef HAVE_DEV_URANDOM
    int32_t value32;

    int len=read(fd,&value32,sizeof(value32));

    if(len<=0){
        perror("urand().read()");
        exit(1);
    }

    if(len != sizeof(value32)){
        printf("%s\n","len is not equal to sizeof(value32)");
        exit(1);
    }

    return value32;
#else
    return (int32_t)rand();
#endif /* #ifdef HAVE_DEV_URANDOM */
}


// vim:ts=4:sw=4:et:
