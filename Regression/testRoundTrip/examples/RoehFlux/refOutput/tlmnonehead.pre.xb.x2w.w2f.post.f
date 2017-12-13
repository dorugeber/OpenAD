
C$OPENAD XXX File_start [head.f]
      SUBROUTINE ad_roehf5(NRM, PRIML, PRIMR, GAMMA, GM1, GM1INV, NLEFIX
     +, LEFIX, MCHEPS, FLUX)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: NRM(1:3)
      INTENT(IN) NRM
      type(active) :: PRIML(1:5)
      INTENT(IN) PRIML
      type(active) :: PRIMR(1:5)
      INTENT(IN) PRIMR
      type(active) :: GAMMA
      INTENT(IN) GAMMA
      type(active) :: GM1
      INTENT(IN) GM1
      type(active) :: GM1INV
      INTENT(IN) GM1INV
      type(active) :: NLEFIX
      INTENT(IN) NLEFIX
      type(active) :: LEFIX
      INTENT(IN) LEFIX
      REAL(w2f__8) MCHEPS
      INTENT(IN) MCHEPS
      type(active) :: FLUX(1:5)
      INTENT(OUT) FLUX
C
C     **** Local Variables and Functions ****
C
      type(active) :: ALAMCM
      type(active) :: ALAMCP
      type(active) :: ALAMU
      type(active) :: ALP1
      type(active) :: ALP15M
      type(active) :: ALP15P
      type(active) :: ALP2
      type(active) :: ALP3
      type(active) :: ALP4
      type(active) :: ALP5
      type(active) :: CAVE
      type(active) :: DE
      type(active) :: DELTA
      type(active) :: DELTA2
      type(active) :: DR
      type(active) :: DRU
      type(active) :: DRV
      type(active) :: DRW
      type(active) :: DSS1
      type(active) :: DSS2
      type(active) :: DSS3
      type(active) :: DSS4
      type(active) :: DSS5
      type(active) :: EL
      type(active) :: ER
      REAL(w2f__4) HALF
      PARAMETER ( HALF = 5.0E-01)
      type(active) :: HAVE
      type(active) :: HL
      type(active) :: HR
      type(active) :: LAM2
      type(active) :: LAMCM
      type(active) :: LAMCP
      type(active) :: LAMU
      type(active) :: MU
      type(active) :: NDDRU
      type(active) :: NSIZE
      type(active) :: NSIZEI
      type(active) :: NXHAT
      type(active) :: NYHAT
      type(active) :: NZHAT
      type(active) :: OMEGA
      REAL(w2f__4) ONE
      PARAMETER ( ONE = 1.0)
      type(active) :: ROEL
      type(active) :: ROER
      type(active) :: RUL
      type(active) :: RUR
      type(active) :: RVL
      type(active) :: RVR
      type(active) :: RWL
      type(active) :: RWR
      type(active) :: THETAL
      type(active) :: THETAR
      type(active) :: THTAVE
      type(active) :: UAVE
      type(active) :: UDDRU
      type(active) :: UHAT
      type(active) :: UHATL
      type(active) :: UTILDE
      type(active) :: VAVE
      type(active) :: VTILDE
      type(active) :: WAVE
      type(active) :: WTILDE
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
      type(active) :: OpenAD_prp_0
      type(active) :: OpenAD_prp_1
      type(active) :: OpenAD_prp_10
      type(active) :: OpenAD_prp_11
      type(active) :: OpenAD_prp_12
      type(active) :: OpenAD_prp_13
      type(active) :: OpenAD_prp_14
      type(active) :: OpenAD_prp_15
      type(active) :: OpenAD_prp_16
      type(active) :: OpenAD_prp_17
      type(active) :: OpenAD_prp_18
      type(active) :: OpenAD_prp_19
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_20
      type(active) :: OpenAD_prp_21
      type(active) :: OpenAD_prp_22
      type(active) :: OpenAD_prp_23
      type(active) :: OpenAD_prp_24
      type(active) :: OpenAD_prp_25
      type(active) :: OpenAD_prp_26
      type(active) :: OpenAD_prp_27
      type(active) :: OpenAD_prp_28
      type(active) :: OpenAD_prp_29
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_30
      type(active) :: OpenAD_prp_31
      type(active) :: OpenAD_prp_4
      type(active) :: OpenAD_prp_5
      type(active) :: OpenAD_prp_6
      type(active) :: OpenAD_prp_7
      type(active) :: OpenAD_prp_8
      type(active) :: OpenAD_prp_9
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
      OpenAD_aux_0 = ((NRM(1)%v**2)+(NRM(2)%v**2)+(NRM(3)%v**2))
      OpenAD_lin_1 = (2*(NRM(1)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_2 = (2*(NRM(2)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_3 = (2*(NRM(3)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_0 = COS(OpenAD_aux_0)
      NSIZE%v = SIN(OpenAD_aux_0)
      OpenAD_acc_0 = (OpenAD_lin_3*OpenAD_lin_0)
      OpenAD_acc_1 = (OpenAD_lin_2*OpenAD_lin_0)
      OpenAD_acc_2 = (OpenAD_lin_1*OpenAD_lin_0)
      CALL sax(OpenAD_acc_0,NRM(3),NSIZE)
      CALL saxpy(OpenAD_acc_1,NRM(2),NSIZE)
      CALL saxpy(OpenAD_acc_2,NRM(1),NSIZE)
      IF (MCHEPS.LT.NSIZE%v) THEN
        OpenAD_lin_4 = (-(1.0D00/(NSIZE%v*NSIZE%v)))
        NSIZEI%v = (1.0D00/NSIZE%v)
        CALL sax(OpenAD_lin_4,NSIZE,NSIZEI)
      ELSE
        NSIZEI%v = 0.0D00
        CALL zero_deriv(NSIZEI)
      ENDIF
      OpenAD_lin_5 = NSIZEI%v
      OpenAD_lin_6 = NRM(1)%v
      NXHAT%v = (NRM(1)%v*NSIZEI%v)
      OpenAD_lin_7 = NSIZEI%v
      OpenAD_lin_8 = NRM(2)%v
      NYHAT%v = (NRM(2)%v*NSIZEI%v)
      OpenAD_lin_9 = NSIZEI%v
      OpenAD_lin_10 = NRM(3)%v
      NZHAT%v = (NRM(3)%v*NSIZEI%v)
      OpenAD_aux_2 = (PRIMR(2)%v/PRIML(2)%v)
      OpenAD_aux_1 = (SIN(OpenAD_aux_2)+1.0D00)
      OpenAD_lin_13 = (INT(1_w2f__i8)/PRIML(2)%v)
      OpenAD_lin_14 = (-(PRIMR(2)%v/(PRIML(2)%v*PRIML(2)%v)))
      OpenAD_lin_12 = COS(OpenAD_aux_2)
      OpenAD_lin_11 = (-(1.0D00/(OpenAD_aux_1*OpenAD_aux_1)))
      ROEL%v = (1.0D00/OpenAD_aux_1)
      ROER%v = (1.0D00-ROEL%v)
      OpenAD_aux_3 = ((PRIML(3)%v**2)+(PRIML(4)%v**2)+(PRIML(5)%v**2))
      OpenAD_lin_15 = (2*(PRIML(3)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_16 = (2*(PRIML(4)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_17 = (2*(PRIML(5)%v**(2-INT(1_w2f__i8))))
      THETAL%v = (OpenAD_aux_3*5.0D-01)
      OpenAD_aux_4 = ((PRIMR(3)%v**2)+(PRIMR(4)%v**2)+(PRIMR(5)%v**2))
      OpenAD_lin_18 = (2*(PRIMR(3)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_19 = (2*(PRIMR(4)%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_20 = (2*(PRIMR(5)%v**(2-INT(1_w2f__i8))))
      THETAR%v = (OpenAD_aux_4*5.0D-01)
      OpenAD_aux_6 = (GAMMA%v*GM1INV%v)
      OpenAD_aux_5 = (PRIML(1)%v*OpenAD_aux_6)
      OpenAD_lin_23 = OpenAD_aux_6
      OpenAD_lin_25 = GM1INV%v
      OpenAD_lin_26 = GAMMA%v
      OpenAD_lin_24 = PRIML(1)%v
      OpenAD_lin_21 = (INT(1_w2f__i8)/PRIML(2)%v)
      OpenAD_lin_22 = (-(OpenAD_aux_5/(PRIML(2)%v*PRIML(2)%v)))
      HL%v = (THETAL%v+(OpenAD_aux_5/PRIML(2)%v))
      OpenAD_aux_8 = (GAMMA%v*GM1INV%v)
      OpenAD_aux_7 = (PRIMR(1)%v*OpenAD_aux_8)
      OpenAD_lin_29 = OpenAD_aux_8
      OpenAD_lin_31 = GM1INV%v
      OpenAD_lin_32 = GAMMA%v
      OpenAD_lin_30 = PRIMR(1)%v
      OpenAD_lin_27 = (INT(1_w2f__i8)/PRIMR(2)%v)
      OpenAD_lin_28 = (-(OpenAD_aux_7/(PRIMR(2)%v*PRIMR(2)%v)))
      HR%v = (THETAR%v+(OpenAD_aux_7/PRIMR(2)%v))
      OpenAD_lin_33 = ROEL%v
      OpenAD_lin_34 = PRIML(3)%v
      OpenAD_lin_35 = ROER%v
      OpenAD_lin_36 = PRIMR(3)%v
      UAVE%v = (PRIML(3)%v*ROEL%v+PRIMR(3)%v*ROER%v)
      OpenAD_lin_37 = ROEL%v
      OpenAD_lin_38 = PRIML(4)%v
      OpenAD_lin_39 = ROER%v
      OpenAD_lin_40 = PRIMR(4)%v
      VAVE%v = (PRIML(4)%v*ROEL%v+PRIMR(4)%v*ROER%v)
      OpenAD_lin_41 = ROEL%v
      OpenAD_lin_42 = PRIML(5)%v
      OpenAD_lin_43 = ROER%v
      OpenAD_lin_44 = PRIMR(5)%v
      WAVE%v = (PRIML(5)%v*ROEL%v+PRIMR(5)%v*ROER%v)
      OpenAD_lin_45 = ROEL%v
      OpenAD_lin_46 = HL%v
      OpenAD_lin_47 = ROER%v
      OpenAD_lin_48 = HR%v
      HAVE%v = (HL%v*ROEL%v+HR%v*ROER%v)
      OpenAD_aux_9 = ((UAVE%v**2)+(VAVE%v**2)+(WAVE%v**2))
      OpenAD_lin_49 = (2*(UAVE%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_50 = (2*(VAVE%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_51 = (2*(WAVE%v**(2-INT(1_w2f__i8))))
      THTAVE%v = (OpenAD_aux_9*5.0D-01)
      OpenAD_aux_10 = (HAVE%v-THTAVE%v)
      OpenAD_lin_52 = OpenAD_aux_10
      OpenAD_lin_53 = GM1%v
      CAVE%v = (GM1%v*OpenAD_aux_10)
      OpenAD_acc_3 = (OpenAD_lin_30*OpenAD_lin_27)
      OpenAD_acc_4 = (OpenAD_lin_24*OpenAD_lin_21)
      OpenAD_acc_5 = (5.0D-01*INT((-1_w2f__i8))*OpenAD_lin_53)
      OpenAD_acc_6 = (OpenAD_lin_47*OpenAD_lin_53)
      OpenAD_acc_7 = (OpenAD_lin_45*OpenAD_lin_53)
      OpenAD_acc_8 = (OpenAD_lin_11*(OpenAD_lin_46*OpenAD_lin_53+INT((-1
     +_w2f__i8))*OpenAD_lin_48*OpenAD_lin_53))
      OpenAD_acc_9 = (OpenAD_lin_11*(OpenAD_lin_42+INT((-1_w2f__i8))*Ope
     +nAD_lin_44))
      OpenAD_acc_10 = (OpenAD_lin_11*(OpenAD_lin_38+INT((-1_w2f__i8))*Op
     +enAD_lin_40))
      OpenAD_acc_11 = (OpenAD_lin_11*(OpenAD_lin_34+INT((-1_w2f__i8))*Op
     +enAD_lin_36))
      OpenAD_acc_12 = (OpenAD_lin_13*OpenAD_lin_12)
      OpenAD_acc_13 = (OpenAD_lin_14*OpenAD_lin_12)
      OpenAD_acc_14 = (OpenAD_lin_23*OpenAD_lin_21)
      OpenAD_acc_15 = (OpenAD_lin_25*OpenAD_acc_4)
      OpenAD_acc_16 = (OpenAD_lin_26*OpenAD_acc_4)
      OpenAD_acc_17 = (OpenAD_lin_29*OpenAD_lin_27)
      OpenAD_acc_18 = (OpenAD_lin_31*OpenAD_acc_3)
      OpenAD_acc_19 = (OpenAD_lin_32*OpenAD_acc_3)
      CALL sax(OpenAD_lin_5,NRM(1),NXHAT)
      CALL saxpy(OpenAD_lin_6,NSIZEI,NXHAT)
      CALL sax(OpenAD_lin_7,NRM(2),NYHAT)
      CALL saxpy(OpenAD_lin_8,NSIZEI,NYHAT)
      CALL sax(OpenAD_lin_9,NRM(3),NZHAT)
      CALL saxpy(OpenAD_lin_10,NSIZEI,NZHAT)
      CALL sax(OpenAD_acc_12,PRIMR(2),OpenAD_prp_0)
      CALL saxpy(OpenAD_acc_13,PRIML(2),OpenAD_prp_0)
      CALL sax(OpenAD_lin_17,PRIML(5),OpenAD_prp_1)
      CALL saxpy(OpenAD_lin_15,PRIML(3),OpenAD_prp_1)
      CALL saxpy(OpenAD_lin_16,PRIML(4),OpenAD_prp_1)
      CALL sax(OpenAD_lin_20,PRIMR(5),OpenAD_prp_2)
      CALL saxpy(OpenAD_lin_18,PRIMR(3),OpenAD_prp_2)
      CALL saxpy(OpenAD_lin_19,PRIMR(4),OpenAD_prp_2)
      CALL sax(5.0D-01,OpenAD_prp_1,THETAL)
      CALL sax(5.0D-01,OpenAD_prp_2,THETAR)
      CALL setderiv(HL,THETAL)
      CALL saxpy(OpenAD_lin_22,PRIML(2),HL)
      CALL saxpy(OpenAD_acc_14,PRIML(1),HL)
      CALL saxpy(OpenAD_acc_15,GAMMA,HL)
      CALL saxpy(OpenAD_acc_16,GM1INV,HL)
      CALL setderiv(HR,THETAR)
      CALL saxpy(OpenAD_lin_28,PRIMR(2),HR)
      CALL saxpy(OpenAD_acc_17,PRIMR(1),HR)
      CALL saxpy(OpenAD_acc_18,GAMMA,HR)
      CALL saxpy(OpenAD_acc_19,GM1INV,HR)
      CALL sax(OpenAD_lin_35,PRIMR(3),UAVE)
      CALL saxpy(OpenAD_acc_11,OpenAD_prp_0,UAVE)
      CALL saxpy(OpenAD_lin_33,PRIML(3),UAVE)
      CALL sax(OpenAD_lin_39,PRIMR(4),VAVE)
      CALL saxpy(OpenAD_lin_37,PRIML(4),VAVE)
      CALL saxpy(OpenAD_acc_10,OpenAD_prp_0,VAVE)
      CALL sax(OpenAD_lin_43,PRIMR(5),WAVE)
      CALL saxpy(OpenAD_lin_41,PRIML(5),WAVE)
      CALL saxpy(OpenAD_acc_9,OpenAD_prp_0,WAVE)
      CALL sax(OpenAD_lin_51,WAVE,OpenAD_prp_3)
      CALL saxpy(OpenAD_lin_50,VAVE,OpenAD_prp_3)
      CALL saxpy(OpenAD_lin_49,UAVE,OpenAD_prp_3)
      CALL sax(5.0D-01,OpenAD_prp_3,THTAVE)
      CALL sax(OpenAD_lin_52,GM1,CAVE)
      CALL saxpy(OpenAD_acc_5,OpenAD_prp_3,CAVE)
      CALL saxpy(OpenAD_acc_6,HR,CAVE)
      CALL saxpy(OpenAD_acc_7,HL,CAVE)
      CALL saxpy(OpenAD_acc_8,OpenAD_prp_0,CAVE)
      OpenAD_lin_54 = COS(CAVE%v)
      CAVE%v = SIN(CAVE%v)
      OpenAD_lin_55 = UAVE%v
      OpenAD_lin_56 = NXHAT%v
      OpenAD_lin_57 = VAVE%v
      OpenAD_lin_58 = NYHAT%v
      OpenAD_lin_59 = WAVE%v
      OpenAD_lin_60 = NZHAT%v
      UHAT%v = (NXHAT%v*UAVE%v+NYHAT%v*VAVE%v+NZHAT%v*WAVE%v)
      OpenAD_lin_61 = WAVE%v
      OpenAD_lin_62 = NYHAT%v
      OpenAD_lin_63 = VAVE%v
      OpenAD_lin_64 = NZHAT%v
      UTILDE%v = (NYHAT%v*WAVE%v-NZHAT%v*VAVE%v)
      OpenAD_lin_65 = UAVE%v
      OpenAD_lin_66 = NZHAT%v
      OpenAD_lin_67 = WAVE%v
      OpenAD_lin_68 = NXHAT%v
      VTILDE%v = (NZHAT%v*UAVE%v-NXHAT%v*WAVE%v)
      OpenAD_lin_69 = VAVE%v
      OpenAD_lin_70 = NXHAT%v
      OpenAD_lin_71 = UAVE%v
      OpenAD_lin_72 = NYHAT%v
      WTILDE%v = (NXHAT%v*VAVE%v-NYHAT%v*UAVE%v)
      LAMCM%v = (UHAT%v-CAVE%v)
      LAMU%v = UHAT%v
      LAMCP%v = (CAVE%v+UHAT%v)
      OpenAD_aux_11 = (CAVE%v+UHAT%v)
      OpenAD_lin_73 = OpenAD_aux_11
      OpenAD_lin_74 = NLEFIX%v
      DELTA%v = (NLEFIX%v*OpenAD_aux_11)
      OpenAD_lin_75 = (2*(DELTA%v**(2-INT(1_w2f__i8))))
      DELTA2%v = (DELTA%v**2)
      OpenAD_lin_76 = (2*(LAMCM%v**(2-INT(1_w2f__i8))))
      LAM2%v = (LAMCM%v**2)
      OpenAD_acc_20 = (OpenAD_lin_73*OpenAD_lin_75)
      OpenAD_acc_21 = (OpenAD_lin_74*OpenAD_lin_75)
      OpenAD_acc_22 = (OpenAD_lin_63*INT((-1_w2f__i8)))
      OpenAD_acc_23 = (OpenAD_lin_64*INT((-1_w2f__i8)))
      OpenAD_acc_24 = (OpenAD_lin_67*INT((-1_w2f__i8)))
      OpenAD_acc_25 = (OpenAD_lin_68*INT((-1_w2f__i8)))
      OpenAD_acc_26 = (OpenAD_lin_71*INT((-1_w2f__i8)))
      OpenAD_acc_27 = (OpenAD_lin_72*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_4,CAVE)
      CALL sax(OpenAD_lin_54,OpenAD_prp_4,CAVE)
      CALL sax(OpenAD_lin_61,NYHAT,UTILDE)
      CALL saxpy(OpenAD_lin_62,WAVE,UTILDE)
      CALL saxpy(OpenAD_acc_22,NZHAT,UTILDE)
      CALL saxpy(OpenAD_acc_23,VAVE,UTILDE)
      CALL sax(OpenAD_lin_65,NZHAT,VTILDE)
      CALL saxpy(OpenAD_lin_66,UAVE,VTILDE)
      CALL saxpy(OpenAD_acc_24,NXHAT,VTILDE)
      CALL saxpy(OpenAD_acc_25,WAVE,VTILDE)
      CALL sax(OpenAD_lin_69,NXHAT,WTILDE)
      CALL saxpy(OpenAD_lin_70,VAVE,WTILDE)
      CALL saxpy(OpenAD_acc_26,NYHAT,WTILDE)
      CALL saxpy(OpenAD_acc_27,UAVE,WTILDE)
      CALL sax(OpenAD_lin_55,NXHAT,UHAT)
      CALL saxpy(OpenAD_lin_56,UAVE,UHAT)
      CALL saxpy(OpenAD_lin_57,NYHAT,UHAT)
      CALL saxpy(OpenAD_lin_58,VAVE,UHAT)
      CALL saxpy(OpenAD_lin_59,NZHAT,UHAT)
      CALL saxpy(OpenAD_lin_60,WAVE,UHAT)
      CALL setderiv(LAMCM,UHAT)
      CALL dec_deriv(LAMCM,CAVE)
      CALL setderiv(LAMU,UHAT)
      CALL setderiv(LAMCP,CAVE)
      CALL inc_deriv(LAMCP,UHAT)
      CALL setderiv(OpenAD_prp_5,CAVE)
      CALL inc_deriv(OpenAD_prp_5,UHAT)
      CALL sax(OpenAD_lin_76,LAMCM,LAM2)
      CALL sax(OpenAD_acc_20,NLEFIX,DELTA2)
      CALL saxpy(OpenAD_acc_21,OpenAD_prp_5,DELTA2)
      IF (DELTA2%v.LT.LAM2%v) THEN
        ALAMCM%v = LAMCM%v
        CALL setderiv(ALAMCM,LAMCM)
      ELSE
        OpenAD_aux_38 = (DELTA2%v+LAM2%v)
        OpenAD_aux_37 = (OpenAD_aux_38*5.0D-01)
        OpenAD_lin_247 = (INT(1_w2f__i8)/DELTA2%v)
        OpenAD_lin_248 = (-(OpenAD_aux_37/(DELTA2%v*DELTA2%v)))
        ALAMCM%v = (OpenAD_aux_37/DELTA2%v)
        OpenAD_acc_80 = (5.0D-01*OpenAD_lin_247)
        CALL setderiv(OpenAD_prp_31,DELTA2)
        CALL inc_deriv(OpenAD_prp_31,LAM2)
        CALL sax(OpenAD_lin_248,DELTA2,ALAMCM)
        CALL saxpy(OpenAD_acc_80,OpenAD_prp_31,ALAMCM)
      ENDIF
      OpenAD_lin_77 = (2*(LAMCP%v**(2-INT(1_w2f__i8))))
      LAM2%v = (LAMCP%v**2)
      CALL sax(OpenAD_lin_77,LAMCP,LAM2)
      IF (DELTA2%v.LT.LAM2%v) THEN
        ALAMCP%v = LAMCP%v
        CALL setderiv(ALAMCP,LAMCP)
      ELSE
        OpenAD_aux_36 = (DELTA2%v+LAM2%v)
        OpenAD_aux_35 = (OpenAD_aux_36*5.0D-01)
        OpenAD_lin_245 = (INT(1_w2f__i8)/DELTA2%v)
        OpenAD_lin_246 = (-(OpenAD_aux_35/(DELTA2%v*DELTA2%v)))
        ALAMCP%v = (OpenAD_aux_35/DELTA2%v)
        OpenAD_acc_79 = (5.0D-01*OpenAD_lin_245)
        CALL setderiv(OpenAD_prp_30,DELTA2)
        CALL inc_deriv(OpenAD_prp_30,LAM2)
        CALL sax(OpenAD_lin_246,DELTA2,ALAMCP)
        CALL saxpy(OpenAD_acc_79,OpenAD_prp_30,ALAMCP)
      ENDIF
      OpenAD_aux_12 = (CAVE%v+UHAT%v)
      OpenAD_lin_78 = OpenAD_aux_12
      OpenAD_lin_79 = LEFIX%v
      DELTA%v = (LEFIX%v*OpenAD_aux_12)
      OpenAD_lin_80 = DELTA%v
      OpenAD_lin_81 = DELTA%v
      DELTA2%v = (DELTA%v*DELTA%v)
      OpenAD_lin_82 = (2*(LAMU%v**(2-INT(1_w2f__i8))))
      LAM2%v = (LAMU%v**2)
      OpenAD_acc_28 = (OpenAD_lin_80+OpenAD_lin_81)
      OpenAD_acc_29 = (OpenAD_lin_78*OpenAD_acc_28)
      OpenAD_acc_30 = (OpenAD_lin_79*OpenAD_acc_28)
      CALL setderiv(OpenAD_prp_6,CAVE)
      CALL inc_deriv(OpenAD_prp_6,UHAT)
      CALL sax(OpenAD_lin_82,LAMU,LAM2)
      CALL sax(OpenAD_acc_29,LEFIX,DELTA2)
      CALL saxpy(OpenAD_acc_30,OpenAD_prp_6,DELTA2)
      IF (DELTA2%v.LT.LAM2%v) THEN
        ALAMU%v = LAMU%v
        CALL setderiv(ALAMU,LAMU)
      ELSE
        OpenAD_aux_34 = (DELTA2%v+LAM2%v)
        OpenAD_aux_33 = (OpenAD_aux_34*5.0D-01)
        OpenAD_lin_243 = (INT(1_w2f__i8)/DELTA2%v)
        OpenAD_lin_244 = (-(OpenAD_aux_33/(DELTA2%v*DELTA2%v)))
        ALAMU%v = (OpenAD_aux_33/DELTA2%v)
        OpenAD_acc_78 = (5.0D-01*OpenAD_lin_243)
        CALL setderiv(OpenAD_prp_29,DELTA2)
        CALL inc_deriv(OpenAD_prp_29,LAM2)
        CALL sax(OpenAD_lin_244,DELTA2,ALAMU)
        CALL saxpy(OpenAD_acc_78,OpenAD_prp_29,ALAMU)
      ENDIF
      OpenAD_aux_13 = (LAMCM%v-ALAMCM%v)
      ALAMCM%v = (OpenAD_aux_13*5.0D-01)
      OpenAD_aux_14 = (LAMCP%v-ALAMCP%v)
      ALAMCP%v = (OpenAD_aux_14*5.0D-01)
      OpenAD_aux_15 = (LAMU%v-ALAMU%v)
      ALAMU%v = (OpenAD_aux_15*5.0D-01)
      OpenAD_lin_83 = GM1INV%v
      OpenAD_lin_84 = PRIML(1)%v
      OpenAD_lin_85 = THETAL%v
      OpenAD_lin_86 = PRIML(2)%v
      EL%v = (PRIML(1)%v*GM1INV%v+PRIML(2)%v*THETAL%v)
      OpenAD_lin_87 = PRIML(3)%v
      OpenAD_lin_88 = PRIML(2)%v
      RUL%v = (PRIML(2)%v*PRIML(3)%v)
      OpenAD_lin_89 = PRIML(4)%v
      OpenAD_lin_90 = PRIML(2)%v
      RVL%v = (PRIML(2)%v*PRIML(4)%v)
      OpenAD_lin_91 = PRIML(5)%v
      OpenAD_lin_92 = PRIML(2)%v
      RWL%v = (PRIML(2)%v*PRIML(5)%v)
      OpenAD_lin_93 = GM1INV%v
      OpenAD_lin_94 = PRIMR(1)%v
      OpenAD_lin_95 = THETAR%v
      OpenAD_lin_96 = PRIMR(2)%v
      ER%v = (PRIMR(1)%v*GM1INV%v+PRIMR(2)%v*THETAR%v)
      OpenAD_lin_97 = PRIMR(3)%v
      OpenAD_lin_98 = PRIMR(2)%v
      RUR%v = (PRIMR(2)%v*PRIMR(3)%v)
      OpenAD_lin_99 = PRIMR(4)%v
      OpenAD_lin_100 = PRIMR(2)%v
      RVR%v = (PRIMR(2)%v*PRIMR(4)%v)
      OpenAD_lin_101 = PRIMR(5)%v
      OpenAD_lin_102 = PRIMR(2)%v
      RWR%v = (PRIMR(2)%v*PRIMR(5)%v)
      DE%v = (ER%v-EL%v)
      DR%v = (PRIMR(2)%v-PRIML(2)%v)
      DRU%v = (RUR%v-RUL%v)
      DRV%v = (RVR%v-RVL%v)
      DRW%v = (RWR%v-RWL%v)
      OpenAD_lin_103 = UAVE%v
      OpenAD_lin_104 = DRU%v
      OpenAD_lin_105 = VAVE%v
      OpenAD_lin_106 = DRV%v
      OpenAD_lin_107 = WAVE%v
      OpenAD_lin_108 = DRW%v
      UDDRU%v = (DRU%v*UAVE%v+DRV%v*VAVE%v+DRW%v*WAVE%v)
      OpenAD_aux_16 = (GM1%v/CAVE%v)
      OpenAD_aux_17 = (DE%v+DR%v*THTAVE%v-UDDRU%v)
      OpenAD_lin_111 = (INT(1_w2f__i8)/CAVE%v)
      OpenAD_lin_112 = (-(GM1%v/(CAVE%v*CAVE%v)))
      OpenAD_lin_109 = OpenAD_aux_17
      OpenAD_lin_113 = THTAVE%v
      OpenAD_lin_114 = DR%v
      OpenAD_lin_110 = OpenAD_aux_16
      OMEGA%v = (OpenAD_aux_16*OpenAD_aux_17)
      OpenAD_lin_115 = NXHAT%v
      OpenAD_lin_116 = DRU%v
      OpenAD_lin_117 = NYHAT%v
      OpenAD_lin_118 = DRV%v
      OpenAD_lin_119 = NZHAT%v
      OpenAD_lin_120 = DRW%v
      NDDRU%v = (DRU%v*NXHAT%v+DRV%v*NYHAT%v+DRW%v*NZHAT%v)
      OpenAD_aux_18 = (OMEGA%v+DR%v*UHAT%v-NDDRU%v)
      OpenAD_lin_121 = UHAT%v
      OpenAD_lin_122 = DR%v
      ALP1%v = (OpenAD_aux_18*5.0D-01)
      OpenAD_aux_19 = (CAVE%v*NXHAT%v-UTILDE%v)
      OpenAD_aux_20 = (-NXHAT%v)
      OpenAD_lin_123 = NYHAT%v
      OpenAD_lin_124 = DRW%v
      OpenAD_lin_125 = OpenAD_aux_19
      OpenAD_lin_127 = NXHAT%v
      OpenAD_lin_128 = CAVE%v
      OpenAD_lin_126 = DR%v
      OpenAD_lin_129 = OpenAD_aux_20
      OpenAD_lin_130 = OMEGA%v
      OpenAD_lin_131 = NZHAT%v
      OpenAD_lin_132 = DRV%v
      ALP2%v = (DRW%v*NYHAT%v+DR%v*OpenAD_aux_19+OMEGA%v*OpenAD_aux_20-D
     +RV%v*NZHAT%v)
      OpenAD_aux_21 = (CAVE%v*NYHAT%v-VTILDE%v)
      OpenAD_aux_22 = (-NYHAT%v)
      OpenAD_lin_133 = NZHAT%v
      OpenAD_lin_134 = DRU%v
      OpenAD_lin_135 = OpenAD_aux_21
      OpenAD_lin_137 = NYHAT%v
      OpenAD_lin_138 = CAVE%v
      OpenAD_lin_136 = DR%v
      OpenAD_lin_139 = OpenAD_aux_22
      OpenAD_lin_140 = OMEGA%v
      OpenAD_lin_141 = NXHAT%v
      OpenAD_lin_142 = DRW%v
      ALP3%v = (DRU%v*NZHAT%v+DR%v*OpenAD_aux_21+OMEGA%v*OpenAD_aux_22-D
     +RW%v*NXHAT%v)
      OpenAD_aux_23 = (CAVE%v*NZHAT%v-WTILDE%v)
      OpenAD_aux_24 = (-NZHAT%v)
      OpenAD_lin_143 = NXHAT%v
      OpenAD_lin_144 = DRV%v
      OpenAD_lin_145 = OpenAD_aux_23
      OpenAD_lin_147 = NZHAT%v
      OpenAD_lin_148 = CAVE%v
      OpenAD_lin_146 = DR%v
      OpenAD_lin_149 = OpenAD_aux_24
      OpenAD_lin_150 = OMEGA%v
      OpenAD_lin_151 = NYHAT%v
      OpenAD_lin_152 = DRU%v
      ALP4%v = (DRV%v*NXHAT%v+DR%v*OpenAD_aux_23+OMEGA%v*OpenAD_aux_24-D
     +RU%v*NYHAT%v)
      OpenAD_aux_25 = (NDDRU%v+OMEGA%v-DR%v*UHAT%v)
      OpenAD_lin_153 = UHAT%v
      OpenAD_lin_154 = DR%v
      ALP5%v = (OpenAD_aux_25*5.0D-01)
      OpenAD_acc_31 = (OpenAD_lin_111*OpenAD_lin_109)
      OpenAD_acc_32 = (OpenAD_lin_112*OpenAD_lin_109)
      OpenAD_acc_33 = (OpenAD_lin_103*INT((-1_w2f__i8)))
      OpenAD_acc_34 = (OpenAD_lin_104*INT((-1_w2f__i8)))
      OpenAD_acc_35 = (OpenAD_lin_105*INT((-1_w2f__i8)))
      OpenAD_acc_36 = (OpenAD_lin_106*INT((-1_w2f__i8)))
      OpenAD_acc_37 = (OpenAD_lin_107*INT((-1_w2f__i8)))
      OpenAD_acc_38 = (OpenAD_lin_108*INT((-1_w2f__i8)))
      OpenAD_acc_39 = (INT((-1_w2f__i8))*OpenAD_lin_130)
      OpenAD_acc_40 = (OpenAD_lin_131*INT((-1_w2f__i8)))
      OpenAD_acc_41 = (OpenAD_lin_132*INT((-1_w2f__i8)))
      OpenAD_acc_42 = (INT((-1_w2f__i8))*OpenAD_lin_140)
      OpenAD_acc_43 = (OpenAD_lin_141*INT((-1_w2f__i8)))
      OpenAD_acc_44 = (OpenAD_lin_142*INT((-1_w2f__i8)))
      OpenAD_acc_45 = (INT((-1_w2f__i8))*OpenAD_lin_150)
      OpenAD_acc_46 = (OpenAD_lin_151*INT((-1_w2f__i8)))
      OpenAD_acc_47 = (OpenAD_lin_152*INT((-1_w2f__i8)))
      OpenAD_acc_48 = (OpenAD_lin_153*INT((-1_w2f__i8)))
      OpenAD_acc_49 = (OpenAD_lin_154*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_7,ALAMCM)
      CALL setderiv(OpenAD_prp_8,ALAMCP)
      CALL setderiv(OpenAD_prp_9,ALAMU)
      CALL setderiv(OpenAD_prp_10,LAMCM)
      CALL dec_deriv(OpenAD_prp_10,OpenAD_prp_7)
      CALL setderiv(OpenAD_prp_11,LAMCP)
      CALL dec_deriv(OpenAD_prp_11,OpenAD_prp_8)
      CALL setderiv(OpenAD_prp_12,LAMU)
      CALL dec_deriv(OpenAD_prp_12,OpenAD_prp_9)
      CALL sax(OpenAD_lin_87,PRIML(2),RUL)
      CALL saxpy(OpenAD_lin_88,PRIML(3),RUL)
      CALL sax(OpenAD_lin_89,PRIML(2),RVL)
      CALL saxpy(OpenAD_lin_90,PRIML(4),RVL)
      CALL sax(OpenAD_lin_91,PRIML(2),RWL)
      CALL saxpy(OpenAD_lin_92,PRIML(5),RWL)
      CALL setderiv(DR,PRIMR(2))
      CALL dec_deriv(DR,PRIML(2))
      CALL set_neg_deriv(DRU,RUL)
      CALL saxpy(OpenAD_lin_97,PRIMR(2),DRU)
      CALL saxpy(OpenAD_lin_98,PRIMR(3),DRU)
      CALL set_neg_deriv(DRV,RVL)
      CALL saxpy(OpenAD_lin_99,PRIMR(2),DRV)
      CALL saxpy(OpenAD_lin_100,PRIMR(4),DRV)
      CALL set_neg_deriv(DRW,RWL)
      CALL saxpy(OpenAD_lin_101,PRIMR(2),DRW)
      CALL saxpy(OpenAD_lin_102,PRIMR(5),DRW)
      CALL sax(5.0D-01,OpenAD_prp_10,ALAMCM)
      CALL sax(5.0D-01,OpenAD_prp_11,ALAMCP)
      CALL sax(5.0D-01,OpenAD_prp_12,ALAMU)
      CALL sax(OpenAD_lin_83,PRIML(1),EL)
      CALL saxpy(OpenAD_lin_84,GM1INV,EL)
      CALL saxpy(OpenAD_lin_85,PRIML(2),EL)
      CALL saxpy(OpenAD_lin_86,THETAL,EL)
      CALL set_neg_deriv(OpenAD_prp_13,UTILDE)
      CALL saxpy(OpenAD_lin_127,CAVE,OpenAD_prp_13)
      CALL saxpy(OpenAD_lin_128,NXHAT,OpenAD_prp_13)
      CALL set_neg_deriv(OpenAD_prp_14,VTILDE)
      CALL saxpy(OpenAD_lin_137,CAVE,OpenAD_prp_14)
      CALL saxpy(OpenAD_lin_138,NYHAT,OpenAD_prp_14)
      CALL set_neg_deriv(OpenAD_prp_15,WTILDE)
      CALL saxpy(OpenAD_lin_147,CAVE,OpenAD_prp_15)
      CALL saxpy(OpenAD_lin_148,NZHAT,OpenAD_prp_15)
      CALL set_neg_deriv(OpenAD_prp_16,EL)
      CALL saxpy(OpenAD_lin_93,PRIMR(1),OpenAD_prp_16)
      CALL saxpy(OpenAD_lin_94,GM1INV,OpenAD_prp_16)
      CALL saxpy(OpenAD_lin_95,PRIMR(2),OpenAD_prp_16)
      CALL saxpy(OpenAD_lin_96,THETAR,OpenAD_prp_16)
      CALL saxpy(OpenAD_lin_113,DR,OpenAD_prp_16)
      CALL saxpy(OpenAD_lin_114,THTAVE,OpenAD_prp_16)
      CALL saxpy(OpenAD_acc_33,DRU,OpenAD_prp_16)
      CALL saxpy(OpenAD_acc_34,UAVE,OpenAD_prp_16)
      CALL saxpy(OpenAD_acc_35,DRV,OpenAD_prp_16)
      CALL saxpy(OpenAD_acc_36,VAVE,OpenAD_prp_16)
      CALL saxpy(OpenAD_acc_37,DRW,OpenAD_prp_16)
      CALL saxpy(OpenAD_acc_38,WAVE,OpenAD_prp_16)
      CALL sax(OpenAD_lin_115,DRU,NDDRU)
      CALL saxpy(OpenAD_lin_116,NXHAT,NDDRU)
      CALL saxpy(OpenAD_lin_117,DRV,NDDRU)
      CALL saxpy(OpenAD_lin_118,NYHAT,NDDRU)
      CALL saxpy(OpenAD_lin_119,DRW,NDDRU)
      CALL saxpy(OpenAD_lin_120,NZHAT,NDDRU)
      CALL sax(OpenAD_lin_110,OpenAD_prp_16,OMEGA)
      CALL saxpy(OpenAD_acc_31,GM1,OMEGA)
      CALL saxpy(OpenAD_acc_32,CAVE,OMEGA)
      CALL setderiv(OpenAD_prp_17,OMEGA)
      CALL dec_deriv(OpenAD_prp_17,NDDRU)
      CALL saxpy(OpenAD_lin_121,DR,OpenAD_prp_17)
      CALL saxpy(OpenAD_lin_122,UHAT,OpenAD_prp_17)
      CALL setderiv(OpenAD_prp_18,NDDRU)
      CALL inc_deriv(OpenAD_prp_18,OMEGA)
      CALL saxpy(OpenAD_acc_48,DR,OpenAD_prp_18)
      CALL saxpy(OpenAD_acc_49,UHAT,OpenAD_prp_18)
      CALL sax(5.0D-01,OpenAD_prp_17,ALP1)
      CALL sax(5.0D-01,OpenAD_prp_18,ALP5)
      CALL sax(OpenAD_lin_123,DRW,ALP2)
      CALL saxpy(OpenAD_lin_124,NYHAT,ALP2)
      CALL saxpy(OpenAD_lin_125,DR,ALP2)
      CALL saxpy(OpenAD_lin_126,OpenAD_prp_13,ALP2)
      CALL saxpy(OpenAD_lin_129,OMEGA,ALP2)
      CALL saxpy(OpenAD_acc_39,NXHAT,ALP2)
      CALL saxpy(OpenAD_acc_40,DRV,ALP2)
      CALL saxpy(OpenAD_acc_41,NZHAT,ALP2)
      CALL sax(OpenAD_lin_133,DRU,ALP3)
      CALL saxpy(OpenAD_lin_134,NZHAT,ALP3)
      CALL saxpy(OpenAD_lin_135,DR,ALP3)
      CALL saxpy(OpenAD_lin_136,OpenAD_prp_14,ALP3)
      CALL saxpy(OpenAD_lin_139,OMEGA,ALP3)
      CALL saxpy(OpenAD_acc_42,NYHAT,ALP3)
      CALL saxpy(OpenAD_acc_43,DRW,ALP3)
      CALL saxpy(OpenAD_acc_44,NXHAT,ALP3)
      CALL sax(OpenAD_lin_143,DRV,ALP4)
      CALL saxpy(OpenAD_lin_144,NXHAT,ALP4)
      CALL saxpy(OpenAD_lin_145,DR,ALP4)
      CALL saxpy(OpenAD_lin_146,OpenAD_prp_15,ALP4)
      CALL saxpy(OpenAD_lin_149,OMEGA,ALP4)
      CALL saxpy(OpenAD_acc_45,NZHAT,ALP4)
      CALL saxpy(OpenAD_acc_46,DRU,ALP4)
      CALL saxpy(OpenAD_acc_47,NYHAT,ALP4)
      OpenAD_lin_155 = ALP1%v
      OpenAD_lin_156 = ALAMCM%v
      ALP1%v = (ALAMCM%v*ALP1%v)
      OpenAD_lin_157 = ALP2%v
      OpenAD_lin_158 = ALAMU%v
      ALP2%v = (ALAMU%v*ALP2%v)
      OpenAD_lin_159 = ALP3%v
      OpenAD_lin_160 = ALAMU%v
      ALP3%v = (ALAMU%v*ALP3%v)
      OpenAD_lin_161 = ALP4%v
      OpenAD_lin_162 = ALAMU%v
      ALP4%v = (ALAMU%v*ALP4%v)
      OpenAD_lin_163 = ALP5%v
      OpenAD_lin_164 = ALAMCP%v
      ALP5%v = (ALAMCP%v*ALP5%v)
      ALP15P%v = (ALP1%v+ALP5%v)
      ALP15M%v = (ALP1%v-ALP5%v)
      OpenAD_lin_165 = (INT(1_w2f__i8)/CAVE%v)
      OpenAD_lin_166 = (-(ALP15P%v/(CAVE%v*CAVE%v)))
      OpenAD_lin_167 = NXHAT%v
      OpenAD_lin_168 = ALP2%v
      OpenAD_lin_169 = NYHAT%v
      OpenAD_lin_170 = ALP3%v
      OpenAD_lin_171 = NZHAT%v
      OpenAD_lin_172 = ALP4%v
      MU%v = ((ALP15P%v/CAVE%v)+ALP2%v*NXHAT%v+ALP3%v*NYHAT%v+ALP4%v*NZH
     +AT%v)
      OpenAD_aux_26 = (GM1INV%v*CAVE%v)
      OpenAD_lin_173 = THTAVE%v
      OpenAD_lin_174 = MU%v
      OpenAD_lin_175 = OpenAD_aux_26
      OpenAD_lin_177 = CAVE%v
      OpenAD_lin_178 = GM1INV%v
      OpenAD_lin_176 = ALP15P%v
      OpenAD_lin_179 = UHAT%v
      OpenAD_lin_180 = ALP15M%v
      OpenAD_lin_181 = UTILDE%v
      OpenAD_lin_182 = ALP2%v
      OpenAD_lin_183 = VTILDE%v
      OpenAD_lin_184 = ALP3%v
      OpenAD_lin_185 = WTILDE%v
      OpenAD_lin_186 = ALP4%v
      DSS1%v = (MU%v*THTAVE%v+ALP15P%v*OpenAD_aux_26-ALP15M%v*UHAT%v+ALP
     +2%v*UTILDE%v+ALP3%v*VTILDE%v+ALP4%v*WTILDE%v)
      DSS2%v = MU%v
      OpenAD_lin_187 = NZHAT%v
      OpenAD_lin_188 = ALP3%v
      OpenAD_lin_189 = UAVE%v
      OpenAD_lin_190 = MU%v
      OpenAD_lin_191 = NXHAT%v
      OpenAD_lin_192 = ALP15M%v
      OpenAD_lin_193 = NYHAT%v
      OpenAD_lin_194 = ALP4%v
      DSS3%v = (ALP3%v*NZHAT%v+MU%v*UAVE%v-ALP15M%v*NXHAT%v-ALP4%v*NYHAT
     +%v)
      OpenAD_lin_195 = NXHAT%v
      OpenAD_lin_196 = ALP4%v
      OpenAD_lin_197 = VAVE%v
      OpenAD_lin_198 = MU%v
      OpenAD_lin_199 = NYHAT%v
      OpenAD_lin_200 = ALP15M%v
      OpenAD_lin_201 = NZHAT%v
      OpenAD_lin_202 = ALP2%v
      DSS4%v = (ALP4%v*NXHAT%v+MU%v*VAVE%v-ALP15M%v*NYHAT%v-ALP2%v*NZHAT
     +%v)
      OpenAD_lin_203 = NYHAT%v
      OpenAD_lin_204 = ALP2%v
      OpenAD_lin_205 = WAVE%v
      OpenAD_lin_206 = MU%v
      OpenAD_lin_207 = NZHAT%v
      OpenAD_lin_208 = ALP15M%v
      OpenAD_lin_209 = NXHAT%v
      OpenAD_lin_210 = ALP3%v
      DSS5%v = (ALP2%v*NYHAT%v+MU%v*WAVE%v-ALP15M%v*NZHAT%v-ALP3%v*NXHAT
     +%v)
      OpenAD_lin_211 = NXHAT%v
      OpenAD_lin_212 = PRIML(3)%v
      OpenAD_lin_213 = NYHAT%v
      OpenAD_lin_214 = PRIML(4)%v
      OpenAD_lin_215 = NZHAT%v
      OpenAD_lin_216 = PRIML(5)%v
      UHATL%v = (PRIML(3)%v*NXHAT%v+PRIML(4)%v*NYHAT%v+PRIML(5)%v*NZHAT%
     +v)
      OpenAD_aux_28 = (PRIML(1)%v+EL%v)
      OpenAD_aux_27 = (DSS1%v+UHATL%v*OpenAD_aux_28)
      OpenAD_lin_217 = OpenAD_aux_27
      OpenAD_lin_219 = OpenAD_aux_28
      OpenAD_lin_220 = UHATL%v
      OpenAD_lin_218 = NSIZE%v
      FLUX(1)%v = (NSIZE%v*OpenAD_aux_27)
      OpenAD_acc_50 = (OpenAD_lin_219*OpenAD_lin_218)
      OpenAD_acc_51 = (OpenAD_lin_220*OpenAD_lin_218)
      OpenAD_acc_52 = (OpenAD_lin_185*OpenAD_lin_218)
      OpenAD_acc_53 = (OpenAD_lin_186*OpenAD_lin_218)
      OpenAD_acc_54 = (OpenAD_lin_183*OpenAD_lin_218)
      OpenAD_acc_55 = (OpenAD_lin_184*OpenAD_lin_218)
      OpenAD_acc_56 = (OpenAD_lin_181*OpenAD_lin_218)
      OpenAD_acc_57 = (OpenAD_lin_182*OpenAD_lin_218)
      OpenAD_acc_58 = (OpenAD_lin_173*OpenAD_lin_218)
      OpenAD_acc_59 = (OpenAD_lin_174*OpenAD_lin_218)
      OpenAD_acc_60 = (OpenAD_lin_175*OpenAD_lin_218)
      OpenAD_acc_61 = (OpenAD_lin_177*OpenAD_lin_176*OpenAD_lin_218)
      OpenAD_acc_62 = (OpenAD_lin_178*OpenAD_lin_176*OpenAD_lin_218)
      OpenAD_acc_63 = (INT((-1_w2f__i8))*OpenAD_lin_218)
      OpenAD_acc_64 = (OpenAD_lin_179*OpenAD_acc_63)
      OpenAD_acc_65 = (OpenAD_lin_180*OpenAD_acc_63)
      OpenAD_acc_66 = (OpenAD_lin_193*INT((-1_w2f__i8)))
      OpenAD_acc_67 = (OpenAD_lin_194*INT((-1_w2f__i8)))
      OpenAD_acc_68 = (OpenAD_lin_201*INT((-1_w2f__i8)))
      OpenAD_acc_69 = (OpenAD_lin_202*INT((-1_w2f__i8)))
      OpenAD_acc_70 = (OpenAD_lin_209*INT((-1_w2f__i8)))
      OpenAD_acc_71 = (OpenAD_lin_210*INT((-1_w2f__i8)))
      OpenAD_acc_72 = (OpenAD_lin_207*INT((-1_w2f__i8)))
      OpenAD_acc_73 = (OpenAD_lin_208*INT((-1_w2f__i8)))
      OpenAD_acc_74 = (OpenAD_lin_199*INT((-1_w2f__i8)))
      OpenAD_acc_75 = (OpenAD_lin_200*INT((-1_w2f__i8)))
      OpenAD_acc_76 = (OpenAD_lin_191*INT((-1_w2f__i8)))
      OpenAD_acc_77 = (OpenAD_lin_192*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_19,ALP1)
      CALL setderiv(OpenAD_prp_20,ALP2)
      CALL setderiv(OpenAD_prp_21,ALP3)
      CALL setderiv(OpenAD_prp_22,ALP4)
      CALL setderiv(OpenAD_prp_23,ALP5)
      CALL sax(OpenAD_lin_155,ALAMCM,ALP1)
      CALL saxpy(OpenAD_lin_156,OpenAD_prp_19,ALP1)
      CALL sax(OpenAD_lin_157,ALAMU,ALP2)
      CALL saxpy(OpenAD_lin_158,OpenAD_prp_20,ALP2)
      CALL sax(OpenAD_lin_159,ALAMU,ALP3)
      CALL saxpy(OpenAD_lin_160,OpenAD_prp_21,ALP3)
      CALL sax(OpenAD_lin_161,ALAMU,ALP4)
      CALL saxpy(OpenAD_lin_162,OpenAD_prp_22,ALP4)
      CALL sax(OpenAD_lin_163,ALAMCP,ALP5)
      CALL saxpy(OpenAD_lin_164,OpenAD_prp_23,ALP5)
      CALL setderiv(ALP15P,ALP1)
      CALL inc_deriv(ALP15P,ALP5)
      CALL setderiv(ALP15M,ALP1)
      CALL dec_deriv(ALP15M,ALP5)
      CALL setderiv(OpenAD_prp_24,PRIML(1))
      CALL inc_deriv(OpenAD_prp_24,EL)
      CALL sax(OpenAD_lin_211,PRIML(3),UHATL)
      CALL saxpy(OpenAD_lin_212,NXHAT,UHATL)
      CALL saxpy(OpenAD_lin_213,PRIML(4),UHATL)
      CALL saxpy(OpenAD_lin_214,NYHAT,UHATL)
      CALL saxpy(OpenAD_lin_215,PRIML(5),UHATL)
      CALL saxpy(OpenAD_lin_216,NZHAT,UHATL)
      CALL sax(OpenAD_lin_165,ALP15P,MU)
      CALL saxpy(OpenAD_lin_166,CAVE,MU)
      CALL saxpy(OpenAD_lin_167,ALP2,MU)
      CALL saxpy(OpenAD_lin_168,NXHAT,MU)
      CALL saxpy(OpenAD_lin_169,ALP3,MU)
      CALL saxpy(OpenAD_lin_170,NYHAT,MU)
      CALL saxpy(OpenAD_lin_171,ALP4,MU)
      CALL saxpy(OpenAD_lin_172,NZHAT,MU)
      CALL setderiv(DSS2,MU)
      CALL sax(OpenAD_lin_189,MU,DSS3)
      CALL saxpy(OpenAD_lin_190,UAVE,DSS3)
      CALL saxpy(OpenAD_acc_66,ALP4,DSS3)
      CALL saxpy(OpenAD_acc_67,NYHAT,DSS3)
      CALL saxpy(OpenAD_lin_187,ALP3,DSS3)
      CALL saxpy(OpenAD_lin_188,NZHAT,DSS3)
      CALL saxpy(OpenAD_acc_76,ALP15M,DSS3)
      CALL saxpy(OpenAD_acc_77,NXHAT,DSS3)
      CALL sax(OpenAD_lin_197,MU,DSS4)
      CALL saxpy(OpenAD_lin_198,VAVE,DSS4)
      CALL saxpy(OpenAD_acc_68,ALP2,DSS4)
      CALL saxpy(OpenAD_acc_69,NZHAT,DSS4)
      CALL saxpy(OpenAD_lin_195,ALP4,DSS4)
      CALL saxpy(OpenAD_lin_196,NXHAT,DSS4)
      CALL saxpy(OpenAD_acc_74,ALP15M,DSS4)
      CALL saxpy(OpenAD_acc_75,NYHAT,DSS4)
      CALL sax(OpenAD_lin_205,MU,DSS5)
      CALL saxpy(OpenAD_lin_206,WAVE,DSS5)
      CALL saxpy(OpenAD_acc_70,ALP3,DSS5)
      CALL saxpy(OpenAD_acc_71,NXHAT,DSS5)
      CALL saxpy(OpenAD_lin_203,ALP2,DSS5)
      CALL saxpy(OpenAD_lin_204,NYHAT,DSS5)
      CALL saxpy(OpenAD_acc_72,ALP15M,DSS5)
      CALL saxpy(OpenAD_acc_73,NZHAT,DSS5)
      CALL sax(OpenAD_lin_217,NSIZE,FLUX(1))
      CALL saxpy(OpenAD_acc_50,UHATL,FLUX(1))
      CALL saxpy(OpenAD_acc_51,OpenAD_prp_24,FLUX(1))
      CALL saxpy(OpenAD_acc_52,ALP4,FLUX(1))
      CALL saxpy(OpenAD_acc_53,WTILDE,FLUX(1))
      CALL saxpy(OpenAD_acc_54,ALP3,FLUX(1))
      CALL saxpy(OpenAD_acc_55,VTILDE,FLUX(1))
      CALL saxpy(OpenAD_acc_56,ALP2,FLUX(1))
      CALL saxpy(OpenAD_acc_57,UTILDE,FLUX(1))
      CALL saxpy(OpenAD_acc_58,MU,FLUX(1))
      CALL saxpy(OpenAD_acc_59,THTAVE,FLUX(1))
      CALL saxpy(OpenAD_acc_60,ALP15P,FLUX(1))
      CALL saxpy(OpenAD_acc_61,GM1INV,FLUX(1))
      CALL saxpy(OpenAD_acc_62,CAVE,FLUX(1))
      CALL saxpy(OpenAD_acc_64,ALP15M,FLUX(1))
      CALL saxpy(OpenAD_acc_65,UHAT,FLUX(1))
      OpenAD_aux_29 = (DSS2%v+PRIML(2)%v*UHATL%v)
      OpenAD_lin_221 = OpenAD_aux_29
      OpenAD_lin_223 = UHATL%v
      OpenAD_lin_224 = PRIML(2)%v
      OpenAD_lin_222 = NSIZE%v
      FLUX(2)%v = (NSIZE%v*OpenAD_aux_29)
      CALL setderiv(OpenAD_prp_25,DSS2)
      CALL saxpy(OpenAD_lin_223,PRIML(2),OpenAD_prp_25)
      CALL saxpy(OpenAD_lin_224,UHATL,OpenAD_prp_25)
      CALL sax(OpenAD_lin_221,NSIZE,FLUX(2))
      CALL saxpy(OpenAD_lin_222,OpenAD_prp_25,FLUX(2))
      OpenAD_aux_30 = (DSS3%v+PRIML(1)%v*NXHAT%v+RUL%v*UHATL%v)
      OpenAD_lin_225 = OpenAD_aux_30
      OpenAD_lin_227 = NXHAT%v
      OpenAD_lin_228 = PRIML(1)%v
      OpenAD_lin_229 = UHATL%v
      OpenAD_lin_230 = RUL%v
      OpenAD_lin_226 = NSIZE%v
      FLUX(3)%v = (NSIZE%v*OpenAD_aux_30)
      CALL setderiv(OpenAD_prp_26,DSS3)
      CALL saxpy(OpenAD_lin_227,PRIML(1),OpenAD_prp_26)
      CALL saxpy(OpenAD_lin_228,NXHAT,OpenAD_prp_26)
      CALL saxpy(OpenAD_lin_229,RUL,OpenAD_prp_26)
      CALL saxpy(OpenAD_lin_230,UHATL,OpenAD_prp_26)
      CALL sax(OpenAD_lin_225,NSIZE,FLUX(3))
      CALL saxpy(OpenAD_lin_226,OpenAD_prp_26,FLUX(3))
      OpenAD_aux_31 = (DSS4%v+PRIML(1)%v*NYHAT%v+RVL%v*UHATL%v)
      OpenAD_lin_231 = OpenAD_aux_31
      OpenAD_lin_233 = NYHAT%v
      OpenAD_lin_234 = PRIML(1)%v
      OpenAD_lin_235 = UHATL%v
      OpenAD_lin_236 = RVL%v
      OpenAD_lin_232 = NSIZE%v
      FLUX(4)%v = (NSIZE%v*OpenAD_aux_31)
      CALL setderiv(OpenAD_prp_27,DSS4)
      CALL saxpy(OpenAD_lin_233,PRIML(1),OpenAD_prp_27)
      CALL saxpy(OpenAD_lin_234,NYHAT,OpenAD_prp_27)
      CALL saxpy(OpenAD_lin_235,RVL,OpenAD_prp_27)
      CALL saxpy(OpenAD_lin_236,UHATL,OpenAD_prp_27)
      CALL sax(OpenAD_lin_231,NSIZE,FLUX(4))
      CALL saxpy(OpenAD_lin_232,OpenAD_prp_27,FLUX(4))
      OpenAD_aux_32 = (DSS5%v+PRIML(1)%v*NZHAT%v+RWL%v*UHATL%v)
      OpenAD_lin_237 = OpenAD_aux_32
      OpenAD_lin_239 = NZHAT%v
      OpenAD_lin_240 = PRIML(1)%v
      OpenAD_lin_241 = UHATL%v
      OpenAD_lin_242 = RWL%v
      OpenAD_lin_238 = NSIZE%v
      FLUX(5)%v = (NSIZE%v*OpenAD_aux_32)
      CALL setderiv(OpenAD_prp_28,DSS5)
      CALL saxpy(OpenAD_lin_239,PRIML(1),OpenAD_prp_28)
      CALL saxpy(OpenAD_lin_240,NZHAT,OpenAD_prp_28)
      CALL saxpy(OpenAD_lin_241,RWL,OpenAD_prp_28)
      CALL saxpy(OpenAD_lin_242,UHATL,OpenAD_prp_28)
      CALL sax(OpenAD_lin_237,NSIZE,FLUX(5))
      CALL saxpy(OpenAD_lin_238,OpenAD_prp_28,FLUX(5))
      END SUBROUTINE
