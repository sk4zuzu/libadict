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


static
adict_node_t*_new(char*keystr){
    adict_node_t*node_p=calloc(1,sizeof(adict_node_t));
    if(node_p){
        int keylen=strlen(keystr);
        node_p->key.data=malloc(keylen+1);
        if(node_p->key.data){
            memcpy(node_p->key.data,keystr,
                                    keylen+1);
            node_p->key.size=keylen;
        }else{
            free(node_p);
            node_p=NULL;
        }
    }
    return node_p;
}


static inline
void _drop(adict_node_t*node_p){
    if(node_p){
        if(node_p->key.data){
            free(node_p->key.data);
        }
        if(node_p->val.type==ADICT_STR && node_p->val.str){
            free(node_p->val.str);
        }
        free(node_p);
    }
}


static
void _proper_drop(adict_t*adict_p,adict_node_t*search_p){
    if(!search_p){
        return;
    }

    adict_node_t*parent_p=search_p->prev_p;

    if(parent_p){
        if(parent_p->left_p == search_p){
            parent_p->left_p=NULL;
        }
        else
        if(parent_p->rght_p == search_p){
            parent_p->rght_p=NULL;
        }
    }else{
        adict_p->root_p=search_p;
    }

    if(search_p->left_p){
        search_p->left_p->prev_p=parent_p;
    }
    if(search_p->rght_p){
        search_p->rght_p->prev_p=parent_p;
    }

    _drop(search_p);
}


static
void _rotate(adict_t*adict_p,adict_node_t*search_p){
    adict_node_t*parent_p=search_p->prev_p;

    if(!parent_p){
        return;
    }

    adict_node_t*grandp_p=parent_p->prev_p;

    if(grandp_p){
        if(grandp_p->left_p == parent_p){
            grandp_p->left_p=search_p;
            search_p->prev_p=grandp_p;
        }
        else
        if(grandp_p->rght_p == parent_p){
            grandp_p->rght_p=search_p;
            search_p->prev_p=grandp_p;
        }
    }else{
        search_p->prev_p=NULL;
        adict_p->root_p=search_p;
    }

    if(parent_p->left_p == search_p){
        adict_node_t*rght_p=search_p->rght_p;
        if(rght_p){
            rght_p->prev_p=parent_p;
        }
        parent_p->left_p=rght_p;
        search_p->rght_p=parent_p;
        parent_p->prev_p=search_p;
    }
    else
    if(parent_p->rght_p == search_p){
        adict_node_t*left_p=search_p->left_p;
        if(left_p){
            left_p->prev_p=parent_p;
        }
        parent_p->rght_p=left_p;
        search_p->left_p=parent_p;
        parent_p->prev_p=search_p;
    }
}


inline
adict_t*adict_new(void){
    return calloc(1,sizeof(adict_t));
}


void adict_drop(adict_t*adict_p){
    if(!adict_p){
        return;
    }
    while(adict_p->root_p){
        if(adict_p->root_p->left_p){
            _rotate(adict_p,adict_p->root_p->left_p);
        }else{
            adict_node_t*drop_p=adict_p->root_p;
            if((adict_p->root_p=adict_p->root_p->rght_p)){
                adict_p->root_p->prev_p=NULL;
            }
            _drop(drop_p);
        }
    }
    free(adict_p);
}


adict_tup_t adict_search(adict_t*adict_p,char*keystr){
    adict_tup_t tuple;

    tuple.prev_p=NULL;
    tuple.next_p=adict_p->root_p;

    while(tuple.next_p){
        adict_node_t*search_p=tuple.next_p;

        if(!search_p){
            tuple.find_p=NULL;
            break;
        }

        int compare=memcmp(keystr,search_p->key.data,
                                  search_p->key.size);

        if(!compare){ // EXACT MATCH
            break;
        }

        tuple.prev_p=tuple.next_p;

        tuple.next_p=compare < 0
                    ?(void*)search_p->left_p
                    :(void*)search_p->rght_p;
    }
    return tuple;
}


adict_tup_t adict_remove(adict_t*adict_p,char*keystr){
    adict_tup_t tuple=adict_search(adict_p,keystr);

    if(!tuple.find_p){
        return tuple;
    }

    adict_node_t*search_p=tuple.find_p;

    if(!search_p){
        tuple.find_p=NULL;
        return tuple;
    }

    for(;;){
        if(search_p->left_p && search_p->rght_p){
            adict_node_t*left_p=search_p->left_p,
                        *rght_p=search_p->rght_p;

            if(left_p->prio > rght_p->prio){
                _rotate(adict_p,search_p->left_p);
                continue;
            }else{
                _rotate(adict_p,search_p->rght_p);
                continue;
            }
        }

        if(search_p->left_p){
            _rotate(adict_p,search_p->left_p);
            continue;
        }
        if(search_p->rght_p){
            _rotate(adict_p,search_p->rght_p);
            continue;
        }

        // LEAF
        _proper_drop(adict_p,tuple.find_p);
        break;
    }

    adict_p->count--;

    return tuple;
}


adict_tup_t adict_insert(adict_t*adict_p,char*keystr){
    adict_tup_t tuple=adict_search(adict_p,keystr);

    if(tuple.find_p){
        return_EMPTY_TUPLE;
    }

    adict_node_t*search_p=_new(keystr);

    if(!search_p){
        return_EMPTY_TUPLE;
    }

    tuple.find_p=search_p;

    if(tuple.prev_p){
        adict_node_t*parent_p=tuple.prev_p;

        if(memcmp(keystr,parent_p->key.data,
                         parent_p->key.size) < 0){
            parent_p->left_p=search_p;
        }else{
            parent_p->rght_p=search_p;
        }
    }else{
        adict_p->root_p=search_p;
    }

    search_p->prev_p=tuple.prev_p;
    search_p->left_p=NULL;
    search_p->rght_p=NULL;
    search_p->prio=urand32();

    // BALANCE THE BST

    while(search_p->prev_p){
        adict_node_t*parent_p=search_p->prev_p;

        if(search_p->prio <= parent_p->prio){
            break;
        }

        _rotate(adict_p,tuple.find_p);
    }

    adict_p->count++;

    tuple.prev_p=search_p->prev_p;
    return tuple;
}


adict_iter_t*adict_iter_new(adict_t*adict_p){
    adict_iter_t*iter_p=calloc(1,sizeof(adict_iter_t));
    if(iter_p){
        iter_p->adict_p=adict_p;
    }
    return iter_p;
}


inline
void adict_iter_drop(adict_iter_t*iter_p){
    if(iter_p){free(iter_p);}
}


adict_node_t*adict_iter_next(adict_iter_t*iter_p){
    adict_node_t*node_p=iter_p->node_p;

    if(!node_p){
        return (iter_p->node_p=iter_p->adict_p->root_p);
    }

    if(node_p->left_p){
        return (iter_p->node_p=node_p->left_p);
    }
    if(node_p->rght_p){
        return (iter_p->node_p=node_p->rght_p);
    }

    adict_node_t*prev_p;

    while((prev_p=node_p->prev_p)){
        if(prev_p->left_p == node_p && prev_p->rght_p){
            return (iter_p->node_p=prev_p->rght_p);
        }
        node_p=prev_p;
    }

    return (iter_p->node_p=NULL);
}


// vim:ts=4:sw=4:et:
