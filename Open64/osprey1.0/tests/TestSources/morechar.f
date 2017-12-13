         program main
		character(len=10) :: str1="1234567890"
                character(len=10) :: origstr
		integer myint
		integer, parameter ::  mykind=kind("ABC")

		origstr=str1
		str1(1:1)=achar(65)
		myint=iachar(str1(1:1))
		str1(2:2)=achar(myint+1)
		str1(3:3)=char(myint+2, mykind)
		myint=ichar(str1(3:3))
                if((myint.EQ.67).AND.(LGE(str1,origstr))) then
                if(LLT(origstr,str1)) then
                   print *, "OK"
                else
                   print *, "failed"
                end if
                else
                   print *, "failed"
                end if
		end program main
		
		