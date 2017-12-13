#!/bin/bash
echo "#generated file" > temp.sed
for i in `grep '^ *SUBROUTINE ' $1 | awk '{print $2}'`
do 
  # extract the name
  srName=${i%%\(*}
  echo "/^ *SUBROUTINE $srName(/i\\" >> temp.sed
  case "$srName" in 
    "box_final_state" | \
    "box_ini_fields" | \
    "box_model_body" ) 
      echo "c\$openad XXX Template ad_template.joint.f" >> temp.sed
      ;;
    "box_model_loop" ) 
      echo "c\$openad XXX Template ad_template.revolve.f" >> temp.sed
      ;;
    *) 
      echo "!\$openad XXX Template ad_template.split.f" >> temp.sed
      ;;
  esac
done
cat $1 | sed -f temp.sed > $2
rm -f temp.sed  
