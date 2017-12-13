program opOrder
double precision :: v(3),s1,s2
v(1)=5.0D0
v(2)=5.0D0
v(3)=1.0D17
s1=(v(1)+v(2))+v(3)
s2=v(1)+(v(2)+v(3))
if (s1>=v(3) .and. s2==v(3)) then 
print *,'OK'
else 
print *,'FAILED', s1,s2
end if
end program
