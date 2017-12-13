for i in `ls *.h *.?pp`
do 
  diff $i ../../../../../UseOA_Open64/Whirl2OA/$i > /dev/null
  if [ $? -eq 1 ]
  then 
    kdiff3 -m $i ../../../../../UseOA_Open64/Whirl2OA/$i -o $i   
  fi
done
