! A simple program with
!   - Fortran Do loops (using cycle, labels)
!   - a WRITE with embedded control flow & infinite loop

subroutine compute(x)
  IMPLICIT NONE
  real, intent(inout) :: x
  x = x / 2.054321
end subroutine

program controlflow_loop4
  IMPLICIT NONE

  integer :: i, lb, ub, step, iter
  real :: x

  lb = 1 ; ub = 10 ; step = 1 
  iter = 0
  x = 3.14159 * 16
  
  infinite: DO
     DO i = lb, ub, step
        iter = iter + 1
        
        x = x + i
        IF (7 < x .AND. x < 8) CYCLE
        call compute(x)
        
        IF (3 < x .AND. x < 4) EXIT infinite
     END DO
  END DO infinite
  
  write(UNIT=*, FMT=*, ERR=5) 'x =', x, 'iter =', iter
  GOTO 10
  
  5 CONTINUE 
  write(UNIT=*, FMT=*, ERR=5) 'IO stutter...'

  10 CONTINUE

end program  

