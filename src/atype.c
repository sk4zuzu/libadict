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


#include <stdlib.h>
#include <string.h>

#include "libadict.h"


#define return_EMPTY_TUPLE \
    tuple.v0id_p=NULL;     \
    tuple.vo1d_p=NULL;     \
    return tuple


/* ADICT INSERT ----------------------------------------- */


adict_tup_t adict_insert_str(adict_t*adict_p,char*keystr,char*valstr){
    adict_tup_t tuple;

     int vallen=strlen(valstr);
    void*valptr=malloc(vallen);

    if(!valptr){
        return_EMPTY_TUPLE;
    }

    tuple=adict_insert(adict_p,keystr);

    if(tuple.find_p){
        memcpy(valptr,
               valstr,
               vallen+1);
        tuple.find_p->val.str=valptr;
        tuple.find_p->val.type=ADICT_STR;
    }else{
        free(valptr);
    }

    return tuple;
}


adict_tup_t adict_insert_ptr(adict_t*adict_p,char*keystr,void*valptr){
    adict_tup_t tuple=adict_insert(adict_p,keystr);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_PTR;
    }
    return tuple;
}


adict_tup_t adict_insert_i32(adict_t*adict_p,char*keystr,int32_t valint){
    adict_tup_t tuple=adict_insert(adict_p,keystr);
    if(tuple.find_p){
        tuple.find_p->val.i32=valint;
        tuple.find_p->val.type=ADICT_I32;
    }
    return tuple;
}


adict_tup_t adict_insert_i64(adict_t*adict_p,char*keystr,int64_t valint){
    adict_tup_t tuple=adict_insert(adict_p,keystr);
    if(tuple.find_p){
        tuple.find_p->val.i64=valint;
        tuple.find_p->val.type=ADICT_I64;
    }
    return tuple;
}


adict_tup_t adict_insert_dict(adict_t*adict_p,char*keystr,adict_t*valptr){
    adict_tup_t tuple=adict_insert(adict_p,keystr);
    if(tuple.find_p){
        tuple.find_p->val.ptr=(void*)valptr;
        tuple.find_p->val.type=ADICT_DICT;
    }
    return tuple;
}


adict_tup_t adict_insert_list(adict_t*adict_p,char*keystr,alist_t*valptr){
    adict_tup_t tuple=adict_insert(adict_p,keystr);
    if(tuple.find_p){
        tuple.find_p->val.ptr=(void*)valptr;
        tuple.find_p->val.type=ADICT_LIST;
    }
    return tuple;
}


/* ALIST PUSH ------------------------------------------- */


alist_tup_t alist_push_str(alist_t*alist_p,char*valstr){
    alist_tup_t tuple;

     int vallen=strlen(valstr);
    void*valptr=malloc(vallen);

    if(!valptr){
        return_EMPTY_TUPLE;
    }

    tuple=alist_push(alist_p);

    if(tuple.find_p){
        memcpy(valptr,
               valstr,
               vallen+1);
        tuple.find_p->val.str=valptr;
        tuple.find_p->val.type=ADICT_STR;
    }else{
        free(valptr);
    }

    return tuple;
}


alist_tup_t alist_push_ptr(alist_t*alist_p,void*valptr){
    alist_tup_t tuple=alist_push(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_PTR;
    }
    return tuple;
}


alist_tup_t alist_push_i32(alist_t*alist_p,int32_t valint){
    alist_tup_t tuple=alist_push(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.i32=valint;
        tuple.find_p->val.type=ADICT_I32;
    }
    return tuple;
}


alist_tup_t alist_push_i64(alist_t*alist_p,int64_t valint){
    alist_tup_t tuple=alist_push(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.i64=valint;
        tuple.find_p->val.type=ADICT_I64;
    }
    return tuple;
}


alist_tup_t alist_push_dict(alist_t*alist_p,adict_t*valptr){
    alist_tup_t tuple=alist_push(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_DICT;
    }
    return tuple;
}


alist_tup_t alist_push_list(alist_t*alist_p,alist_t*valptr){
    alist_tup_t tuple=alist_push(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_LIST;
    }
    return tuple;
}


/* ALIST UNSHIFT ---------------------------------------- */


alist_tup_t alist_unshift_str(alist_t*alist_p,char*valstr){
    alist_tup_t tuple;

     int vallen=strlen(valstr);
    void*valptr=malloc(vallen);

    if(!valptr){
        return_EMPTY_TUPLE;
    }

    tuple=alist_unshift(alist_p);

    if(tuple.find_p){
        memcpy(valptr,
               valstr,
               vallen+1);
        tuple.find_p->val.str=valptr;
        tuple.find_p->val.type=ADICT_STR;
    }else{
        free(valptr);
    }

    return tuple;
}


alist_tup_t alist_unshift_ptr(alist_t*alist_p,void*valptr){
    alist_tup_t tuple=alist_unshift(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_PTR;
    }
    return tuple;
}


alist_tup_t alist_unshift_i32(alist_t*alist_p,int32_t valint){
    alist_tup_t tuple=alist_unshift(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.i32=valint;
        tuple.find_p->val.type=ADICT_I32;
    }
    return tuple;
}


alist_tup_t alist_unshift_i64(alist_t*alist_p,int64_t valint){
    alist_tup_t tuple=alist_unshift(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.i64=valint;
        tuple.find_p->val.type=ADICT_I64;
    }
    return tuple;
}


alist_tup_t alist_unshift_dict(alist_t*alist_p,adict_t*valptr){
    alist_tup_t tuple=alist_unshift(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_DICT;
    }
    return tuple;
}


alist_tup_t alist_unshift_list(alist_t*alist_p,alist_t*valptr){
    alist_tup_t tuple=alist_unshift(alist_p);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_LIST;
    }
    return tuple;
}


/* ALIST INSERT ----------------------------------------- */


alist_tup_t alist_insert_str(alist_t*alist_p,uint32_t idx,char*valstr){
    alist_tup_t tuple;

     int vallen=strlen(valstr);
    void*valptr=malloc(vallen);

    if(!valptr){
        return_EMPTY_TUPLE;
    }

    tuple=alist_insert(alist_p,idx);

    if(tuple.find_p){
        memcpy(valptr,
               valstr,
               vallen+1);
        tuple.find_p->val.str=valptr;
        tuple.find_p->val.type=ADICT_STR;
    }else{
        free(valptr);
    }

    return tuple;
}


alist_tup_t alist_insert_ptr(alist_t*alist_p,uint32_t idx,void*valptr){
    alist_tup_t tuple=alist_insert(alist_p,idx);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_PTR;
    }
    return tuple;
}


alist_tup_t alist_insert_i32(alist_t*alist_p,uint32_t idx,int32_t valint){
    alist_tup_t tuple=alist_insert(alist_p,idx);
    if(tuple.find_p){
        tuple.find_p->val.i32=valint;
        tuple.find_p->val.type=ADICT_I32;
    }
    return tuple;
}


alist_tup_t alist_insert_i64(alist_t*alist_p,uint32_t idx,int64_t valint){
    alist_tup_t tuple=alist_insert(alist_p,idx);
    if(tuple.find_p){
        tuple.find_p->val.i64=valint;
        tuple.find_p->val.type=ADICT_I64;
    }
    return tuple;
}


alist_tup_t alist_insert_dict(alist_t*alist_p,uint32_t idx,adict_t*valptr){
    alist_tup_t tuple=alist_insert(alist_p,idx);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_DICT;
    }
    return tuple;
}


alist_tup_t alist_insert_list(alist_t*alist_p,uint32_t idx,alist_t*valptr){
    alist_tup_t tuple=alist_insert(alist_p,idx);
    if(tuple.find_p){
        tuple.find_p->val.ptr=valptr;
        tuple.find_p->val.type=ADICT_LIST;
    }
    return tuple;
}


// vim:ts=4:sw=4:et:
