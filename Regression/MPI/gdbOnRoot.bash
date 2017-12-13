#!/bin/bash

if [ "$PMI_RANK" == "0" ] 
then
  gdb $*
else
  $*
fi
exit 0