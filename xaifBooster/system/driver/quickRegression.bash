#!/bin/bash
set -e
if [ -z "${XAIFSCHEMAROOT}" ] 
then 
  echo XAIFSCHEMAROOT needs to be set, e.g. /sandbox/OpenAD/xaif
  exit -1 
fi
rm -rf tmp
mkdir tmp
if [ $# -gt 0 ] 
then  
    TESTFILES=$@
else
    TESTFILES='add_mul_ex1 add_mul_ex2 add_mul_ex_duud_lineUp add_mul_ex_no_duud add_mul_ex uwe_ex_1 uwe_ex_2 uwe_ex_2_explicit uwe_ex_3 uwe_ex_4 uwe_ex_5 vecref_ex whole_box_model select_case goto_label_1 loop_continue loop_exit'
fi
for i in `echo ${TESTFILES}`
    do
    command="./oadDriver -v -V -F NO_STYLE -i ${XAIFSCHEMAROOT}/schema/examples/${i}.xaif -o tmp/${i}.out -c ${XAIFSCHEMAROOT}/schema/examples/inlinable_intrinsics.xaif -d tmp/${i}.dbg"
    echo $command
    set +e
    $($command)
    set -e
    debugLines=`wc -l tmp/${i}.dbg | awk '{ print $1}'`
    if [ $debugLines -gt 0 ] 
    then 
	echo "debug messages:"
	cat tmp/${i}.dbg
        echo -n "QUESTION: there was a problem - "
	if [ -z "$BATCHMODE" ] 
          then 
	  echo -n "hit <enter> to continue "
          read answer
	else
	  echo "exiting"
	  exit -1;
        fi
	continue
    fi
    diffCommand="diff testOutput/${i}.out tmp/${i}.out"
    set +e
    $diffCommand > /dev/null
    ret=$?
    set -e
    if [ $ret -eq 2 ] 
    then 
      echo "ERROR in: $diffCommand"; exit -1;
    fi
    if [ $ret -eq 1 ] 
    then 
	echo "diffs base (<) vs. current (>):"
	set +e
	$diffCommand
	# kdiff3 testOutput/${i}.out tmp/${i}.out
	set -e
	if [ -z "$BATCHMODE" ] 
          then 
  	  echo -n "QUESTION: there was a difference - checkin y/[n] : "
          read answer
	  if [ "${answer}" == "y" ] 
	  then 
	    cp tmp/${i}.out testOutput/${i}.out
	  else
            echo -n "QUESTION: there was a problem - hit <enter> to continue "
	    read answer
          fi
	else 
  	  echo -n "there was a difference"; exit -1;
        fi
    fi
done
