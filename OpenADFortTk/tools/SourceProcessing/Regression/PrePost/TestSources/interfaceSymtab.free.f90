MODULE psget_I
  INTERFACE
    REAL(KIND(0.0D0)) FUNCTION psget (I)
      INTEGER, INTENT(IN) :: I
    END FUNCTION psget
  END INTERFACE
END MODULE

program interfaceSymtab
  use psget_I
  integer :: i

  i = 1
  print *,'i = ', i
end program interfaceSymtab
