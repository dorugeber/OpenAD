program p 
  logical:: lgev(3), lgtv(3), llev(3), lltv(3)
  lgev(1)=lge("a","k"); lgev(2)=lge("x ","x"); lgev(3)=lge("ak","")
  lgtv(1)=lgt("a","k"); lgtv(2)=lgt("x ","x"); lgtv(3)=lgt("ak","")
  llev(1)=lle("a","k"); llev(2)=lle("x ","x"); llev(3)=lle("ak","")
  lltv(1)=llt("a","k"); lltv(2)=llt("x ","x"); lltv(3)=llt("ak","")
  if (.not. lgev(1) .and. .not. lgtv(2) .and. .not.llev(3) .and. .not.lltv(3)) then 
     print *,'OK'
  else 
    print *,'lge',lgev
    print *,'lgt',lgtv
    print *,'lle',llev
    print *,'llt',lltv
  end if
end program  
