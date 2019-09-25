#!/bin/bash
# Run a testcase on a remote system, via qemu.
# Copyright (C) 2019 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <http://www.gnu.org/licenses/>.

# usage: cross-test-qemu.sh HOST COMMAND ...
# Run with --help flag to get more detailed help.

progname="$(basename $0)"

usage="usage: ${progname} [--ssh SSH] HOST COMMAND ..."
timeoutfactor=$TIMEOUTFACTOR
addon_libpath=""
while [ $# -gt 0 ]; do
  case "$1" in

      "--timeoutfactor")
      shift
      if [ $# -lt 1 ]; then
	break
      fi
      timeoutfactor="$1"
      ;;

    "--addon-libpath")
	shift
	if [ $# -lt 1 ]; then
	    break
	fi
	addon_libpath="$1"
	;;

    "--help")
      echo "$usage"
      echo "$help"
      exit 0
      ;;

    *)
      break
      ;;
  esac
  shift
done

if [ $# -lt 1 ]; then
  echo "$usage" >&2
  echo "Type '${progname} --help' for more detailed help." >&2
  exit 1
fi

emulator="$1"; shift
envpat="[:alpha:]*=.*"
ldpat=".*/.*ld.*\.so.*"
lgccpat="libgcc_s.so.1"
libpat="--library-path"
ldpath=""
lgccpath=""
envlist=""
liblist=""
command=""
toolchain=`dirname \`dirname $emulator\``
target=`ls $toolchain | grep -e linux-gnu`
# Print the sequence of arguments as strings properly quoted for the
# Bourne shell, separated by spaces.
bourne_quote ()
{
  local arg qarg libflag variant
  libflag=0

  for arg in $@; do
      if [ "x$done" != "x" ]; then
	  command="$command $arg"
      elif [[ $arg =~ $envpat ]]; then
	  if [ -z $envlist ]; then
	     envlist="$arg"
	   else
	       envlist="$arg,$envlist"
	  fi
      elif [[ $arg =~ $ldpat ]]; then
	  ldfile=`basename $arg`
	  variant=`basename \`dirname \\\`dirname $arg\\\`\``
	  libdir=${variant##*_}
	  variant=${variant%_*}
	  variant=${variant#obj_}
	  ldpath=$toolchain/sysroot/$variant
	  if [ ! -f $ldpath/$libdir/$ldfile ]; then
	      ldpath=`dirname $arg`
	  fi
	  lgccpath=$toolchain/$target/lib/$variant/$libdir
	  liblist="$ldpath:$lgccpath:$liblist"
      elif [[ $arg =~ $libpat ]]; then
	  libflag=1
      elif [ $libflag -ne 0 ]; then
	  liblist="$arg:$liblist"
	  libflag=0
      elif [ "x$arg" != "xenv" ]; then
	  if [[ $arg =~ "tst-" ]]; then
	      if [ -f $arg ]; then
		  done=1
	      fi
	  fi
	  command="$command $arg"
      fi
  done
}

# Transform the current argument list into a properly quoted Bourne shell
# command string.
bourne_quote "$@"

liblist=$addon_libpath:$liblist
liblist=`tr -s : <<< $liblist`
liblist=${liblist#:*}
liblist=${liblist%*:}

if [ "x$liblist" != "x" ]; then
    LIBPATH_OPT="-E LD_LIBRARY_PATH=$liblist"
fi

if [ "x$envlist" != "x" ]; then
    ENV_OPT="-E $envlist"
fi

if [ "x$ldpath" != "x" ]; then
    LDPATH_OPT="-L $ldpath"
fi

if [ "x$timeoutfactor" != "x" ]; then
    $emulator $LDPATH_OPT $LIBPATH_OPT $ENV_OPT $command &
    pid=$!
    trap "kill -SIGINT $pid" SIGALRM
    sleep $timeoutfactor && kill -SIGALRM $$
    exit 1
else
    $emulator $LDPATH_OPT $LIBPATH_OPT $ENV_OPT $command
fi

