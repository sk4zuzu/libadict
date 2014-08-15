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


#ifndef __LIBADICT__H
#define __LIBADICT__H 1

#include <stdint.h>


typedef struct adict_s      adict_t;
typedef struct alist_s      alist_t;
typedef struct adict_node_s adict_node_t;
typedef struct alist_node_s alist_node_t;


/* ADICT ------------------------------------------------ */


typedef enum {
    ADICT_INV=0,
    ADICT_STR=1,
    ADICT_PTR,
    ADICT_I32,
    ADICT_I64,
    ADICT_DICT,
    ADICT_LIST
} adict_value_types_t;


typedef union {
     uint8_t  u8; uint16_t u16;
      int8_t  i8;  int16_t i16;
    uint32_t u32; uint64_t u64;
     int32_t i32;  int64_t i64;

    char*str;
    void*ptr;

    struct {
        union {
            uint8_t  _u8; uint16_t _u16;
             int8_t  _i8;  int16_t _i16;
           uint32_t _u32; uint64_t _u64;
            int32_t _i32;  int64_t _i64;

            char*_str;
            void*_ptr;
        };
        int8_t type;        
    };
} adict_obj_t;


typedef struct {
        char*data;
    uint32_t size;
} adict_key_t;


typedef struct {
    union {
                void*v0id_p;
        adict_node_t*next_p;
        adict_node_t*find_p;
    };
    union {
                void*vo1d_p;
        adict_node_t*prev_p;
    };
} adict_tup_t;


struct adict_node_s {
    adict_node_t*prev_p,
                *left_p,
                *rght_p;
        uint32_t prio;
     adict_key_t key;
     adict_obj_t val;
};


struct adict_s {
    adict_node_t*root_p;
        uint32_t count;
};


adict_t    *adict_new    (void);
void        adict_drop   (adict_t*adict_p);
adict_tup_t adict_search (adict_t*adict_p,char*keystr);
adict_tup_t adict_remove (adict_t*adict_p,char*keystr);
adict_tup_t adict_insert (adict_t*adict_p,char*keystr);


typedef struct {
    adict_t     *adict_p;
    adict_node_t*node_p;
} adict_iter_t;


adict_iter_t*adict_iter_new  (adict_t*adict_p);
void         adict_iter_drop (adict_iter_t*iter_p);
adict_node_t*adict_iter_next (adict_iter_t*iter_p);


/* ALIST ------------------------------------------------ */


#define ALIST_LVL_CNT   8
#define ALIST_LVL_CNT_2 (ALIST_LVL_CNT<<1)


typedef struct {
    union {
                void*v0id_p;
        alist_node_t*next_p;
        alist_node_t*find_p;
        alist_node_t*tail_p;
    };
    union {
                void*vo1d_p;
        alist_node_t*prev_p;
    };
} alist_tup_t;


typedef struct {
    alist_node_t*prev_p,
                *next_p;
        uint32_t inc;
} alist_lvl_t;


struct alist_node_s {
    adict_obj_t val;
       uint32_t lvlcnt;
    union {
        struct {
            alist_node_t*prev_p,
                        *next_p;
                uint32_t inc;
        };
        alist_lvl_t _[1]; // all this is a kinda
                          // variable-size-tear-off-struct
                          // such tech wow
    };
};


struct alist_s {
        uint32_t count;
    alist_node_t link; // tear-off-stuff-here too
};


alist_t    *alist_new    (void);
void        alist_drop   (alist_t*alist_p);
alist_tup_t alist_search (alist_t*alist_p,uint32_t idx);

alist_tup_t alist_pop  (alist_t*alist_p);
alist_tup_t alist_push (alist_t*alist_p);

alist_tup_t alist_shift   (alist_t*alist_p);
alist_tup_t alist_unshift (alist_t*alist_p);

alist_tup_t alist_remove (alist_t*alist_p,uint32_t idx);
alist_tup_t alist_insert (alist_t*alist_p,uint32_t idx);


typedef struct {
         alist_t*alist_p;
    alist_node_t*node_p;
} alist_iter_t;


alist_iter_t*alist_iter_new  (alist_t*alist_p);
void         alist_iter_drop (alist_iter_t*iter_p);
alist_node_t*alist_iter_next (alist_iter_t*iter_p);


/* ATYPE ------------------------------------------------ */


adict_tup_t adict_insert_str  (adict_t*adict_p,char*keystr,   char*valstr);
adict_tup_t adict_insert_ptr  (adict_t*adict_p,char*keystr,   void*valptr);
adict_tup_t adict_insert_i32  (adict_t*adict_p,char*keystr,int32_t valint);
adict_tup_t adict_insert_i64  (adict_t*adict_p,char*keystr,int64_t valint);
adict_tup_t adict_insert_dict (adict_t*adict_p,char*keystr,adict_t*valptr);
adict_tup_t adict_insert_list (adict_t*adict_p,char*keystr,alist_t*valptr);


alist_tup_t alist_push_str  (alist_t*alist_p,   char*valstr);
alist_tup_t alist_push_ptr  (alist_t*alist_p,   void*valptr);
alist_tup_t alist_push_i32  (alist_t*alist_p,int32_t valint);
alist_tup_t alist_push_i64  (alist_t*alist_p,int64_t valint);
alist_tup_t alist_push_dict (alist_t*alist_p,adict_t*valptr);
alist_tup_t alist_push_list (alist_t*alist_p,alist_t*valptr);


alist_tup_t alist_unshift_str  (alist_t*alist_p,   char*valstr);
alist_tup_t alist_unshift_ptr  (alist_t*alist_p,   void*valptr);
alist_tup_t alist_unshift_i32  (alist_t*alist_p,int32_t valint);
alist_tup_t alist_unshift_i64  (alist_t*alist_p,int64_t valint);
alist_tup_t alist_unshift_dict (alist_t*alist_p,adict_t*valptr);
alist_tup_t alist_unshift_list (alist_t*alist_p,alist_t*valptr);


alist_tup_t alist_insert_str  (alist_t*alist_p,uint32_t idx,   char*valstr);
alist_tup_t alist_insert_ptr  (alist_t*alist_p,uint32_t idx,   void*valptr);
alist_tup_t alist_insert_i32  (alist_t*alist_p,uint32_t idx,int32_t valint);
alist_tup_t alist_insert_i64  (alist_t*alist_p,uint32_t idx,int64_t valint);
alist_tup_t alist_insert_dict (alist_t*alist_p,uint32_t idx,adict_t*valptr);
alist_tup_t alist_insert_list (alist_t*alist_p,uint32_t idx,alist_t*valptr);


/* URAND ------------------------------------------------ */


void    urand_init (void);
int8_t  urand8     (void);
int16_t urand16    (void);
int32_t urand32    (void);


/* DEBUG ------------------------------------------------ */


int debug_adict_dump       (adict_t*adict_p);
int debug_adict_check_heap (adict_t*adict_p);
int debug_alist_dump       (alist_t*alist_p);


#endif
// vim:ts=4:sw=4:et:
