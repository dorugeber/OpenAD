
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
      SUBROUTINE ad_roehf5(NRM, PRIML, PRIMR, GAMMA, GM1, GM1INV,
     >  NLEFIX, LEFIX, MCHEPS, FLUX)
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      TYPE (OpenADTy_active) NRM(1 : 3)
      INTENT(IN)  NRM
      TYPE (OpenADTy_active) PRIML(1 : 5)
      INTENT(IN)  PRIML
      TYPE (OpenADTy_active) PRIMR(1 : 5)
      INTENT(IN)  PRIMR
      TYPE (OpenADTy_active) GAMMA
      INTENT(IN)  GAMMA
      TYPE (OpenADTy_active) GM1
      INTENT(IN)  GM1
      TYPE (OpenADTy_active) GM1INV
      INTENT(IN)  GM1INV
      TYPE (OpenADTy_active) NLEFIX
      INTENT(IN)  NLEFIX
      TYPE (OpenADTy_active) LEFIX
      INTENT(IN)  LEFIX
      REAL(w2f__8) MCHEPS
      INTENT(IN)  MCHEPS
      TYPE (OpenADTy_active) FLUX(1 : 5)
      INTENT(OUT)  FLUX
C
C     **** Local Variables and Functions ****
C
      TYPE (OpenADTy_active) ALAMCM
      TYPE (OpenADTy_active) ALAMCP
      TYPE (OpenADTy_active) ALAMU
      TYPE (OpenADTy_active) ALP1
      TYPE (OpenADTy_active) ALP15M
      TYPE (OpenADTy_active) ALP15P
      TYPE (OpenADTy_active) ALP2
      TYPE (OpenADTy_active) ALP3
      TYPE (OpenADTy_active) ALP4
      TYPE (OpenADTy_active) ALP5
      TYPE (OpenADTy_active) CAVE
      TYPE (OpenADTy_active) DE
      TYPE (OpenADTy_active) DELTA
      TYPE (OpenADTy_active) DELTA2
      TYPE (OpenADTy_active) DR
      TYPE (OpenADTy_active) DRU
      TYPE (OpenADTy_active) DRV
      TYPE (OpenADTy_active) DRW
      TYPE (OpenADTy_active) DSS1
      TYPE (OpenADTy_active) DSS2
      TYPE (OpenADTy_active) DSS3
      TYPE (OpenADTy_active) DSS4
      TYPE (OpenADTy_active) DSS5
      TYPE (OpenADTy_active) EL
      TYPE (OpenADTy_active) ER
      REAL(w2f__4) HALF
      PARAMETER ( HALF = 5.0E-01)
      TYPE (OpenADTy_active) HAVE
      TYPE (OpenADTy_active) HL
      TYPE (OpenADTy_active) HR
      TYPE (OpenADTy_active) LAM2
      TYPE (OpenADTy_active) LAMCM
      TYPE (OpenADTy_active) LAMCP
      TYPE (OpenADTy_active) LAMU
      TYPE (OpenADTy_active) MU
      TYPE (OpenADTy_active) NDDRU
      TYPE (OpenADTy_active) NSIZE
      TYPE (OpenADTy_active) NSIZEI
      TYPE (OpenADTy_active) NXHAT
      TYPE (OpenADTy_active) NYHAT
      TYPE (OpenADTy_active) NZHAT
      TYPE (OpenADTy_active) OMEGA
      REAL(w2f__4) ONE
      PARAMETER ( ONE = 1.0)
      TYPE (OpenADTy_active) ROEL
      TYPE (OpenADTy_active) ROER
      TYPE (OpenADTy_active) RUL
      TYPE (OpenADTy_active) RUR
      TYPE (OpenADTy_active) RVL
      TYPE (OpenADTy_active) RVR
      TYPE (OpenADTy_active) RWL
      TYPE (OpenADTy_active) RWR
      TYPE (OpenADTy_active) THETAL
      TYPE (OpenADTy_active) THETAR
      TYPE (OpenADTy_active) THTAVE
      TYPE (OpenADTy_active) UAVE
      TYPE (OpenADTy_active) UDDRU
      TYPE (OpenADTy_active) UHAT
      TYPE (OpenADTy_active) UHATL
      TYPE (OpenADTy_active) UTILDE
      TYPE (OpenADTy_active) VAVE
      TYPE (OpenADTy_active) VTILDE
      TYPE (OpenADTy_active) WAVE
      TYPE (OpenADTy_active) WTILDE
      REAL(w2f__4) ZERO
      PARAMETER ( ZERO = 0.0)
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
      REAL(w2f__8) OpenAD_aux_30
      REAL(w2f__8) OpenAD_aux_31
      REAL(w2f__8) OpenAD_aux_32
      REAL(w2f__8) OpenAD_aux_33
      REAL(w2f__8) OpenAD_aux_34
      REAL(w2f__8) OpenAD_aux_35
      REAL(w2f__8) OpenAD_aux_36
      REAL(w2f__8) OpenAD_aux_37
      REAL(w2f__8) OpenAD_aux_38
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
      REAL(w2f__8) OpenAD_lin_233
      REAL(w2f__8) OpenAD_lin_234
      REAL(w2f__8) OpenAD_lin_235
      REAL(w2f__8) OpenAD_lin_236
      REAL(w2f__8) OpenAD_lin_237
      REAL(w2f__8) OpenAD_lin_238
      REAL(w2f__8) OpenAD_lin_239
      REAL(w2f__8) OpenAD_lin_24
      REAL(w2f__8) OpenAD_lin_240
      REAL(w2f__8) OpenAD_lin_241
      REAL(w2f__8) OpenAD_lin_242
      REAL(w2f__8) OpenAD_lin_243
      REAL(w2f__8) OpenAD_lin_244
      REAL(w2f__8) OpenAD_lin_245
      REAL(w2f__8) OpenAD_lin_246
      REAL(w2f__8) OpenAD_lin_247
      REAL(w2f__8) OpenAD_lin_248
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
      TYPE (OpenADTy_active) OpenAD_prp_4
      TYPE (OpenADTy_active) OpenAD_prp_5
      TYPE (OpenADTy_active) OpenAD_prp_6
      TYPE (OpenADTy_active) OpenAD_prp_7
      TYPE (OpenADTy_active) OpenAD_prp_8
      TYPE (OpenADTy_active) OpenAD_prp_9
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(NRM)
C$OPENAD INDEPENDENT(PRIML)
C$OPENAD INDEPENDENT(PRIMR)
C$OPENAD INDEPENDENT(GAMMA)
C$OPENAD INDEPENDENT(GM1)
C$OPENAD INDEPENDENT(GM1INV)
C$OPENAD INDEPENDENT(NLEFIX)
C$OPENAD INDEPENDENT(LEFIX)
C$OPENAD INDEPENDENT(MCHEPS)
C$OPENAD DEPENDENT(FLUX)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = ((__value__(NRM(1)) ** 2) +(__value__(NRM(2)) ** 2
     > ) +(__value__(NRM(3)) ** 2))
      OpenAD_lin_1 = (2 *(__value__(NRM(1)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_2 = (2 *(__value__(NRM(2)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_3 = (2 *(__value__(NRM(3)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_0 = COS(OpenAD_aux_0)
      __value__(NSIZE) = SIN(OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_3 * OpenAD_lin_0)
      OpenAD_acc_1 = (OpenAD_lin_2 * OpenAD_lin_0)
      OpenAD_acc_2 = (OpenAD_lin_1 * OpenAD_lin_0)
      CALL sax(OpenAD_acc_0, __deriv__(NRM(3)), __deriv__(NSIZE))
      CALL saxpy(OpenAD_acc_1, __deriv__(NRM(2)), __deriv__(NSIZE))
      CALL saxpy(OpenAD_acc_2, __deriv__(NRM(1)), __deriv__(NSIZE))
      IF(MCHEPS .LT. __value__(NSIZE)) THEN
        OpenAD_lin_4 = (-(1.0D00 /(__value__(NSIZE) * __value__(NSIZE))
     > ))
        __value__(NSIZEI) = (1.0D00 / __value__(NSIZE))
        CALL sax(OpenAD_lin_4, __deriv__(NSIZE), __deriv__(NSIZEI))
      ELSE
        __value__(NSIZEI) = 0.0D00
        CALL zero_deriv(__deriv__(NSIZEI))
      ENDIF
      OpenAD_lin_5 = __value__(NSIZEI)
      OpenAD_lin_6 = __value__(NRM(1))
      __value__(NXHAT) = (__value__(NRM(1)) * __value__(NSIZEI))
      OpenAD_lin_7 = __value__(NSIZEI)
      OpenAD_lin_8 = __value__(NRM(2))
      __value__(NYHAT) = (__value__(NRM(2)) * __value__(NSIZEI))
      OpenAD_lin_9 = __value__(NSIZEI)
      OpenAD_lin_10 = __value__(NRM(3))
      __value__(NZHAT) = (__value__(NRM(3)) * __value__(NSIZEI))
      OpenAD_aux_2 = (__value__(PRIMR(2)) / __value__(PRIML(2)))
      OpenAD_aux_1 = (SIN(OpenAD_aux_2) + 1.0D00)
      OpenAD_lin_13 = (INT(1_w2f__i8) / __value__(PRIML(2)))
      OpenAD_lin_14 = (-(__value__(PRIMR(2)) /(__value__(PRIML(2)) *
     >  __value__(PRIML(2)))))
      OpenAD_lin_12 = COS(OpenAD_aux_2)
      OpenAD_lin_11 = (-(1.0D00 /(OpenAD_aux_1 * OpenAD_aux_1)))
      __value__(ROEL) = (1.0D00 / OpenAD_aux_1)
      __value__(ROER) = (1.0D00 - __value__(ROEL))
      OpenAD_aux_3 = ((__value__(PRIML(3)) ** 2) +(__value__(PRIML(4))
     >  ** 2) +(__value__(PRIML(5)) ** 2))
      OpenAD_lin_15 = (2 *(__value__(PRIML(3)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_16 = (2 *(__value__(PRIML(4)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_17 = (2 *(__value__(PRIML(5)) **(2 - INT(1_w2f__i8))))
      __value__(THETAL) = (OpenAD_aux_3 * 5.0D-01)
      OpenAD_aux_4 = ((__value__(PRIMR(3)) ** 2) +(__value__(PRIMR(4))
     >  ** 2) +(__value__(PRIMR(5)) ** 2))
      OpenAD_lin_18 = (2 *(__value__(PRIMR(3)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_19 = (2 *(__value__(PRIMR(4)) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_20 = (2 *(__value__(PRIMR(5)) **(2 - INT(1_w2f__i8))))
      __value__(THETAR) = (OpenAD_aux_4 * 5.0D-01)
      OpenAD_aux_6 = (__value__(GAMMA) * __value__(GM1INV))
      OpenAD_aux_5 = (__value__(PRIML(1)) * OpenAD_aux_6)
      OpenAD_lin_23 = OpenAD_aux_6
      OpenAD_lin_25 = __value__(GM1INV)
      OpenAD_lin_26 = __value__(GAMMA)
      OpenAD_lin_24 = __value__(PRIML(1))
      OpenAD_lin_21 = (INT(1_w2f__i8) / __value__(PRIML(2)))
      OpenAD_lin_22 = (-(OpenAD_aux_5 /(__value__(PRIML(2)) * __value__
     > (PRIML(2)))))
      __value__(HL) = (__value__(THETAL) +(OpenAD_aux_5 / __value__(
     > PRIML(2))))
      OpenAD_aux_8 = (__value__(GAMMA) * __value__(GM1INV))
      OpenAD_aux_7 = (__value__(PRIMR(1)) * OpenAD_aux_8)
      OpenAD_lin_29 = OpenAD_aux_8
      OpenAD_lin_31 = __value__(GM1INV)
      OpenAD_lin_32 = __value__(GAMMA)
      OpenAD_lin_30 = __value__(PRIMR(1))
      OpenAD_lin_27 = (INT(1_w2f__i8) / __value__(PRIMR(2)))
      OpenAD_lin_28 = (-(OpenAD_aux_7 /(__value__(PRIMR(2)) * __value__
     > (PRIMR(2)))))
      __value__(HR) = (__value__(THETAR) +(OpenAD_aux_7 / __value__(
     > PRIMR(2))))
      OpenAD_lin_33 = __value__(ROEL)
      OpenAD_lin_34 = __value__(PRIML(3))
      OpenAD_lin_35 = __value__(ROER)
      OpenAD_lin_36 = __value__(PRIMR(3))
      __value__(UAVE) = (__value__(PRIML(3)) * __value__(ROEL) +
     >  __value__(PRIMR(3)) * __value__(ROER))
      OpenAD_lin_37 = __value__(ROEL)
      OpenAD_lin_38 = __value__(PRIML(4))
      OpenAD_lin_39 = __value__(ROER)
      OpenAD_lin_40 = __value__(PRIMR(4))
      __value__(VAVE) = (__value__(PRIML(4)) * __value__(ROEL) +
     >  __value__(PRIMR(4)) * __value__(ROER))
      OpenAD_lin_41 = __value__(ROEL)
      OpenAD_lin_42 = __value__(PRIML(5))
      OpenAD_lin_43 = __value__(ROER)
      OpenAD_lin_44 = __value__(PRIMR(5))
      __value__(WAVE) = (__value__(PRIML(5)) * __value__(ROEL) +
     >  __value__(PRIMR(5)) * __value__(ROER))
      OpenAD_lin_45 = __value__(ROEL)
      OpenAD_lin_46 = __value__(HL)
      OpenAD_lin_47 = __value__(ROER)
      OpenAD_lin_48 = __value__(HR)
      __value__(HAVE) = (__value__(HL) * __value__(ROEL) + __value__(HR
     > ) * __value__(ROER))
      OpenAD_aux_9 = ((__value__(UAVE) ** 2) +(__value__(VAVE) ** 2) +(
     > __value__(WAVE) ** 2))
      OpenAD_lin_49 = (2 *(__value__(UAVE) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_50 = (2 *(__value__(VAVE) **(2 - INT(1_w2f__i8))))
      OpenAD_lin_51 = (2 *(__value__(WAVE) **(2 - INT(1_w2f__i8))))
      __value__(THTAVE) = (OpenAD_aux_9 * 5.0D-01)
      OpenAD_aux_10 = (__value__(HAVE) - __value__(THTAVE))
      OpenAD_lin_52 = OpenAD_aux_10
      OpenAD_lin_53 = __value__(GM1)
      __value__(CAVE) = (__value__(GM1) * OpenAD_aux_10)
      OpenAD_acc_3 = (OpenAD_lin_30 * OpenAD_lin_27)
      OpenAD_acc_4 = (OpenAD_lin_24 * OpenAD_lin_21)
      OpenAD_acc_5 = (5.0D-01 * INT((-1_w2f__i8)) * OpenAD_lin_53)
      OpenAD_acc_6 = (OpenAD_lin_47 * OpenAD_lin_53)
      OpenAD_acc_7 = (OpenAD_lin_45 * OpenAD_lin_53)
      OpenAD_acc_8 = (OpenAD_lin_11 *(OpenAD_lin_46 * OpenAD_lin_53 +
     >  INT((-1_w2f__i8)) * OpenAD_lin_48 * OpenAD_lin_53))
      OpenAD_acc_9 = (OpenAD_lin_11 *(OpenAD_lin_42 + INT((-1_w2f__i8))
     >  * OpenAD_lin_44))
      OpenAD_acc_10 = (OpenAD_lin_11 *(OpenAD_lin_38 + INT((-1_w2f__i8)
     > ) * OpenAD_lin_40))
      OpenAD_acc_11 = (OpenAD_lin_11 *(OpenAD_lin_34 + INT((-1_w2f__i8)
     > ) * OpenAD_lin_36))
      OpenAD_acc_12 = (OpenAD_lin_13 * OpenAD_lin_12)
      OpenAD_acc_13 = (OpenAD_lin_14 * OpenAD_lin_12)
      OpenAD_acc_14 = (OpenAD_lin_23 * OpenAD_lin_21)
      OpenAD_acc_15 = (OpenAD_lin_25 * OpenAD_acc_4)
      OpenAD_acc_16 = (OpenAD_lin_26 * OpenAD_acc_4)
      OpenAD_acc_17 = (OpenAD_lin_29 * OpenAD_lin_27)
      OpenAD_acc_18 = (OpenAD_lin_31 * OpenAD_acc_3)
      OpenAD_acc_19 = (OpenAD_lin_32 * OpenAD_acc_3)
      CALL sax(OpenAD_lin_5, __deriv__(NRM(1)), __deriv__(NXHAT))
      CALL saxpy(OpenAD_lin_6, __deriv__(NSIZEI), __deriv__(NXHAT))
      CALL sax(OpenAD_lin_7, __deriv__(NRM(2)), __deriv__(NYHAT))
      CALL saxpy(OpenAD_lin_8, __deriv__(NSIZEI), __deriv__(NYHAT))
      CALL sax(OpenAD_lin_9, __deriv__(NRM(3)), __deriv__(NZHAT))
      CALL saxpy(OpenAD_lin_10, __deriv__(NSIZEI), __deriv__(NZHAT))
      CALL sax(OpenAD_acc_12, __deriv__(PRIMR(2)), __deriv__(
     > OpenAD_prp_0))
      CALL saxpy(OpenAD_acc_13, __deriv__(PRIML(2)), __deriv__(
     > OpenAD_prp_0))
      CALL sax(OpenAD_lin_17, __deriv__(PRIML(5)), __deriv__(
     > OpenAD_prp_1))
      CALL saxpy(OpenAD_lin_15, __deriv__(PRIML(3)), __deriv__(
     > OpenAD_prp_1))
      CALL saxpy(OpenAD_lin_16, __deriv__(PRIML(4)), __deriv__(
     > OpenAD_prp_1))
      CALL sax(OpenAD_lin_20, __deriv__(PRIMR(5)), __deriv__(
     > OpenAD_prp_2))
      CALL saxpy(OpenAD_lin_18, __deriv__(PRIMR(3)), __deriv__(
     > OpenAD_prp_2))
      CALL saxpy(OpenAD_lin_19, __deriv__(PRIMR(4)), __deriv__(
     > OpenAD_prp_2))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_1), __deriv__(THETAL))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_2), __deriv__(THETAR))
      CALL setderiv(__deriv__(HL), __deriv__(THETAL))
      CALL saxpy(OpenAD_lin_22, __deriv__(PRIML(2)), __deriv__(HL))
      CALL saxpy(OpenAD_acc_14, __deriv__(PRIML(1)), __deriv__(HL))
      CALL saxpy(OpenAD_acc_15, __deriv__(GAMMA), __deriv__(HL))
      CALL saxpy(OpenAD_acc_16, __deriv__(GM1INV), __deriv__(HL))
      CALL setderiv(__deriv__(HR), __deriv__(THETAR))
      CALL saxpy(OpenAD_lin_28, __deriv__(PRIMR(2)), __deriv__(HR))
      CALL saxpy(OpenAD_acc_17, __deriv__(PRIMR(1)), __deriv__(HR))
      CALL saxpy(OpenAD_acc_18, __deriv__(GAMMA), __deriv__(HR))
      CALL saxpy(OpenAD_acc_19, __deriv__(GM1INV), __deriv__(HR))
      CALL sax(OpenAD_lin_35, __deriv__(PRIMR(3)), __deriv__(UAVE))
      CALL saxpy(OpenAD_acc_11, __deriv__(OpenAD_prp_0), __deriv__(UAVE
     > ))
      CALL saxpy(OpenAD_lin_33, __deriv__(PRIML(3)), __deriv__(UAVE))
      CALL sax(OpenAD_lin_39, __deriv__(PRIMR(4)), __deriv__(VAVE))
      CALL saxpy(OpenAD_lin_37, __deriv__(PRIML(4)), __deriv__(VAVE))
      CALL saxpy(OpenAD_acc_10, __deriv__(OpenAD_prp_0), __deriv__(VAVE
     > ))
      CALL sax(OpenAD_lin_43, __deriv__(PRIMR(5)), __deriv__(WAVE))
      CALL saxpy(OpenAD_lin_41, __deriv__(PRIML(5)), __deriv__(WAVE))
      CALL saxpy(OpenAD_acc_9, __deriv__(OpenAD_prp_0), __deriv__(WAVE)
     > )
      CALL sax(OpenAD_lin_51, __deriv__(WAVE), __deriv__(OpenAD_prp_3))
      CALL saxpy(OpenAD_lin_50, __deriv__(VAVE), __deriv__(OpenAD_prp_3
     > ))
      CALL saxpy(OpenAD_lin_49, __deriv__(UAVE), __deriv__(OpenAD_prp_3
     > ))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_3), __deriv__(THTAVE))
      CALL sax(OpenAD_lin_52, __deriv__(GM1), __deriv__(CAVE))
      CALL saxpy(OpenAD_acc_5, __deriv__(OpenAD_prp_3), __deriv__(CAVE)
     > )
      CALL saxpy(OpenAD_acc_6, __deriv__(HR), __deriv__(CAVE))
      CALL saxpy(OpenAD_acc_7, __deriv__(HL), __deriv__(CAVE))
      CALL saxpy(OpenAD_acc_8, __deriv__(OpenAD_prp_0), __deriv__(CAVE)
     > )
      OpenAD_lin_54 = COS(__value__(CAVE))
      __value__(CAVE) = SIN(__value__(CAVE))
      OpenAD_lin_55 = __value__(UAVE)
      OpenAD_lin_56 = __value__(NXHAT)
      OpenAD_lin_57 = __value__(VAVE)
      OpenAD_lin_58 = __value__(NYHAT)
      OpenAD_lin_59 = __value__(WAVE)
      OpenAD_lin_60 = __value__(NZHAT)
      __value__(UHAT) = (__value__(NXHAT) * __value__(UAVE) + __value__
     > (NYHAT) * __value__(VAVE) + __value__(NZHAT) * __value__(WAVE))
      OpenAD_lin_61 = __value__(WAVE)
      OpenAD_lin_62 = __value__(NYHAT)
      OpenAD_lin_63 = __value__(VAVE)
      OpenAD_lin_64 = __value__(NZHAT)
      __value__(UTILDE) = (__value__(NYHAT) * __value__(WAVE) -
     >  __value__(NZHAT) * __value__(VAVE))
      OpenAD_lin_65 = __value__(UAVE)
      OpenAD_lin_66 = __value__(NZHAT)
      OpenAD_lin_67 = __value__(WAVE)
      OpenAD_lin_68 = __value__(NXHAT)
      __value__(VTILDE) = (__value__(NZHAT) * __value__(UAVE) -
     >  __value__(NXHAT) * __value__(WAVE))
      OpenAD_lin_69 = __value__(VAVE)
      OpenAD_lin_70 = __value__(NXHAT)
      OpenAD_lin_71 = __value__(UAVE)
      OpenAD_lin_72 = __value__(NYHAT)
      __value__(WTILDE) = (__value__(NXHAT) * __value__(VAVE) -
     >  __value__(NYHAT) * __value__(UAVE))
      __value__(LAMCM) = (__value__(UHAT) - __value__(CAVE))
      __value__(LAMU) = __value__(UHAT)
      __value__(LAMCP) = (__value__(CAVE) + __value__(UHAT))
      OpenAD_aux_11 = (__value__(CAVE) + __value__(UHAT))
      OpenAD_lin_73 = OpenAD_aux_11
      OpenAD_lin_74 = __value__(NLEFIX)
      __value__(DELTA) = (__value__(NLEFIX) * OpenAD_aux_11)
      OpenAD_lin_75 = (2 *(__value__(DELTA) **(2 - INT(1_w2f__i8))))
      __value__(DELTA2) = (__value__(DELTA) ** 2)
      OpenAD_lin_76 = (2 *(__value__(LAMCM) **(2 - INT(1_w2f__i8))))
      __value__(LAM2) = (__value__(LAMCM) ** 2)
      OpenAD_acc_20 = (OpenAD_lin_73 * OpenAD_lin_75)
      OpenAD_acc_21 = (OpenAD_lin_74 * OpenAD_lin_75)
      OpenAD_acc_22 = (OpenAD_lin_63 * INT((-1_w2f__i8)))
      OpenAD_acc_23 = (OpenAD_lin_64 * INT((-1_w2f__i8)))
      OpenAD_acc_24 = (OpenAD_lin_67 * INT((-1_w2f__i8)))
      OpenAD_acc_25 = (OpenAD_lin_68 * INT((-1_w2f__i8)))
      OpenAD_acc_26 = (OpenAD_lin_71 * INT((-1_w2f__i8)))
      OpenAD_acc_27 = (OpenAD_lin_72 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_4), __deriv__(CAVE))
      CALL sax(OpenAD_lin_54, __deriv__(OpenAD_prp_4), __deriv__(CAVE))
      CALL sax(OpenAD_lin_61, __deriv__(NYHAT), __deriv__(UTILDE))
      CALL saxpy(OpenAD_lin_62, __deriv__(WAVE), __deriv__(UTILDE))
      CALL saxpy(OpenAD_acc_22, __deriv__(NZHAT), __deriv__(UTILDE))
      CALL saxpy(OpenAD_acc_23, __deriv__(VAVE), __deriv__(UTILDE))
      CALL sax(OpenAD_lin_65, __deriv__(NZHAT), __deriv__(VTILDE))
      CALL saxpy(OpenAD_lin_66, __deriv__(UAVE), __deriv__(VTILDE))
      CALL saxpy(OpenAD_acc_24, __deriv__(NXHAT), __deriv__(VTILDE))
      CALL saxpy(OpenAD_acc_25, __deriv__(WAVE), __deriv__(VTILDE))
      CALL sax(OpenAD_lin_69, __deriv__(NXHAT), __deriv__(WTILDE))
      CALL saxpy(OpenAD_lin_70, __deriv__(VAVE), __deriv__(WTILDE))
      CALL saxpy(OpenAD_acc_26, __deriv__(NYHAT), __deriv__(WTILDE))
      CALL saxpy(OpenAD_acc_27, __deriv__(UAVE), __deriv__(WTILDE))
      CALL sax(OpenAD_lin_55, __deriv__(NXHAT), __deriv__(UHAT))
      CALL saxpy(OpenAD_lin_56, __deriv__(UAVE), __deriv__(UHAT))
      CALL saxpy(OpenAD_lin_57, __deriv__(NYHAT), __deriv__(UHAT))
      CALL saxpy(OpenAD_lin_58, __deriv__(VAVE), __deriv__(UHAT))
      CALL saxpy(OpenAD_lin_59, __deriv__(NZHAT), __deriv__(UHAT))
      CALL saxpy(OpenAD_lin_60, __deriv__(WAVE), __deriv__(UHAT))
      CALL setderiv(__deriv__(LAMCM), __deriv__(UHAT))
      CALL dec_deriv(__deriv__(LAMCM), __deriv__(CAVE))
      CALL setderiv(__deriv__(LAMU), __deriv__(UHAT))
      CALL setderiv(__deriv__(LAMCP), __deriv__(CAVE))
      CALL inc_deriv(__deriv__(LAMCP), __deriv__(UHAT))
      CALL setderiv(__deriv__(OpenAD_prp_5), __deriv__(CAVE))
      CALL inc_deriv(__deriv__(OpenAD_prp_5), __deriv__(UHAT))
      CALL sax(OpenAD_lin_76, __deriv__(LAMCM), __deriv__(LAM2))
      CALL sax(OpenAD_acc_20, __deriv__(NLEFIX), __deriv__(DELTA2))
      CALL saxpy(OpenAD_acc_21, __deriv__(OpenAD_prp_5), __deriv__(
     > DELTA2))
      IF(__value__(DELTA2) .LT. __value__(LAM2)) THEN
        __value__(ALAMCM) = __value__(LAMCM)
        CALL setderiv(__deriv__(ALAMCM), __deriv__(LAMCM))
      ELSE
        OpenAD_aux_38 = (__value__(DELTA2) + __value__(LAM2))
        OpenAD_aux_37 = (OpenAD_aux_38 * 5.0D-01)
        OpenAD_lin_247 = (INT(1_w2f__i8) / __value__(DELTA2))
        OpenAD_lin_248 = (-(OpenAD_aux_37 /(__value__(DELTA2) *
     >  __value__(DELTA2))))
        __value__(ALAMCM) = (OpenAD_aux_37 / __value__(DELTA2))
        OpenAD_acc_80 = (5.0D-01 * OpenAD_lin_247)
        CALL setderiv(__deriv__(OpenAD_prp_31), __deriv__(DELTA2))
        CALL inc_deriv(__deriv__(OpenAD_prp_31), __deriv__(LAM2))
        CALL sax(OpenAD_lin_248, __deriv__(DELTA2), __deriv__(ALAMCM))
        CALL saxpy(OpenAD_acc_80, __deriv__(OpenAD_prp_31), __deriv__(
     > ALAMCM))
      ENDIF
      OpenAD_lin_77 = (2 *(__value__(LAMCP) **(2 - INT(1_w2f__i8))))
      __value__(LAM2) = (__value__(LAMCP) ** 2)
      CALL sax(OpenAD_lin_77, __deriv__(LAMCP), __deriv__(LAM2))
      IF(__value__(DELTA2) .LT. __value__(LAM2)) THEN
        __value__(ALAMCP) = __value__(LAMCP)
        CALL setderiv(__deriv__(ALAMCP), __deriv__(LAMCP))
      ELSE
        OpenAD_aux_36 = (__value__(DELTA2) + __value__(LAM2))
        OpenAD_aux_35 = (OpenAD_aux_36 * 5.0D-01)
        OpenAD_lin_245 = (INT(1_w2f__i8) / __value__(DELTA2))
        OpenAD_lin_246 = (-(OpenAD_aux_35 /(__value__(DELTA2) *
     >  __value__(DELTA2))))
        __value__(ALAMCP) = (OpenAD_aux_35 / __value__(DELTA2))
        OpenAD_acc_79 = (5.0D-01 * OpenAD_lin_245)
        CALL setderiv(__deriv__(OpenAD_prp_30), __deriv__(DELTA2))
        CALL inc_deriv(__deriv__(OpenAD_prp_30), __deriv__(LAM2))
        CALL sax(OpenAD_lin_246, __deriv__(DELTA2), __deriv__(ALAMCP))
        CALL saxpy(OpenAD_acc_79, __deriv__(OpenAD_prp_30), __deriv__(
     > ALAMCP))
      ENDIF
      OpenAD_aux_12 = (__value__(CAVE) + __value__(UHAT))
      OpenAD_lin_78 = OpenAD_aux_12
      OpenAD_lin_79 = __value__(LEFIX)
      __value__(DELTA) = (__value__(LEFIX) * OpenAD_aux_12)
      OpenAD_lin_80 = __value__(DELTA)
      OpenAD_lin_81 = __value__(DELTA)
      __value__(DELTA2) = (__value__(DELTA) * __value__(DELTA))
      OpenAD_lin_82 = (2 *(__value__(LAMU) **(2 - INT(1_w2f__i8))))
      __value__(LAM2) = (__value__(LAMU) ** 2)
      OpenAD_acc_28 = (OpenAD_lin_80 + OpenAD_lin_81)
      OpenAD_acc_29 = (OpenAD_lin_78 * OpenAD_acc_28)
      OpenAD_acc_30 = (OpenAD_lin_79 * OpenAD_acc_28)
      CALL setderiv(__deriv__(OpenAD_prp_6), __deriv__(CAVE))
      CALL inc_deriv(__deriv__(OpenAD_prp_6), __deriv__(UHAT))
      CALL sax(OpenAD_lin_82, __deriv__(LAMU), __deriv__(LAM2))
      CALL sax(OpenAD_acc_29, __deriv__(LEFIX), __deriv__(DELTA2))
      CALL saxpy(OpenAD_acc_30, __deriv__(OpenAD_prp_6), __deriv__(
     > DELTA2))
      IF(__value__(DELTA2) .LT. __value__(LAM2)) THEN
        __value__(ALAMU) = __value__(LAMU)
        CALL setderiv(__deriv__(ALAMU), __deriv__(LAMU))
      ELSE
        OpenAD_aux_34 = (__value__(DELTA2) + __value__(LAM2))
        OpenAD_aux_33 = (OpenAD_aux_34 * 5.0D-01)
        OpenAD_lin_243 = (INT(1_w2f__i8) / __value__(DELTA2))
        OpenAD_lin_244 = (-(OpenAD_aux_33 /(__value__(DELTA2) *
     >  __value__(DELTA2))))
        __value__(ALAMU) = (OpenAD_aux_33 / __value__(DELTA2))
        OpenAD_acc_78 = (5.0D-01 * OpenAD_lin_243)
        CALL setderiv(__deriv__(OpenAD_prp_29), __deriv__(DELTA2))
        CALL inc_deriv(__deriv__(OpenAD_prp_29), __deriv__(LAM2))
        CALL sax(OpenAD_lin_244, __deriv__(DELTA2), __deriv__(ALAMU))
        CALL saxpy(OpenAD_acc_78, __deriv__(OpenAD_prp_29), __deriv__(
     > ALAMU))
      ENDIF
      OpenAD_aux_13 = (__value__(LAMCM) - __value__(ALAMCM))
      __value__(ALAMCM) = (OpenAD_aux_13 * 5.0D-01)
      OpenAD_aux_14 = (__value__(LAMCP) - __value__(ALAMCP))
      __value__(ALAMCP) = (OpenAD_aux_14 * 5.0D-01)
      OpenAD_aux_15 = (__value__(LAMU) - __value__(ALAMU))
      __value__(ALAMU) = (OpenAD_aux_15 * 5.0D-01)
      OpenAD_lin_83 = __value__(GM1INV)
      OpenAD_lin_84 = __value__(PRIML(1))
      OpenAD_lin_85 = __value__(THETAL)
      OpenAD_lin_86 = __value__(PRIML(2))
      __value__(EL) = (__value__(PRIML(1)) * __value__(GM1INV) +
     >  __value__(PRIML(2)) * __value__(THETAL))
      OpenAD_lin_87 = __value__(PRIML(3))
      OpenAD_lin_88 = __value__(PRIML(2))
      __value__(RUL) = (__value__(PRIML(2)) * __value__(PRIML(3)))
      OpenAD_lin_89 = __value__(PRIML(4))
      OpenAD_lin_90 = __value__(PRIML(2))
      __value__(RVL) = (__value__(PRIML(2)) * __value__(PRIML(4)))
      OpenAD_lin_91 = __value__(PRIML(5))
      OpenAD_lin_92 = __value__(PRIML(2))
      __value__(RWL) = (__value__(PRIML(2)) * __value__(PRIML(5)))
      OpenAD_lin_93 = __value__(GM1INV)
      OpenAD_lin_94 = __value__(PRIMR(1))
      OpenAD_lin_95 = __value__(THETAR)
      OpenAD_lin_96 = __value__(PRIMR(2))
      __value__(ER) = (__value__(PRIMR(1)) * __value__(GM1INV) +
     >  __value__(PRIMR(2)) * __value__(THETAR))
      OpenAD_lin_97 = __value__(PRIMR(3))
      OpenAD_lin_98 = __value__(PRIMR(2))
      __value__(RUR) = (__value__(PRIMR(2)) * __value__(PRIMR(3)))
      OpenAD_lin_99 = __value__(PRIMR(4))
      OpenAD_lin_100 = __value__(PRIMR(2))
      __value__(RVR) = (__value__(PRIMR(2)) * __value__(PRIMR(4)))
      OpenAD_lin_101 = __value__(PRIMR(5))
      OpenAD_lin_102 = __value__(PRIMR(2))
      __value__(RWR) = (__value__(PRIMR(2)) * __value__(PRIMR(5)))
      __value__(DE) = (__value__(ER) - __value__(EL))
      __value__(DR) = (__value__(PRIMR(2)) - __value__(PRIML(2)))
      __value__(DRU) = (__value__(RUR) - __value__(RUL))
      __value__(DRV) = (__value__(RVR) - __value__(RVL))
      __value__(DRW) = (__value__(RWR) - __value__(RWL))
      OpenAD_lin_103 = __value__(UAVE)
      OpenAD_lin_104 = __value__(DRU)
      OpenAD_lin_105 = __value__(VAVE)
      OpenAD_lin_106 = __value__(DRV)
      OpenAD_lin_107 = __value__(WAVE)
      OpenAD_lin_108 = __value__(DRW)
      __value__(UDDRU) = (__value__(DRU) * __value__(UAVE) + __value__(
     > DRV) * __value__(VAVE) + __value__(DRW) * __value__(WAVE))
      OpenAD_aux_16 = (__value__(GM1) / __value__(CAVE))
      OpenAD_aux_17 = (__value__(DE) + __value__(DR) * __value__(THTAVE
     > ) - __value__(UDDRU))
      OpenAD_lin_111 = (INT(1_w2f__i8) / __value__(CAVE))
      OpenAD_lin_112 = (-(__value__(GM1) /(__value__(CAVE) * __value__(
     > CAVE))))
      OpenAD_lin_109 = OpenAD_aux_17
      OpenAD_lin_113 = __value__(THTAVE)
      OpenAD_lin_114 = __value__(DR)
      OpenAD_lin_110 = OpenAD_aux_16
      __value__(OMEGA) = (OpenAD_aux_16 * OpenAD_aux_17)
      OpenAD_lin_115 = __value__(NXHAT)
      OpenAD_lin_116 = __value__(DRU)
      OpenAD_lin_117 = __value__(NYHAT)
      OpenAD_lin_118 = __value__(DRV)
      OpenAD_lin_119 = __value__(NZHAT)
      OpenAD_lin_120 = __value__(DRW)
      __value__(NDDRU) = (__value__(DRU) * __value__(NXHAT) + __value__
     > (DRV) * __value__(NYHAT) + __value__(DRW) * __value__(NZHAT))
      OpenAD_aux_18 = (__value__(OMEGA) + __value__(DR) * __value__(
     > UHAT) - __value__(NDDRU))
      OpenAD_lin_121 = __value__(UHAT)
      OpenAD_lin_122 = __value__(DR)
      __value__(ALP1) = (OpenAD_aux_18 * 5.0D-01)
      OpenAD_aux_19 = (__value__(CAVE) * __value__(NXHAT) - __value__(
     > UTILDE))
      OpenAD_aux_20 = (- __value__(NXHAT))
      OpenAD_lin_123 = __value__(NYHAT)
      OpenAD_lin_124 = __value__(DRW)
      OpenAD_lin_125 = OpenAD_aux_19
      OpenAD_lin_127 = __value__(NXHAT)
      OpenAD_lin_128 = __value__(CAVE)
      OpenAD_lin_126 = __value__(DR)
      OpenAD_lin_129 = OpenAD_aux_20
      OpenAD_lin_130 = __value__(OMEGA)
      OpenAD_lin_131 = __value__(NZHAT)
      OpenAD_lin_132 = __value__(DRV)
      __value__(ALP2) = (__value__(DRW) * __value__(NYHAT) + __value__(
     > DR) * OpenAD_aux_19 + __value__(OMEGA) * OpenAD_aux_20 -
     >  __value__(DRV) * __value__(NZHAT))
      OpenAD_aux_21 = (__value__(CAVE) * __value__(NYHAT) - __value__(
     > VTILDE))
      OpenAD_aux_22 = (- __value__(NYHAT))
      OpenAD_lin_133 = __value__(NZHAT)
      OpenAD_lin_134 = __value__(DRU)
      OpenAD_lin_135 = OpenAD_aux_21
      OpenAD_lin_137 = __value__(NYHAT)
      OpenAD_lin_138 = __value__(CAVE)
      OpenAD_lin_136 = __value__(DR)
      OpenAD_lin_139 = OpenAD_aux_22
      OpenAD_lin_140 = __value__(OMEGA)
      OpenAD_lin_141 = __value__(NXHAT)
      OpenAD_lin_142 = __value__(DRW)
      __value__(ALP3) = (__value__(DRU) * __value__(NZHAT) + __value__(
     > DR) * OpenAD_aux_21 + __value__(OMEGA) * OpenAD_aux_22 -
     >  __value__(DRW) * __value__(NXHAT))
      OpenAD_aux_23 = (__value__(CAVE) * __value__(NZHAT) - __value__(
     > WTILDE))
      OpenAD_aux_24 = (- __value__(NZHAT))
      OpenAD_lin_143 = __value__(NXHAT)
      OpenAD_lin_144 = __value__(DRV)
      OpenAD_lin_145 = OpenAD_aux_23
      OpenAD_lin_147 = __value__(NZHAT)
      OpenAD_lin_148 = __value__(CAVE)
      OpenAD_lin_146 = __value__(DR)
      OpenAD_lin_149 = OpenAD_aux_24
      OpenAD_lin_150 = __value__(OMEGA)
      OpenAD_lin_151 = __value__(NYHAT)
      OpenAD_lin_152 = __value__(DRU)
      __value__(ALP4) = (__value__(DRV) * __value__(NXHAT) + __value__(
     > DR) * OpenAD_aux_23 + __value__(OMEGA) * OpenAD_aux_24 -
     >  __value__(DRU) * __value__(NYHAT))
      OpenAD_aux_25 = (__value__(NDDRU) + __value__(OMEGA) - __value__(
     > DR) * __value__(UHAT))
      OpenAD_lin_153 = __value__(UHAT)
      OpenAD_lin_154 = __value__(DR)
      __value__(ALP5) = (OpenAD_aux_25 * 5.0D-01)
      OpenAD_acc_31 = (OpenAD_lin_111 * OpenAD_lin_109)
      OpenAD_acc_32 = (OpenAD_lin_112 * OpenAD_lin_109)
      OpenAD_acc_33 = (OpenAD_lin_103 * INT((-1_w2f__i8)))
      OpenAD_acc_34 = (OpenAD_lin_104 * INT((-1_w2f__i8)))
      OpenAD_acc_35 = (OpenAD_lin_105 * INT((-1_w2f__i8)))
      OpenAD_acc_36 = (OpenAD_lin_106 * INT((-1_w2f__i8)))
      OpenAD_acc_37 = (OpenAD_lin_107 * INT((-1_w2f__i8)))
      OpenAD_acc_38 = (OpenAD_lin_108 * INT((-1_w2f__i8)))
      OpenAD_acc_39 = (INT((-1_w2f__i8)) * OpenAD_lin_130)
      OpenAD_acc_40 = (OpenAD_lin_131 * INT((-1_w2f__i8)))
      OpenAD_acc_41 = (OpenAD_lin_132 * INT((-1_w2f__i8)))
      OpenAD_acc_42 = (INT((-1_w2f__i8)) * OpenAD_lin_140)
      OpenAD_acc_43 = (OpenAD_lin_141 * INT((-1_w2f__i8)))
      OpenAD_acc_44 = (OpenAD_lin_142 * INT((-1_w2f__i8)))
      OpenAD_acc_45 = (INT((-1_w2f__i8)) * OpenAD_lin_150)
      OpenAD_acc_46 = (OpenAD_lin_151 * INT((-1_w2f__i8)))
      OpenAD_acc_47 = (OpenAD_lin_152 * INT((-1_w2f__i8)))
      OpenAD_acc_48 = (OpenAD_lin_153 * INT((-1_w2f__i8)))
      OpenAD_acc_49 = (OpenAD_lin_154 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_7), __deriv__(ALAMCM))
      CALL setderiv(__deriv__(OpenAD_prp_8), __deriv__(ALAMCP))
      CALL setderiv(__deriv__(OpenAD_prp_9), __deriv__(ALAMU))
      CALL setderiv(__deriv__(OpenAD_prp_10), __deriv__(LAMCM))
      CALL dec_deriv(__deriv__(OpenAD_prp_10), __deriv__(OpenAD_prp_7))
      CALL setderiv(__deriv__(OpenAD_prp_11), __deriv__(LAMCP))
      CALL dec_deriv(__deriv__(OpenAD_prp_11), __deriv__(OpenAD_prp_8))
      CALL setderiv(__deriv__(OpenAD_prp_12), __deriv__(LAMU))
      CALL dec_deriv(__deriv__(OpenAD_prp_12), __deriv__(OpenAD_prp_9))
      CALL sax(OpenAD_lin_87, __deriv__(PRIML(2)), __deriv__(RUL))
      CALL saxpy(OpenAD_lin_88, __deriv__(PRIML(3)), __deriv__(RUL))
      CALL sax(OpenAD_lin_89, __deriv__(PRIML(2)), __deriv__(RVL))
      CALL saxpy(OpenAD_lin_90, __deriv__(PRIML(4)), __deriv__(RVL))
      CALL sax(OpenAD_lin_91, __deriv__(PRIML(2)), __deriv__(RWL))
      CALL saxpy(OpenAD_lin_92, __deriv__(PRIML(5)), __deriv__(RWL))
      CALL setderiv(__deriv__(DR), __deriv__(PRIMR(2)))
      CALL dec_deriv(__deriv__(DR), __deriv__(PRIML(2)))
      CALL set_neg_deriv(__deriv__(DRU), __deriv__(RUL))
      CALL saxpy(OpenAD_lin_97, __deriv__(PRIMR(2)), __deriv__(DRU))
      CALL saxpy(OpenAD_lin_98, __deriv__(PRIMR(3)), __deriv__(DRU))
      CALL set_neg_deriv(__deriv__(DRV), __deriv__(RVL))
      CALL saxpy(OpenAD_lin_99, __deriv__(PRIMR(2)), __deriv__(DRV))
      CALL saxpy(OpenAD_lin_100, __deriv__(PRIMR(4)), __deriv__(DRV))
      CALL set_neg_deriv(__deriv__(DRW), __deriv__(RWL))
      CALL saxpy(OpenAD_lin_101, __deriv__(PRIMR(2)), __deriv__(DRW))
      CALL saxpy(OpenAD_lin_102, __deriv__(PRIMR(5)), __deriv__(DRW))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_10), __deriv__(ALAMCM))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_11), __deriv__(ALAMCP))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_12), __deriv__(ALAMU))
      CALL sax(OpenAD_lin_83, __deriv__(PRIML(1)), __deriv__(EL))
      CALL saxpy(OpenAD_lin_84, __deriv__(GM1INV), __deriv__(EL))
      CALL saxpy(OpenAD_lin_85, __deriv__(PRIML(2)), __deriv__(EL))
      CALL saxpy(OpenAD_lin_86, __deriv__(THETAL), __deriv__(EL))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_13), __deriv__(UTILDE))
      CALL saxpy(OpenAD_lin_127, __deriv__(CAVE), __deriv__(
     > OpenAD_prp_13))
      CALL saxpy(OpenAD_lin_128, __deriv__(NXHAT), __deriv__(
     > OpenAD_prp_13))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_14), __deriv__(VTILDE))
      CALL saxpy(OpenAD_lin_137, __deriv__(CAVE), __deriv__(
     > OpenAD_prp_14))
      CALL saxpy(OpenAD_lin_138, __deriv__(NYHAT), __deriv__(
     > OpenAD_prp_14))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_15), __deriv__(WTILDE))
      CALL saxpy(OpenAD_lin_147, __deriv__(CAVE), __deriv__(
     > OpenAD_prp_15))
      CALL saxpy(OpenAD_lin_148, __deriv__(NZHAT), __deriv__(
     > OpenAD_prp_15))
      CALL set_neg_deriv(__deriv__(OpenAD_prp_16), __deriv__(EL))
      CALL saxpy(OpenAD_lin_93, __deriv__(PRIMR(1)), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_lin_94, __deriv__(GM1INV), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_lin_95, __deriv__(PRIMR(2)), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_lin_96, __deriv__(THETAR), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_lin_113, __deriv__(DR), __deriv__(OpenAD_prp_16
     > ))
      CALL saxpy(OpenAD_lin_114, __deriv__(THTAVE), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_acc_33, __deriv__(DRU), __deriv__(OpenAD_prp_16
     > ))
      CALL saxpy(OpenAD_acc_34, __deriv__(UAVE), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_acc_35, __deriv__(DRV), __deriv__(OpenAD_prp_16
     > ))
      CALL saxpy(OpenAD_acc_36, __deriv__(VAVE), __deriv__(
     > OpenAD_prp_16))
      CALL saxpy(OpenAD_acc_37, __deriv__(DRW), __deriv__(OpenAD_prp_16
     > ))
      CALL saxpy(OpenAD_acc_38, __deriv__(WAVE), __deriv__(
     > OpenAD_prp_16))
      CALL sax(OpenAD_lin_115, __deriv__(DRU), __deriv__(NDDRU))
      CALL saxpy(OpenAD_lin_116, __deriv__(NXHAT), __deriv__(NDDRU))
      CALL saxpy(OpenAD_lin_117, __deriv__(DRV), __deriv__(NDDRU))
      CALL saxpy(OpenAD_lin_118, __deriv__(NYHAT), __deriv__(NDDRU))
      CALL saxpy(OpenAD_lin_119, __deriv__(DRW), __deriv__(NDDRU))
      CALL saxpy(OpenAD_lin_120, __deriv__(NZHAT), __deriv__(NDDRU))
      CALL sax(OpenAD_lin_110, __deriv__(OpenAD_prp_16), __deriv__(
     > OMEGA))
      CALL saxpy(OpenAD_acc_31, __deriv__(GM1), __deriv__(OMEGA))
      CALL saxpy(OpenAD_acc_32, __deriv__(CAVE), __deriv__(OMEGA))
      CALL setderiv(__deriv__(OpenAD_prp_17), __deriv__(OMEGA))
      CALL dec_deriv(__deriv__(OpenAD_prp_17), __deriv__(NDDRU))
      CALL saxpy(OpenAD_lin_121, __deriv__(DR), __deriv__(OpenAD_prp_17
     > ))
      CALL saxpy(OpenAD_lin_122, __deriv__(UHAT), __deriv__(
     > OpenAD_prp_17))
      CALL setderiv(__deriv__(OpenAD_prp_18), __deriv__(NDDRU))
      CALL inc_deriv(__deriv__(OpenAD_prp_18), __deriv__(OMEGA))
      CALL saxpy(OpenAD_acc_48, __deriv__(DR), __deriv__(OpenAD_prp_18)
     > )
      CALL saxpy(OpenAD_acc_49, __deriv__(UHAT), __deriv__(
     > OpenAD_prp_18))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_17), __deriv__(ALP1))
      CALL sax(5.0D-01, __deriv__(OpenAD_prp_18), __deriv__(ALP5))
      CALL sax(OpenAD_lin_123, __deriv__(DRW), __deriv__(ALP2))
      CALL saxpy(OpenAD_lin_124, __deriv__(NYHAT), __deriv__(ALP2))
      CALL saxpy(OpenAD_lin_125, __deriv__(DR), __deriv__(ALP2))
      CALL saxpy(OpenAD_lin_126, __deriv__(OpenAD_prp_13), __deriv__(
     > ALP2))
      CALL saxpy(OpenAD_lin_129, __deriv__(OMEGA), __deriv__(ALP2))
      CALL saxpy(OpenAD_acc_39, __deriv__(NXHAT), __deriv__(ALP2))
      CALL saxpy(OpenAD_acc_40, __deriv__(DRV), __deriv__(ALP2))
      CALL saxpy(OpenAD_acc_41, __deriv__(NZHAT), __deriv__(ALP2))
      CALL sax(OpenAD_lin_133, __deriv__(DRU), __deriv__(ALP3))
      CALL saxpy(OpenAD_lin_134, __deriv__(NZHAT), __deriv__(ALP3))
      CALL saxpy(OpenAD_lin_135, __deriv__(DR), __deriv__(ALP3))
      CALL saxpy(OpenAD_lin_136, __deriv__(OpenAD_prp_14), __deriv__(
     > ALP3))
      CALL saxpy(OpenAD_lin_139, __deriv__(OMEGA), __deriv__(ALP3))
      CALL saxpy(OpenAD_acc_42, __deriv__(NYHAT), __deriv__(ALP3))
      CALL saxpy(OpenAD_acc_43, __deriv__(DRW), __deriv__(ALP3))
      CALL saxpy(OpenAD_acc_44, __deriv__(NXHAT), __deriv__(ALP3))
      CALL sax(OpenAD_lin_143, __deriv__(DRV), __deriv__(ALP4))
      CALL saxpy(OpenAD_lin_144, __deriv__(NXHAT), __deriv__(ALP4))
      CALL saxpy(OpenAD_lin_145, __deriv__(DR), __deriv__(ALP4))
      CALL saxpy(OpenAD_lin_146, __deriv__(OpenAD_prp_15), __deriv__(
     > ALP4))
      CALL saxpy(OpenAD_lin_149, __deriv__(OMEGA), __deriv__(ALP4))
      CALL saxpy(OpenAD_acc_45, __deriv__(NZHAT), __deriv__(ALP4))
      CALL saxpy(OpenAD_acc_46, __deriv__(DRU), __deriv__(ALP4))
      CALL saxpy(OpenAD_acc_47, __deriv__(NYHAT), __deriv__(ALP4))
      OpenAD_lin_155 = __value__(ALP1)
      OpenAD_lin_156 = __value__(ALAMCM)
      __value__(ALP1) = (__value__(ALAMCM) * __value__(ALP1))
      OpenAD_lin_157 = __value__(ALP2)
      OpenAD_lin_158 = __value__(ALAMU)
      __value__(ALP2) = (__value__(ALAMU) * __value__(ALP2))
      OpenAD_lin_159 = __value__(ALP3)
      OpenAD_lin_160 = __value__(ALAMU)
      __value__(ALP3) = (__value__(ALAMU) * __value__(ALP3))
      OpenAD_lin_161 = __value__(ALP4)
      OpenAD_lin_162 = __value__(ALAMU)
      __value__(ALP4) = (__value__(ALAMU) * __value__(ALP4))
      OpenAD_lin_163 = __value__(ALP5)
      OpenAD_lin_164 = __value__(ALAMCP)
      __value__(ALP5) = (__value__(ALAMCP) * __value__(ALP5))
      __value__(ALP15P) = (__value__(ALP1) + __value__(ALP5))
      __value__(ALP15M) = (__value__(ALP1) - __value__(ALP5))
      OpenAD_lin_165 = (INT(1_w2f__i8) / __value__(CAVE))
      OpenAD_lin_166 = (-(__value__(ALP15P) /(__value__(CAVE) *
     >  __value__(CAVE))))
      OpenAD_lin_167 = __value__(NXHAT)
      OpenAD_lin_168 = __value__(ALP2)
      OpenAD_lin_169 = __value__(NYHAT)
      OpenAD_lin_170 = __value__(ALP3)
      OpenAD_lin_171 = __value__(NZHAT)
      OpenAD_lin_172 = __value__(ALP4)
      __value__(MU) = ((__value__(ALP15P) / __value__(CAVE)) +
     >  __value__(ALP2) * __value__(NXHAT) + __value__(ALP3) *
     >  __value__(NYHAT) + __value__(ALP4) * __value__(NZHAT))
      OpenAD_aux_26 = (__value__(GM1INV) * __value__(CAVE))
      OpenAD_lin_173 = __value__(THTAVE)
      OpenAD_lin_174 = __value__(MU)
      OpenAD_lin_175 = OpenAD_aux_26
      OpenAD_lin_177 = __value__(CAVE)
      OpenAD_lin_178 = __value__(GM1INV)
      OpenAD_lin_176 = __value__(ALP15P)
      OpenAD_lin_179 = __value__(UHAT)
      OpenAD_lin_180 = __value__(ALP15M)
      OpenAD_lin_181 = __value__(UTILDE)
      OpenAD_lin_182 = __value__(ALP2)
      OpenAD_lin_183 = __value__(VTILDE)
      OpenAD_lin_184 = __value__(ALP3)
      OpenAD_lin_185 = __value__(WTILDE)
      OpenAD_lin_186 = __value__(ALP4)
      __value__(DSS1) = (__value__(MU) * __value__(THTAVE) + __value__(
     > ALP15P) * OpenAD_aux_26 - __value__(ALP15M) * __value__(UHAT) +
     >  __value__(ALP2) * __value__(UTILDE) + __value__(ALP3) *
     >  __value__(VTILDE) + __value__(ALP4) * __value__(WTILDE))
      __value__(DSS2) = __value__(MU)
      OpenAD_lin_187 = __value__(NZHAT)
      OpenAD_lin_188 = __value__(ALP3)
      OpenAD_lin_189 = __value__(UAVE)
      OpenAD_lin_190 = __value__(MU)
      OpenAD_lin_191 = __value__(NXHAT)
      OpenAD_lin_192 = __value__(ALP15M)
      OpenAD_lin_193 = __value__(NYHAT)
      OpenAD_lin_194 = __value__(ALP4)
      __value__(DSS3) = (__value__(ALP3) * __value__(NZHAT) + __value__
     > (MU) * __value__(UAVE) - __value__(ALP15M) * __value__(NXHAT) -
     >  __value__(ALP4) * __value__(NYHAT))
      OpenAD_lin_195 = __value__(NXHAT)
      OpenAD_lin_196 = __value__(ALP4)
      OpenAD_lin_197 = __value__(VAVE)
      OpenAD_lin_198 = __value__(MU)
      OpenAD_lin_199 = __value__(NYHAT)
      OpenAD_lin_200 = __value__(ALP15M)
      OpenAD_lin_201 = __value__(NZHAT)
      OpenAD_lin_202 = __value__(ALP2)
      __value__(DSS4) = (__value__(ALP4) * __value__(NXHAT) + __value__
     > (MU) * __value__(VAVE) - __value__(ALP15M) * __value__(NYHAT) -
     >  __value__(ALP2) * __value__(NZHAT))
      OpenAD_lin_203 = __value__(NYHAT)
      OpenAD_lin_204 = __value__(ALP2)
      OpenAD_lin_205 = __value__(WAVE)
      OpenAD_lin_206 = __value__(MU)
      OpenAD_lin_207 = __value__(NZHAT)
      OpenAD_lin_208 = __value__(ALP15M)
      OpenAD_lin_209 = __value__(NXHAT)
      OpenAD_lin_210 = __value__(ALP3)
      __value__(DSS5) = (__value__(ALP2) * __value__(NYHAT) + __value__
     > (MU) * __value__(WAVE) - __value__(ALP15M) * __value__(NZHAT) -
     >  __value__(ALP3) * __value__(NXHAT))
      OpenAD_lin_211 = __value__(NXHAT)
      OpenAD_lin_212 = __value__(PRIML(3))
      OpenAD_lin_213 = __value__(NYHAT)
      OpenAD_lin_214 = __value__(PRIML(4))
      OpenAD_lin_215 = __value__(NZHAT)
      OpenAD_lin_216 = __value__(PRIML(5))
      __value__(UHATL) = (__value__(PRIML(3)) * __value__(NXHAT) +
     >  __value__(PRIML(4)) * __value__(NYHAT) + __value__(PRIML(5)) *
     >  __value__(NZHAT))
      OpenAD_aux_28 = (__value__(PRIML(1)) + __value__(EL))
      OpenAD_aux_27 = (__value__(DSS1) + __value__(UHATL) *
     >  OpenAD_aux_28)
      OpenAD_lin_217 = OpenAD_aux_27
      OpenAD_lin_219 = OpenAD_aux_28
      OpenAD_lin_220 = __value__(UHATL)
      OpenAD_lin_218 = __value__(NSIZE)
      __value__(FLUX(1)) = (__value__(NSIZE) * OpenAD_aux_27)
      OpenAD_acc_50 = (OpenAD_lin_219 * OpenAD_lin_218)
      OpenAD_acc_51 = (OpenAD_lin_220 * OpenAD_lin_218)
      OpenAD_acc_52 = (OpenAD_lin_185 * OpenAD_lin_218)
      OpenAD_acc_53 = (OpenAD_lin_186 * OpenAD_lin_218)
      OpenAD_acc_54 = (OpenAD_lin_183 * OpenAD_lin_218)
      OpenAD_acc_55 = (OpenAD_lin_184 * OpenAD_lin_218)
      OpenAD_acc_56 = (OpenAD_lin_181 * OpenAD_lin_218)
      OpenAD_acc_57 = (OpenAD_lin_182 * OpenAD_lin_218)
      OpenAD_acc_58 = (OpenAD_lin_173 * OpenAD_lin_218)
      OpenAD_acc_59 = (OpenAD_lin_174 * OpenAD_lin_218)
      OpenAD_acc_60 = (OpenAD_lin_175 * OpenAD_lin_218)
      OpenAD_acc_61 = (OpenAD_lin_177 * OpenAD_lin_176 * OpenAD_lin_218
     > )
      OpenAD_acc_62 = (OpenAD_lin_178 * OpenAD_lin_176 * OpenAD_lin_218
     > )
      OpenAD_acc_63 = (INT((-1_w2f__i8)) * OpenAD_lin_218)
      OpenAD_acc_64 = (OpenAD_lin_179 * OpenAD_acc_63)
      OpenAD_acc_65 = (OpenAD_lin_180 * OpenAD_acc_63)
      OpenAD_acc_66 = (OpenAD_lin_193 * INT((-1_w2f__i8)))
      OpenAD_acc_67 = (OpenAD_lin_194 * INT((-1_w2f__i8)))
      OpenAD_acc_68 = (OpenAD_lin_201 * INT((-1_w2f__i8)))
      OpenAD_acc_69 = (OpenAD_lin_202 * INT((-1_w2f__i8)))
      OpenAD_acc_70 = (OpenAD_lin_209 * INT((-1_w2f__i8)))
      OpenAD_acc_71 = (OpenAD_lin_210 * INT((-1_w2f__i8)))
      OpenAD_acc_72 = (OpenAD_lin_207 * INT((-1_w2f__i8)))
      OpenAD_acc_73 = (OpenAD_lin_208 * INT((-1_w2f__i8)))
      OpenAD_acc_74 = (OpenAD_lin_199 * INT((-1_w2f__i8)))
      OpenAD_acc_75 = (OpenAD_lin_200 * INT((-1_w2f__i8)))
      OpenAD_acc_76 = (OpenAD_lin_191 * INT((-1_w2f__i8)))
      OpenAD_acc_77 = (OpenAD_lin_192 * INT((-1_w2f__i8)))
      CALL setderiv(__deriv__(OpenAD_prp_19), __deriv__(ALP1))
      CALL setderiv(__deriv__(OpenAD_prp_20), __deriv__(ALP2))
      CALL setderiv(__deriv__(OpenAD_prp_21), __deriv__(ALP3))
      CALL setderiv(__deriv__(OpenAD_prp_22), __deriv__(ALP4))
      CALL setderiv(__deriv__(OpenAD_prp_23), __deriv__(ALP5))
      CALL sax(OpenAD_lin_155, __deriv__(ALAMCM), __deriv__(ALP1))
      CALL saxpy(OpenAD_lin_156, __deriv__(OpenAD_prp_19), __deriv__(
     > ALP1))
      CALL sax(OpenAD_lin_157, __deriv__(ALAMU), __deriv__(ALP2))
      CALL saxpy(OpenAD_lin_158, __deriv__(OpenAD_prp_20), __deriv__(
     > ALP2))
      CALL sax(OpenAD_lin_159, __deriv__(ALAMU), __deriv__(ALP3))
      CALL saxpy(OpenAD_lin_160, __deriv__(OpenAD_prp_21), __deriv__(
     > ALP3))
      CALL sax(OpenAD_lin_161, __deriv__(ALAMU), __deriv__(ALP4))
      CALL saxpy(OpenAD_lin_162, __deriv__(OpenAD_prp_22), __deriv__(
     > ALP4))
      CALL sax(OpenAD_lin_163, __deriv__(ALAMCP), __deriv__(ALP5))
      CALL saxpy(OpenAD_lin_164, __deriv__(OpenAD_prp_23), __deriv__(
     > ALP5))
      CALL setderiv(__deriv__(ALP15P), __deriv__(ALP1))
      CALL inc_deriv(__deriv__(ALP15P), __deriv__(ALP5))
      CALL setderiv(__deriv__(ALP15M), __deriv__(ALP1))
      CALL dec_deriv(__deriv__(ALP15M), __deriv__(ALP5))
      CALL setderiv(__deriv__(OpenAD_prp_24), __deriv__(PRIML(1)))
      CALL inc_deriv(__deriv__(OpenAD_prp_24), __deriv__(EL))
      CALL sax(OpenAD_lin_211, __deriv__(PRIML(3)), __deriv__(UHATL))
      CALL saxpy(OpenAD_lin_212, __deriv__(NXHAT), __deriv__(UHATL))
      CALL saxpy(OpenAD_lin_213, __deriv__(PRIML(4)), __deriv__(UHATL))
      CALL saxpy(OpenAD_lin_214, __deriv__(NYHAT), __deriv__(UHATL))
      CALL saxpy(OpenAD_lin_215, __deriv__(PRIML(5)), __deriv__(UHATL))
      CALL saxpy(OpenAD_lin_216, __deriv__(NZHAT), __deriv__(UHATL))
      CALL sax(OpenAD_lin_165, __deriv__(ALP15P), __deriv__(MU))
      CALL saxpy(OpenAD_lin_166, __deriv__(CAVE), __deriv__(MU))
      CALL saxpy(OpenAD_lin_167, __deriv__(ALP2), __deriv__(MU))
      CALL saxpy(OpenAD_lin_168, __deriv__(NXHAT), __deriv__(MU))
      CALL saxpy(OpenAD_lin_169, __deriv__(ALP3), __deriv__(MU))
      CALL saxpy(OpenAD_lin_170, __deriv__(NYHAT), __deriv__(MU))
      CALL saxpy(OpenAD_lin_171, __deriv__(ALP4), __deriv__(MU))
      CALL saxpy(OpenAD_lin_172, __deriv__(NZHAT), __deriv__(MU))
      CALL setderiv(__deriv__(DSS2), __deriv__(MU))
      CALL sax(OpenAD_lin_189, __deriv__(MU), __deriv__(DSS3))
      CALL saxpy(OpenAD_lin_190, __deriv__(UAVE), __deriv__(DSS3))
      CALL saxpy(OpenAD_acc_66, __deriv__(ALP4), __deriv__(DSS3))
      CALL saxpy(OpenAD_acc_67, __deriv__(NYHAT), __deriv__(DSS3))
      CALL saxpy(OpenAD_lin_187, __deriv__(ALP3), __deriv__(DSS3))
      CALL saxpy(OpenAD_lin_188, __deriv__(NZHAT), __deriv__(DSS3))
      CALL saxpy(OpenAD_acc_76, __deriv__(ALP15M), __deriv__(DSS3))
      CALL saxpy(OpenAD_acc_77, __deriv__(NXHAT), __deriv__(DSS3))
      CALL sax(OpenAD_lin_197, __deriv__(MU), __deriv__(DSS4))
      CALL saxpy(OpenAD_lin_198, __deriv__(VAVE), __deriv__(DSS4))
      CALL saxpy(OpenAD_acc_68, __deriv__(ALP2), __deriv__(DSS4))
      CALL saxpy(OpenAD_acc_69, __deriv__(NZHAT), __deriv__(DSS4))
      CALL saxpy(OpenAD_lin_195, __deriv__(ALP4), __deriv__(DSS4))
      CALL saxpy(OpenAD_lin_196, __deriv__(NXHAT), __deriv__(DSS4))
      CALL saxpy(OpenAD_acc_74, __deriv__(ALP15M), __deriv__(DSS4))
      CALL saxpy(OpenAD_acc_75, __deriv__(NYHAT), __deriv__(DSS4))
      CALL sax(OpenAD_lin_205, __deriv__(MU), __deriv__(DSS5))
      CALL saxpy(OpenAD_lin_206, __deriv__(WAVE), __deriv__(DSS5))
      CALL saxpy(OpenAD_acc_70, __deriv__(ALP3), __deriv__(DSS5))
      CALL saxpy(OpenAD_acc_71, __deriv__(NXHAT), __deriv__(DSS5))
      CALL saxpy(OpenAD_lin_203, __deriv__(ALP2), __deriv__(DSS5))
      CALL saxpy(OpenAD_lin_204, __deriv__(NYHAT), __deriv__(DSS5))
      CALL saxpy(OpenAD_acc_72, __deriv__(ALP15M), __deriv__(DSS5))
      CALL saxpy(OpenAD_acc_73, __deriv__(NZHAT), __deriv__(DSS5))
      CALL sax(OpenAD_lin_217, __deriv__(NSIZE), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_50, __deriv__(UHATL), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_51, __deriv__(OpenAD_prp_24), __deriv__(
     > FLUX(1)))
      CALL saxpy(OpenAD_acc_52, __deriv__(ALP4), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_53, __deriv__(WTILDE), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_54, __deriv__(ALP3), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_55, __deriv__(VTILDE), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_56, __deriv__(ALP2), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_57, __deriv__(UTILDE), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_58, __deriv__(MU), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_59, __deriv__(THTAVE), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_60, __deriv__(ALP15P), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_61, __deriv__(GM1INV), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_62, __deriv__(CAVE), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_64, __deriv__(ALP15M), __deriv__(FLUX(1)))
      CALL saxpy(OpenAD_acc_65, __deriv__(UHAT), __deriv__(FLUX(1)))
      OpenAD_aux_29 = (__value__(DSS2) + __value__(PRIML(2)) *
     >  __value__(UHATL))
      OpenAD_lin_221 = OpenAD_aux_29
      OpenAD_lin_223 = __value__(UHATL)
      OpenAD_lin_224 = __value__(PRIML(2))
      OpenAD_lin_222 = __value__(NSIZE)
      __value__(FLUX(2)) = (__value__(NSIZE) * OpenAD_aux_29)
      CALL setderiv(__deriv__(OpenAD_prp_25), __deriv__(DSS2))
      CALL saxpy(OpenAD_lin_223, __deriv__(PRIML(2)), __deriv__(
     > OpenAD_prp_25))
      CALL saxpy(OpenAD_lin_224, __deriv__(UHATL), __deriv__(
     > OpenAD_prp_25))
      CALL sax(OpenAD_lin_221, __deriv__(NSIZE), __deriv__(FLUX(2)))
      CALL saxpy(OpenAD_lin_222, __deriv__(OpenAD_prp_25), __deriv__(
     > FLUX(2)))
      OpenAD_aux_30 = (__value__(DSS3) + __value__(PRIML(1)) *
     >  __value__(NXHAT) + __value__(RUL) * __value__(UHATL))
      OpenAD_lin_225 = OpenAD_aux_30
      OpenAD_lin_227 = __value__(NXHAT)
      OpenAD_lin_228 = __value__(PRIML(1))
      OpenAD_lin_229 = __value__(UHATL)
      OpenAD_lin_230 = __value__(RUL)
      OpenAD_lin_226 = __value__(NSIZE)
      __value__(FLUX(3)) = (__value__(NSIZE) * OpenAD_aux_30)
      CALL setderiv(__deriv__(OpenAD_prp_26), __deriv__(DSS3))
      CALL saxpy(OpenAD_lin_227, __deriv__(PRIML(1)), __deriv__(
     > OpenAD_prp_26))
      CALL saxpy(OpenAD_lin_228, __deriv__(NXHAT), __deriv__(
     > OpenAD_prp_26))
      CALL saxpy(OpenAD_lin_229, __deriv__(RUL), __deriv__(
     > OpenAD_prp_26))
      CALL saxpy(OpenAD_lin_230, __deriv__(UHATL), __deriv__(
     > OpenAD_prp_26))
      CALL sax(OpenAD_lin_225, __deriv__(NSIZE), __deriv__(FLUX(3)))
      CALL saxpy(OpenAD_lin_226, __deriv__(OpenAD_prp_26), __deriv__(
     > FLUX(3)))
      OpenAD_aux_31 = (__value__(DSS4) + __value__(PRIML(1)) *
     >  __value__(NYHAT) + __value__(RVL) * __value__(UHATL))
      OpenAD_lin_231 = OpenAD_aux_31
      OpenAD_lin_233 = __value__(NYHAT)
      OpenAD_lin_234 = __value__(PRIML(1))
      OpenAD_lin_235 = __value__(UHATL)
      OpenAD_lin_236 = __value__(RVL)
      OpenAD_lin_232 = __value__(NSIZE)
      __value__(FLUX(4)) = (__value__(NSIZE) * OpenAD_aux_31)
      CALL setderiv(__deriv__(OpenAD_prp_27), __deriv__(DSS4))
      CALL saxpy(OpenAD_lin_233, __deriv__(PRIML(1)), __deriv__(
     > OpenAD_prp_27))
      CALL saxpy(OpenAD_lin_234, __deriv__(NYHAT), __deriv__(
     > OpenAD_prp_27))
      CALL saxpy(OpenAD_lin_235, __deriv__(RVL), __deriv__(
     > OpenAD_prp_27))
      CALL saxpy(OpenAD_lin_236, __deriv__(UHATL), __deriv__(
     > OpenAD_prp_27))
      CALL sax(OpenAD_lin_231, __deriv__(NSIZE), __deriv__(FLUX(4)))
      CALL saxpy(OpenAD_lin_232, __deriv__(OpenAD_prp_27), __deriv__(
     > FLUX(4)))
      OpenAD_aux_32 = (__value__(DSS5) + __value__(PRIML(1)) *
     >  __value__(NZHAT) + __value__(RWL) * __value__(UHATL))
      OpenAD_lin_237 = OpenAD_aux_32
      OpenAD_lin_239 = __value__(NZHAT)
      OpenAD_lin_240 = __value__(PRIML(1))
      OpenAD_lin_241 = __value__(UHATL)
      OpenAD_lin_242 = __value__(RWL)
      OpenAD_lin_238 = __value__(NSIZE)
      __value__(FLUX(5)) = (__value__(NSIZE) * OpenAD_aux_32)
      CALL setderiv(__deriv__(OpenAD_prp_28), __deriv__(DSS5))
      CALL saxpy(OpenAD_lin_239, __deriv__(PRIML(1)), __deriv__(
     > OpenAD_prp_28))
      CALL saxpy(OpenAD_lin_240, __deriv__(NZHAT), __deriv__(
     > OpenAD_prp_28))
      CALL saxpy(OpenAD_lin_241, __deriv__(RWL), __deriv__(
     > OpenAD_prp_28))
      CALL saxpy(OpenAD_lin_242, __deriv__(UHATL), __deriv__(
     > OpenAD_prp_28))
      CALL sax(OpenAD_lin_237, __deriv__(NSIZE), __deriv__(FLUX(5)))
      CALL saxpy(OpenAD_lin_238, __deriv__(OpenAD_prp_28), __deriv__(
     > FLUX(5)))
      END SUBROUTINE
