c$openad XXX Template ad_template.f
       subroutine foo() 

         use all_globals_mod , z=>y 
         implicit none
         z(1)=x(1)*x(2)

       end subroutine

       subroutine head() 

         use all_globals_mod

c$openad INDEPENDENT(x)
         call foo()
c$openad DEPENDENT(y)

       end subroutine
