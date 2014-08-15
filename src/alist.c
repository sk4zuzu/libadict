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


#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libadict.h"


#define return_EMPTY_TUPLE \
    tuple.v0id_p=NULL;     \
    tuple.vo1d_p=NULL;     \
    return tuple


#define MINXY(x,y) ((y)^(((x)^(y))&-((x)<(y)))) // BTH


uint32_t _lvlrand(){
    register uint32_t x=urand32();

    x<<=(sizeof(x)<<3)-ALIST_LVL_CNT_2-1;
    x>>=(sizeof(x)<<3)-ALIST_LVL_CNT_2-1;

    register int lvl=ALIST_LVL_CNT_2;

    for(;lvl>ALIST_LVL_CNT;lvl--){
        if(x>>lvl){
            return (ALIST_LVL_CNT_2-lvl);
        }
    }
    for(;lvl>0;lvl--){
        if(x>>lvl){
            return (ALIST_LVL_CNT-lvl);
        }
    }
    return (ALIST_LVL_CNT-1);
}


static
alist_node_t*_new(){
    uint32_t lvlcnt=_lvlrand()+1;

    uint32_t allocsize=sizeof(alist_node_t)
                      +sizeof(alist_lvl_t[lvlcnt-1]);

    alist_node_t*new_p=calloc(1,allocsize);
    if(new_p){
        new_p->lvlcnt=lvlcnt;
    }
    return new_p;
}


inline
alist_t*alist_new(void){
    uint32_t lvlcnt=ALIST_LVL_CNT;

    uint32_t allocsize=sizeof(alist_t)
                      +sizeof(alist_lvl_t[lvlcnt-1]);

    alist_t*alist_p=calloc(1,allocsize);
    if(alist_p){
        alist_node_t*link_p=&alist_p->link;

        link_p->lvlcnt=lvlcnt;

        int lvl;
        for(lvl=0;lvl<(lvlcnt);lvl++){
            link_p->_[lvl].next_p=link_p; // head
            link_p->_[lvl].prev_p=link_p; // tail
        }
    }
    return alist_p;
}


static inline
void _drop(alist_node_t*node_p){
    if(node_p){
        if(node_p->val.type==ADICT_STR && node_p->val.str){
            free(node_p->val.str);
        }
        free(node_p);
    }
}


void alist_drop(alist_t*alist_p){
    if(!alist_p){
        return;
    }
    alist_node_t*link_p=&alist_p->link,
                *node_p=link_p->next_p, // head
                *next_p;
    while((node_p!=link_p)){
        next_p=node_p->next_p;
        _drop(node_p);
        node_p=next_p;
    }
    free(alist_p);
}


alist_tup_t alist_search(alist_t*alist_p,uint32_t idx){
    alist_tup_t tuple;
    alist_node_t*node_p=&alist_p->link,
                *next_p;
    int lvl=ALIST_LVL_CNT-1,
        inc=0;
    while(lvl>=0){
        if((next_p=node_p->_[lvl].next_p)==&alist_p->link){
            lvl--;
            continue;
        }
        uint32_t tmp=inc+(next_p->_[lvl].inc);
        if(tmp>idx){
            lvl--;
            continue;
        }
        if(tmp<idx){
               inc=tmp;
            node_p=next_p;
            continue;
        }
        tuple.find_p=next_p;
        tuple.prev_p=node_p;
        return tuple;
    }
    return_EMPTY_TUPLE;
}


static
alist_node_t*_pop(alist_t*alist_p){
    alist_node_t*link_p=&alist_p->link,
                *node_p=link_p->prev_p, // tail
                *prev_p;
    int lvl;
    for(lvl=0;lvl<(node_p->lvlcnt);lvl++){
        prev_p=node_p->_[lvl].prev_p;
        prev_p->_[lvl].next_p=link_p;
        link_p->_[lvl].prev_p=prev_p;
    }
    alist_p->count--;
    return node_p;
}


alist_tup_t alist_pop(alist_t*alist_p){
    alist_tup_t tuple;
    alist_node_t*node_p=_pop(alist_p);
    if(node_p){
        tuple.find_p=node_p;
        tuple.prev_p=node_p->prev_p;
        _drop(node_p);
        return tuple;
    }
    return_EMPTY_TUPLE;
}


static
void _push(alist_t*alist_p,alist_node_t*new_p){
    alist_node_t*link_p=&alist_p->link,
                *prev_p=link_p->prev_p; // tail
    int inc=(alist_p->count)?1:0,
        lvl=0;
    while(lvl<(new_p->lvlcnt)){
        uint32_t mincnt=MINXY(prev_p->lvlcnt,
                               new_p->lvlcnt);
        for(;lvl<(mincnt);lvl++){
            prev_p->_[lvl].next_p=new_p;
            link_p->_[lvl].prev_p=new_p; // tail
             new_p->_[lvl].next_p=link_p;
             new_p->_[lvl].prev_p=prev_p;
             new_p->_[lvl].inc=inc;
        }
          inc+=prev_p->_[lvl-1].inc;
        prev_p=prev_p->_[lvl-1].prev_p;
    }
    alist_p->count++;
}


alist_tup_t alist_push(alist_t*alist_p){
    alist_tup_t tuple;
    alist_node_t*new_p=_new();
    if(new_p){
        tuple.tail_p=new_p;
        tuple.prev_p=alist_p->link.prev_p; // tail
        _push(alist_p,new_p);
        return tuple;
    }
    return_EMPTY_TUPLE;
}


static
alist_node_t*_shift(alist_t*alist_p){
    alist_node_t*link_p=&alist_p->link,
                *node_p=link_p->next_p, // head
                *next_p;
    int lvl;
    for(lvl=0;lvl<(node_p->lvlcnt);lvl++){
        next_p=node_p->_[lvl].next_p;
        next_p->_[lvl].inc--;
        next_p->_[lvl].prev_p=link_p;
        link_p->_[lvl].next_p=next_p;
    }
    while(lvl<(ALIST_LVL_CNT)){
        if((next_p=link_p->_[lvl].next_p)==link_p){
            break;
        }
        for(;lvl<(next_p->lvlcnt);lvl++){
            next_p->_[lvl].inc--;
        }
    }
    alist_p->count--;
    return node_p;
}


alist_tup_t alist_shift(alist_t*alist_p){
    alist_tup_t tuple;
    alist_node_t*node_p=_shift(alist_p);
    if(node_p){
        tuple.find_p=node_p;
        tuple.prev_p=node_p->prev_p;
        _drop(node_p);
        return tuple;
    }
    return_EMPTY_TUPLE;
}


static
void _unshift(alist_t*alist_p,alist_node_t*new_p){
    alist_node_t*link_p=&alist_p->link,
                *next_p;
    int lvl;
    for(lvl=0;lvl<(new_p->lvlcnt);lvl++){
        next_p=link_p->_[lvl].next_p;
        next_p->_[lvl].prev_p=new_p;
        next_p->_[lvl].inc++;
        link_p->_[lvl].next_p=new_p;
         new_p->_[lvl].next_p=next_p;
         new_p->_[lvl].prev_p=link_p;
    }
    for(;lvl<(ALIST_LVL_CNT);lvl++){
        if((next_p=link_p->_[lvl].next_p)==link_p){
            break;
        }
        next_p->_[lvl].inc++;
    }
    alist_p->count++;
}


alist_tup_t alist_unshift(alist_t*alist_p){
    alist_tup_t tuple;
    alist_node_t*new_p=_new();
    if(new_p){
        tuple.find_p=new_p;
        tuple.prev_p=&alist_p->link;
        _unshift(alist_p,new_p);
        return tuple;
    }
    return_EMPTY_TUPLE;
}


static
alist_node_t*_remove(alist_t*alist_p,alist_node_t*node_p){
    alist_node_t*prev_p,
                *next_p;
    int lvl;
    for(lvl=0;lvl<(node_p->lvlcnt);lvl++){
        next_p=node_p->_[lvl].next_p;
        prev_p=node_p->_[lvl].prev_p;
        next_p->_[lvl].prev_p=prev_p;
        prev_p->_[lvl].next_p=next_p;
        next_p->_[lvl].inc+=node_p->_[lvl].inc-1;
    }
    while(lvl<(ALIST_LVL_CNT)){
        if(next_p==&(alist_p->link)){
            break;
        }
        for(;lvl<(next_p->lvlcnt);lvl++){
            next_p->_[lvl].inc--;
        }
        next_p=next_p->_[lvl-1].next_p;
    }
    alist_p->count--;
    return node_p;
}


alist_tup_t alist_remove(alist_t*alist_p,uint32_t idx){
    if(idx==0){
        return alist_shift(alist_p);
    }
    if(idx==(alist_p->count-1)){
        return alist_pop(alist_p);
    }
    alist_tup_t tuple=alist_search(alist_p,idx);
    if(tuple.find_p){
        _remove(alist_p,tuple.find_p);
        _drop(tuple.find_p);
    }
    return tuple;
}


static
void _insert(alist_t*alist_p,alist_node_t*prev_p,
                             alist_node_t*new_p){
    alist_node_t*link_p=&alist_p->link,
                *next_p;
    int inc=1,
        lvl=0;
    while(lvl<(new_p->lvlcnt)){
        uint32_t mincnt=MINXY(prev_p->lvlcnt,
                               new_p->lvlcnt);
        for(;lvl<(mincnt);lvl++){
            next_p=prev_p->_[lvl].next_p;
            next_p->_[lvl].prev_p=new_p;
            prev_p->_[lvl].next_p=new_p;
             new_p->_[lvl].prev_p=prev_p;
             new_p->_[lvl].next_p=next_p;
             new_p->_[lvl].inc=inc; 
        }
          inc+=prev_p->_[lvl-1].inc;
        prev_p=prev_p->_[lvl-1].prev_p;
    }
    while(lvl<(ALIST_LVL_CNT)){
        if(next_p==&(alist_p->link)){
            break;
        }
        for(;lvl<(next_p->lvlcnt);lvl++){
            next_p->_[lvl].inc++;
        }
        next_p=next_p->_[lvl-1].next_p;
    }
    alist_p->count++;
}


alist_tup_t alist_insert(alist_t*alist_p,uint32_t idx){
    if(idx==0){
        return alist_unshift(alist_p);
    }
    if(idx==alist_p->count){
        return alist_push(alist_p);
    }
    alist_tup_t tuple=alist_search(alist_p,idx-1);
    if(tuple.find_p){
        alist_node_t*new_p=_new();
        if(new_p){
            _insert(alist_p,tuple.find_p,new_p);
            tuple.find_p=new_p;
            tuple.prev_p=new_p->prev_p;
            return tuple;
        }
    }
    return_EMPTY_TUPLE;
}


alist_iter_t*alist_iter_new(alist_t*alist_p){
    alist_iter_t*iter_p=calloc(1,sizeof(alist_iter_t));
    if(iter_p){
        iter_p->alist_p=alist_p;
        iter_p->node_p=&alist_p->link;
    }
    return iter_p;
}


inline
void alist_iter_drop(alist_iter_t*iter_p){
    if(iter_p){free(iter_p);}
}


inline
alist_node_t*alist_iter_next(alist_iter_t*iter_p){
    return
    (iter_p->node_p=(iter_p->node_p->next_p==&iter_p->alist_p->link)
                   ?NULL
                   :iter_p->node_p->next_p);
}


// vim:ts=4:sw=4:et:
