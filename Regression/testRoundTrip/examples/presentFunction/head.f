c$openad XXX Template ad_template.f
        subroutine opt(reqarg,optarg,outarg) 
          double precision,intent(in) :: reqarg
          double precision,optional,intent(in) :: optarg
          double precision,intent(inout) :: outarg
          if(present(optarg)) then
            if(optarg .le. 2.0) then
              outarg = outarg*optarg*reqarg
            end if
          else 
	   outarg = reqarg
          end if
        end subroutine opt

        subroutine head(x,y) 
          double precision,dimension(2) :: x
          double precision,dimension(1) :: y
c$openad INDEPENDENT(x)
          interface
            subroutine opt(reqarg,optarg,outarg)
              double precision,intent(in) :: reqarg
              double precision,optional,intent(in) :: optarg
              double precision,intent(inout) :: outarg
            end subroutine opt
          end interface
          y(1) = x(1)*2.0
          call opt(x(2),x(1),y(1))
c$openad DEPENDENT(y)
        end subroutine head

