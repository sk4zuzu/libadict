#
# LIBADICT 0.2 20140815 copyright sk4zuzu@gmail.com 2014
#
# This file is part of LIBADICT.
#
# LIBADICT is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LIBADICT is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LIBADICT.  If not, see <http://www.gnu.org/licenses/>.
#


def writer(libadict_so):
    alist_p=libadict_so.alist_new()

    for k in range(16):
        libadict_so.alist_push_i32(alist_p,k)
    libadict_so.debug_alist_dump(alist_p)

    for _ in range(12):
        libadict_so.alist_pop(alist_p)
    libadict_so.debug_alist_dump(alist_p)

    for k in range(16):
        libadict_so.alist_unshift_i32(alist_p,k)
    libadict_so.debug_alist_dump(alist_p)

    for _ in range(12):
        libadict_so.alist_shift(alist_p)
    libadict_so.debug_alist_dump(alist_p)

    libadict_so.alist_drop(alist_p)


def reader(lines,libadict_so):
    checklist=[
       '16 items present in the list',
        '4 items present in the list',
       '20 items present in the list',
        '8 items present in the list'
    ]
    for line in lines:
        if len(checklist):
            if line==checklist[0]:
                print(line)
                del checklist[0]
        else:
            break
    return not len(checklist)


# vim:ts=4:sw=4:et:
