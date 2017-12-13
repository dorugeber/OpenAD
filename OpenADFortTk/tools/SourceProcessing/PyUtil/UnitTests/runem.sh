#! /bin/sh
#

for t in t_*.py; do
   clear
   echo ---------- Test $t     ------------
   python $t
   if test $? -ne 0 
   then 
     echo -n 'press ENTER to keep going...'
     read answer
   fi
   echo ---------- End test $t ------------
   echo
done
