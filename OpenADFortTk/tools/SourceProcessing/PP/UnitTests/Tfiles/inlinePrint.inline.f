      subroutine push_s0(x)
C $OpenAD$ INLINE DECLS
      double precision :: x
C $OpenAD$ END DECLS
      if(double_tape_size .lt. double_tape_pointer) then 
         allocate(double_tmp_tape(double_tape_size),
     +STAT=cp_loop_variable_1)
         if (cp_loop_variable_1 .gt. 0 ) then 
            print *,'allocation failed with', cp_loop_variable_1
            stop
         end if
         double_tmp_tape=double_tape
         deallocate(double_tape)
         allocate(double_tape(double_tape_size*2))
         print *, "DT+"
         double_tape(1:double_tape_size) = double_tmp_tape
         deallocate(double_tmp_tape)
         double_tape_size=double_tape_size*2
      end if
      double_tape(double_tape_pointer)=x
      double_tape_pointer=double_tape_pointer+1
      end subroutine 
