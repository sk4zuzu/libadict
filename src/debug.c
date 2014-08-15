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


#include <stdio.h>

#include "config.h"
#include "libadict.h"


int debug_adict_dump(adict_t*adict_p){
    adict_node_t*node_p;
    adict_iter_t*iter_p=adict_iter_new(adict_p);

    int k=0;
    while((node_p=adict_iter_next(iter_p))){
        fprintf(stderr,"%s=%08d"
                      " %s=%s"
                      " %s=%08x"
                      " %s=%p"
                      " %s=%p"
                      " %s=%p\n",
                             "k",++k,
                          " key",node_p->key.data,
                         " PRIO",node_p->prio,
                            " P",node_p->prev_p,
                            " L",node_p->left_p,
                            " R",node_p->rght_p);
    }

    adict_iter_drop(iter_p);

    fprintf(stdout,"%d keys present in the dict\n",k);
    return k;
}


int debug_adict_check_heap(adict_t*adict_p){
    adict_node_t*node_p;
    adict_iter_t*iter_p=adict_iter_new(adict_p);

    unsigned depth,max_depth=0;

    while((node_p=adict_iter_next(iter_p))){
        if(node_p->left_p || node_p->rght_p){
            continue;
        }

        for(depth=0;node_p->prev_p;depth++){
            adict_node_t*prev_p=node_p->prev_p;

            if(node_p->prio > prev_p->prio){
                fprintf(stdout,"%s\n","heap is invalid");
                return 0;
            }

            node_p=prev_p;
        }

        max_depth=(depth > max_depth)?depth
                                     :max_depth;
    }

    adict_iter_drop(iter_p);

    fprintf(stdout,"%s, %s=%d\n","heap is valid",
                                     "max_depth",max_depth);
    return 1;
}


static inline
void _alist_dump(alist_t*alist_p,alist_node_t*node_p,int k){
    fprintf(stderr,"k=%d,"
                   "node_p=%p,"
                   "node_p->lvlcnt=%d\n",k,
                                         node_p,
                                         node_p->lvlcnt);
    int lvl;
    for(lvl=node_p->lvlcnt;--lvl>=0;){
        fprintf(stderr,"    "
                       "lvl=%d,"
                       "inc=%d;"
                       "prev_p=%p,"
                       "next_p=%p\n",lvl,
                                     node_p->_[lvl].inc,
                                     node_p->_[lvl].prev_p,
                                     node_p->_[lvl].next_p);
    }
    fprintf(stderr,"\n");
}

int debug_alist_dump(alist_t*alist_p){
    alist_node_t*node_p;
    alist_iter_t*iter_p=alist_iter_new(alist_p);

    _alist_dump(alist_p,&alist_p->link,-1);

    int k;
    for(k=0;(node_p=alist_iter_next(iter_p));k++){
        _alist_dump(alist_p,node_p,k);
    }

    alist_iter_drop(iter_p);

    fprintf(stdout,"%d items present in the list\n",k);
    return k;
}


// vim:ts=4:sw=4:et:
