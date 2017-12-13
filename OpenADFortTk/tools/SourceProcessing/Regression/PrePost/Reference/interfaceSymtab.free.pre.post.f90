module OAD_intrinsics
end module
MODULE psget_I
  use OAD_active
  INTERFACE
    REAL(KIND(0.0D0)) FUNCTION psget (I)
      use OAD_active
      INTEGER, INTENT(IN) :: I
    END FUNCTION psget
    subroutine oad_s_psget(I,psget)
    use OAD_active
    real(KIND(0.0D0)),intent(out) :: psget
      INTEGER, INTENT(IN) :: I
    end subroutine
  END INTERFACE
END MODULE

program interfaceSymtab
  use OAD_active
  use OAD_intrinsics
  use psget_I
  integer :: i

  i = 1
  print *,'i = ',i
end program interfaceSymtab
