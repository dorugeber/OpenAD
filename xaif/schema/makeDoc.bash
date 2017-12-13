#!/bin/bash
for i in `ls *.xsd`
do
  basename=${i%%.xsd}
  if [ ! -d ./docNew/${basename} ] 
  then 
    mkdir -p ./docNew/${basename}
  fi
  xsddoc -t "${basename}" -o ./docNew/${basename} -verbose $i
done

