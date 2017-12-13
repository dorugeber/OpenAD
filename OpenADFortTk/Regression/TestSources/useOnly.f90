      MODULE erro_I
      END MODULE 

      module data_C
        integer :: a, b, c
      end module data_C

      program useOnly
        use erro_I
        USE data_C , ONLY: b, c
        implicit none
        integer :: d, e
      end program useOnly

