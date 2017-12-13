      subroutine foo(depthz)
      EXTERNAL different_multiple
      LOGICAL different_multiple

      REAL(8) DEPTHz(1 : *),t
      REAL(8) bar(1:2)
      integer i,j,k
      if (Different_Multiple(i,j)) then
        k=1*3
      end if
      t=DEPTHZ(k)+1
      end subroutine
