c$openad XXX Template ad_template.f
      subroutine bar(k,a,p) 
       double precision a
       double precision p
       integer k
       a = a*p*k
      end subroutine

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
       double precision x
       double precision y
       double precision :: p(2)
       integer i,j
c$openad INDEPENDENT(x)
c$openad xxx simple loop
       p(1)=3.0D0
       p(2)=6.0D0
       do j=1,2
         call bar(j,x ,p(j))
       enddo
       y = x
c$openad DEPENDENT(y)
      end subroutine

