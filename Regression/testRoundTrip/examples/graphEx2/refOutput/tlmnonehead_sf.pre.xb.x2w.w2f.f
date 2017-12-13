
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
      SUBROUTINE head(X01, X02, X03, X04, X05, X06, Y01, Y02, Y03, Y04,
     >  Y05, Y06, Y07, Y08, Y09, Y10, Y11, Y12, Y13, Y14, Y15, Y16, Y17
     > , Y18, Y19, Y20, Y21, Y22, Y23, Y24, Y25, Y26, Y27, Y28)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) X01
      INTENT(IN)  X01
      TYPE (OpenADTy_active) X02
      INTENT(IN)  X02
      TYPE (OpenADTy_active) X03
      INTENT(IN)  X03
      TYPE (OpenADTy_active) X04
      INTENT(IN)  X04
      TYPE (OpenADTy_active) X05
      INTENT(IN)  X05
      TYPE (OpenADTy_active) X06
      INTENT(IN)  X06
      TYPE (OpenADTy_active) Y01
      INTENT(OUT)  Y01
      TYPE (OpenADTy_active) Y02
      INTENT(OUT)  Y02
      TYPE (OpenADTy_active) Y03
      INTENT(OUT)  Y03
      TYPE (OpenADTy_active) Y04
      INTENT(OUT)  Y04
      TYPE (OpenADTy_active) Y05
      INTENT(OUT)  Y05
      TYPE (OpenADTy_active) Y06
      INTENT(OUT)  Y06
      TYPE (OpenADTy_active) Y07
      INTENT(OUT)  Y07
      TYPE (OpenADTy_active) Y08
      INTENT(OUT)  Y08
      TYPE (OpenADTy_active) Y09
      INTENT(OUT)  Y09
      TYPE (OpenADTy_active) Y10
      INTENT(OUT)  Y10
      TYPE (OpenADTy_active) Y11
      INTENT(OUT)  Y11
      TYPE (OpenADTy_active) Y12
      INTENT(OUT)  Y12
      TYPE (OpenADTy_active) Y13
      INTENT(OUT)  Y13
      TYPE (OpenADTy_active) Y14
      INTENT(OUT)  Y14
      TYPE (OpenADTy_active) Y15
      INTENT(OUT)  Y15
      TYPE (OpenADTy_active) Y16
      INTENT(OUT)  Y16
      TYPE (OpenADTy_active) Y17
      INTENT(OUT)  Y17
      TYPE (OpenADTy_active) Y18
      INTENT(OUT)  Y18
      TYPE (OpenADTy_active) Y19
      INTENT(OUT)  Y19
      TYPE (OpenADTy_active) Y20
      INTENT(OUT)  Y20
      TYPE (OpenADTy_active) Y21
      INTENT(OUT)  Y21
      TYPE (OpenADTy_active) Y22
      INTENT(OUT)  Y22
      TYPE (OpenADTy_active) Y23
      INTENT(OUT)  Y23
      TYPE (OpenADTy_active) Y24
      INTENT(OUT)  Y24
      TYPE (OpenADTy_active) Y25
      INTENT(OUT)  Y25
      TYPE (OpenADTy_active) Y26
      INTENT(OUT)  Y26
      TYPE (OpenADTy_active) Y27
      INTENT(OUT)  Y27
      TYPE (OpenADTy_active) Y28
      INTENT(OUT)  Y28
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__8) A
      REAL(w2f__8) AB
      TYPE (OpenADTy_active) ADJ_M0
      TYPE (OpenADTy_active) ADJ_M1
      TYPE (OpenADTy_active) ADJ_M2
      TYPE (OpenADTy_active) ADJ_M3
      REAL(w2f__8) B
      REAL(w2f__8) BM1
      TYPE (OpenADTy_active) D00
      TYPE (OpenADTy_active) D01
      TYPE (OpenADTy_active) D03
      TYPE (OpenADTy_active) D11
      TYPE (OpenADTy_active) D12
      TYPE (OpenADTy_active) F
      REAL(w2f__8) FOUR3RD
      TYPE (OpenADTy_active) G
      TYPE (OpenADTy_active) GG
      TYPE (OpenADTy_active) G_OBJ0
      TYPE (OpenADTy_active) G_OBJ1
      TYPE (OpenADTy_active) G_OBJ2
      TYPE (OpenADTy_active) G_OBJ3
      TYPE (OpenADTy_active) G_OBJ4
      TYPE (OpenADTy_active) G_OBJ5
      TYPE (OpenADTy_active) H_OBJ0
      TYPE (OpenADTy_active) H_OBJ1
      TYPE (OpenADTy_active) H_OBJ10
      TYPE (OpenADTy_active) H_OBJ11
      TYPE (OpenADTy_active) H_OBJ12
      TYPE (OpenADTy_active) H_OBJ13
      TYPE (OpenADTy_active) H_OBJ14
      TYPE (OpenADTy_active) H_OBJ15
      TYPE (OpenADTy_active) H_OBJ16
      TYPE (OpenADTy_active) H_OBJ17
      TYPE (OpenADTy_active) H_OBJ18
      TYPE (OpenADTy_active) H_OBJ19
      TYPE (OpenADTy_active) H_OBJ2
      TYPE (OpenADTy_active) H_OBJ20
      TYPE (OpenADTy_active) H_OBJ3
      TYPE (OpenADTy_active) H_OBJ4
      TYPE (OpenADTy_active) H_OBJ5
      TYPE (OpenADTy_active) H_OBJ6
      TYPE (OpenADTy_active) H_OBJ7
      TYPE (OpenADTy_active) H_OBJ8
      TYPE (OpenADTy_active) H_OBJ9
      TYPE (OpenADTy_active) LOC1
      TYPE (OpenADTy_active) LOC2
      TYPE (OpenADTy_active) M00
      TYPE (OpenADTy_active) M01
      TYPE (OpenADTy_active) M02
      TYPE (OpenADTy_active) M03
      TYPE (OpenADTy_active) M11
      TYPE (OpenADTy_active) M12
      TYPE (OpenADTy_active) M13
      TYPE (OpenADTy_active) M22
      TYPE (OpenADTy_active) M23
      TYPE (OpenADTy_active) M33
      TYPE (OpenADTy_active) MATR0
      TYPE (OpenADTy_active) MATR1
      TYPE (OpenADTy_active) MATR2
      TYPE (OpenADTy_active) MATR3
      TYPE (OpenADTy_active) OBJ
      TYPE (OpenADTy_active) R00
      TYPE (OpenADTy_active) R02
      TYPE (OpenADTy_active) R03
      TYPE (OpenADTy_active) R11
      TYPE (OpenADTy_active) R12
      TYPE (OpenADTy_active) R13
      REAL(w2f__8) R22
      REAL(w2f__8) R33
      REAL(w2f__8) SQRT3
      REAL(w2f__8) THIRD
      REAL(w2f__8) TSQRT3
      REAL(w2f__8) TWO3RD
      REAL(w2f__8) TWOSQRT3
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_10
      REAL(w2f__8) OpenAD_acc_11
      REAL(w2f__8) OpenAD_acc_12
      REAL(w2f__8) OpenAD_acc_13
      REAL(w2f__8) OpenAD_acc_14
      REAL(w2f__8) OpenAD_acc_15
      REAL(w2f__8) OpenAD_acc_16
      REAL(w2f__8) OpenAD_acc_17
      REAL(w2f__8) OpenAD_acc_18
      REAL(w2f__8) OpenAD_acc_19
      REAL(w2f__8) OpenAD_acc_2
      REAL(w2f__8) OpenAD_acc_20
      REAL(w2f__8) OpenAD_acc_21
      REAL(w2f__8) OpenAD_acc_22
      REAL(w2f__8) OpenAD_acc_23
      REAL(w2f__8) OpenAD_acc_24
      REAL(w2f__8) OpenAD_acc_25
      REAL(w2f__8) OpenAD_acc_26
      REAL(w2f__8) OpenAD_acc_27
      REAL(w2f__8) OpenAD_acc_28
      REAL(w2f__8) OpenAD_acc_29
      REAL(w2f__8) OpenAD_acc_3
      REAL(w2f__8) OpenAD_acc_30
      REAL(w2f__8) OpenAD_acc_31
      REAL(w2f__8) OpenAD_acc_32
      REAL(w2f__8) OpenAD_acc_33
      REAL(w2f__8) OpenAD_acc_34
      REAL(w2f__8) OpenAD_acc_35
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_9
      REAL(w2f__8) OpenAD_aux_0
      REAL(w2f__8) OpenAD_aux_1
      REAL(w2f__8) OpenAD_aux_10
      REAL(w2f__8) OpenAD_aux_11
      REAL(w2f__8) OpenAD_aux_12
      REAL(w2f__8) OpenAD_aux_13
      REAL(w2f__8) OpenAD_aux_14
      REAL(w2f__8) OpenAD_aux_15
      REAL(w2f__8) OpenAD_aux_16
      REAL(w2f__8) OpenAD_aux_17
      REAL(w2f__8) OpenAD_aux_18
      REAL(w2f__8) OpenAD_aux_19
      REAL(w2f__8) OpenAD_aux_2
      REAL(w2f__8) OpenAD_aux_20
      REAL(w2f__8) OpenAD_aux_21
      REAL(w2f__8) OpenAD_aux_22
      REAL(w2f__8) OpenAD_aux_23
      REAL(w2f__8) OpenAD_aux_24
      REAL(w2f__8) OpenAD_aux_25
      REAL(w2f__8) OpenAD_aux_26
      REAL(w2f__8) OpenAD_aux_27
      REAL(w2f__8) OpenAD_aux_28
      REAL(w2f__8) OpenAD_aux_29
      REAL(w2f__8) OpenAD_aux_3
      REAL(w2f__8) OpenAD_aux_4
      REAL(w2f__8) OpenAD_aux_5
      REAL(w2f__8) OpenAD_aux_6
      REAL(w2f__8) OpenAD_aux_7
      REAL(w2f__8) OpenAD_aux_8
      REAL(w2f__8) OpenAD_aux_9
      REAL(w2f__8) OpenAD_lin_0
      REAL(w2f__8) OpenAD_lin_1
      REAL(w2f__8) OpenAD_lin_10
      REAL(w2f__8) OpenAD_lin_100
      REAL(w2f__8) OpenAD_lin_101
      REAL(w2f__8) OpenAD_lin_102
      REAL(w2f__8) OpenAD_lin_103
      REAL(w2f__8) OpenAD_lin_104
      REAL(w2f__8) OpenAD_lin_105
      REAL(w2f__8) OpenAD_lin_106
      REAL(w2f__8) OpenAD_lin_107
      REAL(w2f__8) OpenAD_lin_108
      REAL(w2f__8) OpenAD_lin_109
      REAL(w2f__8) OpenAD_lin_11
      REAL(w2f__8) OpenAD_lin_110
      REAL(w2f__8) OpenAD_lin_111
      REAL(w2f__8) OpenAD_lin_112
      REAL(w2f__8) OpenAD_lin_113
      REAL(w2f__8) OpenAD_lin_114
      REAL(w2f__8) OpenAD_lin_115
      REAL(w2f__8) OpenAD_lin_116
      REAL(w2f__8) OpenAD_lin_117
      REAL(w2f__8) OpenAD_lin_118
      REAL(w2f__8) OpenAD_lin_119
      REAL(w2f__8) OpenAD_lin_12
      REAL(w2f__8) OpenAD_lin_120
      REAL(w2f__8) OpenAD_lin_121
      REAL(w2f__8) OpenAD_lin_122
      REAL(w2f__8) OpenAD_lin_123
      REAL(w2f__8) OpenAD_lin_124
      REAL(w2f__8) OpenAD_lin_125
      REAL(w2f__8) OpenAD_lin_126
      REAL(w2f__8) OpenAD_lin_127
      REAL(w2f__8) OpenAD_lin_128
      REAL(w2f__8) OpenAD_lin_129
      REAL(w2f__8) OpenAD_lin_13
      REAL(w2f__8) OpenAD_lin_130
      REAL(w2f__8) OpenAD_lin_131
      REAL(w2f__8) OpenAD_lin_132
      REAL(w2f__8) OpenAD_lin_133
      REAL(w2f__8) OpenAD_lin_134
      REAL(w2f__8) OpenAD_lin_135
      REAL(w2f__8) OpenAD_lin_136
      REAL(w2f__8) OpenAD_lin_137
      REAL(w2f__8) OpenAD_lin_138
      REAL(w2f__8) OpenAD_lin_139
      REAL(w2f__8) OpenAD_lin_14
      REAL(w2f__8) OpenAD_lin_140
      REAL(w2f__8) OpenAD_lin_141
      REAL(w2f__8) OpenAD_lin_142
      REAL(w2f__8) OpenAD_lin_143
      REAL(w2f__8) OpenAD_lin_144
      REAL(w2f__8) OpenAD_lin_145
      REAL(w2f__8) OpenAD_lin_146
      REAL(w2f__8) OpenAD_lin_147
      REAL(w2f__8) OpenAD_lin_148
      REAL(w2f__8) OpenAD_lin_149
      REAL(w2f__8) OpenAD_lin_15
      REAL(w2f__8) OpenAD_lin_150
      REAL(w2f__8) OpenAD_lin_151
      REAL(w2f__8) OpenAD_lin_152
      REAL(w2f__8) OpenAD_lin_153
      REAL(w2f__8) OpenAD_lin_154
      REAL(w2f__8) OpenAD_lin_155
      REAL(w2f__8) OpenAD_lin_156
      REAL(w2f__8) OpenAD_lin_157
      REAL(w2f__8) OpenAD_lin_158
      REAL(w2f__8) OpenAD_lin_159
      REAL(w2f__8) OpenAD_lin_16
      REAL(w2f__8) OpenAD_lin_160
      REAL(w2f__8) OpenAD_lin_161
      REAL(w2f__8) OpenAD_lin_162
      REAL(w2f__8) OpenAD_lin_163
      REAL(w2f__8) OpenAD_lin_164
      REAL(w2f__8) OpenAD_lin_165
      REAL(w2f__8) OpenAD_lin_166
      REAL(w2f__8) OpenAD_lin_167
      REAL(w2f__8) OpenAD_lin_168
      REAL(w2f__8) OpenAD_lin_169
      REAL(w2f__8) OpenAD_lin_17
      REAL(w2f__8) OpenAD_lin_170
      REAL(w2f__8) OpenAD_lin_171
      REAL(w2f__8) OpenAD_lin_172
      REAL(w2f__8) OpenAD_lin_173
      REAL(w2f__8) OpenAD_lin_174
      REAL(w2f__8) OpenAD_lin_175
      REAL(w2f__8) OpenAD_lin_176
      REAL(w2f__8) OpenAD_lin_177
      REAL(w2f__8) OpenAD_lin_178
      REAL(w2f__8) OpenAD_lin_179
      REAL(w2f__8) OpenAD_lin_18
      REAL(w2f__8) OpenAD_lin_180
      REAL(w2f__8) OpenAD_lin_181
      REAL(w2f__8) OpenAD_lin_182
      REAL(w2f__8) OpenAD_lin_183
      REAL(w2f__8) OpenAD_lin_184
      REAL(w2f__8) OpenAD_lin_185
      REAL(w2f__8) OpenAD_lin_186
      REAL(w2f__8) OpenAD_lin_187
      REAL(w2f__8) OpenAD_lin_188
      REAL(w2f__8) OpenAD_lin_189
      REAL(w2f__8) OpenAD_lin_19
      REAL(w2f__8) OpenAD_lin_190
      REAL(w2f__8) OpenAD_lin_191
      REAL(w2f__8) OpenAD_lin_192
      REAL(w2f__8) OpenAD_lin_193
      REAL(w2f__8) OpenAD_lin_194
      REAL(w2f__8) OpenAD_lin_195
      REAL(w2f__8) OpenAD_lin_196
      REAL(w2f__8) OpenAD_lin_197
      REAL(w2f__8) OpenAD_lin_198
      REAL(w2f__8) OpenAD_lin_199
      REAL(w2f__8) OpenAD_lin_2
      REAL(w2f__8) OpenAD_lin_20
      REAL(w2f__8) OpenAD_lin_200
      REAL(w2f__8) OpenAD_lin_201
      REAL(w2f__8) OpenAD_lin_202
      REAL(w2f__8) OpenAD_lin_203
      REAL(w2f__8) OpenAD_lin_204
      REAL(w2f__8) OpenAD_lin_205
      REAL(w2f__8) OpenAD_lin_206
      REAL(w2f__8) OpenAD_lin_207
      REAL(w2f__8) OpenAD_lin_208
      REAL(w2f__8) OpenAD_lin_209
      REAL(w2f__8) OpenAD_lin_21
      REAL(w2f__8) OpenAD_lin_22
      REAL(w2f__8) OpenAD_lin_23
      REAL(w2f__8) OpenAD_lin_24
      REAL(w2f__8) OpenAD_lin_25
      REAL(w2f__8) OpenAD_lin_26
      REAL(w2f__8) OpenAD_lin_27
      REAL(w2f__8) OpenAD_lin_28
      REAL(w2f__8) OpenAD_lin_29
      REAL(w2f__8) OpenAD_lin_3
      REAL(w2f__8) OpenAD_lin_30
      REAL(w2f__8) OpenAD_lin_31
      REAL(w2f__8) OpenAD_lin_32
      REAL(w2f__8) OpenAD_lin_33
      REAL(w2f__8) OpenAD_lin_34
      REAL(w2f__8) OpenAD_lin_35
      REAL(w2f__8) OpenAD_lin_36
      REAL(w2f__8) OpenAD_lin_37
      REAL(w2f__8) OpenAD_lin_38
      REAL(w2f__8) OpenAD_lin_39
      REAL(w2f__8) OpenAD_lin_4
      REAL(w2f__8) OpenAD_lin_40
      REAL(w2f__8) OpenAD_lin_41
      REAL(w2f__8) OpenAD_lin_42
      REAL(w2f__8) OpenAD_lin_43
      REAL(w2f__8) OpenAD_lin_44
      REAL(w2f__8) OpenAD_lin_45
      REAL(w2f__8) OpenAD_lin_46
      REAL(w2f__8) OpenAD_lin_47
      REAL(w2f__8) OpenAD_lin_48
      REAL(w2f__8) OpenAD_lin_49
      REAL(w2f__8) OpenAD_lin_5
      REAL(w2f__8) OpenAD_lin_50
      REAL(w2f__8) OpenAD_lin_51
      REAL(w2f__8) OpenAD_lin_52
      REAL(w2f__8) OpenAD_lin_53
      REAL(w2f__8) OpenAD_lin_54
      REAL(w2f__8) OpenAD_lin_55
      REAL(w2f__8) OpenAD_lin_56
      REAL(w2f__8) OpenAD_lin_57
      REAL(w2f__8) OpenAD_lin_58
      REAL(w2f__8) OpenAD_lin_59
      REAL(w2f__8) OpenAD_lin_6
      REAL(w2f__8) OpenAD_lin_60
      REAL(w2f__8) OpenAD_lin_61
      REAL(w2f__8) OpenAD_lin_62
      REAL(w2f__8) OpenAD_lin_63
      REAL(w2f__8) OpenAD_lin_64
      REAL(w2f__8) OpenAD_lin_65
      REAL(w2f__8) OpenAD_lin_66
      REAL(w2f__8) OpenAD_lin_67
      REAL(w2f__8) OpenAD_lin_68
      REAL(w2f__8) OpenAD_lin_69
      REAL(w2f__8) OpenAD_lin_7
      REAL(w2f__8) OpenAD_lin_70
      REAL(w2f__8) OpenAD_lin_71
      REAL(w2f__8) OpenAD_lin_72
      REAL(w2f__8) OpenAD_lin_73
      REAL(w2f__8) OpenAD_lin_74
      REAL(w2f__8) OpenAD_lin_75
      REAL(w2f__8) OpenAD_lin_76
      REAL(w2f__8) OpenAD_lin_77
      REAL(w2f__8) OpenAD_lin_78
      REAL(w2f__8) OpenAD_lin_79
      REAL(w2f__8) OpenAD_lin_8
      REAL(w2f__8) OpenAD_lin_80
      REAL(w2f__8) OpenAD_lin_81
      REAL(w2f__8) OpenAD_lin_82
      REAL(w2f__8) OpenAD_lin_83
      REAL(w2f__8) OpenAD_lin_84
      REAL(w2f__8) OpenAD_lin_85
      REAL(w2f__8) OpenAD_lin_86
      REAL(w2f__8) OpenAD_lin_87
      REAL(w2f__8) OpenAD_lin_88
      REAL(w2f__8) OpenAD_lin_89
      REAL(w2f__8) OpenAD_lin_9
      REAL(w2f__8) OpenAD_lin_90
      REAL(w2f__8) OpenAD_lin_91
      REAL(w2f__8) OpenAD_lin_92
      REAL(w2f__8) OpenAD_lin_93
      REAL(w2f__8) OpenAD_lin_94
      REAL(w2f__8) OpenAD_lin_95
      REAL(w2f__8) OpenAD_lin_96
      REAL(w2f__8) OpenAD_lin_97
      REAL(w2f__8) OpenAD_lin_98
      REAL(w2f__8) OpenAD_lin_99
      TYPE (OpenADTy_active) OpenAD_prp_0
      TYPE (OpenADTy_active) OpenAD_prp_1
      TYPE (OpenADTy_active) OpenAD_prp_10
      TYPE (OpenADTy_active) OpenAD_prp_11
      TYPE (OpenADTy_active) OpenAD_prp_12
      TYPE (OpenADTy_active) OpenAD_prp_13
      TYPE (OpenADTy_active) OpenAD_prp_14
      TYPE (OpenADTy_active) OpenAD_prp_15
      TYPE (OpenADTy_active) OpenAD_prp_16
      TYPE (OpenADTy_active) OpenAD_prp_17
      TYPE (OpenADTy_active) OpenAD_prp_18
      TYPE (OpenADTy_active) OpenAD_prp_19
      TYPE (OpenADTy_active) OpenAD_prp_2
      TYPE (OpenADTy_active) OpenAD_prp_20
      TYPE (OpenADTy_active) OpenAD_prp_21
      TYPE (OpenADTy_active) OpenAD_prp_22
      TYPE (OpenADTy_active) OpenAD_prp_23
      TYPE (OpenADTy_active) OpenAD_prp_24
      TYPE (OpenADTy_active) OpenAD_prp_25
      TYPE (OpenADTy_active) OpenAD_prp_26
      TYPE (OpenADTy_active) OpenAD_prp_27
      TYPE (OpenADTy_active) OpenAD_prp_28
      TYPE (OpenADTy_active) OpenAD_prp_29
      TYPE (OpenADTy_active) OpenAD_prp_3
      TYPE (OpenADTy_active) OpenAD_prp_30
      TYPE (OpenADTy_active) OpenAD_prp_31
      TYPE (OpenADTy_active) OpenAD_prp_32
      TYPE (OpenADTy_active) OpenAD_prp_33
      TYPE (OpenADTy_active) OpenAD_prp_34
      TYPE (OpenADTy_active) OpenAD_prp_35
      TYPE (OpenADTy_active) OpenAD_prp_36
      TYPE (OpenADTy_active) OpenAD_prp_37
      TYPE (OpenADTy_active) OpenAD_prp_38
      TYPE (OpenADTy_active) OpenAD_prp_39
      TYPE (OpenADTy_active) OpenAD_prp_4
      TYPE (OpenADTy_active) OpenAD_prp_40
      TYPE (OpenADTy_active) OpenAD_prp_41
      TYPE (OpenADTy_active) OpenAD_prp_42
      TYPE (OpenADTy_active) OpenAD_prp_43
      TYPE (OpenADTy_active) OpenAD_prp_44
      TYPE (OpenADTy_active) OpenAD_prp_45
      TYPE (OpenADTy_active) OpenAD_prp_46
      TYPE (OpenADTy_active) OpenAD_prp_47
      TYPE (OpenADTy_active) OpenAD_prp_48
      TYPE (OpenADTy_active) OpenAD_prp_49
      TYPE (OpenADTy_active) OpenAD_prp_5
      TYPE (OpenADTy_active) OpenAD_prp_6
      TYPE (OpenADTy_active) OpenAD_prp_7
      TYPE (OpenADTy_active) OpenAD_prp_8
      TYPE (OpenADTy_active) OpenAD_prp_9
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(X01)
C$OPENAD INDEPENDENT(X02)
C$OPENAD INDEPENDENT(X03)
C$OPENAD INDEPENDENT(X04)
C$OPENAD INDEPENDENT(X05)
C$OPENAD INDEPENDENT(X06)
C$OPENAD DEPENDENT(Y01)
C$OPENAD DEPENDENT(Y02)
C$OPENAD DEPENDENT(Y03)
C$OPENAD DEPENDENT(Y04)
C$OPENAD DEPENDENT(Y05)
C$OPENAD DEPENDENT(Y06)
C$OPENAD DEPENDENT(Y07)
C$OPENAD DEPENDENT(Y08)
C$OPENAD DEPENDENT(Y09)
C$OPENAD DEPENDENT(Y10)
C$OPENAD DEPENDENT(Y11)
C$OPENAD DEPENDENT(Y12)
C$OPENAD DEPENDENT(Y13)
C$OPENAD DEPENDENT(Y14)
C$OPENAD DEPENDENT(Y15)
C$OPENAD DEPENDENT(Y16)
C$OPENAD DEPENDENT(Y17)
C$OPENAD DEPENDENT(Y18)
C$OPENAD DEPENDENT(Y19)
C$OPENAD DEPENDENT(Y20)
C$OPENAD DEPENDENT(Y21)
C$OPENAD DEPENDENT(Y22)
C$OPENAD DEPENDENT(Y23)
C$OPENAD DEPENDENT(Y24)
C$OPENAD DEPENDENT(Y25)
C$OPENAD DEPENDENT(Y26)
C$OPENAD DEPENDENT(Y27)
C$OPENAD DEPENDENT(Y28)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      A = 5.0D-01
      B = (-1.0D00)
      AB = (A * B)
      SQRT3 = 5.77350258827209472656D-01
      TSQRT3 = 1.15470051765441894531D00
      TWOSQRT3 = 1.15470051765441894531D00
      THIRD = 3.33333343267440795898D-01
      TWO3RD = 6.66666686534881591797D-01
      FOUR3RD = 1.33333337306976318359D00
      BM1 = (-2.0D00)
      __value__(MATR0) = (__value__(X02) - __value__(X01))
      OpenAD_aux_0 = (__value__(X03) * 2.0D00 - __value__(X02) -
     >  __value__(X01))
      OpenAD_lin_0 = SQRT3
      __value__(MATR1) = (SQRT3 * OpenAD_aux_0)
      __value__(MATR2) = (__value__(X05) - __value__(X04))
      OpenAD_aux_1 = (__value__(X06) * 2.0D00 - __value__(X05) -
     >  __value__(X04))
      OpenAD_lin_1 = SQRT3
      __value__(MATR3) = (SQRT3 * OpenAD_aux_1)
      OpenAD_lin_2 = __value__(MATR3)
      OpenAD_lin_3 = __value__(MATR0)
      __value__(M03) = (__value__(MATR0) * __value__(MATR3))
      OpenAD_lin_4 = __value__(MATR2)
      OpenAD_lin_5 = __value__(MATR1)
      __value__(M12) = (__value__(MATR1) * __value__(MATR2))
      __value__(G) = (__value__(M03) - __value__(M12))
      __value__(D00) = (__value__(M03) * 2.0D00)
      __value__(D11) = (__value__(M12) * 2.0D00)
      OpenAD_lin_6 = __value__(MATR0)
      OpenAD_lin_7 = __value__(MATR0)
      __value__(M00) = (__value__(MATR0) * __value__(MATR0))
      OpenAD_lin_8 = __value__(MATR1)
      OpenAD_lin_9 = __value__(MATR1)
      __value__(M11) = (__value__(MATR1) * __value__(MATR1))
      OpenAD_lin_10 = __value__(MATR2)
      OpenAD_lin_11 = __value__(MATR2)
      __value__(M22) = (__value__(MATR2) * __value__(MATR2))
      OpenAD_lin_12 = __value__(MATR3)
      OpenAD_lin_13 = __value__(MATR3)
      __value__(M33) = (__value__(MATR3) * __value__(MATR3))
      OpenAD_lin_14 = __value__(M33)
      OpenAD_lin_15 = __value__(M00)
      __value__(D12) = (__value__(M00) * __value__(M33))
      OpenAD_lin_16 = __value__(M22)
      OpenAD_lin_17 = __value__(M11)
      __value__(D03) = (__value__(M11) * __value__(M22))
      __value__(F) = (__value__(D03) + __value__(D12))
      OpenAD_aux_2 = (1.0D00 / __value__(G))
      OpenAD_lin_19 = (-(1.0D00 /(__value__(G) * __value__(G))))
      OpenAD_lin_18 = A
      __value__(LOC1) = (A * OpenAD_aux_2)
      OpenAD_lin_20 = __value__(LOC1)
      OpenAD_lin_21 = __value__(F)
      __value__(OBJ) = (__value__(F) * __value__(LOC1))
      OpenAD_aux_4 = (__value__(G) * __value__(G))
      OpenAD_aux_3 = (1.0D00 / OpenAD_aux_4)
      OpenAD_lin_24 = __value__(G)
      OpenAD_lin_25 = __value__(G)
      OpenAD_lin_23 = (-(1.0D00 /(OpenAD_aux_4 * OpenAD_aux_4)))
      OpenAD_lin_22 = AB
      __value__(LOC2) = (AB * OpenAD_aux_3)
      OpenAD_aux_5 = (AB * B)
      OpenAD_aux_8 = (__value__(G) * __value__(G))
      OpenAD_aux_7 = (__value__(G) * OpenAD_aux_8)
      OpenAD_aux_6 = (1.0D00 / OpenAD_aux_7)
      OpenAD_lin_28 = OpenAD_aux_8
      OpenAD_lin_30 = __value__(G)
      OpenAD_lin_31 = __value__(G)
      OpenAD_lin_29 = __value__(G)
      OpenAD_lin_27 = (-(1.0D00 /(OpenAD_aux_7 * OpenAD_aux_7)))
      OpenAD_lin_26 = OpenAD_aux_5
      __value__(GG) = (OpenAD_aux_5 * OpenAD_aux_6)
      OpenAD_acc_0 = (OpenAD_lin_19 * OpenAD_lin_18)
      OpenAD_acc_1 = (OpenAD_lin_6 + OpenAD_lin_7)
      OpenAD_acc_2 = (OpenAD_lin_8 + OpenAD_lin_9)
      OpenAD_acc_3 = (OpenAD_lin_10 + OpenAD_lin_11)
      OpenAD_acc_4 = (OpenAD_lin_12 + OpenAD_lin_13)
      OpenAD_acc_5 = ((OpenAD_lin_28 +(OpenAD_lin_30 + OpenAD_lin_31) *
     >  OpenAD_lin_29) * OpenAD_lin_27 * OpenAD_lin_26)
      OpenAD_acc_6 = ((OpenAD_lin_24 + OpenAD_lin_25) * OpenAD_lin_23 *
     >  OpenAD_lin_22)
      CALL setderiv(__deriv__(MATR0), __deriv__(X02))
      CALL dec_deriv(__deriv__(MATR0), __deriv__(X01))
      CALL setderiv(__deriv__(MATR2), __deriv__(X05))
      CALL dec_deriv(__deriv__(MATR2), __deriv__(X04))
      CALL sax(OpenAD_acc_1, __deriv__(MATR0), __deriv__(M00))
      CALL sax(OpenAD_acc_3, __deriv__(MATR2), __deriv__(M22))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_0), __deriv__(X01))
      CALL dec_deriv(__deriv__(OpenAD_prp_0), __deriv__(X02))
      CALL saxpy(2.0D00, __deriv__(X03), __deriv__(OpenAD_prp_0))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_1), __deriv__(X04))
      CALL dec_deriv(__deriv__(OpenAD_prp_1), __deriv__(X05))
      CALL saxpy(2.0D00, __deriv__(X06), __deriv__(OpenAD_prp_1))
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(MATR1))
      CALL sax(OpenAD_lin_1, __deriv__(OpenAD_prp_1), __deriv__(MATR3))
      CALL sax(OpenAD_lin_2, __deriv__(MATR0), __deriv__(M03))
      CALL saxpy(OpenAD_lin_3, __deriv__(MATR3), __deriv__(M03))
      CALL sax(OpenAD_lin_4, __deriv__(MATR1), __deriv__(M12))
      CALL saxpy(OpenAD_lin_5, __deriv__(MATR2), __deriv__(M12))
      CALL setderiv(__deriv__(G), __deriv__(M03))
      CALL dec_deriv(__deriv__(G), __deriv__(M12))
      CALL sax(2.0D00, __deriv__(M03), __deriv__(D00))
      CALL sax(2.0D00, __deriv__(M12), __deriv__(D11))
      CALL sax(OpenAD_acc_2, __deriv__(MATR1), __deriv__(M11))
      CALL sax(OpenAD_acc_4, __deriv__(MATR3), __deriv__(M33))
      CALL sax(OpenAD_lin_14, __deriv__(M00), __deriv__(D12))
      CALL saxpy(OpenAD_lin_15, __deriv__(M33), __deriv__(D12))
      CALL sax(OpenAD_lin_16, __deriv__(M11), __deriv__(D03))
      CALL saxpy(OpenAD_lin_17, __deriv__(M22), __deriv__(D03))
      CALL setderiv(__deriv__(F), __deriv__(D03))
      CALL inc_deriv(__deriv__(F), __deriv__(D12))
      CALL sax(OpenAD_acc_0, __deriv__(G), __deriv__(LOC1))
      CALL sax(OpenAD_lin_20, __deriv__(F), __deriv__(OBJ))
      CALL saxpy(OpenAD_lin_21, __deriv__(LOC1), __deriv__(OBJ))
      CALL sax(OpenAD_acc_6, __deriv__(G), __deriv__(LOC2))
      CALL sax(OpenAD_acc_5, __deriv__(G), __deriv__(GG))
      OpenAD_lin_32 = __value__(LOC2)
      OpenAD_lin_33 = __value__(F)
      __value__(G) = (__value__(F) * __value__(LOC2))
      __value__(F) = (__value__(LOC1) * 2.0D00)
      OpenAD_lin_34 = __value__(MATR0)
      OpenAD_lin_35 = __value__(F)
      OpenAD_lin_36 = __value__(MATR3)
      OpenAD_lin_37 = __value__(G)
      __value__(ADJ_M0) = (__value__(F) * __value__(MATR0) + __value__(
     > G) * __value__(MATR3))
      OpenAD_lin_38 = __value__(MATR1)
      OpenAD_lin_39 = __value__(F)
      OpenAD_lin_40 = __value__(MATR2)
      OpenAD_lin_41 = __value__(G)
      __value__(ADJ_M1) = (__value__(F) * __value__(MATR1) - __value__(
     > G) * __value__(MATR2))
      OpenAD_lin_42 = __value__(MATR2)
      OpenAD_lin_43 = __value__(F)
      OpenAD_lin_44 = __value__(MATR1)
      OpenAD_lin_45 = __value__(G)
      __value__(ADJ_M2) = (__value__(F) * __value__(MATR2) - __value__(
     > G) * __value__(MATR1))
      OpenAD_lin_46 = __value__(MATR3)
      OpenAD_lin_47 = __value__(F)
      OpenAD_lin_48 = __value__(MATR0)
      OpenAD_lin_49 = __value__(G)
      __value__(ADJ_M3) = (__value__(F) * __value__(MATR3) + __value__(
     > G) * __value__(MATR0))
      OpenAD_lin_50 = SQRT3
      __value__(LOC1) = (__value__(ADJ_M1) * SQRT3)
      __value__(G_OBJ0) = (-(__value__(ADJ_M0) + __value__(LOC1)))
      __value__(G_OBJ1) = (__value__(ADJ_M0) - __value__(LOC1))
      __value__(G_OBJ2) = (__value__(LOC1) * 2.0D00)
      OpenAD_acc_7 = (2.0D00 * OpenAD_lin_34)
      OpenAD_acc_8 = (2.0D00 * OpenAD_lin_38)
      OpenAD_acc_9 = (OpenAD_lin_40 * INT((-1_w2f__i8)))
      OpenAD_acc_10 = (OpenAD_lin_41 * INT((-1_w2f__i8)))
      OpenAD_acc_11 = (2.0D00 * OpenAD_lin_42)
      OpenAD_acc_12 = (OpenAD_lin_44 * INT((-1_w2f__i8)))
      OpenAD_acc_13 = (OpenAD_lin_45 * INT((-1_w2f__i8)))
      OpenAD_acc_14 = (2.0D00 * OpenAD_lin_46)
      CALL setderiv(__deriv__(OpenAD_prp_2), __deriv__(LOC1))
      CALL sax(OpenAD_lin_32, __deriv__(F), __deriv__(G))
      CALL saxpy(OpenAD_lin_33, __deriv__(LOC2), __deriv__(G))
      CALL sax(OpenAD_lin_35, __deriv__(MATR0), __deriv__(ADJ_M0))
      CALL saxpy(OpenAD_acc_7, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M0))
      CALL saxpy(OpenAD_lin_36, __deriv__(G), __deriv__(ADJ_M0))
      CALL saxpy(OpenAD_lin_37, __deriv__(MATR3), __deriv__(ADJ_M0))
      CALL sax(OpenAD_lin_39, __deriv__(MATR1), __deriv__(ADJ_M1))
      CALL saxpy(OpenAD_acc_8, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M1))
      CALL saxpy(OpenAD_acc_9, __deriv__(G), __deriv__(ADJ_M1))
      CALL saxpy(OpenAD_acc_10, __deriv__(MATR2), __deriv__(ADJ_M1))
      CALL sax(OpenAD_lin_43, __deriv__(MATR2), __deriv__(ADJ_M2))
      CALL saxpy(OpenAD_acc_11, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M2))
      CALL saxpy(OpenAD_acc_12, __deriv__(G), __deriv__(ADJ_M2))
      CALL saxpy(OpenAD_acc_13, __deriv__(MATR1), __deriv__(ADJ_M2))
      CALL sax(OpenAD_lin_47, __deriv__(MATR3), __deriv__(ADJ_M3))
      CALL saxpy(OpenAD_acc_14, __deriv__(OpenAD_prp_2), __deriv__(
     > ADJ_M3))
      CALL saxpy(OpenAD_lin_48, __deriv__(G), __deriv__(ADJ_M3))
      CALL saxpy(OpenAD_lin_49, __deriv__(MATR0), __deriv__(ADJ_M3))
      CALL sax(OpenAD_lin_50, __deriv__(ADJ_M1), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ1), __deriv__(ADJ_M0))
      CALL dec_deriv(__deriv__(G_OBJ1), __deriv__(LOC1))
      CALL sax(2.0D00, __deriv__(LOC1), __deriv__(G_OBJ2))
      CALL set_neg_deriv(__deriv__(G_OBJ0), __deriv__(ADJ_M0))
      CALL dec_deriv(__deriv__(G_OBJ0), __deriv__(LOC1))
      OpenAD_lin_51 = SQRT3
      __value__(LOC1) = (__value__(ADJ_M3) * SQRT3)
      __value__(G_OBJ3) = (-(__value__(ADJ_M2) + __value__(LOC1)))
      __value__(G_OBJ4) = (__value__(ADJ_M2) - __value__(LOC1))
      __value__(G_OBJ5) = (__value__(LOC1) * 2.0D00)
      __value__(LOC2) = (__value__(LOC2) * 2.0D00)
      OpenAD_lin_52 = __value__(LOC2)
      OpenAD_lin_53 = __value__(D00)
      OpenAD_lin_54 = __value__(M33)
      OpenAD_lin_55 = __value__(GG)
      __value__(R00) = (__value__(D00) * __value__(LOC2) + __value__(GG
     > ) * __value__(M33))
      OpenAD_lin_56 = __value__(LOC2)
      OpenAD_lin_57 = __value__(D11)
      OpenAD_lin_58 = __value__(M22)
      OpenAD_lin_59 = __value__(GG)
      __value__(R11) = (__value__(D11) * __value__(LOC2) + __value__(GG
     > ) * __value__(M22))
      R22 = (__value__(D11) * __value__(LOC2) + __value__(GG) *
     >  __value__(M11))
      R33 = (__value__(D00) * __value__(LOC2) + __value__(GG) *
     >  __value__(M00))
      OpenAD_lin_60 = __value__(MATR1)
      OpenAD_lin_61 = __value__(MATR0)
      __value__(M01) = (__value__(MATR0) * __value__(MATR1))
      OpenAD_lin_62 = __value__(MATR2)
      OpenAD_lin_63 = __value__(MATR0)
      __value__(M02) = (__value__(MATR0) * __value__(MATR2))
      OpenAD_lin_64 = __value__(MATR2)
      OpenAD_lin_65 = __value__(MATR1)
      __value__(M12) = (__value__(MATR1) * __value__(MATR2))
      OpenAD_lin_66 = __value__(MATR3)
      OpenAD_lin_67 = __value__(MATR1)
      __value__(M13) = (__value__(MATR1) * __value__(MATR3))
      OpenAD_lin_68 = __value__(MATR3)
      OpenAD_lin_69 = __value__(MATR2)
      __value__(M23) = (__value__(MATR2) * __value__(MATR3))
      OpenAD_aux_9 = (__value__(M02) + __value__(M13))
      OpenAD_lin_70 = OpenAD_aux_9
      OpenAD_lin_71 = __value__(LOC2)
      __value__(D01) = (__value__(LOC2) * OpenAD_aux_9)
      CALL setderiv(__deriv__(OpenAD_prp_3), __deriv__(LOC2))
      CALL sax(OpenAD_lin_51, __deriv__(ADJ_M3), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ4), __deriv__(ADJ_M2))
      CALL dec_deriv(__deriv__(G_OBJ4), __deriv__(LOC1))
      CALL sax(2.0D00, __deriv__(LOC1), __deriv__(G_OBJ5))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_3), __deriv__(LOC2))
      CALL sax(OpenAD_lin_60, __deriv__(MATR0), __deriv__(M01))
      CALL saxpy(OpenAD_lin_61, __deriv__(MATR1), __deriv__(M01))
      CALL sax(OpenAD_lin_62, __deriv__(MATR0), __deriv__(M02))
      CALL saxpy(OpenAD_lin_63, __deriv__(MATR2), __deriv__(M02))
      CALL sax(OpenAD_lin_64, __deriv__(MATR1), __deriv__(M12))
      CALL saxpy(OpenAD_lin_65, __deriv__(MATR2), __deriv__(M12))
      CALL sax(OpenAD_lin_66, __deriv__(MATR1), __deriv__(M13))
      CALL saxpy(OpenAD_lin_67, __deriv__(MATR3), __deriv__(M13))
      CALL sax(OpenAD_lin_68, __deriv__(MATR2), __deriv__(M23))
      CALL saxpy(OpenAD_lin_69, __deriv__(MATR3), __deriv__(M23))
      CALL setderiv(__deriv__(OpenAD_prp_4), __deriv__(M02))
      CALL inc_deriv(__deriv__(OpenAD_prp_4), __deriv__(M13))
      CALL set_neg_deriv(__deriv__(G_OBJ3), __deriv__(ADJ_M2))
      CALL dec_deriv(__deriv__(G_OBJ3), __deriv__(LOC1))
      CALL sax(OpenAD_lin_52, __deriv__(D00), __deriv__(R00))
      CALL saxpy(OpenAD_lin_53, __deriv__(LOC2), __deriv__(R00))
      CALL saxpy(OpenAD_lin_54, __deriv__(GG), __deriv__(R00))
      CALL saxpy(OpenAD_lin_55, __deriv__(M33), __deriv__(R00))
      CALL sax(OpenAD_lin_56, __deriv__(D11), __deriv__(R11))
      CALL saxpy(OpenAD_lin_57, __deriv__(LOC2), __deriv__(R11))
      CALL saxpy(OpenAD_lin_58, __deriv__(GG), __deriv__(R11))
      CALL saxpy(OpenAD_lin_59, __deriv__(M22), __deriv__(R11))
      CALL sax(OpenAD_lin_70, __deriv__(LOC2), __deriv__(D01))
      CALL saxpy(OpenAD_lin_71, __deriv__(OpenAD_prp_4), __deriv__(D01)
     > )
      OpenAD_lin_72 = THIRD
      __value__(LOC1) = (__value__(R11) * THIRD)
      OpenAD_aux_10 = (__value__(D01) + __value__(GG) * __value__(M23))
      OpenAD_lin_74 = __value__(M23)
      OpenAD_lin_75 = __value__(GG)
      OpenAD_lin_73 = TWOSQRT3
      __value__(F) = (TWOSQRT3 * OpenAD_aux_10)
      __value__(G) = (__value__(LOC1) + __value__(R00))
      __value__(H_OBJ1) = (__value__(LOC1) - __value__(R00))
      __value__(H_OBJ0) = (__value__(F) + __value__(G))
      __value__(H_OBJ3) = (__value__(G) - __value__(F))
      CALL sax(OpenAD_lin_72, __deriv__(R11), __deriv__(LOC1))
      CALL setderiv(__deriv__(G), __deriv__(LOC1))
      CALL inc_deriv(__deriv__(G), __deriv__(R00))
      CALL setderiv(__deriv__(H_OBJ1), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ1), __deriv__(R00))
      CALL setderiv(__deriv__(OpenAD_prp_5), __deriv__(D01))
      CALL saxpy(OpenAD_lin_74, __deriv__(GG), __deriv__(OpenAD_prp_5))
      CALL saxpy(OpenAD_lin_75, __deriv__(M23), __deriv__(OpenAD_prp_5)
     > )
      CALL sax(OpenAD_lin_73, __deriv__(OpenAD_prp_5), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ0), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ0), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ3), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ3), __deriv__(F))
      OpenAD_lin_76 = TWO3RD
      __value__(LOC1) = (-(__value__(R11) * TWO3RD))
      __value__(H_OBJ2) = (__value__(LOC1) - __value__(F))
      __value__(H_OBJ4) = (__value__(F) + __value__(LOC1))
      OpenAD_lin_77 = FOUR3RD
      __value__(H_OBJ5) = (FOUR3RD * __value__(R11))
      OpenAD_acc_15 = (OpenAD_lin_76 * INT((-1_w2f__i8)))
      CALL sax(OpenAD_lin_77, __deriv__(R11), __deriv__(H_OBJ5))
      CALL sax(OpenAD_acc_15, __deriv__(R11), __deriv__(LOC1))
      CALL setderiv(__deriv__(H_OBJ2), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ2), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ4), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ4), __deriv__(LOC1))
      OpenAD_lin_78 = THIRD
      __value__(LOC1) = (__value__(R00) * THIRD)
      OpenAD_aux_11 = (__value__(D01) + __value__(GG) * __value__(M01))
      OpenAD_lin_80 = __value__(M01)
      OpenAD_lin_81 = __value__(GG)
      OpenAD_lin_79 = TWOSQRT3
      __value__(F) = (TWOSQRT3 * OpenAD_aux_11)
      __value__(G) = (__value__(LOC1) + __value__(R11))
      __value__(H_OBJ16) = (__value__(LOC1) - __value__(R11))
      __value__(H_OBJ15) = (__value__(F) + __value__(G))
      __value__(H_OBJ18) = (__value__(G) - __value__(F))
      CALL sax(OpenAD_lin_78, __deriv__(R00), __deriv__(LOC1))
      CALL setderiv(__deriv__(G), __deriv__(LOC1))
      CALL inc_deriv(__deriv__(G), __deriv__(R11))
      CALL setderiv(__deriv__(H_OBJ16), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ16), __deriv__(R11))
      CALL setderiv(__deriv__(OpenAD_prp_6), __deriv__(D01))
      CALL saxpy(OpenAD_lin_80, __deriv__(GG), __deriv__(OpenAD_prp_6))
      CALL saxpy(OpenAD_lin_81, __deriv__(M01), __deriv__(OpenAD_prp_6)
     > )
      CALL sax(OpenAD_lin_79, __deriv__(OpenAD_prp_6), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ15), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ15), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ18), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ18), __deriv__(F))
      OpenAD_lin_82 = TWO3RD
      __value__(LOC1) = (-(__value__(R00) * TWO3RD))
      __value__(H_OBJ17) = (__value__(LOC1) - __value__(F))
      __value__(H_OBJ19) = (__value__(F) + __value__(LOC1))
      OpenAD_lin_83 = FOUR3RD
      __value__(H_OBJ20) = (FOUR3RD * __value__(R00))
      OpenAD_acc_16 = (OpenAD_lin_82 * INT((-1_w2f__i8)))
      CALL sax(OpenAD_lin_83, __deriv__(R00), __deriv__(H_OBJ20))
      CALL sax(OpenAD_acc_16, __deriv__(R00), __deriv__(LOC1))
      CALL setderiv(__deriv__(H_OBJ17), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ17), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ19), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ19), __deriv__(LOC1))
      OpenAD_aux_12 = (__value__(M01) + __value__(M23))
      OpenAD_lin_84 = OpenAD_aux_12
      OpenAD_lin_85 = __value__(LOC2)
      __value__(LOC1) = (__value__(LOC2) * OpenAD_aux_12)
      OpenAD_lin_86 = __value__(M02)
      OpenAD_lin_87 = __value__(GG)
      __value__(R13) = (__value__(LOC1) + __value__(GG) * __value__(M02
     > ))
      OpenAD_lin_88 = __value__(M13)
      OpenAD_lin_89 = __value__(GG)
      __value__(R02) = (__value__(LOC1) + __value__(GG) * __value__(M13
     > ))
      OpenAD_lin_90 = __value__(LOC2)
      OpenAD_lin_91 = __value__(D03)
      OpenAD_lin_92 = __value__(M03)
      OpenAD_lin_93 = __value__(GG)
      __value__(R03) = (__value__(D03) * __value__(LOC2) + __value__(GG
     > ) * __value__(M03))
      OpenAD_lin_94 = __value__(LOC2)
      OpenAD_lin_95 = __value__(D12)
      OpenAD_lin_96 = __value__(M12)
      OpenAD_lin_97 = __value__(GG)
      __value__(R12) = (__value__(D12) * __value__(LOC2) + __value__(GG
     > ) * __value__(M12))
      OpenAD_lin_98 = TWO3RD
      __value__(G) = (-(__value__(R13) * TWO3RD))
      OpenAD_lin_99 = TWOSQRT3
      __value__(F) = (__value__(R03) * TWOSQRT3)
      __value__(H_OBJ8) = (__value__(G) - __value__(F))
      __value__(H_OBJ11) = (__value__(F) + __value__(G))
      OpenAD_acc_17 = (OpenAD_lin_98 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_7), __deriv__(M01))
      CALL inc_deriv(__deriv__(OpenAD_prp_7), __deriv__(M23))
      CALL sax(OpenAD_lin_84, __deriv__(LOC2), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_85, __deriv__(OpenAD_prp_7), __deriv__(LOC1
     > ))
      CALL setderiv(__deriv__(R13), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_86, __deriv__(GG), __deriv__(R13))
      CALL saxpy(OpenAD_lin_87, __deriv__(M02), __deriv__(R13))
      CALL setderiv(__deriv__(R02), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_88, __deriv__(GG), __deriv__(R02))
      CALL saxpy(OpenAD_lin_89, __deriv__(M13), __deriv__(R02))
      CALL sax(OpenAD_lin_90, __deriv__(D03), __deriv__(R03))
      CALL saxpy(OpenAD_lin_91, __deriv__(LOC2), __deriv__(R03))
      CALL saxpy(OpenAD_lin_92, __deriv__(GG), __deriv__(R03))
      CALL saxpy(OpenAD_lin_93, __deriv__(M03), __deriv__(R03))
      CALL sax(OpenAD_lin_94, __deriv__(D12), __deriv__(R12))
      CALL saxpy(OpenAD_lin_95, __deriv__(LOC2), __deriv__(R12))
      CALL saxpy(OpenAD_lin_96, __deriv__(GG), __deriv__(R12))
      CALL saxpy(OpenAD_lin_97, __deriv__(M12), __deriv__(R12))
      CALL sax(OpenAD_lin_99, __deriv__(R03), __deriv__(F))
      CALL sax(OpenAD_acc_17, __deriv__(R13), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ8), __deriv__(G))
      CALL dec_deriv(__deriv__(H_OBJ8), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ11), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ11), __deriv__(G))
      OpenAD_lin_100 = TWOSQRT3
      __value__(F) = (__value__(R12) * TWOSQRT3)
      __value__(H_OBJ12) = (__value__(G) - __value__(F))
      __value__(H_OBJ13) = (__value__(F) + __value__(G))
      __value__(H_OBJ14) = (__value__(R13) * 1.2D+01)
      OpenAD_lin_101 = FOUR3RD
      __value__(G) = (FOUR3RD * __value__(R02))
      CALL setderiv(__deriv__(OpenAD_prp_8), __deriv__(G))
      CALL sax(OpenAD_lin_100, __deriv__(R12), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ12), __deriv__(OpenAD_prp_8))
      CALL dec_deriv(__deriv__(H_OBJ12), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ13), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ13), __deriv__(OpenAD_prp_8))
      CALL sax(1.2D+01, __deriv__(R13), __deriv__(H_OBJ14))
      CALL sax(OpenAD_lin_101, __deriv__(R02), __deriv__(G))
      OpenAD_aux_13 = (__value__(R03) + __value__(R12))
      OpenAD_lin_102 = SQRT3
      __value__(F) = (SQRT3 * OpenAD_aux_13)
      __value__(H_OBJ10) = (__value__(G) - __value__(F))
      __value__(H_OBJ6) = (__value__(F) + __value__(G))
      OpenAD_lin_103 = TWO3RD
      __value__(G) = (__value__(R02) * TWO3RD)
      CALL setderiv(__deriv__(OpenAD_prp_9), __deriv__(G))
      CALL setderiv(__deriv__(OpenAD_prp_10), __deriv__(R03))
      CALL inc_deriv(__deriv__(OpenAD_prp_10), __deriv__(R12))
      CALL sax(OpenAD_lin_103, __deriv__(R02), __deriv__(G))
      CALL sax(OpenAD_lin_102, __deriv__(OpenAD_prp_10), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ10), __deriv__(OpenAD_prp_9))
      CALL dec_deriv(__deriv__(H_OBJ10), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ6), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ6), __deriv__(OpenAD_prp_9))
      OpenAD_aux_14 = (__value__(R03) - __value__(R12))
      OpenAD_lin_104 = SQRT3
      __value__(F) = (SQRT3 * OpenAD_aux_14)
      __value__(H_OBJ9) = (__value__(G) - __value__(F))
      __value__(H_OBJ7) = (__value__(F) + __value__(G))
      __value__(MATR0) = (__value__(G_OBJ1) - __value__(G_OBJ0))
      OpenAD_aux_15 = (__value__(G_OBJ2) * 2.0D00 - __value__(G_OBJ1) -
     >  __value__(G_OBJ2))
      OpenAD_lin_105 = SQRT3
      __value__(MATR1) = (SQRT3 * OpenAD_aux_15)
      __value__(MATR2) = (__value__(G_OBJ4) - __value__(G_OBJ3))
      OpenAD_aux_16 = (__value__(G_OBJ5) * 2.0D00 - __value__(G_OBJ4) -
     >  __value__(G_OBJ3))
      OpenAD_lin_106 = SQRT3
      __value__(MATR3) = (SQRT3 * OpenAD_aux_16)
      OpenAD_lin_107 = __value__(MATR3)
      OpenAD_lin_108 = __value__(MATR0)
      __value__(M03) = (__value__(MATR0) * __value__(MATR3))
      OpenAD_lin_109 = __value__(MATR2)
      OpenAD_lin_110 = __value__(MATR1)
      __value__(M12) = (__value__(MATR1) * __value__(MATR2))
      __value__(G) = (__value__(M03) - __value__(M12))
      __value__(D00) = (__value__(M03) * 2.0D00)
      __value__(D11) = (__value__(M12) * 2.0D00)
      OpenAD_lin_111 = __value__(MATR0)
      OpenAD_lin_112 = __value__(MATR0)
      __value__(M00) = (__value__(MATR0) * __value__(MATR0))
      OpenAD_lin_113 = __value__(MATR1)
      OpenAD_lin_114 = __value__(MATR1)
      __value__(M11) = (__value__(MATR1) * __value__(MATR1))
      OpenAD_lin_115 = __value__(MATR2)
      OpenAD_lin_116 = __value__(MATR2)
      __value__(M22) = (__value__(MATR2) * __value__(MATR2))
      OpenAD_lin_117 = __value__(MATR3)
      OpenAD_lin_118 = __value__(MATR3)
      __value__(M33) = (__value__(MATR3) * __value__(MATR3))
      OpenAD_lin_119 = __value__(M33)
      OpenAD_lin_120 = __value__(M00)
      __value__(D12) = (__value__(M00) * __value__(M33))
      OpenAD_lin_121 = __value__(M22)
      OpenAD_lin_122 = __value__(M11)
      __value__(D03) = (__value__(M11) * __value__(M22))
      OpenAD_acc_18 = (OpenAD_lin_111 + OpenAD_lin_112)
      OpenAD_acc_19 = (OpenAD_lin_113 + OpenAD_lin_114)
      OpenAD_acc_20 = (OpenAD_lin_115 + OpenAD_lin_116)
      OpenAD_acc_21 = (OpenAD_lin_117 + OpenAD_lin_118)
      CALL setderiv(__deriv__(OpenAD_prp_11), __deriv__(G))
      CALL setderiv(__deriv__(OpenAD_prp_12), __deriv__(R03))
      CALL dec_deriv(__deriv__(OpenAD_prp_12), __deriv__(R12))
      CALL setderiv(__deriv__(MATR0), __deriv__(G_OBJ1))
      CALL dec_deriv(__deriv__(MATR0), __deriv__(G_OBJ0))
      CALL setderiv(__deriv__(MATR2), __deriv__(G_OBJ4))
      CALL dec_deriv(__deriv__(MATR2), __deriv__(G_OBJ3))
      CALL sax(OpenAD_acc_18, __deriv__(MATR0), __deriv__(M00))
      CALL sax(OpenAD_acc_20, __deriv__(MATR2), __deriv__(M22))
      CALL sax(OpenAD_lin_104, __deriv__(OpenAD_prp_12), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ9), __deriv__(OpenAD_prp_11))
      CALL dec_deriv(__deriv__(H_OBJ9), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ7), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ7), __deriv__(OpenAD_prp_11))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_13), __deriv__(G_OBJ1))
      CALL saxpy(1.0D00, __deriv__(G_OBJ2), __deriv__(OpenAD_prp_13))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_14), __deriv__(G_OBJ3))
      CALL dec_deriv(__deriv__(OpenAD_prp_14), __deriv__(G_OBJ4))
      CALL saxpy(2.0D00, __deriv__(G_OBJ5), __deriv__(OpenAD_prp_14))
      CALL sax(OpenAD_lin_105, __deriv__(OpenAD_prp_13), __deriv__(
     > MATR1))
      CALL sax(OpenAD_lin_106, __deriv__(OpenAD_prp_14), __deriv__(
     > MATR3))
      CALL sax(OpenAD_lin_107, __deriv__(MATR0), __deriv__(M03))
      CALL saxpy(OpenAD_lin_108, __deriv__(MATR3), __deriv__(M03))
      CALL sax(OpenAD_lin_109, __deriv__(MATR1), __deriv__(M12))
      CALL saxpy(OpenAD_lin_110, __deriv__(MATR2), __deriv__(M12))
      CALL setderiv(__deriv__(G), __deriv__(M03))
      CALL dec_deriv(__deriv__(G), __deriv__(M12))
      CALL sax(2.0D00, __deriv__(M03), __deriv__(D00))
      CALL sax(2.0D00, __deriv__(M12), __deriv__(D11))
      CALL sax(OpenAD_acc_19, __deriv__(MATR1), __deriv__(M11))
      CALL sax(OpenAD_acc_21, __deriv__(MATR3), __deriv__(M33))
      CALL sax(OpenAD_lin_119, __deriv__(M00), __deriv__(D12))
      CALL saxpy(OpenAD_lin_120, __deriv__(M33), __deriv__(D12))
      CALL sax(OpenAD_lin_121, __deriv__(M11), __deriv__(D03))
      CALL saxpy(OpenAD_lin_122, __deriv__(M22), __deriv__(D03))
      __value__(F) = (__value__(D03) + __value__(D12))
      OpenAD_aux_17 = (1.0D00 / __value__(G))
      OpenAD_lin_124 = (-(1.0D00 /(__value__(G) * __value__(G))))
      OpenAD_lin_123 = A
      __value__(LOC1) = (A * OpenAD_aux_17)
      OpenAD_lin_125 = __value__(LOC1)
      OpenAD_lin_126 = __value__(F)
      __value__(OBJ) = (__value__(F) * __value__(LOC1))
      OpenAD_aux_19 = (__value__(G) * __value__(G))
      OpenAD_aux_18 = (1.0D00 / OpenAD_aux_19)
      OpenAD_lin_129 = __value__(G)
      OpenAD_lin_130 = __value__(G)
      OpenAD_lin_128 = (-(1.0D00 /(OpenAD_aux_19 * OpenAD_aux_19)))
      OpenAD_lin_127 = AB
      __value__(LOC2) = (AB * OpenAD_aux_18)
      OpenAD_aux_20 = (AB * B)
      OpenAD_aux_23 = (__value__(G) * __value__(G))
      OpenAD_aux_22 = (__value__(G) * OpenAD_aux_23)
      OpenAD_aux_21 = (1.0D00 / OpenAD_aux_22)
      OpenAD_lin_133 = OpenAD_aux_23
      OpenAD_lin_135 = __value__(G)
      OpenAD_lin_136 = __value__(G)
      OpenAD_lin_134 = __value__(G)
      OpenAD_lin_132 = (-(1.0D00 /(OpenAD_aux_22 * OpenAD_aux_22)))
      OpenAD_lin_131 = OpenAD_aux_20
      __value__(GG) = (OpenAD_aux_20 * OpenAD_aux_21)
      OpenAD_lin_137 = __value__(LOC2)
      OpenAD_lin_138 = __value__(F)
      __value__(G) = (__value__(F) * __value__(LOC2))
      OpenAD_acc_22 = (OpenAD_lin_124 * OpenAD_lin_123)
      OpenAD_acc_23 = ((OpenAD_lin_133 +(OpenAD_lin_135 +
     >  OpenAD_lin_136) * OpenAD_lin_134) * OpenAD_lin_132 *
     >  OpenAD_lin_131)
      OpenAD_acc_24 = ((OpenAD_lin_129 + OpenAD_lin_130) *
     >  OpenAD_lin_128 * OpenAD_lin_127)
      CALL setderiv(__deriv__(OpenAD_prp_15), __deriv__(G))
      CALL setderiv(__deriv__(F), __deriv__(D03))
      CALL inc_deriv(__deriv__(F), __deriv__(D12))
      CALL sax(OpenAD_acc_22, __deriv__(OpenAD_prp_15), __deriv__(LOC1)
     > )
      CALL sax(OpenAD_lin_125, __deriv__(F), __deriv__(OBJ))
      CALL saxpy(OpenAD_lin_126, __deriv__(LOC1), __deriv__(OBJ))
      CALL sax(OpenAD_acc_24, __deriv__(OpenAD_prp_15), __deriv__(LOC2)
     > )
      CALL sax(OpenAD_lin_137, __deriv__(F), __deriv__(G))
      CALL saxpy(OpenAD_lin_138, __deriv__(LOC2), __deriv__(G))
      CALL sax(OpenAD_acc_23, __deriv__(OpenAD_prp_15), __deriv__(GG))
      __value__(F) = (__value__(LOC1) * 2.0D00)
      OpenAD_lin_139 = __value__(MATR0)
      OpenAD_lin_140 = __value__(F)
      OpenAD_lin_141 = __value__(MATR3)
      OpenAD_lin_142 = __value__(G)
      __value__(ADJ_M0) = (__value__(F) * __value__(MATR0) + __value__(
     > G) * __value__(MATR3))
      OpenAD_lin_143 = __value__(MATR1)
      OpenAD_lin_144 = __value__(F)
      OpenAD_lin_145 = __value__(MATR2)
      OpenAD_lin_146 = __value__(G)
      __value__(ADJ_M1) = (__value__(F) * __value__(MATR1) - __value__(
     > G) * __value__(MATR2))
      OpenAD_lin_147 = __value__(MATR2)
      OpenAD_lin_148 = __value__(F)
      OpenAD_lin_149 = __value__(MATR1)
      OpenAD_lin_150 = __value__(G)
      __value__(ADJ_M2) = (__value__(F) * __value__(MATR2) - __value__(
     > G) * __value__(MATR1))
      OpenAD_lin_151 = __value__(MATR3)
      OpenAD_lin_152 = __value__(F)
      OpenAD_lin_153 = __value__(MATR0)
      OpenAD_lin_154 = __value__(G)
      __value__(ADJ_M3) = (__value__(F) * __value__(MATR3) + __value__(
     > G) * __value__(MATR0))
      OpenAD_lin_155 = SQRT3
      __value__(LOC1) = (__value__(ADJ_M1) * SQRT3)
      __value__(G_OBJ0) = (__value__(G_OBJ0) - __value__(ADJ_M0) -
     >  __value__(LOC1))
      __value__(G_OBJ1) = (__value__(ADJ_M0) + __value__(G_OBJ1) -
     >  __value__(LOC1))
      __value__(G_OBJ2) = (__value__(G_OBJ1) + __value__(LOC1) * 2.0D00
     > )
      OpenAD_acc_25 = (2.0D00 * OpenAD_lin_139)
      OpenAD_acc_26 = (2.0D00 * OpenAD_lin_143)
      OpenAD_acc_27 = (OpenAD_lin_145 * INT((-1_w2f__i8)))
      OpenAD_acc_28 = (OpenAD_lin_146 * INT((-1_w2f__i8)))
      OpenAD_acc_29 = (2.0D00 * OpenAD_lin_147)
      OpenAD_acc_30 = (OpenAD_lin_149 * INT((-1_w2f__i8)))
      OpenAD_acc_31 = (OpenAD_lin_150 * INT((-1_w2f__i8)))
      OpenAD_acc_32 = (2.0D00 * OpenAD_lin_151)
      CALL setderiv(__deriv__(OpenAD_prp_16), __deriv__(LOC1))
      CALL setderiv(__deriv__(OpenAD_prp_17), __deriv__(G_OBJ0))
      CALL setderiv(__deriv__(OpenAD_prp_18), __deriv__(G_OBJ1))
      CALL sax(OpenAD_lin_140, __deriv__(MATR0), __deriv__(ADJ_M0))
      CALL saxpy(OpenAD_acc_25, __deriv__(OpenAD_prp_16), __deriv__(
     > ADJ_M0))
      CALL saxpy(OpenAD_lin_141, __deriv__(G), __deriv__(ADJ_M0))
      CALL saxpy(OpenAD_lin_142, __deriv__(MATR3), __deriv__(ADJ_M0))
      CALL sax(OpenAD_lin_144, __deriv__(MATR1), __deriv__(ADJ_M1))
      CALL saxpy(OpenAD_acc_26, __deriv__(OpenAD_prp_16), __deriv__(
     > ADJ_M1))
      CALL saxpy(OpenAD_acc_27, __deriv__(G), __deriv__(ADJ_M1))
      CALL saxpy(OpenAD_acc_28, __deriv__(MATR2), __deriv__(ADJ_M1))
      CALL sax(OpenAD_lin_148, __deriv__(MATR2), __deriv__(ADJ_M2))
      CALL saxpy(OpenAD_acc_29, __deriv__(OpenAD_prp_16), __deriv__(
     > ADJ_M2))
      CALL saxpy(OpenAD_acc_30, __deriv__(G), __deriv__(ADJ_M2))
      CALL saxpy(OpenAD_acc_31, __deriv__(MATR1), __deriv__(ADJ_M2))
      CALL sax(OpenAD_lin_152, __deriv__(MATR3), __deriv__(ADJ_M3))
      CALL saxpy(OpenAD_acc_32, __deriv__(OpenAD_prp_16), __deriv__(
     > ADJ_M3))
      CALL saxpy(OpenAD_lin_153, __deriv__(G), __deriv__(ADJ_M3))
      CALL saxpy(OpenAD_lin_154, __deriv__(MATR0), __deriv__(ADJ_M3))
      CALL sax(OpenAD_lin_155, __deriv__(ADJ_M1), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ0), __deriv__(OpenAD_prp_17))
      CALL dec_deriv(__deriv__(G_OBJ0), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(G_OBJ0), __deriv__(ADJ_M0))
      CALL setderiv(__deriv__(G_OBJ1), __deriv__(ADJ_M0))
      CALL inc_deriv(__deriv__(G_OBJ1), __deriv__(OpenAD_prp_18))
      CALL dec_deriv(__deriv__(G_OBJ1), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ2), __deriv__(G_OBJ1))
      CALL saxpy(2.0D00, __deriv__(LOC1), __deriv__(G_OBJ2))
      OpenAD_lin_156 = SQRT3
      __value__(LOC1) = (__value__(ADJ_M3) * SQRT3)
      __value__(G_OBJ3) = (__value__(G_OBJ3) - __value__(ADJ_M2) -
     >  __value__(LOC1))
      __value__(G_OBJ4) = (__value__(ADJ_M2) + __value__(G_OBJ4) -
     >  __value__(LOC1))
      __value__(G_OBJ5) = (__value__(G_OBJ5) + __value__(LOC1) * 2.0D00
     > )
      __value__(LOC2) = (__value__(LOC2) * 2.0D00)
      OpenAD_lin_157 = __value__(LOC2)
      OpenAD_lin_158 = __value__(D00)
      OpenAD_lin_159 = __value__(M33)
      OpenAD_lin_160 = __value__(GG)
      __value__(R00) = (__value__(D00) * __value__(LOC2) + __value__(GG
     > ) * __value__(M33))
      OpenAD_lin_161 = __value__(LOC2)
      OpenAD_lin_162 = __value__(D11)
      OpenAD_lin_163 = __value__(M22)
      OpenAD_lin_164 = __value__(GG)
      __value__(R11) = (__value__(D11) * __value__(LOC2) + __value__(GG
     > ) * __value__(M22))
      R22 = (__value__(D11) * __value__(LOC2) + __value__(GG) *
     >  __value__(M11))
      R33 = (__value__(D00) * __value__(LOC2) + __value__(GG) *
     >  __value__(M00))
      OpenAD_lin_165 = __value__(MATR1)
      OpenAD_lin_166 = __value__(MATR0)
      __value__(M01) = (__value__(MATR0) * __value__(MATR1))
      OpenAD_lin_167 = __value__(MATR2)
      OpenAD_lin_168 = __value__(MATR0)
      __value__(M02) = (__value__(MATR0) * __value__(MATR2))
      OpenAD_lin_169 = __value__(MATR2)
      OpenAD_lin_170 = __value__(MATR1)
      __value__(M12) = (__value__(MATR1) * __value__(MATR2))
      OpenAD_lin_171 = __value__(MATR3)
      OpenAD_lin_172 = __value__(MATR1)
      __value__(M13) = (__value__(MATR1) * __value__(MATR3))
      OpenAD_lin_173 = __value__(MATR3)
      OpenAD_lin_174 = __value__(MATR2)
      __value__(M23) = (__value__(MATR2) * __value__(MATR3))
      OpenAD_aux_24 = (__value__(M02) + __value__(M13))
      OpenAD_lin_175 = OpenAD_aux_24
      OpenAD_lin_176 = __value__(LOC2)
      __value__(D01) = (__value__(LOC2) * OpenAD_aux_24)
      CALL setderiv(__deriv__(OpenAD_prp_19), __deriv__(G_OBJ3))
      CALL setderiv(__deriv__(OpenAD_prp_20), __deriv__(G_OBJ4))
      CALL setderiv(__deriv__(OpenAD_prp_21), __deriv__(G_OBJ5))
      CALL setderiv(__deriv__(OpenAD_prp_22), __deriv__(LOC2))
      CALL sax(OpenAD_lin_156, __deriv__(ADJ_M3), __deriv__(LOC1))
      CALL sax(2.0D00, __deriv__(OpenAD_prp_22), __deriv__(LOC2))
      CALL sax(OpenAD_lin_165, __deriv__(MATR0), __deriv__(M01))
      CALL saxpy(OpenAD_lin_166, __deriv__(MATR1), __deriv__(M01))
      CALL sax(OpenAD_lin_167, __deriv__(MATR0), __deriv__(M02))
      CALL saxpy(OpenAD_lin_168, __deriv__(MATR2), __deriv__(M02))
      CALL sax(OpenAD_lin_169, __deriv__(MATR1), __deriv__(M12))
      CALL saxpy(OpenAD_lin_170, __deriv__(MATR2), __deriv__(M12))
      CALL sax(OpenAD_lin_171, __deriv__(MATR1), __deriv__(M13))
      CALL saxpy(OpenAD_lin_172, __deriv__(MATR3), __deriv__(M13))
      CALL sax(OpenAD_lin_173, __deriv__(MATR2), __deriv__(M23))
      CALL saxpy(OpenAD_lin_174, __deriv__(MATR3), __deriv__(M23))
      CALL setderiv(__deriv__(OpenAD_prp_23), __deriv__(M02))
      CALL inc_deriv(__deriv__(OpenAD_prp_23), __deriv__(M13))
      CALL setderiv(__deriv__(G_OBJ3), __deriv__(OpenAD_prp_19))
      CALL dec_deriv(__deriv__(G_OBJ3), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(G_OBJ3), __deriv__(ADJ_M2))
      CALL setderiv(__deriv__(G_OBJ4), __deriv__(ADJ_M2))
      CALL inc_deriv(__deriv__(G_OBJ4), __deriv__(OpenAD_prp_20))
      CALL dec_deriv(__deriv__(G_OBJ4), __deriv__(LOC1))
      CALL setderiv(__deriv__(G_OBJ5), __deriv__(OpenAD_prp_21))
      CALL saxpy(2.0D00, __deriv__(LOC1), __deriv__(G_OBJ5))
      CALL sax(OpenAD_lin_157, __deriv__(D00), __deriv__(R00))
      CALL saxpy(OpenAD_lin_158, __deriv__(LOC2), __deriv__(R00))
      CALL saxpy(OpenAD_lin_159, __deriv__(GG), __deriv__(R00))
      CALL saxpy(OpenAD_lin_160, __deriv__(M33), __deriv__(R00))
      CALL sax(OpenAD_lin_161, __deriv__(D11), __deriv__(R11))
      CALL saxpy(OpenAD_lin_162, __deriv__(LOC2), __deriv__(R11))
      CALL saxpy(OpenAD_lin_163, __deriv__(GG), __deriv__(R11))
      CALL saxpy(OpenAD_lin_164, __deriv__(M22), __deriv__(R11))
      CALL sax(OpenAD_lin_175, __deriv__(LOC2), __deriv__(D01))
      CALL saxpy(OpenAD_lin_176, __deriv__(OpenAD_prp_23), __deriv__(
     > D01))
      OpenAD_lin_177 = THIRD
      __value__(LOC1) = (__value__(R11) * THIRD)
      OpenAD_aux_25 = (__value__(D01) + __value__(GG) * __value__(M23))
      OpenAD_lin_179 = __value__(M23)
      OpenAD_lin_180 = __value__(GG)
      OpenAD_lin_178 = TWOSQRT3
      __value__(F) = (TWOSQRT3 * OpenAD_aux_25)
      __value__(G) = (__value__(LOC1) + __value__(R00))
      __value__(H_OBJ1) = (__value__(H_OBJ1) + __value__(LOC1) -
     >  __value__(R00))
      __value__(H_OBJ0) = (__value__(F) + __value__(G) + __value__(
     > H_OBJ0))
      __value__(H_OBJ3) = (__value__(G) + __value__(H_OBJ3) - __value__
     > (F))
      CALL setderiv(__deriv__(OpenAD_prp_24), __deriv__(H_OBJ1))
      CALL setderiv(__deriv__(OpenAD_prp_25), __deriv__(H_OBJ0))
      CALL setderiv(__deriv__(OpenAD_prp_26), __deriv__(H_OBJ3))
      CALL sax(OpenAD_lin_177, __deriv__(R11), __deriv__(LOC1))
      CALL setderiv(__deriv__(G), __deriv__(LOC1))
      CALL inc_deriv(__deriv__(G), __deriv__(R00))
      CALL setderiv(__deriv__(OpenAD_prp_27), __deriv__(D01))
      CALL saxpy(OpenAD_lin_179, __deriv__(GG), __deriv__(OpenAD_prp_27
     > ))
      CALL saxpy(OpenAD_lin_180, __deriv__(M23), __deriv__(
     > OpenAD_prp_27))
      CALL setderiv(__deriv__(H_OBJ1), __deriv__(OpenAD_prp_24))
      CALL inc_deriv(__deriv__(H_OBJ1), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ1), __deriv__(R00))
      CALL sax(OpenAD_lin_178, __deriv__(OpenAD_prp_27), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ0), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ0), __deriv__(G))
      CALL inc_deriv(__deriv__(H_OBJ0), __deriv__(OpenAD_prp_25))
      CALL setderiv(__deriv__(H_OBJ3), __deriv__(G))
      CALL inc_deriv(__deriv__(H_OBJ3), __deriv__(OpenAD_prp_26))
      CALL dec_deriv(__deriv__(H_OBJ3), __deriv__(F))
      OpenAD_lin_181 = TWO3RD
      __value__(LOC1) = (-(__value__(R11) * TWO3RD))
      __value__(H_OBJ2) = (__value__(H_OBJ2) + __value__(LOC1) -
     >  __value__(F))
      __value__(H_OBJ4) = (__value__(F) + __value__(H_OBJ4) + __value__
     > (LOC1))
      OpenAD_lin_182 = FOUR3RD
      __value__(H_OBJ5) = (__value__(H_OBJ5) + FOUR3RD * __value__(R11)
     > )
      OpenAD_acc_33 = (OpenAD_lin_181 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_28), __deriv__(H_OBJ2))
      CALL setderiv(__deriv__(OpenAD_prp_29), __deriv__(H_OBJ4))
      CALL setderiv(__deriv__(OpenAD_prp_30), __deriv__(H_OBJ5))
      CALL sax(OpenAD_acc_33, __deriv__(R11), __deriv__(LOC1))
      CALL setderiv(__deriv__(H_OBJ5), __deriv__(OpenAD_prp_30))
      CALL saxpy(OpenAD_lin_182, __deriv__(R11), __deriv__(H_OBJ5))
      CALL setderiv(__deriv__(H_OBJ2), __deriv__(OpenAD_prp_28))
      CALL inc_deriv(__deriv__(H_OBJ2), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ2), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ4), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ4), __deriv__(OpenAD_prp_29))
      CALL inc_deriv(__deriv__(H_OBJ4), __deriv__(LOC1))
      OpenAD_lin_183 = THIRD
      __value__(LOC1) = (__value__(R00) * THIRD)
      OpenAD_aux_26 = (__value__(D01) + __value__(GG) * __value__(M01))
      OpenAD_lin_185 = __value__(M01)
      OpenAD_lin_186 = __value__(GG)
      OpenAD_lin_184 = TWOSQRT3
      __value__(F) = (TWOSQRT3 * OpenAD_aux_26)
      __value__(G) = (__value__(LOC1) + __value__(R11))
      __value__(H_OBJ16) = (__value__(H_OBJ16) + __value__(LOC1) -
     >  __value__(R11))
      __value__(H_OBJ15) = (__value__(F) + __value__(G) + __value__(
     > H_OBJ15))
      __value__(H_OBJ18) = (__value__(G) + __value__(H_OBJ18) -
     >  __value__(F))
      CALL setderiv(__deriv__(OpenAD_prp_31), __deriv__(H_OBJ16))
      CALL setderiv(__deriv__(OpenAD_prp_32), __deriv__(H_OBJ15))
      CALL setderiv(__deriv__(OpenAD_prp_33), __deriv__(H_OBJ18))
      CALL sax(OpenAD_lin_183, __deriv__(R00), __deriv__(LOC1))
      CALL setderiv(__deriv__(G), __deriv__(LOC1))
      CALL inc_deriv(__deriv__(G), __deriv__(R11))
      CALL setderiv(__deriv__(OpenAD_prp_34), __deriv__(D01))
      CALL saxpy(OpenAD_lin_185, __deriv__(GG), __deriv__(OpenAD_prp_34
     > ))
      CALL saxpy(OpenAD_lin_186, __deriv__(M01), __deriv__(
     > OpenAD_prp_34))
      CALL setderiv(__deriv__(H_OBJ16), __deriv__(OpenAD_prp_31))
      CALL inc_deriv(__deriv__(H_OBJ16), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ16), __deriv__(R11))
      CALL sax(OpenAD_lin_184, __deriv__(OpenAD_prp_34), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ15), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ15), __deriv__(G))
      CALL inc_deriv(__deriv__(H_OBJ15), __deriv__(OpenAD_prp_32))
      CALL setderiv(__deriv__(H_OBJ18), __deriv__(G))
      CALL inc_deriv(__deriv__(H_OBJ18), __deriv__(OpenAD_prp_33))
      CALL dec_deriv(__deriv__(H_OBJ18), __deriv__(F))
      OpenAD_lin_187 = TWO3RD
      __value__(LOC1) = (-(__value__(R00) * TWO3RD))
      __value__(H_OBJ17) = (__value__(H_OBJ17) + __value__(LOC1) -
     >  __value__(F))
      __value__(H_OBJ19) = (__value__(F) + __value__(H_OBJ19) +
     >  __value__(LOC1))
      OpenAD_lin_188 = FOUR3RD
      __value__(H_OBJ20) = (__value__(H_OBJ20) + FOUR3RD * __value__(
     > R00))
      OpenAD_acc_34 = (OpenAD_lin_187 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_35), __deriv__(H_OBJ17))
      CALL setderiv(__deriv__(OpenAD_prp_36), __deriv__(H_OBJ19))
      CALL setderiv(__deriv__(OpenAD_prp_37), __deriv__(H_OBJ20))
      CALL sax(OpenAD_acc_34, __deriv__(R00), __deriv__(LOC1))
      CALL setderiv(__deriv__(H_OBJ20), __deriv__(OpenAD_prp_37))
      CALL saxpy(OpenAD_lin_188, __deriv__(R00), __deriv__(H_OBJ20))
      CALL setderiv(__deriv__(H_OBJ17), __deriv__(OpenAD_prp_35))
      CALL inc_deriv(__deriv__(H_OBJ17), __deriv__(LOC1))
      CALL dec_deriv(__deriv__(H_OBJ17), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ19), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ19), __deriv__(OpenAD_prp_36))
      CALL inc_deriv(__deriv__(H_OBJ19), __deriv__(LOC1))
      OpenAD_aux_27 = (__value__(M01) + __value__(M23))
      OpenAD_lin_189 = OpenAD_aux_27
      OpenAD_lin_190 = __value__(LOC2)
      __value__(LOC1) = (__value__(LOC2) * OpenAD_aux_27)
      OpenAD_lin_191 = __value__(M02)
      OpenAD_lin_192 = __value__(GG)
      __value__(R13) = (__value__(LOC1) + __value__(GG) * __value__(M02
     > ))
      OpenAD_lin_193 = __value__(M13)
      OpenAD_lin_194 = __value__(GG)
      __value__(R02) = (__value__(LOC1) + __value__(GG) * __value__(M13
     > ))
      OpenAD_lin_195 = __value__(LOC2)
      OpenAD_lin_196 = __value__(D03)
      OpenAD_lin_197 = __value__(M03)
      OpenAD_lin_198 = __value__(GG)
      __value__(R03) = (__value__(D03) * __value__(LOC2) + __value__(GG
     > ) * __value__(M03))
      OpenAD_lin_199 = __value__(LOC2)
      OpenAD_lin_200 = __value__(D12)
      OpenAD_lin_201 = __value__(M12)
      OpenAD_lin_202 = __value__(GG)
      __value__(R12) = (__value__(D12) * __value__(LOC2) + __value__(GG
     > ) * __value__(M12))
      OpenAD_lin_203 = TWO3RD
      __value__(G) = (-(__value__(R13) * TWO3RD))
      OpenAD_lin_204 = TWOSQRT3
      __value__(F) = (__value__(R03) * TWOSQRT3)
      __value__(H_OBJ8) = (__value__(G) + __value__(H_OBJ8) - __value__
     > (F))
      __value__(H_OBJ11) = (__value__(F) + __value__(G) + __value__(
     > H_OBJ11))
      OpenAD_acc_35 = (OpenAD_lin_203 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_38), __deriv__(H_OBJ8))
      CALL setderiv(__deriv__(OpenAD_prp_39), __deriv__(H_OBJ11))
      CALL setderiv(__deriv__(OpenAD_prp_40), __deriv__(M01))
      CALL inc_deriv(__deriv__(OpenAD_prp_40), __deriv__(M23))
      CALL sax(OpenAD_lin_189, __deriv__(LOC2), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_190, __deriv__(OpenAD_prp_40), __deriv__(
     > LOC1))
      CALL setderiv(__deriv__(R13), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_191, __deriv__(GG), __deriv__(R13))
      CALL saxpy(OpenAD_lin_192, __deriv__(M02), __deriv__(R13))
      CALL setderiv(__deriv__(R02), __deriv__(LOC1))
      CALL saxpy(OpenAD_lin_193, __deriv__(GG), __deriv__(R02))
      CALL saxpy(OpenAD_lin_194, __deriv__(M13), __deriv__(R02))
      CALL sax(OpenAD_lin_195, __deriv__(D03), __deriv__(R03))
      CALL saxpy(OpenAD_lin_196, __deriv__(LOC2), __deriv__(R03))
      CALL saxpy(OpenAD_lin_197, __deriv__(GG), __deriv__(R03))
      CALL saxpy(OpenAD_lin_198, __deriv__(M03), __deriv__(R03))
      CALL sax(OpenAD_lin_199, __deriv__(D12), __deriv__(R12))
      CALL saxpy(OpenAD_lin_200, __deriv__(LOC2), __deriv__(R12))
      CALL saxpy(OpenAD_lin_201, __deriv__(GG), __deriv__(R12))
      CALL saxpy(OpenAD_lin_202, __deriv__(M12), __deriv__(R12))
      CALL sax(OpenAD_lin_204, __deriv__(R03), __deriv__(F))
      CALL sax(OpenAD_acc_35, __deriv__(R13), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ8), __deriv__(G))
      CALL inc_deriv(__deriv__(H_OBJ8), __deriv__(OpenAD_prp_38))
      CALL dec_deriv(__deriv__(H_OBJ8), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ11), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ11), __deriv__(G))
      CALL inc_deriv(__deriv__(H_OBJ11), __deriv__(OpenAD_prp_39))
      OpenAD_lin_205 = TWOSQRT3
      __value__(F) = (__value__(R12) * TWOSQRT3)
      __value__(H_OBJ12) = (__value__(G) + __value__(H_OBJ12) -
     >  __value__(F))
      __value__(H_OBJ13) = (__value__(F) + __value__(G) + __value__(
     > H_OBJ13))
      __value__(H_OBJ14) = (__value__(H_OBJ14) + __value__(R13) *
     >  1.2D+01)
      OpenAD_lin_206 = FOUR3RD
      __value__(G) = (FOUR3RD * __value__(R02))
      CALL setderiv(__deriv__(OpenAD_prp_41), __deriv__(G))
      CALL setderiv(__deriv__(OpenAD_prp_42), __deriv__(H_OBJ12))
      CALL setderiv(__deriv__(OpenAD_prp_43), __deriv__(H_OBJ13))
      CALL setderiv(__deriv__(OpenAD_prp_44), __deriv__(H_OBJ14))
      CALL sax(OpenAD_lin_205, __deriv__(R12), __deriv__(F))
      CALL sax(OpenAD_lin_206, __deriv__(R02), __deriv__(G))
      CALL setderiv(__deriv__(H_OBJ12), __deriv__(OpenAD_prp_41))
      CALL inc_deriv(__deriv__(H_OBJ12), __deriv__(OpenAD_prp_42))
      CALL dec_deriv(__deriv__(H_OBJ12), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ13), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ13), __deriv__(OpenAD_prp_41))
      CALL inc_deriv(__deriv__(H_OBJ13), __deriv__(OpenAD_prp_43))
      CALL setderiv(__deriv__(H_OBJ14), __deriv__(OpenAD_prp_44))
      CALL saxpy(1.2D+01, __deriv__(R13), __deriv__(H_OBJ14))
      OpenAD_aux_28 = (__value__(R03) + __value__(R12))
      OpenAD_lin_207 = SQRT3
      __value__(F) = (SQRT3 * OpenAD_aux_28)
      __value__(H_OBJ10) = (__value__(G) + __value__(H_OBJ10) -
     >  __value__(F))
      __value__(H_OBJ6) = (__value__(F) + __value__(G) + __value__(
     > H_OBJ6))
      OpenAD_lin_208 = TWO3RD
      __value__(G) = (__value__(R02) * TWO3RD)
      CALL setderiv(__deriv__(OpenAD_prp_45), __deriv__(G))
      CALL setderiv(__deriv__(OpenAD_prp_46), __deriv__(H_OBJ10))
      CALL setderiv(__deriv__(OpenAD_prp_47), __deriv__(H_OBJ6))
      CALL setderiv(__deriv__(OpenAD_prp_48), __deriv__(R03))
      CALL inc_deriv(__deriv__(OpenAD_prp_48), __deriv__(R12))
      CALL sax(OpenAD_lin_208, __deriv__(R02), __deriv__(G))
      CALL sax(OpenAD_lin_207, __deriv__(OpenAD_prp_48), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ10), __deriv__(OpenAD_prp_45))
      CALL inc_deriv(__deriv__(H_OBJ10), __deriv__(OpenAD_prp_46))
      CALL dec_deriv(__deriv__(H_OBJ10), __deriv__(F))
      CALL setderiv(__deriv__(H_OBJ6), __deriv__(F))
      CALL inc_deriv(__deriv__(H_OBJ6), __deriv__(OpenAD_prp_45))
      CALL inc_deriv(__deriv__(H_OBJ6), __deriv__(OpenAD_prp_47))
      OpenAD_aux_29 = (__value__(R03) - __value__(R12))
      OpenAD_lin_209 = SQRT3
      __value__(F) = (SQRT3 * OpenAD_aux_29)
      __value__(H_OBJ9) = (__value__(G) + __value__(H_OBJ9) - __value__
     > (F))
      __value__(H_OBJ7) = (__value__(F) + __value__(G) + __value__(
     > H_OBJ7))
      __value__(Y01) = __value__(OBJ)
      __value__(Y02) = __value__(G_OBJ0)
      __value__(Y03) = __value__(G_OBJ1)
      __value__(Y04) = __value__(G_OBJ2)
      __value__(Y05) = __value__(G_OBJ3)
      __value__(Y06) = __value__(G_OBJ4)
      __value__(Y07) = __value__(G_OBJ5)
      __value__(Y08) = __value__(H_OBJ0)
      __value__(Y09) = __value__(H_OBJ1)
      __value__(Y10) = __value__(H_OBJ2)
      __value__(Y11) = __value__(H_OBJ3)
      __value__(Y12) = __value__(H_OBJ4)
      __value__(Y13) = __value__(H_OBJ5)
      __value__(Y14) = __value__(H_OBJ6)
      __value__(Y15) = __value__(H_OBJ7)
      __value__(Y16) = __value__(H_OBJ8)
      __value__(Y17) = __value__(H_OBJ9)
      __value__(Y18) = __value__(H_OBJ10)
      __value__(Y19) = __value__(H_OBJ11)
      __value__(Y20) = __value__(H_OBJ12)
      __value__(Y21) = __value__(H_OBJ13)
      __value__(Y22) = __value__(H_OBJ14)
      __value__(Y23) = __value__(H_OBJ15)
      __value__(Y24) = __value__(H_OBJ16)
      __value__(Y25) = __value__(H_OBJ17)
      __value__(Y26) = __value__(H_OBJ18)
      __value__(Y27) = __value__(H_OBJ19)
      __value__(Y28) = __value__(H_OBJ20)
      CALL setderiv(__deriv__(OpenAD_prp_49), __deriv__(R03))
      CALL dec_deriv(__deriv__(OpenAD_prp_49), __deriv__(R12))
      CALL setderiv(__deriv__(Y01), __deriv__(OBJ))
      CALL setderiv(__deriv__(Y02), __deriv__(G_OBJ0))
      CALL setderiv(__deriv__(Y03), __deriv__(G_OBJ1))
      CALL setderiv(__deriv__(Y04), __deriv__(G_OBJ2))
      CALL setderiv(__deriv__(Y05), __deriv__(G_OBJ3))
      CALL setderiv(__deriv__(Y06), __deriv__(G_OBJ4))
      CALL setderiv(__deriv__(Y07), __deriv__(G_OBJ5))
      CALL setderiv(__deriv__(Y08), __deriv__(H_OBJ0))
      CALL setderiv(__deriv__(Y09), __deriv__(H_OBJ1))
      CALL setderiv(__deriv__(Y10), __deriv__(H_OBJ2))
      CALL setderiv(__deriv__(Y11), __deriv__(H_OBJ3))
      CALL setderiv(__deriv__(Y12), __deriv__(H_OBJ4))
      CALL setderiv(__deriv__(Y13), __deriv__(H_OBJ5))
      CALL setderiv(__deriv__(Y14), __deriv__(H_OBJ6))
      CALL setderiv(__deriv__(Y16), __deriv__(H_OBJ8))
      CALL setderiv(__deriv__(Y18), __deriv__(H_OBJ10))
      CALL setderiv(__deriv__(Y19), __deriv__(H_OBJ11))
      CALL setderiv(__deriv__(Y20), __deriv__(H_OBJ12))
      CALL setderiv(__deriv__(Y21), __deriv__(H_OBJ13))
      CALL setderiv(__deriv__(Y22), __deriv__(H_OBJ14))
      CALL setderiv(__deriv__(Y23), __deriv__(H_OBJ15))
      CALL setderiv(__deriv__(Y24), __deriv__(H_OBJ16))
      CALL setderiv(__deriv__(Y25), __deriv__(H_OBJ17))
      CALL setderiv(__deriv__(Y26), __deriv__(H_OBJ18))
      CALL setderiv(__deriv__(Y27), __deriv__(H_OBJ19))
      CALL setderiv(__deriv__(Y28), __deriv__(H_OBJ20))
      CALL sax(OpenAD_lin_209, __deriv__(OpenAD_prp_49), __deriv__(F))
      CALL setderiv(__deriv__(Y15), __deriv__(F))
      CALL inc_deriv(__deriv__(Y15), __deriv__(G))
      CALL inc_deriv(__deriv__(Y15), __deriv__(H_OBJ7))
      CALL setderiv(__deriv__(Y17), __deriv__(G))
      CALL inc_deriv(__deriv__(Y17), __deriv__(H_OBJ9))
      CALL dec_deriv(__deriv__(Y17), __deriv__(F))
      END SUBROUTINE
