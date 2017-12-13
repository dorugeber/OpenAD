      module foo
        implicit none
        private
        public  :: bar

        contains
        function bar(x)
          double precision :: bar
          double precision,intent(in) :: x
          bar = max(x,4.0D0)
        end function
      end module foo

c$openad XXX Template ad_template.f
      subroutine head(x,y) 
        use foo
        implicit none
        double precision, dimension(1), intent(inout) :: x
        double precision, dimension(1), intent(inout) :: y
        double precision :: t
c$openad INDEPENDENT(x)
        y(1) = bar(x(1))*2.0D0
        t = bar(y(1))
c$openad DEPENDENT(y)
      end subroutine head

