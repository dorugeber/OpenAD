module m

  interface erf
    module procedure real_erf, double_erf
  end interface

  interface erfc
    module procedure real_erfc, double_erfc
  end interface

  contains

    real function real_erf (z) result (zr)
      real :: z
      zr = z*4.3E0
    end function real_erf

    double precision function double_erf (z) result (zr)
      double precision :: z
      zr = z*5.0D0
    end function double_erf

    real function real_erfc (z) result (zr)
      real :: z
      zr = z*7.0E0
    end function real_erfc

    double precision function double_erfc (z) result (zr)
      double precision :: z
      zr = z*3.0D0
    end function double_erfc

end module m

program multipleModuleProcedures
  use m
  implicit none
  real :: a
  double precision :: b

  a = real_erf(1.0E0)+real_erfc(2.0E0)
  b = double_erf(1.0D0)+double_erfc(2.0D0)
  print *,a,' ',b
end program multipleModuleProcedures
