c$openad XXX Template ad_template.f
       subroutine foo() 

         use all_globals_mod 

         y(1)=x(1)*x(2)

       end subroutine

c$openad XXX Template ad_template.f
       subroutine head() 

         use all_globals_mod

c$openad INDEPENDENT(x)
         call foo()
c$openad DEPENDENT(y)

       end subroutine
