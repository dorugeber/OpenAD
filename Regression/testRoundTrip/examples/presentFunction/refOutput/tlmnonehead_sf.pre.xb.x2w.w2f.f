
C$OPENAD XXX File_start [OAD_intrinsics.f90]
      MODULE oad_intrinsics
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [all_globals_mod.f]
      MODULE all_globals_mod
      use w2f__types
      IMPLICIT NONE
      SAVE
C
C     **** Statements ****
C
      END MODULE

C$OPENAD XXX File_start [head.f]
      SUBROUTINE opt(REQARG, OPTARG, OUTARG)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) REQARG
      INTENT(IN)  REQARG
      TYPE (OpenADTy_active) OPTARG
      OPTIONAL  OPTARG
      INTENT(IN)  OPTARG
      TYPE (OpenADTy_active) OUTARG
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_3
      TYPE (OpenADTy_active) OpenAD_prp_0
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      IF(PRESENT(__value__(OPTARG))) THEN
        IF(__value__(OPTARG) .LE. 2.0D00) THEN
          OpenAD_aux_0 = (__value__(OPTARG) * __value__(OUTARG))
          OpenAD_lin_0 = OpenAD_aux_0
          OpenAD_lin_2 = __value__(OUTARG)
          OpenAD_lin_3 = __value__(OPTARG)
          OpenAD_lin_1 = __value__(REQARG)
          __value__(OUTARG) = (__value__(REQARG) * OpenAD_aux_0)
          OpenAD_acc_0 = (OpenAD_lin_2 * OpenAD_lin_1)
          OpenAD_acc_1 = (OpenAD_lin_3 * OpenAD_lin_1)
          CALL setderiv(__deriv__(OpenAD_prp_0), __deriv__(OUTARG))
          CALL sax(OpenAD_lin_0, __deriv__(REQARG), __deriv__(OUTARG))
          CALL saxpy(OpenAD_acc_0, __deriv__(OPTARG), __deriv__(OUTARG)
     > )
          CALL saxpy(OpenAD_acc_1, __deriv__(OpenAD_prp_0), __deriv__(
     > OUTARG))
        ENDIF
      ELSE
        __value__(OUTARG) = __value__(REQARG)
        CALL setderiv(__deriv__(OUTARG), __deriv__(REQARG))
      ENDIF
      END SUBROUTINE

      SUBROUTINE head(X, Y)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X(1 : 2)
      TYPE (OpenADTy_active) Y(1 : 1)
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X)
C$OPENAD DEPENDENT(Y)
      interface 
        SUBROUTINE opt(REQARG, OPTARG, OUTARG)
        use w2f__types
        TYPE (OpenADTy_active) REQARG
        INTENT(in)  REQARG
        TYPE (OpenADTy_active) OPTARG
        OPTIONAL  OPTARG
        INTENT(in)  OPTARG
        TYPE (OpenADTy_active) OUTARG
        END SUBROUTINE

      end interface 
      
C
C     **** Statements ****
C
      __value__(Y(1)) = (__value__(X(1)) * 2.0D00)
      CALL sax(2.0D00, __deriv__(X(1)), __deriv__(Y(1)))
      CALL opt(__deriv__(X(2)), __deriv__(X(1)), __deriv__(Y(1)))
      END SUBROUTINE
