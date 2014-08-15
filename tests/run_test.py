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


import os,sys


def _red(text):
    return '\033[91m%s\033[0m'%text

def _green(text):
    return '\033[92m%s\033[0m'%text

def _yellow(text):
    return '\033[93m%s\033[0m'%text


def run_test(module,*args):
    if not hasattr(run_test,'counter'):
        setattr(run_test,'counter',0)

    if not hasattr(run_test,'failed'):
        setattr(run_test,'failed',0)

    run_test.counter+=1

    def _spawn_reader(func,r,w,*args):
        if not os.fork():
            os.close(w)
            print(_yellow(':TEST {} ({}):').format(run_test.counter,
                                                    module.__name__))
            with os.fdopen(r) as lines:
                ret=func((line.rstrip() for line in lines),*args)
            if ret:
                print(_green(':PASS:'))
                print()
            else:
                print(_red(':FAIL:'))
                print()
                run_test.failed+=1
            sys.exit(ret)

    def _spawn_writer(func,r,w,*args):
        if not os.fork():
            os.close(r)
            os.dup2(w,sys.stdout.fileno())
            os.dup2(w,sys.stderr.fileno())
            os.close(w)
            ret=func(*args)
            sys.exit(ret)

    r,w=os.pipe()
    _spawn_reader(module.reader,r,w,*args)
    _spawn_writer(module.writer,r,w,*args)
    os.close(r)
    os.close(w)
    os.wait()
    os.wait()


# vim:ts=4:sw=4:et:
