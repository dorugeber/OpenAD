s/xaifBooster//g 
s/Alg//g
s/\/inc.*\.hpp// 
s/{//g 
s/public//g 
s/virtual//g 
s/^\.\/\([a-zA-Z]*\)\:/\1+/
s/ :  */->/
/Sequence/d 
s/\([a-zA-Z][a-zA-Z]*\)Base/Base::\1/g
s/\(^[a-zA-Z]*\)+ \( class \)/\1\+ \1::/
s/\(^[a-zA-Z]*\)+\(.*\) \([a-zA-Z][a-zA-Z]*[ ,$]\)/\1+\2\1::\3/g
s/^[a-zA-Z]*+ *//
