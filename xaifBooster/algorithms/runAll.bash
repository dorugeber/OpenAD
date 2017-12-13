#!/bin/bash
set -e

function doTest {
if [ "$1" == "full" ] 
then 
  TESTSET="add_mul_ex1 add_mul_ex2 add_mul_ex_duud_lineUp add_mul_ex_no_duud add_mul_ex uwe_ex_1 uwe_ex_2 uwe_ex_2_explicit uwe_ex_3 uwe_ex_4 uwe_ex_5 vecref_ex whole_box_model select_case goto_label_1 loop_continue loop_exit"
fi
if [ "$1" == "partial" ] 
then 
  TESTSET="add_mul_ex1 add_mul_ex2 add_mul_ex_duud_lineUp add_mul_ex_no_duud add_mul_ex uwe_ex_1 uwe_ex_2 uwe_ex_2_explicit uwe_ex_3 uwe_ex_4 uwe_ex_5 vecref_ex whole_box_model select_case"
fi
shift 
for i in $*
do  
if [ -f $i/driver/oadDriver ] 
then 
  echo "***********************************************************"  
  echo "testing in $i:"
  cd $i/driver
  if [ -z $BATCHMODE ] 
  then 
    ./quickRegression.bash $TESTSET
  else
    ./quickRegression.bash $TESTSET > /dev/null
  fi
  cd ../../ 
fi
done
}

ALGDIRSFULL="TypeChange Linearization BasicBlockPreaccumulation BasicBlockPreaccumulationTape BasicBlockPreaccumulationTapeAdjoint"
ALGDIRSPARTIAL="ControlFlowReversal AddressArithmetic PushPop BasicBlockPreaccumulationReverse TraceDiff"

doTest "full" $ALGDIRSFULL
doTest "partial" $ALGDIRSPARTIAL
