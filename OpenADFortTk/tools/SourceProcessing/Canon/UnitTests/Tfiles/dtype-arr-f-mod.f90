module q
   type baz
      integer :: f1
   end type
end module

subroutine bar(a)
  use q
  type(baz), dimension(4) :: it

   it(1)%f1 = 3
end

