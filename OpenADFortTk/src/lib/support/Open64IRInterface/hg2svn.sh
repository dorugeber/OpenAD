for i in `ls *.h *.?pp`
do 
  diff ../../../../../UseOA_Open64/Whirl2OA/$i $i > /dev/null
  if [ $? -eq 1 ]
  then 
    kdiff3 -m ../../../../../UseOA_Open64/Whirl2OA/$i $i -o ../../../../../UseOA_Open64/Whirl2OA/$i 
  fi
done
