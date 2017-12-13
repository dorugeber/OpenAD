
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
      SUBROUTINE head(X01, X02, X03, X04, X05, X06, X07, X08, X09, X10,
     >  X11, X12, X13, X14, X15, X16, X17, X18, X19, Y1, Y2, Y3, Y4, Y5
     > )
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
      TYPE (OpenADTy_active) X07
      INTENT(IN)  X07
      TYPE (OpenADTy_active) X08
      INTENT(IN)  X08
      TYPE (OpenADTy_active) X09
      INTENT(IN)  X09
      TYPE (OpenADTy_active) X10
      INTENT(IN)  X10
      TYPE (OpenADTy_active) X11
      INTENT(IN)  X11
      TYPE (OpenADTy_active) X12
      INTENT(IN)  X12
      TYPE (OpenADTy_active) X13
      INTENT(IN)  X13
      TYPE (OpenADTy_active) X14
      INTENT(IN)  X14
      TYPE (OpenADTy_active) X15
      INTENT(IN)  X15
      TYPE (OpenADTy_active) X16
      INTENT(IN)  X16
      REAL(w2f__8) X17
      INTENT(IN)  X17
      REAL(w2f__8) X18
      INTENT(IN)  X18
      REAL(w2f__8) X19
      INTENT(IN)  X19
      TYPE (OpenADTy_active) Y1
      INTENT(OUT)  Y1
      TYPE (OpenADTy_active) Y2
      INTENT(OUT)  Y2
      TYPE (OpenADTy_active) Y3
      INTENT(OUT)  Y3
      TYPE (OpenADTy_active) Y4
      INTENT(OUT)  Y4
      TYPE (OpenADTy_active) Y5
      INTENT(OUT)  Y5
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) HALF
      PARAMETER ( HALF = 5.0E-01)
      REAL(w2f__4) ONE
      PARAMETER ( ONE = 1.0)
      TYPE (OpenADTy_active) T01
      TYPE (OpenADTy_active) T02
      TYPE (OpenADTy_active) T03
      TYPE (OpenADTy_active) T04
      TYPE (OpenADTy_active) T05
      TYPE (OpenADTy_active) T06
      TYPE (OpenADTy_active) T07
      TYPE (OpenADTy_active) T08
      TYPE (OpenADTy_active) T09
      TYPE (OpenADTy_active) T10
      TYPE (OpenADTy_active) T11
      TYPE (OpenADTy_active) T12
      TYPE (OpenADTy_active) T13
      TYPE (OpenADTy_active) T14
      TYPE (OpenADTy_active) T15
      TYPE (OpenADTy_active) T16
      TYPE (OpenADTy_active) T17
      TYPE (OpenADTy_active) T18
      TYPE (OpenADTy_active) T19
      TYPE (OpenADTy_active) T20
      TYPE (OpenADTy_active) T21
      TYPE (OpenADTy_active) T22
      TYPE (OpenADTy_active) T23
      TYPE (OpenADTy_active) T24
      TYPE (OpenADTy_active) T25
      TYPE (OpenADTy_active) T26
      TYPE (OpenADTy_active) T27
      TYPE (OpenADTy_active) T28
      TYPE (OpenADTy_active) T29
      TYPE (OpenADTy_active) T30
      TYPE (OpenADTy_active) T31
      TYPE (OpenADTy_active) T32
      TYPE (OpenADTy_active) T33
      TYPE (OpenADTy_active) T34
      TYPE (OpenADTy_active) T35
      TYPE (OpenADTy_active) T36
      TYPE (OpenADTy_active) T37
      TYPE (OpenADTy_active) T38
      TYPE (OpenADTy_active) T39
      TYPE (OpenADTy_active) T40
      TYPE (OpenADTy_active) T41
      TYPE (OpenADTy_active) T42
      TYPE (OpenADTy_active) T43
      TYPE (OpenADTy_active) T44
      TYPE (OpenADTy_active) T45
      TYPE (OpenADTy_active) T46
      TYPE (OpenADTy_active) T47
      TYPE (OpenADTy_active) T48
      TYPE (OpenADTy_active) T49
      TYPE (OpenADTy_active) T50
      TYPE (OpenADTy_active) T51
      TYPE (OpenADTy_active) T52
      TYPE (OpenADTy_active) T53
      TYPE (OpenADTy_active) T54
      TYPE (OpenADTy_active) T55
      TYPE (OpenADTy_active) T56
      TYPE (OpenADTy_active) T57
      REAL(w2f__8) OpenAD_acc_0
      REAL(w2f__8) OpenAD_acc_1
      REAL(w2f__8) OpenAD_acc_10
      REAL(w2f__8) OpenAD_acc_100
      REAL(w2f__8) OpenAD_acc_101
      REAL(w2f__8) OpenAD_acc_102
      REAL(w2f__8) OpenAD_acc_103
      REAL(w2f__8) OpenAD_acc_104
      REAL(w2f__8) OpenAD_acc_105
      REAL(w2f__8) OpenAD_acc_106
      REAL(w2f__8) OpenAD_acc_107
      REAL(w2f__8) OpenAD_acc_108
      REAL(w2f__8) OpenAD_acc_109
      REAL(w2f__8) OpenAD_acc_11
      REAL(w2f__8) OpenAD_acc_110
      REAL(w2f__8) OpenAD_acc_111
      REAL(w2f__8) OpenAD_acc_112
      REAL(w2f__8) OpenAD_acc_113
      REAL(w2f__8) OpenAD_acc_114
      REAL(w2f__8) OpenAD_acc_115
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
      REAL(w2f__8) OpenAD_acc_36
      REAL(w2f__8) OpenAD_acc_37
      REAL(w2f__8) OpenAD_acc_38
      REAL(w2f__8) OpenAD_acc_39
      REAL(w2f__8) OpenAD_acc_4
      REAL(w2f__8) OpenAD_acc_40
      REAL(w2f__8) OpenAD_acc_41
      REAL(w2f__8) OpenAD_acc_42
      REAL(w2f__8) OpenAD_acc_43
      REAL(w2f__8) OpenAD_acc_44
      REAL(w2f__8) OpenAD_acc_45
      REAL(w2f__8) OpenAD_acc_46
      REAL(w2f__8) OpenAD_acc_47
      REAL(w2f__8) OpenAD_acc_48
      REAL(w2f__8) OpenAD_acc_49
      REAL(w2f__8) OpenAD_acc_5
      REAL(w2f__8) OpenAD_acc_50
      REAL(w2f__8) OpenAD_acc_51
      REAL(w2f__8) OpenAD_acc_52
      REAL(w2f__8) OpenAD_acc_53
      REAL(w2f__8) OpenAD_acc_54
      REAL(w2f__8) OpenAD_acc_55
      REAL(w2f__8) OpenAD_acc_56
      REAL(w2f__8) OpenAD_acc_57
      REAL(w2f__8) OpenAD_acc_58
      REAL(w2f__8) OpenAD_acc_59
      REAL(w2f__8) OpenAD_acc_6
      REAL(w2f__8) OpenAD_acc_60
      REAL(w2f__8) OpenAD_acc_61
      REAL(w2f__8) OpenAD_acc_62
      REAL(w2f__8) OpenAD_acc_63
      REAL(w2f__8) OpenAD_acc_64
      REAL(w2f__8) OpenAD_acc_65
      REAL(w2f__8) OpenAD_acc_66
      REAL(w2f__8) OpenAD_acc_67
      REAL(w2f__8) OpenAD_acc_68
      REAL(w2f__8) OpenAD_acc_69
      REAL(w2f__8) OpenAD_acc_7
      REAL(w2f__8) OpenAD_acc_70
      REAL(w2f__8) OpenAD_acc_71
      REAL(w2f__8) OpenAD_acc_72
      REAL(w2f__8) OpenAD_acc_73
      REAL(w2f__8) OpenAD_acc_74
      REAL(w2f__8) OpenAD_acc_75
      REAL(w2f__8) OpenAD_acc_76
      REAL(w2f__8) OpenAD_acc_77
      REAL(w2f__8) OpenAD_acc_78
      REAL(w2f__8) OpenAD_acc_79
      REAL(w2f__8) OpenAD_acc_8
      REAL(w2f__8) OpenAD_acc_80
      REAL(w2f__8) OpenAD_acc_81
      REAL(w2f__8) OpenAD_acc_82
      REAL(w2f__8) OpenAD_acc_83
      REAL(w2f__8) OpenAD_acc_84
      REAL(w2f__8) OpenAD_acc_85
      REAL(w2f__8) OpenAD_acc_86
      REAL(w2f__8) OpenAD_acc_87
      REAL(w2f__8) OpenAD_acc_88
      REAL(w2f__8) OpenAD_acc_89
      REAL(w2f__8) OpenAD_acc_9
      REAL(w2f__8) OpenAD_acc_90
      REAL(w2f__8) OpenAD_acc_91
      REAL(w2f__8) OpenAD_acc_92
      REAL(w2f__8) OpenAD_acc_93
      REAL(w2f__8) OpenAD_acc_94
      REAL(w2f__8) OpenAD_acc_95
      REAL(w2f__8) OpenAD_acc_96
      REAL(w2f__8) OpenAD_acc_97
      REAL(w2f__8) OpenAD_acc_98
      REAL(w2f__8) OpenAD_acc_99
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
      REAL(w2f__8) OpenAD_aux_30
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
      REAL(w2f__8) OpenAD_lin_210
      REAL(w2f__8) OpenAD_lin_211
      REAL(w2f__8) OpenAD_lin_212
      REAL(w2f__8) OpenAD_lin_213
      REAL(w2f__8) OpenAD_lin_214
      REAL(w2f__8) OpenAD_lin_215
      REAL(w2f__8) OpenAD_lin_216
      REAL(w2f__8) OpenAD_lin_217
      REAL(w2f__8) OpenAD_lin_218
      REAL(w2f__8) OpenAD_lin_219
      REAL(w2f__8) OpenAD_lin_22
      REAL(w2f__8) OpenAD_lin_220
      REAL(w2f__8) OpenAD_lin_221
      REAL(w2f__8) OpenAD_lin_222
      REAL(w2f__8) OpenAD_lin_223
      REAL(w2f__8) OpenAD_lin_224
      REAL(w2f__8) OpenAD_lin_225
      REAL(w2f__8) OpenAD_lin_226
      REAL(w2f__8) OpenAD_lin_227
      REAL(w2f__8) OpenAD_lin_228
      REAL(w2f__8) OpenAD_lin_229
      REAL(w2f__8) OpenAD_lin_23
      REAL(w2f__8) OpenAD_lin_230
      REAL(w2f__8) OpenAD_lin_231
      REAL(w2f__8) OpenAD_lin_232
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
      TYPE (OpenADTy_active) OpenAD_prp_2
      TYPE (OpenADTy_active) OpenAD_prp_3
      TYPE (OpenADTy_active) OpenAD_prp_4
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
C$OPENAD INDEPENDENT(X07)
C$OPENAD INDEPENDENT(X08)
C$OPENAD INDEPENDENT(X09)
C$OPENAD INDEPENDENT(X10)
C$OPENAD INDEPENDENT(X11)
C$OPENAD INDEPENDENT(X12)
C$OPENAD INDEPENDENT(X13)
C$OPENAD INDEPENDENT(X14)
C$OPENAD INDEPENDENT(X15)
C$OPENAD INDEPENDENT(X16)
C$OPENAD INDEPENDENT(X17)
C$OPENAD INDEPENDENT(X18)
C$OPENAD INDEPENDENT(X19)
C$OPENAD DEPENDENT(Y1)
C$OPENAD DEPENDENT(Y2)
C$OPENAD DEPENDENT(Y3)
C$OPENAD DEPENDENT(Y4)
C$OPENAD DEPENDENT(Y5)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = ((__value__(X01) ** 2) +(__value__(X02) ** 2) +(
     > __value__(X03) ** 2))
      OpenAD_lin_1 = (2 *(__value__(X01) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_2 = (2 *(__value__(X02) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_3 = (2 *(__value__(X03) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_0 = COS(OpenAD_aux_0)
      __value__(T01) = SIN(OpenAD_aux_0)
      OpenAD_lin_4 = (-(1.0D00 /(__value__(T01) * __value__(T01))))
      __value__(T02) = (1.0D00 / __value__(T01))
      OpenAD_lin_5 = __value__(T02)
      OpenAD_lin_6 = __value__(X01)
      __value__(T03) = (__value__(X01) * __value__(T02))
      OpenAD_lin_7 = __value__(T02)
      OpenAD_lin_8 = __value__(X02)
      __value__(T04) = (__value__(X02) * __value__(T02))
      OpenAD_lin_9 = __value__(T02)
      OpenAD_lin_10 = __value__(X03)
      __value__(T05) = (__value__(X03) * __value__(T02))
      OpenAD_aux_2 = (__value__(X10) / __value__(X05))
      OpenAD_aux_1 = (SIN(OpenAD_aux_2) + 1.0D00)
      OpenAD_lin_13 = (INT(1_w2f__i8) / __value__(X05))
      OpenAD_lin_14 = (-(__value__(X10) /(__value__(X05) * __value__(
     > X05))))
      OpenAD_lin_12 = COS(OpenAD_aux_2)
      OpenAD_lin_11 = (-(1.0D00 /(OpenAD_aux_1 * OpenAD_aux_1)))
      __value__(T06) = (1.0D00 / OpenAD_aux_1)
      __value__(T07) = (1.0D00 - __value__(T06))
      OpenAD_aux_3 = ((__value__(X06) ** 2) +(__value__(X07) ** 2) +(
     > __value__(X08) ** 2))
      OpenAD_lin_15 = (2 *(__value__(X06) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_16 = (2 *(__value__(X07) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_17 = (2 *(__value__(X08) **(2 - INT(1_w2f__i8))))
      __value__(T08) = (OpenAD_aux_3 * 5.0D-01)
      OpenAD_aux_4 = ((__value__(X11) ** 2) +(__value__(X12) ** 2) +(
     > __value__(X13) ** 2))
      OpenAD_lin_18 = (2 *(__value__(X11) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_19 = (2 *(__value__(X12) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_20 = (2 *(__value__(X13) **(2 - INT(1_w2f__i8))))
      __value__(T09) = (OpenAD_aux_4 * 5.0D-01)
      OpenAD_aux_6 = (__value__(X14) * __value__(X16))
      OpenAD_aux_5 = (__value__(X04) * OpenAD_aux_6)
      OpenAD_lin_23 = OpenAD_aux_6
      OpenAD_lin_25 = __value__(X16)
      OpenAD_lin_26 = __value__(X14)
      OpenAD_lin_24 = __value__(X04)
      OpenAD_lin_21 = (INT(1_w2f__i8) / __value__(X05))
      OpenAD_lin_22 = (-(OpenAD_aux_5 /(__value__(X05) * __value__(X05)
     > )))
      __value__(T10) = (__value__(T08) +(OpenAD_aux_5 / __value__(X05))
     > )
      OpenAD_aux_8 = (__value__(X14) * __value__(X16))
      OpenAD_aux_7 = (__value__(X09) * OpenAD_aux_8)
      OpenAD_lin_29 = OpenAD_aux_8
      OpenAD_lin_31 = __value__(X16)
      OpenAD_lin_32 = __value__(X14)
      OpenAD_lin_30 = __value__(X09)
      OpenAD_lin_27 = (INT(1_w2f__i8) / __value__(X10))
      OpenAD_lin_28 = (-(OpenAD_aux_7 /(__value__(X10) * __value__(X10)
     > )))
      __value__(T11) = (__value__(T09) +(OpenAD_aux_7 / __value__(X10))
     > )
      OpenAD_lin_33 = __value__(T06)
      OpenAD_lin_34 = __value__(X06)
      OpenAD_lin_35 = __value__(T07)
      OpenAD_lin_36 = __value__(X11)
      __value__(T12) = (__value__(X06) * __value__(T06) + __value__(X11
     > ) * __value__(T07))
      OpenAD_lin_37 = __value__(T06)
      OpenAD_lin_38 = __value__(X07)
      OpenAD_lin_39 = __value__(T07)
      OpenAD_lin_40 = __value__(X12)
      __value__(T13) = (__value__(X07) * __value__(T06) + __value__(X12
     > ) * __value__(T07))
      OpenAD_lin_41 = __value__(T06)
      OpenAD_lin_42 = __value__(X08)
      OpenAD_lin_43 = __value__(T07)
      OpenAD_lin_44 = __value__(X13)
      __value__(T14) = (__value__(X08) * __value__(T06) + __value__(X13
     > ) * __value__(T07))
      OpenAD_lin_45 = __value__(T10)
      OpenAD_lin_46 = __value__(T06)
      OpenAD_lin_47 = __value__(T11)
      OpenAD_lin_48 = __value__(T07)
      __value__(T15) = (__value__(T06) * __value__(T10) + __value__(T07
     > ) * __value__(T11))
      OpenAD_aux_9 = ((__value__(T12) ** 2) +(__value__(T13) ** 2) +(
     > __value__(T14) ** 2))
      OpenAD_lin_49 = (2 *(__value__(T12) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_50 = (2 *(__value__(T13) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_51 = (2 *(__value__(T14) **(2 - INT(1_w2f__i8))))
      __value__(T18) = (OpenAD_aux_9 * 5.0D-01)
      OpenAD_aux_10 = (__value__(T15) - __value__(T18))
      OpenAD_lin_52 = OpenAD_aux_10
      OpenAD_lin_53 = __value__(X15)
      __value__(T16) = (__value__(X15) * OpenAD_aux_10)
      OpenAD_acc_0 = (OpenAD_lin_30 * OpenAD_lin_27)
      OpenAD_acc_1 = (OpenAD_lin_24 * OpenAD_lin_21)
      OpenAD_acc_2 = (OpenAD_lin_4 * OpenAD_lin_6)
      OpenAD_acc_3 = (OpenAD_lin_4 * OpenAD_lin_8)
      OpenAD_acc_4 = (OpenAD_lin_4 * OpenAD_lin_10)
      OpenAD_acc_5 = (5.0D-01 * INT((-1_w2f__i8)) * OpenAD_lin_53)
      OpenAD_acc_6 = (OpenAD_lin_48 * OpenAD_lin_53)
      OpenAD_acc_7 = (OpenAD_lin_46 * OpenAD_lin_53)
      OpenAD_acc_8 = (OpenAD_lin_11 *(OpenAD_lin_45 * OpenAD_lin_53 +
     >  INT((-1_w2f__i8)) * OpenAD_lin_47 * OpenAD_lin_53))
      OpenAD_acc_9 = (OpenAD_lin_11 *(OpenAD_lin_42 + INT((-1_w2f__i8))
     >  * OpenAD_lin_44))
      OpenAD_acc_10 = (OpenAD_lin_11 *(OpenAD_lin_38 + INT((-1_w2f__i8)
     > ) * OpenAD_lin_40))
      OpenAD_acc_11 = (OpenAD_lin_11 *(OpenAD_lin_34 + INT((-1_w2f__i8)
     > ) * OpenAD_lin_36))
      OpenAD_acc_12 = (OpenAD_lin_13 * OpenAD_lin_12)
      OpenAD_acc_13 = (OpenAD_lin_14 * OpenAD_lin_12)
      OpenAD_acc_14 = (OpenAD_lin_23 * OpenAD_lin_21)
      OpenAD_acc_15 = (OpenAD_lin_25 * OpenAD_acc_1)
      OpenAD_acc_16 = (OpenAD_lin_26 * OpenAD_acc_1)
      OpenAD_acc_17 = (OpenAD_lin_29 * OpenAD_lin_27)
      OpenAD_acc_18 = (OpenAD_lin_31 * OpenAD_acc_0)
      OpenAD_acc_19 = (OpenAD_lin_32 * OpenAD_acc_0)
      CALL sax(OpenAD_lin_3, __deriv__(X03), __deriv__(OpenAD_prp_0))
      CALL saxpy(OpenAD_lin_1, __deriv__(X01), __deriv__(OpenAD_prp_0))
      CALL saxpy(OpenAD_lin_2, __deriv__(X02), __deriv__(OpenAD_prp_0))
      CALL sax(OpenAD_acc_12, __deriv__(X10), __deriv__(OpenAD_prp_1))
      CALL saxpy(OpenAD_acc_13, __deriv__(X05), __deriv__(OpenAD_prp_1)
     > )
      CALL sax(OpenAD_lin_17, __deriv__(X08), __deriv__(OpenAD_prp_2))
      CALL saxpy(OpenAD_lin_15, __deriv__(X06), __deriv__(OpenAD_prp_2)
     > )
      CALL saxpy(OpenAD_lin_16, __deriv__(X07), __deriv__(OpenAD_prp_2)
     > )
      CALL sax(OpenAD_lin_20, __deriv__(X13), __deriv__(OpenAD_prp_3))
      CALL saxpy(OpenAD_lin_18, __deriv__(X11), __deriv__(OpenAD_prp_3)
     > )
      CALL saxpy(OpenAD_lin_19, __deriv__(X12), __deriv__(OpenAD_prp_3)
     > )
      CALL sax(OpenAD_lin_0, __deriv__(OpenAD_prp_0), __deriv__(T01))
      CALL sax(OpenAD_lin_5, __deriv__(X01), __deriv__(T03))
      CALL saxpy(OpenAD_acc_2, __deriv__(T01), __deriv__(T03))
      CALL sax(OpenAD_lin_7, __deriv__(X02), __deriv__(T04))
      CALL saxpy(OpenAD_acc_3, __deriv__(T01), __deriv__(T04))
      CALL sax(OpenAD_lin_9, __deriv__(X03), __deriv__(T05))
      CALL saxpy(OpenAD_acc_4, __deriv__(T01), __deriv__(T05))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_2), __deriv__(T08))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_3), __deriv__(T09))
      CALL setderiv(__deriv__(T10), __deriv__(T08))
      CALL saxpy(OpenAD_lin_22, __deriv__(X05), __deriv__(T10))
      CALL saxpy(OpenAD_acc_14, __deriv__(X04), __deriv__(T10))
      CALL saxpy(OpenAD_acc_15, __deriv__(X14), __deriv__(T10))
      CALL saxpy(OpenAD_acc_16, __deriv__(X16), __deriv__(T10))
      CALL setderiv(__deriv__(T11), __deriv__(T09))
      CALL saxpy(OpenAD_lin_28, __deriv__(X10), __deriv__(T11))
      CALL saxpy(OpenAD_acc_17, __deriv__(X09), __deriv__(T11))
      CALL saxpy(OpenAD_acc_18, __deriv__(X14), __deriv__(T11))
      CALL saxpy(OpenAD_acc_19, __deriv__(X16), __deriv__(T11))
      CALL sax(OpenAD_lin_35, __deriv__(X11), __deriv__(T12))
      CALL saxpy(OpenAD_acc_11, __deriv__(OpenAD_prp_1), __deriv__(T12)
     > )
      CALL saxpy(OpenAD_lin_33, __deriv__(X06), __deriv__(T12))
      CALL sax(OpenAD_lin_39, __deriv__(X12), __deriv__(T13))
      CALL saxpy(OpenAD_lin_37, __deriv__(X07), __deriv__(T13))
      CALL saxpy(OpenAD_acc_10, __deriv__(OpenAD_prp_1), __deriv__(T13)
     > )
      CALL sax(OpenAD_lin_43, __deriv__(X13), __deriv__(T14))
      CALL saxpy(OpenAD_lin_41, __deriv__(X08), __deriv__(T14))
      CALL saxpy(OpenAD_acc_9, __deriv__(OpenAD_prp_1), __deriv__(T14))
      CALL sax(OpenAD_lin_51, __deriv__(T14), __deriv__(OpenAD_prp_4))
      CALL saxpy(OpenAD_lin_50, __deriv__(T13), __deriv__(OpenAD_prp_4)
     > )
      CALL saxpy(OpenAD_lin_49, __deriv__(T12), __deriv__(OpenAD_prp_4)
     > )
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_4), __deriv__(T18))
      CALL sax(OpenAD_lin_52, __deriv__(X15), __deriv__(T16))
      CALL saxpy(OpenAD_acc_5, __deriv__(OpenAD_prp_4), __deriv__(T16))
      CALL saxpy(OpenAD_acc_6, __deriv__(T11), __deriv__(T16))
      CALL saxpy(OpenAD_acc_7, __deriv__(T10), __deriv__(T16))
      CALL saxpy(OpenAD_acc_8, __deriv__(OpenAD_prp_1), __deriv__(T16))
      OpenAD_lin_54 = COS(__value__(T16))
      __value__(T16) = SIN(__value__(T16))
      OpenAD_lin_55 = __value__(T12)
      OpenAD_lin_56 = __value__(T03)
      OpenAD_lin_57 = __value__(T13)
      OpenAD_lin_58 = __value__(T04)
      OpenAD_lin_59 = __value__(T14)
      OpenAD_lin_60 = __value__(T05)
      __value__(T17) = (__value__(T03) * __value__(T12) + __value__(T04
     > ) * __value__(T13) + __value__(T05) * __value__(T14))
      OpenAD_lin_61 = __value__(T14)
      OpenAD_lin_62 = __value__(T04)
      OpenAD_lin_63 = __value__(T13)
      OpenAD_lin_64 = __value__(T05)
      __value__(T19) = (__value__(T04) * __value__(T14) - __value__(T05
     > ) * __value__(T13))
      OpenAD_lin_65 = __value__(T12)
      OpenAD_lin_66 = __value__(T05)
      OpenAD_lin_67 = __value__(T14)
      OpenAD_lin_68 = __value__(T03)
      __value__(T20) = (__value__(T05) * __value__(T12) - __value__(T03
     > ) * __value__(T14))
      OpenAD_lin_69 = __value__(T13)
      OpenAD_lin_70 = __value__(T03)
      OpenAD_lin_71 = __value__(T12)
      OpenAD_lin_72 = __value__(T04)
      __value__(T21) = (__value__(T03) * __value__(T13) - __value__(T04
     > ) * __value__(T12))
      __value__(T22) = (__value__(T17) - __value__(T16))
      __value__(T24) = __value__(T17)
      __value__(T23) = (__value__(T16) + __value__(T17))
      __value__(T25) = __value__(T22)
      __value__(T26) = __value__(T23)
      __value__(T27) = __value__(T24)
      OpenAD_acc_20 = (OpenAD_lin_63 * INT((-1_w2f__i8)))
      OpenAD_acc_21 = (OpenAD_lin_64 * INT((-1_w2f__i8)))
      OpenAD_acc_22 = (OpenAD_lin_67 * INT((-1_w2f__i8)))
      OpenAD_acc_23 = (OpenAD_lin_68 * INT((-1_w2f__i8)))
      OpenAD_acc_24 = (OpenAD_lin_71 * INT((-1_w2f__i8)))
      OpenAD_acc_25 = (OpenAD_lin_72 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_5), __deriv__(T16))
      CALL sax(OpenAD_lin_54, __deriv__(OpenAD_prp_5), __deriv__(T16))
      CALL sax(OpenAD_lin_61, __deriv__(T04), __deriv__(T19))
      CALL saxpy(OpenAD_lin_62, __deriv__(T14), __deriv__(T19))
      CALL saxpy(OpenAD_acc_20, __deriv__(T05), __deriv__(T19))
      CALL saxpy(OpenAD_acc_21, __deriv__(T13), __deriv__(T19))
      CALL sax(OpenAD_lin_65, __deriv__(T05), __deriv__(T20))
      CALL saxpy(OpenAD_lin_66, __deriv__(T12), __deriv__(T20))
      CALL saxpy(OpenAD_acc_22, __deriv__(T03), __deriv__(T20))
      CALL saxpy(OpenAD_acc_23, __deriv__(T14), __deriv__(T20))
      CALL sax(OpenAD_lin_69, __deriv__(T03), __deriv__(T21))
      CALL saxpy(OpenAD_lin_70, __deriv__(T13), __deriv__(T21))
      CALL saxpy(OpenAD_acc_24, __deriv__(T04), __deriv__(T21))
      CALL saxpy(OpenAD_acc_25, __deriv__(T12), __deriv__(T21))
      CALL sax(OpenAD_lin_55, __deriv__(T03), __deriv__(T17))
      CALL saxpy(OpenAD_lin_56, __deriv__(T12), __deriv__(T17))
      CALL saxpy(OpenAD_lin_57, __deriv__(T04), __deriv__(T17))
      CALL saxpy(OpenAD_lin_58, __deriv__(T13), __deriv__(T17))
      CALL saxpy(OpenAD_lin_59, __deriv__(T05), __deriv__(T17))
      CALL saxpy(OpenAD_lin_60, __deriv__(T14), __deriv__(T17))
      CALL setderiv(__deriv__(T22), __deriv__(T17))
      CALL dec_deriv(__deriv__(T22), __deriv__(T16))
      CALL setderiv(__deriv__(T24), __deriv__(T17))
      CALL setderiv(__deriv__(T23), __deriv__(T16))
      CALL inc_deriv(__deriv__(T23), __deriv__(T17))
      CALL setderiv(__deriv__(T25), __deriv__(T22))
      CALL setderiv(__deriv__(T26), __deriv__(T23))
      CALL setderiv(__deriv__(T27), __deriv__(T24))
      OpenAD_aux_11 = (__value__(T22) - __value__(T25))
      __value__(T25) = (OpenAD_aux_11 * 5.0D-01)
      OpenAD_aux_12 = (__value__(T23) - __value__(T26))
      __value__(T26) = (OpenAD_aux_12 * 5.0D-01)
      OpenAD_aux_13 = (__value__(T24) - __value__(T27))
      __value__(T27) = (OpenAD_aux_13 * 5.0D-01)
      OpenAD_lin_73 = __value__(X16)
      OpenAD_lin_74 = __value__(X04)
      OpenAD_lin_75 = __value__(T08)
      OpenAD_lin_76 = __value__(X05)
      __value__(T28) = (__value__(X04) * __value__(X16) + __value__(X05
     > ) * __value__(T08))
      OpenAD_lin_77 = __value__(X06)
      OpenAD_lin_78 = __value__(X05)
      __value__(T29) = (__value__(X05) * __value__(X06))
      OpenAD_lin_79 = __value__(X07)
      OpenAD_lin_80 = __value__(X05)
      __value__(T30) = (__value__(X05) * __value__(X07))
      OpenAD_lin_81 = __value__(X08)
      OpenAD_lin_82 = __value__(X05)
      __value__(T31) = (__value__(X05) * __value__(X08))
      OpenAD_lin_83 = __value__(X16)
      OpenAD_lin_84 = __value__(X09)
      OpenAD_lin_85 = __value__(T09)
      OpenAD_lin_86 = __value__(X10)
      __value__(T32) = (__value__(X09) * __value__(X16) + __value__(X10
     > ) * __value__(T09))
      OpenAD_lin_87 = __value__(X11)
      OpenAD_lin_88 = __value__(X10)
      __value__(T33) = (__value__(X10) * __value__(X11))
      OpenAD_lin_89 = __value__(X12)
      OpenAD_lin_90 = __value__(X10)
      __value__(T34) = (__value__(X10) * __value__(X12))
      OpenAD_lin_91 = __value__(X13)
      OpenAD_lin_92 = __value__(X10)
      __value__(T35) = (__value__(X10) * __value__(X13))
      __value__(T36) = (__value__(T32) - __value__(T28))
      __value__(T37) = (__value__(X10) - __value__(X05))
      __value__(T38) = (__value__(T33) - __value__(T29))
      __value__(T39) = (__value__(T34) - __value__(T30))
      __value__(T40) = (__value__(T35) - __value__(T31))
      OpenAD_lin_93 = __value__(T38)
      OpenAD_lin_94 = __value__(T12)
      OpenAD_lin_95 = __value__(T39)
      OpenAD_lin_96 = __value__(T13)
      OpenAD_lin_97 = __value__(T40)
      OpenAD_lin_98 = __value__(T14)
      __value__(T46) = (__value__(T12) * __value__(T38) + __value__(T13
     > ) * __value__(T39) + __value__(T14) * __value__(T40))
      OpenAD_aux_14 = (__value__(X15) / __value__(T16))
      OpenAD_aux_15 = (__value__(T36) + __value__(T18) * __value__(T37)
     >  - __value__(T46))
      OpenAD_lin_101 = (INT(1_w2f__i8) / __value__(T16))
      OpenAD_lin_102 = (-(__value__(X15) /(__value__(T16) * __value__(
     > T16))))
      OpenAD_lin_99 = OpenAD_aux_15
      OpenAD_lin_103 = __value__(T37)
      OpenAD_lin_104 = __value__(T18)
      OpenAD_lin_100 = OpenAD_aux_14
      __value__(T47) = (OpenAD_aux_14 * OpenAD_aux_15)
      OpenAD_lin_105 = __value__(T38)
      OpenAD_lin_106 = __value__(T03)
      OpenAD_lin_107 = __value__(T39)
      OpenAD_lin_108 = __value__(T04)
      OpenAD_lin_109 = __value__(T40)
      OpenAD_lin_110 = __value__(T05)
      __value__(T48) = (__value__(T03) * __value__(T38) + __value__(T04
     > ) * __value__(T39) + __value__(T05) * __value__(T40))
      OpenAD_aux_16 = (__value__(T47) + __value__(T17) * __value__(T37)
     >  - __value__(T48))
      OpenAD_lin_111 = __value__(T37)
      OpenAD_lin_112 = __value__(T17)
      __value__(T41) = (OpenAD_aux_16 * 5.0D-01)
      OpenAD_aux_17 = (__value__(T03) * __value__(T16) - __value__(T19)
     > )
      OpenAD_aux_18 = (- __value__(T03))
      OpenAD_lin_113 = __value__(T40)
      OpenAD_lin_114 = __value__(T04)
      OpenAD_lin_115 = OpenAD_aux_17
      OpenAD_lin_117 = __value__(T16)
      OpenAD_lin_118 = __value__(T03)
      OpenAD_lin_116 = __value__(T37)
      OpenAD_lin_119 = OpenAD_aux_18
      OpenAD_lin_120 = __value__(T47)
      OpenAD_lin_121 = __value__(T39)
      OpenAD_lin_122 = __value__(T05)
      __value__(T42) = (__value__(T04) * __value__(T40) + __value__(T37
     > ) * OpenAD_aux_17 + __value__(T47) * OpenAD_aux_18 - __value__(
     > T05) * __value__(T39))
      OpenAD_aux_19 = (__value__(T04) * __value__(T16) - __value__(T20)
     > )
      OpenAD_aux_20 = (- __value__(T04))
      OpenAD_lin_123 = __value__(T38)
      OpenAD_lin_124 = __value__(T05)
      OpenAD_lin_125 = OpenAD_aux_19
      OpenAD_lin_127 = __value__(T16)
      OpenAD_lin_128 = __value__(T04)
      OpenAD_lin_126 = __value__(T37)
      OpenAD_lin_129 = OpenAD_aux_20
      OpenAD_lin_130 = __value__(T47)
      OpenAD_lin_131 = __value__(T40)
      OpenAD_lin_132 = __value__(T03)
      __value__(T43) = (__value__(T05) * __value__(T38) + __value__(T37
     > ) * OpenAD_aux_19 + __value__(T47) * OpenAD_aux_20 - __value__(
     > T03) * __value__(T40))
      OpenAD_aux_21 = (__value__(T05) * __value__(T16) - __value__(T21)
     > )
      OpenAD_aux_22 = (- __value__(T05))
      OpenAD_lin_133 = __value__(T39)
      OpenAD_lin_134 = __value__(T03)
      OpenAD_lin_135 = OpenAD_aux_21
      OpenAD_lin_137 = __value__(T16)
      OpenAD_lin_138 = __value__(T05)
      OpenAD_lin_136 = __value__(T37)
      OpenAD_lin_139 = OpenAD_aux_22
      OpenAD_lin_140 = __value__(T47)
      OpenAD_lin_141 = __value__(T38)
      OpenAD_lin_142 = __value__(T04)
      __value__(T44) = (__value__(T03) * __value__(T39) + __value__(T37
     > ) * OpenAD_aux_21 + __value__(T47) * OpenAD_aux_22 - __value__(
     > T04) * __value__(T38))
      OpenAD_aux_23 = (__value__(T48) + __value__(T47) - __value__(T17)
     >  * __value__(T37))
      OpenAD_lin_143 = __value__(T37)
      OpenAD_lin_144 = __value__(T17)
      __value__(T45) = (OpenAD_aux_23 * 5.0D-01)
      OpenAD_acc_26 = (OpenAD_lin_101 * OpenAD_lin_99)
      OpenAD_acc_27 = (OpenAD_lin_102 * OpenAD_lin_99)
      OpenAD_acc_28 = (OpenAD_lin_143 *(-5.0D-01))
      OpenAD_acc_29 = (OpenAD_lin_111 * 5.0D-01)
      OpenAD_acc_30 = (INT((-1_w2f__i8)) * OpenAD_lin_100)
      OpenAD_acc_31 = (OpenAD_lin_97 * OpenAD_acc_30)
      OpenAD_acc_32 = (OpenAD_lin_98 * OpenAD_acc_30)
      OpenAD_acc_33 = (OpenAD_lin_138 * OpenAD_lin_136)
      OpenAD_acc_34 = (OpenAD_lin_137 * OpenAD_lin_136 + INT((
     > -1_w2f__i8)) * OpenAD_lin_140)
      OpenAD_acc_35 = (INT((-1_w2f__i8)) * OpenAD_lin_136)
      OpenAD_acc_36 = (OpenAD_lin_128 * OpenAD_lin_126)
      OpenAD_acc_37 = (OpenAD_lin_127 * OpenAD_lin_126 + INT((
     > -1_w2f__i8)) * OpenAD_lin_130)
      OpenAD_acc_38 = (INT((-1_w2f__i8)) * OpenAD_lin_126)
      OpenAD_acc_39 = (OpenAD_lin_118 * OpenAD_lin_116)
      OpenAD_acc_40 = (OpenAD_lin_117 * OpenAD_lin_116 + INT((
     > -1_w2f__i8)) * OpenAD_lin_120)
      OpenAD_acc_41 = (INT((-1_w2f__i8)) * OpenAD_lin_116)
      OpenAD_acc_42 = (OpenAD_lin_103 * OpenAD_lin_100)
      OpenAD_acc_43 = (OpenAD_lin_104 * OpenAD_lin_100)
      OpenAD_acc_44 = (OpenAD_lin_144 *(-5.0D-01) + OpenAD_acc_43 *
     >  5.0D-01)
      OpenAD_acc_45 = (OpenAD_lin_135 + OpenAD_acc_43 * OpenAD_lin_139)
      OpenAD_acc_46 = (OpenAD_lin_125 + OpenAD_acc_43 * OpenAD_lin_129)
      OpenAD_acc_47 = (OpenAD_lin_115 + OpenAD_acc_43 * OpenAD_lin_119)
      OpenAD_acc_48 = (OpenAD_lin_112 * 5.0D-01 + OpenAD_acc_43 *
     >  5.0D-01)
      OpenAD_acc_49 = (OpenAD_lin_95 * OpenAD_acc_30)
      OpenAD_acc_50 = (OpenAD_lin_96 * OpenAD_acc_30)
      OpenAD_acc_51 = (OpenAD_lin_93 * OpenAD_acc_30)
      OpenAD_acc_52 = (OpenAD_lin_94 * OpenAD_acc_30)
      OpenAD_acc_53 = (OpenAD_lin_141 * INT((-1_w2f__i8)))
      OpenAD_acc_54 = (OpenAD_lin_142 * INT((-1_w2f__i8)))
      OpenAD_acc_55 = (OpenAD_lin_131 * INT((-1_w2f__i8)))
      OpenAD_acc_56 = (OpenAD_lin_132 * INT((-1_w2f__i8)))
      OpenAD_acc_57 = (OpenAD_lin_121 * INT((-1_w2f__i8)))
      OpenAD_acc_58 = (OpenAD_lin_122 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_6), __deriv__(T25))
      CALL setderiv(__deriv__(OpenAD_prp_7), __deriv__(T26))
      CALL setderiv(__deriv__(OpenAD_prp_8), __deriv__(T27))
      CALL setderiv(__deriv__(OpenAD_prp_9), __deriv__(T22))
      CALL dec_deriv(__deriv__(OpenAD_prp_9), __deriv__(OpenAD_prp_6))
      CALL setderiv(__deriv__(OpenAD_prp_10), __deriv__(T23))
      CALL dec_deriv(__deriv__(OpenAD_prp_10), __deriv__(OpenAD_prp_7))
      CALL setderiv(__deriv__(OpenAD_prp_11), __deriv__(T24))
      CALL dec_deriv(__deriv__(OpenAD_prp_11), __deriv__(OpenAD_prp_8))
      CALL sax(OpenAD_lin_77, __deriv__(X05), __deriv__(T29))
      CALL saxpy(OpenAD_lin_78, __deriv__(X06), __deriv__(T29))
      CALL sax(OpenAD_lin_79, __deriv__(X05), __deriv__(T30))
      CALL saxpy(OpenAD_lin_80, __deriv__(X07), __deriv__(T30))
      CALL sax(OpenAD_lin_81, __deriv__(X05), __deriv__(T31))
      CALL saxpy(OpenAD_lin_82, __deriv__(X08), __deriv__(T31))
      CALL setderiv(__deriv__(T37), __deriv__(X10))
      CALL dec_deriv(__deriv__(T37), __deriv__(X05))
      CALL set_neg_deriv(__deriv__(T38), __deriv__(T29))
      CALL saxpy(OpenAD_lin_87, __deriv__(X10), __deriv__(T38))
      CALL saxpy(OpenAD_lin_88, __deriv__(X11), __deriv__(T38))
      CALL set_neg_deriv(__deriv__(T39), __deriv__(T30))
      CALL saxpy(OpenAD_lin_89, __deriv__(X10), __deriv__(T39))
      CALL saxpy(OpenAD_lin_90, __deriv__(X12), __deriv__(T39))
      CALL set_neg_deriv(__deriv__(T40), __deriv__(T31))
      CALL saxpy(OpenAD_lin_91, __deriv__(X10), __deriv__(T40))
      CALL saxpy(OpenAD_lin_92, __deriv__(X13), __deriv__(T40))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_9), __deriv__(T25))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_10), __deriv__(T26))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_11), __deriv__(T27))
      CALL sax(OpenAD_lin_73, __deriv__(X04), __deriv__(T28))
      CALL saxpy(OpenAD_lin_74, __deriv__(X16), __deriv__(T28))
      CALL saxpy(OpenAD_lin_75, __deriv__(X05), __deriv__(T28))
      CALL saxpy(OpenAD_lin_76, __deriv__(T08), __deriv__(T28))
      CALL set_neg_deriv(__deriv__(T36), __deriv__(T28))
      CALL saxpy(OpenAD_lin_83, __deriv__(X09), __deriv__(T36))
      CALL saxpy(OpenAD_lin_84, __deriv__(X16), __deriv__(T36))
      CALL saxpy(OpenAD_lin_85, __deriv__(X10), __deriv__(T36))
      CALL saxpy(OpenAD_lin_86, __deriv__(T09), __deriv__(T36))
      CALL sax(OpenAD_lin_109, __deriv__(T05), __deriv__(T48))
      CALL saxpy(OpenAD_lin_110, __deriv__(T40), __deriv__(T48))
      CALL saxpy(OpenAD_lin_107, __deriv__(T04), __deriv__(T48))
      CALL saxpy(OpenAD_lin_108, __deriv__(T39), __deriv__(T48))
      CALL saxpy(OpenAD_lin_105, __deriv__(T03), __deriv__(T48))
      CALL saxpy(OpenAD_lin_106, __deriv__(T38), __deriv__(T48))
      CALL sax(OpenAD_acc_26, __deriv__(X15), __deriv__(T47))
      CALL saxpy(OpenAD_acc_27, __deriv__(T16), __deriv__(T47))
      CALL saxpy(OpenAD_acc_31, __deriv__(T14), __deriv__(T47))
      CALL saxpy(OpenAD_acc_32, __deriv__(T40), __deriv__(T47))
      CALL saxpy(OpenAD_lin_100, __deriv__(T36), __deriv__(T47))
      CALL saxpy(OpenAD_acc_42, __deriv__(T18), __deriv__(T47))
      CALL saxpy(OpenAD_acc_49, __deriv__(T13), __deriv__(T47))
      CALL saxpy(OpenAD_acc_50, __deriv__(T39), __deriv__(T47))
      CALL saxpy(OpenAD_acc_51, __deriv__(T12), __deriv__(T47))
      CALL saxpy(OpenAD_acc_52, __deriv__(T38), __deriv__(T47))
      CALL sax(-5.0D-01, __deriv__(T48), __deriv__(T41))
      CALL saxpy(5.0D-01, __deriv__(T47), __deriv__(T41))
      CALL saxpy(OpenAD_acc_29, __deriv__(T17), __deriv__(T41))
      CALL saxpy(OpenAD_acc_48, __deriv__(T37), __deriv__(T41))
      CALL sax(5.0D-01, __deriv__(T48), __deriv__(T45))
      CALL saxpy(5.0D-01, __deriv__(T47), __deriv__(T45))
      CALL saxpy(OpenAD_acc_28, __deriv__(T17), __deriv__(T45))
      CALL saxpy(OpenAD_acc_44, __deriv__(T37), __deriv__(T45))
      CALL sax(OpenAD_lin_119, __deriv__(T47), __deriv__(T42))
      CALL saxpy(OpenAD_acc_47, __deriv__(T37), __deriv__(T42))
      CALL saxpy(OpenAD_acc_40, __deriv__(T03), __deriv__(T42))
      CALL saxpy(OpenAD_acc_39, __deriv__(T16), __deriv__(T42))
      CALL saxpy(OpenAD_acc_41, __deriv__(T19), __deriv__(T42))
      CALL saxpy(OpenAD_lin_113, __deriv__(T04), __deriv__(T42))
      CALL saxpy(OpenAD_lin_114, __deriv__(T40), __deriv__(T42))
      CALL saxpy(OpenAD_acc_57, __deriv__(T05), __deriv__(T42))
      CALL saxpy(OpenAD_acc_58, __deriv__(T39), __deriv__(T42))
      CALL sax(OpenAD_lin_129, __deriv__(T47), __deriv__(T43))
      CALL saxpy(OpenAD_acc_46, __deriv__(T37), __deriv__(T43))
      CALL saxpy(OpenAD_acc_37, __deriv__(T04), __deriv__(T43))
      CALL saxpy(OpenAD_acc_36, __deriv__(T16), __deriv__(T43))
      CALL saxpy(OpenAD_acc_38, __deriv__(T20), __deriv__(T43))
      CALL saxpy(OpenAD_lin_123, __deriv__(T05), __deriv__(T43))
      CALL saxpy(OpenAD_lin_124, __deriv__(T38), __deriv__(T43))
      CALL saxpy(OpenAD_acc_55, __deriv__(T03), __deriv__(T43))
      CALL saxpy(OpenAD_acc_56, __deriv__(T40), __deriv__(T43))
      CALL sax(OpenAD_lin_139, __deriv__(T47), __deriv__(T44))
      CALL saxpy(OpenAD_acc_45, __deriv__(T37), __deriv__(T44))
      CALL saxpy(OpenAD_acc_34, __deriv__(T05), __deriv__(T44))
      CALL saxpy(OpenAD_acc_33, __deriv__(T16), __deriv__(T44))
      CALL saxpy(OpenAD_acc_35, __deriv__(T21), __deriv__(T44))
      CALL saxpy(OpenAD_lin_133, __deriv__(T03), __deriv__(T44))
      CALL saxpy(OpenAD_lin_134, __deriv__(T39), __deriv__(T44))
      CALL saxpy(OpenAD_acc_53, __deriv__(T04), __deriv__(T44))
      CALL saxpy(OpenAD_acc_54, __deriv__(T38), __deriv__(T44))
      OpenAD_lin_145 = __value__(T41)
      OpenAD_lin_146 = __value__(T25)
      __value__(T41) = (__value__(T25) * __value__(T41))
      OpenAD_lin_147 = __value__(T42)
      OpenAD_lin_148 = __value__(T27)
      __value__(T42) = (__value__(T27) * __value__(T42))
      OpenAD_lin_149 = __value__(T43)
      OpenAD_lin_150 = __value__(T27)
      __value__(T43) = (__value__(T27) * __value__(T43))
      OpenAD_lin_151 = __value__(T44)
      OpenAD_lin_152 = __value__(T27)
      __value__(T44) = (__value__(T27) * __value__(T44))
      OpenAD_lin_153 = __value__(T45)
      OpenAD_lin_154 = __value__(T26)
      __value__(T45) = (__value__(T26) * __value__(T45))
      __value__(T49) = (__value__(T41) + __value__(T45))
      __value__(T50) = (__value__(T41) - __value__(T45))
      OpenAD_lin_155 = (INT(1_w2f__i8) / __value__(T16))
      OpenAD_lin_156 = (-(__value__(T49) /(__value__(T16) * __value__(
     > T16))))
      OpenAD_lin_157 = __value__(T42)
      OpenAD_lin_158 = __value__(T03)
      OpenAD_lin_159 = __value__(T43)
      OpenAD_lin_160 = __value__(T04)
      OpenAD_lin_161 = __value__(T44)
      OpenAD_lin_162 = __value__(T05)
      __value__(T56) = ((__value__(T49) / __value__(T16)) + __value__(
     > T03) * __value__(T42) + __value__(T04) * __value__(T43) +
     >  __value__(T05) * __value__(T44))
      OpenAD_aux_24 = (__value__(X16) * __value__(T16))
      OpenAD_lin_163 = __value__(T56)
      OpenAD_lin_164 = __value__(T18)
      OpenAD_lin_165 = OpenAD_aux_24
      OpenAD_lin_167 = __value__(T16)
      OpenAD_lin_168 = __value__(X16)
      OpenAD_lin_166 = __value__(T49)
      OpenAD_lin_169 = __value__(T50)
      OpenAD_lin_170 = __value__(T17)
      OpenAD_lin_171 = __value__(T42)
      OpenAD_lin_172 = __value__(T19)
      OpenAD_lin_173 = __value__(T43)
      OpenAD_lin_174 = __value__(T20)
      OpenAD_lin_175 = __value__(T44)
      OpenAD_lin_176 = __value__(T21)
      __value__(T51) = (__value__(T18) * __value__(T56) + __value__(T49
     > ) * OpenAD_aux_24 - __value__(T17) * __value__(T50) + __value__(
     > T19) * __value__(T42) + __value__(T20) * __value__(T43) +
     >  __value__(T21) * __value__(T44))
      __value__(T52) = __value__(T56)
      OpenAD_lin_177 = __value__(T43)
      OpenAD_lin_178 = __value__(T05)
      OpenAD_lin_179 = __value__(T56)
      OpenAD_lin_180 = __value__(T12)
      OpenAD_lin_181 = __value__(T50)
      OpenAD_lin_182 = __value__(T03)
      OpenAD_lin_183 = __value__(T44)
      OpenAD_lin_184 = __value__(T04)
      __value__(T53) = (__value__(T05) * __value__(T43) + __value__(T12
     > ) * __value__(T56) - __value__(T03) * __value__(T50) - __value__
     > (T04) * __value__(T44))
      OpenAD_lin_185 = __value__(T44)
      OpenAD_lin_186 = __value__(T03)
      OpenAD_lin_187 = __value__(T56)
      OpenAD_lin_188 = __value__(T13)
      OpenAD_lin_189 = __value__(T50)
      OpenAD_lin_190 = __value__(T04)
      OpenAD_lin_191 = __value__(T42)
      OpenAD_lin_192 = __value__(T05)
      __value__(T54) = (__value__(T03) * __value__(T44) + __value__(T13
     > ) * __value__(T56) - __value__(T04) * __value__(T50) - __value__
     > (T05) * __value__(T42))
      OpenAD_lin_193 = __value__(T42)
      OpenAD_lin_194 = __value__(T04)
      OpenAD_lin_195 = __value__(T56)
      OpenAD_lin_196 = __value__(T14)
      OpenAD_lin_197 = __value__(T50)
      OpenAD_lin_198 = __value__(T05)
      OpenAD_lin_199 = __value__(T43)
      OpenAD_lin_200 = __value__(T03)
      __value__(T55) = (__value__(T04) * __value__(T42) + __value__(T14
     > ) * __value__(T56) - __value__(T05) * __value__(T50) - __value__
     > (T03) * __value__(T43))
      OpenAD_lin_201 = __value__(T03)
      OpenAD_lin_202 = __value__(X06)
      OpenAD_lin_203 = __value__(T04)
      OpenAD_lin_204 = __value__(X07)
      OpenAD_lin_205 = __value__(T05)
      OpenAD_lin_206 = __value__(X08)
      __value__(T57) = (__value__(X06) * __value__(T03) + __value__(X07
     > ) * __value__(T04) + __value__(X08) * __value__(T05))
      OpenAD_aux_26 = (__value__(X04) + __value__(T28))
      OpenAD_aux_25 = (__value__(T51) + __value__(T57) * OpenAD_aux_26)
      OpenAD_lin_207 = OpenAD_aux_25
      OpenAD_lin_209 = OpenAD_aux_26
      OpenAD_lin_210 = __value__(T57)
      OpenAD_lin_208 = __value__(T01)
      __value__(Y1) = (__value__(T01) * OpenAD_aux_25)
      OpenAD_aux_27 = (__value__(T52) + __value__(X05) * __value__(T57)
     > )
      OpenAD_lin_211 = OpenAD_aux_27
      OpenAD_lin_213 = __value__(T57)
      OpenAD_lin_214 = __value__(X05)
      OpenAD_lin_212 = __value__(T01)
      __value__(Y2) = (__value__(T01) * OpenAD_aux_27)
      OpenAD_aux_28 = (__value__(T53) + __value__(X04) * __value__(T03)
     >  + __value__(T29) * __value__(T57))
      OpenAD_lin_215 = OpenAD_aux_28
      OpenAD_lin_217 = __value__(T03)
      OpenAD_lin_218 = __value__(X04)
      OpenAD_lin_219 = __value__(T57)
      OpenAD_lin_220 = __value__(T29)
      OpenAD_lin_216 = __value__(T01)
      __value__(Y3) = (__value__(T01) * OpenAD_aux_28)
      OpenAD_aux_29 = (__value__(T54) + __value__(X04) * __value__(T04)
     >  + __value__(T30) * __value__(T57))
      OpenAD_lin_221 = OpenAD_aux_29
      OpenAD_lin_223 = __value__(T04)
      OpenAD_lin_224 = __value__(X04)
      OpenAD_lin_225 = __value__(T57)
      OpenAD_lin_226 = __value__(T30)
      OpenAD_lin_222 = __value__(T01)
      __value__(Y4) = (__value__(T01) * OpenAD_aux_29)
      OpenAD_aux_30 = (__value__(T55) + __value__(X04) * __value__(T05)
     >  + __value__(T31) * __value__(T57))
      OpenAD_lin_227 = OpenAD_aux_30
      OpenAD_lin_229 = __value__(T05)
      OpenAD_lin_230 = __value__(X04)
      OpenAD_lin_231 = __value__(T57)
      OpenAD_lin_232 = __value__(T31)
      OpenAD_lin_228 = __value__(T01)
      __value__(Y5) = (__value__(T01) * OpenAD_aux_30)
      OpenAD_acc_59 = (OpenAD_lin_209 * OpenAD_lin_208)
      OpenAD_acc_60 = (OpenAD_lin_210 * OpenAD_lin_208)
      OpenAD_acc_61 = (OpenAD_lin_175 * OpenAD_lin_208)
      OpenAD_acc_62 = (OpenAD_lin_176 * OpenAD_lin_208)
      OpenAD_acc_63 = (INT((-1_w2f__i8)) * OpenAD_lin_228)
      OpenAD_acc_64 = (OpenAD_lin_199 * OpenAD_acc_63)
      OpenAD_acc_65 = (OpenAD_lin_200 * OpenAD_acc_63)
      OpenAD_acc_66 = (INT((-1_w2f__i8)) * OpenAD_lin_222)
      OpenAD_acc_67 = (OpenAD_lin_191 * OpenAD_acc_66)
      OpenAD_acc_68 = (OpenAD_lin_192 * OpenAD_acc_66)
      OpenAD_acc_69 = (INT((-1_w2f__i8)) * OpenAD_lin_216)
      OpenAD_acc_70 = (OpenAD_lin_183 * OpenAD_acc_69)
      OpenAD_acc_71 = (OpenAD_lin_184 * OpenAD_acc_69)
      OpenAD_acc_72 = (OpenAD_lin_173 * OpenAD_lin_208)
      OpenAD_acc_73 = (OpenAD_lin_174 * OpenAD_lin_208)
      OpenAD_acc_74 = (OpenAD_lin_193 * OpenAD_lin_228)
      OpenAD_acc_75 = (OpenAD_lin_194 * OpenAD_lin_228)
      OpenAD_acc_76 = (OpenAD_lin_185 * OpenAD_lin_222)
      OpenAD_acc_77 = (OpenAD_lin_186 * OpenAD_lin_222)
      OpenAD_acc_78 = (OpenAD_lin_177 * OpenAD_lin_216)
      OpenAD_acc_79 = (OpenAD_lin_178 * OpenAD_lin_216)
      OpenAD_acc_80 = (OpenAD_lin_171 * OpenAD_lin_208)
      OpenAD_acc_81 = (OpenAD_lin_172 * OpenAD_lin_208)
      OpenAD_acc_82 = (OpenAD_lin_195 * OpenAD_lin_228)
      OpenAD_acc_83 = (OpenAD_lin_196 * OpenAD_lin_228)
      OpenAD_acc_84 = (INT((-1_w2f__i8)) * OpenAD_lin_228)
      OpenAD_acc_85 = (OpenAD_lin_198 * OpenAD_acc_84)
      OpenAD_acc_86 = (OpenAD_lin_187 * OpenAD_lin_222)
      OpenAD_acc_87 = (OpenAD_lin_188 * OpenAD_lin_222)
      OpenAD_acc_88 = (INT((-1_w2f__i8)) * OpenAD_lin_222)
      OpenAD_acc_89 = (OpenAD_lin_190 * OpenAD_acc_88)
      OpenAD_acc_90 = (OpenAD_lin_179 * OpenAD_lin_216)
      OpenAD_acc_91 = (OpenAD_lin_180 * OpenAD_lin_216)
      OpenAD_acc_92 = (INT((-1_w2f__i8)) * OpenAD_lin_216)
      OpenAD_acc_93 = (OpenAD_lin_182 * OpenAD_acc_92)
      OpenAD_acc_94 = (OpenAD_lin_163 * OpenAD_lin_208)
      OpenAD_acc_95 = (OpenAD_lin_164 * OpenAD_lin_208)
      OpenAD_acc_96 = (OpenAD_lin_231 * OpenAD_lin_228)
      OpenAD_acc_97 = (OpenAD_lin_232 * OpenAD_lin_228)
      OpenAD_acc_98 = (OpenAD_lin_197 * OpenAD_acc_84 + OpenAD_lin_230
     >  * OpenAD_lin_228)
      OpenAD_acc_99 = (OpenAD_lin_229 * OpenAD_lin_228)
      OpenAD_acc_100 = (OpenAD_lin_225 * OpenAD_lin_222)
      OpenAD_acc_101 = (OpenAD_lin_226 * OpenAD_lin_222)
      OpenAD_acc_102 = (OpenAD_lin_189 * OpenAD_acc_88 + OpenAD_lin_224
     >  * OpenAD_lin_222)
      OpenAD_acc_103 = (OpenAD_lin_223 * OpenAD_lin_222)
      OpenAD_acc_104 = (OpenAD_lin_219 * OpenAD_lin_216)
      OpenAD_acc_105 = (OpenAD_lin_220 * OpenAD_lin_216)
      OpenAD_acc_106 = (OpenAD_lin_181 * OpenAD_acc_92 + OpenAD_lin_218
     >  * OpenAD_lin_216)
      OpenAD_acc_107 = (OpenAD_lin_217 * OpenAD_lin_216)
      OpenAD_acc_108 = (OpenAD_lin_213 * OpenAD_lin_212)
      OpenAD_acc_109 = (OpenAD_lin_214 * OpenAD_lin_212)
      OpenAD_acc_110 = (OpenAD_lin_165 * OpenAD_lin_208)
      OpenAD_acc_111 = (OpenAD_lin_167 * OpenAD_lin_166 *
     >  OpenAD_lin_208)
      OpenAD_acc_112 = (OpenAD_lin_168 * OpenAD_lin_166 *
     >  OpenAD_lin_208)
      OpenAD_acc_113 = (INT((-1_w2f__i8)) * OpenAD_lin_208)
      OpenAD_acc_114 = (OpenAD_lin_169 * OpenAD_acc_113)
      OpenAD_acc_115 = (OpenAD_lin_170 * OpenAD_acc_113)
      CALL setderiv(__deriv__(OpenAD_prp_12), __deriv__(T41))
      CALL setderiv(__deriv__(OpenAD_prp_13), __deriv__(T42))
      CALL setderiv(__deriv__(OpenAD_prp_14), __deriv__(T43))
      CALL setderiv(__deriv__(OpenAD_prp_15), __deriv__(T44))
      CALL setderiv(__deriv__(OpenAD_prp_16), __deriv__(T45))
      CALL sax(OpenAD_lin_145, __deriv__(T25), __deriv__(T41))
      CALL saxpy(OpenAD_lin_146, __deriv__(OpenAD_prp_12), __deriv__(
     > T41))
      CALL sax(OpenAD_lin_147, __deriv__(T27), __deriv__(T42))
      CALL saxpy(OpenAD_lin_148, __deriv__(OpenAD_prp_13), __deriv__(
     > T42))
      CALL sax(OpenAD_lin_149, __deriv__(T27), __deriv__(T43))
      CALL saxpy(OpenAD_lin_150, __deriv__(OpenAD_prp_14), __deriv__(
     > T43))
      CALL sax(OpenAD_lin_151, __deriv__(T27), __deriv__(T44))
      CALL saxpy(OpenAD_lin_152, __deriv__(OpenAD_prp_15), __deriv__(
     > T44))
      CALL sax(OpenAD_lin_153, __deriv__(T26), __deriv__(T45))
      CALL saxpy(OpenAD_lin_154, __deriv__(OpenAD_prp_16), __deriv__(
     > T45))
      CALL setderiv(__deriv__(T49), __deriv__(T41))
      CALL inc_deriv(__deriv__(T49), __deriv__(T45))
      CALL setderiv(__deriv__(T50), __deriv__(T41))
      CALL dec_deriv(__deriv__(T50), __deriv__(T45))
      CALL setderiv(__deriv__(OpenAD_prp_17), __deriv__(X04))
      CALL inc_deriv(__deriv__(OpenAD_prp_17), __deriv__(T28))
      CALL sax(OpenAD_lin_201, __deriv__(X06), __deriv__(T57))
      CALL saxpy(OpenAD_lin_202, __deriv__(T03), __deriv__(T57))
      CALL saxpy(OpenAD_lin_203, __deriv__(X07), __deriv__(T57))
      CALL saxpy(OpenAD_lin_204, __deriv__(T04), __deriv__(T57))
      CALL saxpy(OpenAD_lin_205, __deriv__(X08), __deriv__(T57))
      CALL saxpy(OpenAD_lin_206, __deriv__(T05), __deriv__(T57))
      CALL sax(OpenAD_lin_155, __deriv__(T49), __deriv__(T56))
      CALL saxpy(OpenAD_lin_156, __deriv__(T16), __deriv__(T56))
      CALL saxpy(OpenAD_lin_157, __deriv__(T03), __deriv__(T56))
      CALL saxpy(OpenAD_lin_158, __deriv__(T42), __deriv__(T56))
      CALL saxpy(OpenAD_lin_159, __deriv__(T04), __deriv__(T56))
      CALL saxpy(OpenAD_lin_160, __deriv__(T43), __deriv__(T56))
      CALL saxpy(OpenAD_lin_161, __deriv__(T05), __deriv__(T56))
      CALL saxpy(OpenAD_lin_162, __deriv__(T44), __deriv__(T56))
      CALL sax(OpenAD_lin_211, __deriv__(T01), __deriv__(Y2))
      CALL saxpy(OpenAD_acc_108, __deriv__(X05), __deriv__(Y2))
      CALL saxpy(OpenAD_acc_109, __deriv__(T57), __deriv__(Y2))
      CALL saxpy(OpenAD_lin_212, __deriv__(T56), __deriv__(Y2))
      CALL sax(OpenAD_lin_215, __deriv__(T01), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_70, __deriv__(T04), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_71, __deriv__(T44), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_78, __deriv__(T05), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_79, __deriv__(T43), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_90, __deriv__(T12), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_91, __deriv__(T56), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_106, __deriv__(T03), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_93, __deriv__(T50), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_104, __deriv__(T29), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_105, __deriv__(T57), __deriv__(Y3))
      CALL saxpy(OpenAD_acc_107, __deriv__(X04), __deriv__(Y3))
      CALL sax(OpenAD_lin_221, __deriv__(T01), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_67, __deriv__(T05), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_68, __deriv__(T42), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_76, __deriv__(T03), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_77, __deriv__(T44), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_86, __deriv__(T13), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_87, __deriv__(T56), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_102, __deriv__(T04), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_89, __deriv__(T50), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_100, __deriv__(T30), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_101, __deriv__(T57), __deriv__(Y4))
      CALL saxpy(OpenAD_acc_103, __deriv__(X04), __deriv__(Y4))
      CALL sax(OpenAD_lin_227, __deriv__(T01), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_64, __deriv__(T03), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_65, __deriv__(T43), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_74, __deriv__(T04), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_75, __deriv__(T42), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_82, __deriv__(T14), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_83, __deriv__(T56), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_98, __deriv__(T05), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_85, __deriv__(T50), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_96, __deriv__(T31), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_97, __deriv__(T57), __deriv__(Y5))
      CALL saxpy(OpenAD_acc_99, __deriv__(X04), __deriv__(Y5))
      CALL sax(OpenAD_lin_207, __deriv__(T01), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_59, __deriv__(T57), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_60, __deriv__(OpenAD_prp_17), __deriv__(Y1)
     > )
      CALL saxpy(OpenAD_acc_61, __deriv__(T21), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_62, __deriv__(T44), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_72, __deriv__(T20), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_73, __deriv__(T43), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_80, __deriv__(T19), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_81, __deriv__(T42), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_94, __deriv__(T18), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_95, __deriv__(T56), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_110, __deriv__(T49), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_111, __deriv__(X16), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_112, __deriv__(T16), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_114, __deriv__(T17), __deriv__(Y1))
      CALL saxpy(OpenAD_acc_115, __deriv__(T50), __deriv__(Y1))
      END SUBROUTINE
