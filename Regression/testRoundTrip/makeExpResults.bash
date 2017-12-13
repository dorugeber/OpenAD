if [ $# -eq 1 ] 
then 
  OFILE=$1
else
  OFILE=expResults.html
fi

cat > ${OFILE} <<EOT

expected test results:<br>
<font size=\"-1\">
<table style="width: 100%; text-align: left;" border="1" cellpadding="2"
 cellspacing="2">
  <tbody>
    <tr>
      <td colspan="1" rowspan="3"
 style="vertical-align: top; background-color: rgb(238, 238, 238);">name<br>
      </td>
      <td colspan="3" rowspan="1"
 style="vertical-align: top; background-color: rgb(238, 238, 238);">issues
(small numerical differences are always expected)</td>
    </tr>
    <tr>
      <td colspan="1" rowspan="2"
 style="vertical-align: top; background-color: rgb(238, 238, 238);">tangent-linear
      <br>
(using <span style="font-family: courier new,courier,monospace;">BasicBlockPreaccumulation</span>)<br>
      </td>
      <td colspan="2" rowspan="1"
 style="vertical-align: top; background-color: rgb(238, 238, 238);">adjoint
(reverse, using <span
 style="font-family: courier new,courier,monospace;">BasicBlockPreaccumulationReverse</span>)
      <br>
      </td>
    </tr>
    <tr>
      <td
 style="vertical-align: top; background-color: rgb(238, 238, 238);">joint<br>
      </td>
      <td
 style="vertical-align: top; background-color: rgb(238, 238, 238);">split<br>
      </td>
    </tr>
EOT

for i in `ls examples`
do 
  echo "<tr><td style=\"vertical-align: top; font-family: monospace;\">${i}</td>" >> ${OFILE}
  for j in "tlm_none" "adm_split" "adm_joint"
  do 
    if [ -f examples/${i}/FAILREASON_${j} ] 
    then 
      echo "<td style=\"vertical-align: top; background-color: rgb(255, 153, 0);\">" >> ${OFILE} 
      cat examples/$i/FAILREASON_${j} >> ${OFILE}
      echo "</td>" >> ${OFILE}
    else
      echo "<td style=\"vertical-align: top; color: rgb(0, 255, 0);\">&#10004;</td> " >> ${OFILE}
    fi
  done
  echo "</tr>" >> ${OFILE}
done

cat >> ${OFILE} <<EOT
</tbody>
</table>
</font>
EOT
