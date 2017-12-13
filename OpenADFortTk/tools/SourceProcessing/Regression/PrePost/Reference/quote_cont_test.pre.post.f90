module OAD_intrinsics
end module
      program quote_cont_test
      use OAD_active
      use OAD_intrinsics
      character(len=16),dimension(12),parameter :: cap = (/"     '''''      "," &
     &    '''''      ","     '''''      ","      '''       ","       '        ",&
     &'                ','                ','                ','                &
     &','                ','                ','                '/)



      print *,cap

      end program quote_cont_test
