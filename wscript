#!/usr/bin/env python
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


from waflib.Build import BuildContext
class DebugBuildContext(BuildContext):
    cmd='debug'


def options(ctx):
    ctx.load('compiler_c')

def configure(ctx):
    ctx.load('compiler_c')
    ctx.check_cc(msg='Checking for /dev/urandom',
         define_name='HAVE_DEV_URANDOM',
    fragment='''
        #include <unistd.h>
        #include <stdint.h>
        #include <stdlib.h>
        #include <fcntl.h>
        int main(){
            int fd=open("/dev/urandom",O_RDONLY);
            if(fd>=0){
                uint32_t value32;
                if(read(fd,&value32,sizeof(value32))>0){
                    close(fd);
                    return EXIT_SUCCESS;
                }
                close(fd);
            }
            return EXIT_FAILURE;
        }
    ''')
    ctx.write_config_header('config.h')

def build(ctx):
    if ctx.cmd=='debug': ctx.env.CFLAGS=['-g','-O0','-DDEBUG=1']

    ctx.shlib(target='adict',
            includes='.',
              source=ctx.path.ant_glob(incl='src/**.c'))


# vim:ts=4:sw=4:et:
