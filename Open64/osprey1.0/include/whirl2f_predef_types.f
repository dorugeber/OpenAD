C***************************************************************
C
C   This file is supposed to be compiled at the very beginning
C   when you compile Open64 generated code.
C   The types in  unpasered code are  based on this module 
C   BUT this file can only compiled once,don't try to compile
C   "whirl2f_predef_types.w2f.f",since all the parameters 
C   dispeared from "whirl2f_predef_types.w2f.f"
C
C***************************************************************


	module w2f__types

        integer :: w2f__4, w2f__8, w2f__16
        parameter (w2f__4 = kind(0.0))
        parameter (w2f__8 = kind(0.0d0))
        parameter (w2f__16 = selected_real_kind(p=30))

        integer :: w2f__i1, w2f__i2, w2f__i4,w2f__i8
        parameter (w2f__i1 = selected_int_kind(r=2))
        parameter (w2f__i2 = selected_int_kind(r=3))
        parameter (w2f__i4 = selected_int_kind(r=8))
        parameter (w2f__i8 = selected_int_kind(r=16))

        end module w2f__types
