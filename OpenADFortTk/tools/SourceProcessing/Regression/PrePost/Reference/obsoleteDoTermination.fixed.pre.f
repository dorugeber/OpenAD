      module OAD_intrinsics
      end module
       program p
        use OAD_intrinsics
        integer i
        integer j
        j=1
        do 4 i=1,3
           j = j+1
        if (j>2) then
          write(*,*) j
        end if
 4      end do
       end program

