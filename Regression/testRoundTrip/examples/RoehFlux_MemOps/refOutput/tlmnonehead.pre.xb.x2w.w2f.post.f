
C$OPENAD XXX File_start [head.f]
      SUBROUTINE ad_roehf5(NRM1, NRM2, NRM3, PRIML1, PRIML2, PRIML3, PRI
     +ML4, PRIML5, PRIMR1, PRIMR2, PRIMR3, PRIMR4, PRIMR5, GAMMA, GM1, G
     +M1INV, NLEFIX, LEFIX, MCHEPS, FLUX1, FLUX2, FLUX3, FLUX4, FLUX5)
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: NRM1
      INTENT(IN) NRM1
      type(active) :: NRM2
      INTENT(IN) NRM2
      type(active) :: NRM3
      INTENT(IN) NRM3
      type(active) :: PRIML1
      INTENT(IN) PRIML1
      type(active) :: PRIML2
      INTENT(IN) PRIML2
      type(active) :: PRIML3
      INTENT(IN) PRIML3
      type(active) :: PRIML4
      INTENT(IN) PRIML4
      type(active) :: PRIML5
      INTENT(IN) PRIML5
      type(active) :: PRIMR1
      INTENT(IN) PRIMR1
      type(active) :: PRIMR2
      INTENT(IN) PRIMR2
      type(active) :: PRIMR3
      INTENT(IN) PRIMR3
      type(active) :: PRIMR4
      INTENT(IN) PRIMR4
      type(active) :: PRIMR5
      INTENT(IN) PRIMR5
      type(active) :: GAMMA
      INTENT(IN) GAMMA
      type(active) :: GM1
      INTENT(IN) GM1
      type(active) :: GM1INV
      INTENT(IN) GM1INV
      REAL(w2f__8) NLEFIX
      INTENT(IN) NLEFIX
      REAL(w2f__8) LEFIX
      INTENT(IN) LEFIX
      REAL(w2f__8) MCHEPS
      INTENT(IN) MCHEPS
      type(active) :: FLUX1
      INTENT(OUT) FLUX1
      type(active) :: FLUX2
      INTENT(OUT) FLUX2
      type(active) :: FLUX3
      INTENT(OUT) FLUX3
      type(active) :: FLUX4
      INTENT(OUT) FLUX4
      type(active) :: FLUX5
      INTENT(OUT) FLUX5
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
      type(active) :: OpenAD_prp_2
      type(active) :: OpenAD_prp_3
      type(active) :: OpenAD_prp_4
      type(active) :: OpenAD_prp_5
      type(active) :: OpenAD_prp_6
      type(active) :: OpenAD_prp_7
      type(active) :: OpenAD_prp_8
      type(active) :: OpenAD_prp_9
C
C     **** Top Level Pragmas ****
C
C$OPENAD INDEPENDENT(NRM1)
C$OPENAD INDEPENDENT(NRM2)
C$OPENAD INDEPENDENT(NRM3)
C$OPENAD INDEPENDENT(PRIML1)
C$OPENAD INDEPENDENT(PRIML2)
C$OPENAD INDEPENDENT(PRIML3)
C$OPENAD INDEPENDENT(PRIML4)
C$OPENAD INDEPENDENT(PRIML5)
C$OPENAD INDEPENDENT(PRIMR1)
C$OPENAD INDEPENDENT(PRIMR2)
C$OPENAD INDEPENDENT(PRIMR3)
C$OPENAD INDEPENDENT(PRIMR4)
C$OPENAD INDEPENDENT(PRIMR5)
C$OPENAD INDEPENDENT(GAMMA)
C$OPENAD INDEPENDENT(GM1)
C$OPENAD INDEPENDENT(GM1INV)
C$OPENAD INDEPENDENT(NLEFIX)
C$OPENAD INDEPENDENT(LEFIX)
C$OPENAD INDEPENDENT(MCHEPS)
C$OPENAD DEPENDENT(FLUX1)
C$OPENAD DEPENDENT(FLUX2)
C$OPENAD DEPENDENT(FLUX3)
C$OPENAD DEPENDENT(FLUX4)
C$OPENAD DEPENDENT(FLUX5)
C
C     **** Statements ****
C
C$OPENAD XXX Template ad_template.f
      OpenAD_aux_0 = ((NRM1%v**2)+(NRM2%v**2)+(NRM3%v**2))
      OpenAD_lin_1 = (2*(NRM1%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_2 = (2*(NRM2%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_3 = (2*(NRM3%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_0 = COS(OpenAD_aux_0)
      NSIZE%v = SIN(OpenAD_aux_0)
      OpenAD_lin_4 = (-(1.0D00/(NSIZE%v*NSIZE%v)))
      NSIZEI%v = (1.0D00/NSIZE%v)
      OpenAD_lin_5 = NSIZEI%v
      OpenAD_lin_6 = NRM1%v
      NXHAT%v = (NRM1%v*NSIZEI%v)
      OpenAD_lin_7 = NSIZEI%v
      OpenAD_lin_8 = NRM2%v
      NYHAT%v = (NRM2%v*NSIZEI%v)
      OpenAD_lin_9 = NSIZEI%v
      OpenAD_lin_10 = NRM3%v
      NZHAT%v = (NRM3%v*NSIZEI%v)
      OpenAD_aux_2 = (PRIMR2%v/PRIML2%v)
      OpenAD_aux_1 = (SIN(OpenAD_aux_2)+1.0D00)
      OpenAD_lin_13 = (INT(1_w2f__i8)/PRIML2%v)
      OpenAD_lin_14 = (-(PRIMR2%v/(PRIML2%v*PRIML2%v)))
      OpenAD_lin_12 = COS(OpenAD_aux_2)
      OpenAD_lin_11 = (-(1.0D00/(OpenAD_aux_1*OpenAD_aux_1)))
      ROEL%v = (1.0D00/OpenAD_aux_1)
      ROER%v = (1.0D00-ROEL%v)
      OpenAD_aux_3 = ((PRIML3%v**2)+(PRIML4%v**2)+(PRIML5%v**2))
      OpenAD_lin_15 = (2*(PRIML3%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_16 = (2*(PRIML4%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_17 = (2*(PRIML5%v**(2-INT(1_w2f__i8))))
      THETAL%v = (OpenAD_aux_3*5.0D-01)
      OpenAD_aux_4 = ((PRIMR3%v**2)+(PRIMR4%v**2)+(PRIMR5%v**2))
      OpenAD_lin_18 = (2*(PRIMR3%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_19 = (2*(PRIMR4%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_20 = (2*(PRIMR5%v**(2-INT(1_w2f__i8))))
      THETAR%v = (OpenAD_aux_4*5.0D-01)
      OpenAD_aux_6 = (GAMMA%v*GM1INV%v)
      OpenAD_aux_5 = (PRIML1%v*OpenAD_aux_6)
      OpenAD_lin_23 = OpenAD_aux_6
      OpenAD_lin_25 = GM1INV%v
      OpenAD_lin_26 = GAMMA%v
      OpenAD_lin_24 = PRIML1%v
      OpenAD_lin_21 = (INT(1_w2f__i8)/PRIML2%v)
      OpenAD_lin_22 = (-(OpenAD_aux_5/(PRIML2%v*PRIML2%v)))
      HL%v = (THETAL%v+(OpenAD_aux_5/PRIML2%v))
      OpenAD_aux_8 = (GAMMA%v*GM1INV%v)
      OpenAD_aux_7 = (PRIMR1%v*OpenAD_aux_8)
      OpenAD_lin_29 = OpenAD_aux_8
      OpenAD_lin_31 = GM1INV%v
      OpenAD_lin_32 = GAMMA%v
      OpenAD_lin_30 = PRIMR1%v
      OpenAD_lin_27 = (INT(1_w2f__i8)/PRIMR2%v)
      OpenAD_lin_28 = (-(OpenAD_aux_7/(PRIMR2%v*PRIMR2%v)))
      HR%v = (THETAR%v+(OpenAD_aux_7/PRIMR2%v))
      OpenAD_lin_33 = ROEL%v
      OpenAD_lin_34 = PRIML3%v
      OpenAD_lin_35 = ROER%v
      OpenAD_lin_36 = PRIMR3%v
      UAVE%v = (PRIML3%v*ROEL%v+PRIMR3%v*ROER%v)
      OpenAD_lin_37 = ROEL%v
      OpenAD_lin_38 = PRIML4%v
      OpenAD_lin_39 = ROER%v
      OpenAD_lin_40 = PRIMR4%v
      VAVE%v = (PRIML4%v*ROEL%v+PRIMR4%v*ROER%v)
      OpenAD_lin_41 = ROEL%v
      OpenAD_lin_42 = PRIML5%v
      OpenAD_lin_43 = ROER%v
      OpenAD_lin_44 = PRIMR5%v
      WAVE%v = (PRIML5%v*ROEL%v+PRIMR5%v*ROER%v)
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
      OpenAD_acc_0 = (OpenAD_lin_30*OpenAD_lin_27)
      OpenAD_acc_1 = (OpenAD_lin_24*OpenAD_lin_21)
      OpenAD_acc_2 = (OpenAD_lin_4*OpenAD_lin_6)
      OpenAD_acc_3 = (OpenAD_lin_4*OpenAD_lin_8)
      OpenAD_acc_4 = (OpenAD_lin_4*OpenAD_lin_10)
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
      OpenAD_acc_15 = (OpenAD_lin_25*OpenAD_acc_1)
      OpenAD_acc_16 = (OpenAD_lin_26*OpenAD_acc_1)
      OpenAD_acc_17 = (OpenAD_lin_29*OpenAD_lin_27)
      OpenAD_acc_18 = (OpenAD_lin_31*OpenAD_acc_0)
      OpenAD_acc_19 = (OpenAD_lin_32*OpenAD_acc_0)
      CALL sax(OpenAD_lin_3,NRM3,OpenAD_prp_0)
      CALL saxpy(OpenAD_lin_1,NRM1,OpenAD_prp_0)
      CALL saxpy(OpenAD_lin_2,NRM2,OpenAD_prp_0)
      CALL sax(OpenAD_acc_12,PRIMR2,OpenAD_prp_1)
      CALL saxpy(OpenAD_acc_13,PRIML2,OpenAD_prp_1)
      CALL sax(OpenAD_lin_17,PRIML5,OpenAD_prp_2)
      CALL saxpy(OpenAD_lin_15,PRIML3,OpenAD_prp_2)
      CALL saxpy(OpenAD_lin_16,PRIML4,OpenAD_prp_2)
      CALL sax(OpenAD_lin_20,PRIMR5,OpenAD_prp_3)
      CALL saxpy(OpenAD_lin_18,PRIMR3,OpenAD_prp_3)
      CALL saxpy(OpenAD_lin_19,PRIMR4,OpenAD_prp_3)
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,NSIZE)
      CALL sax(OpenAD_lin_5,NRM1,NXHAT)
      CALL saxpy(OpenAD_acc_2,NSIZE,NXHAT)
      CALL sax(OpenAD_lin_7,NRM2,NYHAT)
      CALL saxpy(OpenAD_acc_3,NSIZE,NYHAT)
      CALL sax(OpenAD_lin_9,NRM3,NZHAT)
      CALL saxpy(OpenAD_acc_4,NSIZE,NZHAT)
      CALL sax(5.0D-01,OpenAD_prp_2,THETAL)
      CALL sax(5.0D-01,OpenAD_prp_3,THETAR)
      CALL setderiv(HL,THETAL)
      CALL saxpy(OpenAD_lin_22,PRIML2,HL)
      CALL saxpy(OpenAD_acc_14,PRIML1,HL)
      CALL saxpy(OpenAD_acc_15,GAMMA,HL)
      CALL saxpy(OpenAD_acc_16,GM1INV,HL)
      CALL setderiv(HR,THETAR)
      CALL saxpy(OpenAD_lin_28,PRIMR2,HR)
      CALL saxpy(OpenAD_acc_17,PRIMR1,HR)
      CALL saxpy(OpenAD_acc_18,GAMMA,HR)
      CALL saxpy(OpenAD_acc_19,GM1INV,HR)
      CALL sax(OpenAD_lin_35,PRIMR3,UAVE)
      CALL saxpy(OpenAD_acc_11,OpenAD_prp_1,UAVE)
      CALL saxpy(OpenAD_lin_33,PRIML3,UAVE)
      CALL sax(OpenAD_lin_39,PRIMR4,VAVE)
      CALL saxpy(OpenAD_lin_37,PRIML4,VAVE)
      CALL saxpy(OpenAD_acc_10,OpenAD_prp_1,VAVE)
      CALL sax(OpenAD_lin_43,PRIMR5,WAVE)
      CALL saxpy(OpenAD_lin_41,PRIML5,WAVE)
      CALL saxpy(OpenAD_acc_9,OpenAD_prp_1,WAVE)
      CALL sax(OpenAD_lin_51,WAVE,OpenAD_prp_4)
      CALL saxpy(OpenAD_lin_50,VAVE,OpenAD_prp_4)
      CALL saxpy(OpenAD_lin_49,UAVE,OpenAD_prp_4)
      CALL sax(5.0D-01,OpenAD_prp_4,THTAVE)
      CALL sax(OpenAD_lin_52,GM1,CAVE)
      CALL saxpy(OpenAD_acc_5,OpenAD_prp_4,CAVE)
      CALL saxpy(OpenAD_acc_6,HR,CAVE)
      CALL saxpy(OpenAD_acc_7,HL,CAVE)
      CALL saxpy(OpenAD_acc_8,OpenAD_prp_1,CAVE)
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
      ALAMCM%v = LAMCM%v
      ALAMCP%v = LAMCP%v
      ALAMU%v = LAMU%v
      OpenAD_acc_20 = (OpenAD_lin_63*INT((-1_w2f__i8)))
      OpenAD_acc_21 = (OpenAD_lin_64*INT((-1_w2f__i8)))
      OpenAD_acc_22 = (OpenAD_lin_67*INT((-1_w2f__i8)))
      OpenAD_acc_23 = (OpenAD_lin_68*INT((-1_w2f__i8)))
      OpenAD_acc_24 = (OpenAD_lin_71*INT((-1_w2f__i8)))
      OpenAD_acc_25 = (OpenAD_lin_72*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_5,CAVE)
      CALL sax(OpenAD_lin_54,OpenAD_prp_5,CAVE)
      CALL sax(OpenAD_lin_61,NYHAT,UTILDE)
      CALL saxpy(OpenAD_lin_62,WAVE,UTILDE)
      CALL saxpy(OpenAD_acc_20,NZHAT,UTILDE)
      CALL saxpy(OpenAD_acc_21,VAVE,UTILDE)
      CALL sax(OpenAD_lin_65,NZHAT,VTILDE)
      CALL saxpy(OpenAD_lin_66,UAVE,VTILDE)
      CALL saxpy(OpenAD_acc_22,NXHAT,VTILDE)
      CALL saxpy(OpenAD_acc_23,WAVE,VTILDE)
      CALL sax(OpenAD_lin_69,NXHAT,WTILDE)
      CALL saxpy(OpenAD_lin_70,VAVE,WTILDE)
      CALL saxpy(OpenAD_acc_24,NYHAT,WTILDE)
      CALL saxpy(OpenAD_acc_25,UAVE,WTILDE)
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
      CALL setderiv(ALAMCM,LAMCM)
      CALL setderiv(ALAMCP,LAMCP)
      CALL setderiv(ALAMU,LAMU)
      OpenAD_aux_11 = (LAMCM%v-ALAMCM%v)
      ALAMCM%v = (OpenAD_aux_11*5.0D-01)
      OpenAD_aux_12 = (LAMCP%v-ALAMCP%v)
      ALAMCP%v = (OpenAD_aux_12*5.0D-01)
      OpenAD_aux_13 = (LAMU%v-ALAMU%v)
      ALAMU%v = (OpenAD_aux_13*5.0D-01)
      OpenAD_lin_73 = GM1INV%v
      OpenAD_lin_74 = PRIML1%v
      OpenAD_lin_75 = THETAL%v
      OpenAD_lin_76 = PRIML2%v
      EL%v = (PRIML1%v*GM1INV%v+PRIML2%v*THETAL%v)
      OpenAD_lin_77 = PRIML3%v
      OpenAD_lin_78 = PRIML2%v
      RUL%v = (PRIML2%v*PRIML3%v)
      OpenAD_lin_79 = PRIML4%v
      OpenAD_lin_80 = PRIML2%v
      RVL%v = (PRIML2%v*PRIML4%v)
      OpenAD_lin_81 = PRIML5%v
      OpenAD_lin_82 = PRIML2%v
      RWL%v = (PRIML2%v*PRIML5%v)
      OpenAD_lin_83 = GM1INV%v
      OpenAD_lin_84 = PRIMR1%v
      OpenAD_lin_85 = THETAR%v
      OpenAD_lin_86 = PRIMR2%v
      ER%v = (PRIMR1%v*GM1INV%v+PRIMR2%v*THETAR%v)
      OpenAD_lin_87 = PRIMR3%v
      OpenAD_lin_88 = PRIMR2%v
      RUR%v = (PRIMR2%v*PRIMR3%v)
      OpenAD_lin_89 = PRIMR4%v
      OpenAD_lin_90 = PRIMR2%v
      RVR%v = (PRIMR2%v*PRIMR4%v)
      OpenAD_lin_91 = PRIMR5%v
      OpenAD_lin_92 = PRIMR2%v
      RWR%v = (PRIMR2%v*PRIMR5%v)
      DE%v = (ER%v-EL%v)
      DR%v = (PRIMR2%v-PRIML2%v)
      DRU%v = (RUR%v-RUL%v)
      DRV%v = (RVR%v-RVL%v)
      DRW%v = (RWR%v-RWL%v)
      OpenAD_lin_93 = UAVE%v
      OpenAD_lin_94 = DRU%v
      OpenAD_lin_95 = VAVE%v
      OpenAD_lin_96 = DRV%v
      OpenAD_lin_97 = WAVE%v
      OpenAD_lin_98 = DRW%v
      UDDRU%v = (DRU%v*UAVE%v+DRV%v*VAVE%v+DRW%v*WAVE%v)
      OpenAD_aux_14 = (GM1%v/CAVE%v)
      OpenAD_aux_15 = (DE%v+DR%v*THTAVE%v-UDDRU%v)
      OpenAD_lin_101 = (INT(1_w2f__i8)/CAVE%v)
      OpenAD_lin_102 = (-(GM1%v/(CAVE%v*CAVE%v)))
      OpenAD_lin_99 = OpenAD_aux_15
      OpenAD_lin_103 = THTAVE%v
      OpenAD_lin_104 = DR%v
      OpenAD_lin_100 = OpenAD_aux_14
      OMEGA%v = (OpenAD_aux_14*OpenAD_aux_15)
      OpenAD_lin_105 = NXHAT%v
      OpenAD_lin_106 = DRU%v
      OpenAD_lin_107 = NYHAT%v
      OpenAD_lin_108 = DRV%v
      OpenAD_lin_109 = NZHAT%v
      OpenAD_lin_110 = DRW%v
      NDDRU%v = (DRU%v*NXHAT%v+DRV%v*NYHAT%v+DRW%v*NZHAT%v)
      OpenAD_aux_16 = (OMEGA%v+DR%v*UHAT%v-NDDRU%v)
      OpenAD_lin_111 = UHAT%v
      OpenAD_lin_112 = DR%v
      ALP1%v = (OpenAD_aux_16*5.0D-01)
      OpenAD_aux_17 = (CAVE%v*NXHAT%v-UTILDE%v)
      OpenAD_aux_18 = (-NXHAT%v)
      OpenAD_lin_113 = NYHAT%v
      OpenAD_lin_114 = DRW%v
      OpenAD_lin_115 = OpenAD_aux_17
      OpenAD_lin_117 = NXHAT%v
      OpenAD_lin_118 = CAVE%v
      OpenAD_lin_116 = DR%v
      OpenAD_lin_119 = OpenAD_aux_18
      OpenAD_lin_120 = OMEGA%v
      OpenAD_lin_121 = NZHAT%v
      OpenAD_lin_122 = DRV%v
      ALP2%v = (DRW%v*NYHAT%v+DR%v*OpenAD_aux_17+OMEGA%v*OpenAD_aux_18-D
     +RV%v*NZHAT%v)
      OpenAD_aux_19 = (CAVE%v*NYHAT%v-VTILDE%v)
      OpenAD_aux_20 = (-NYHAT%v)
      OpenAD_lin_123 = NZHAT%v
      OpenAD_lin_124 = DRU%v
      OpenAD_lin_125 = OpenAD_aux_19
      OpenAD_lin_127 = NYHAT%v
      OpenAD_lin_128 = CAVE%v
      OpenAD_lin_126 = DR%v
      OpenAD_lin_129 = OpenAD_aux_20
      OpenAD_lin_130 = OMEGA%v
      OpenAD_lin_131 = NXHAT%v
      OpenAD_lin_132 = DRW%v
      ALP3%v = (DRU%v*NZHAT%v+DR%v*OpenAD_aux_19+OMEGA%v*OpenAD_aux_20-D
     +RW%v*NXHAT%v)
      OpenAD_aux_21 = (CAVE%v*NZHAT%v-WTILDE%v)
      OpenAD_aux_22 = (-NZHAT%v)
      OpenAD_lin_133 = NXHAT%v
      OpenAD_lin_134 = DRV%v
      OpenAD_lin_135 = OpenAD_aux_21
      OpenAD_lin_137 = NZHAT%v
      OpenAD_lin_138 = CAVE%v
      OpenAD_lin_136 = DR%v
      OpenAD_lin_139 = OpenAD_aux_22
      OpenAD_lin_140 = OMEGA%v
      OpenAD_lin_141 = NYHAT%v
      OpenAD_lin_142 = DRU%v
      ALP4%v = (DRV%v*NXHAT%v+DR%v*OpenAD_aux_21+OMEGA%v*OpenAD_aux_22-D
     +RU%v*NYHAT%v)
      OpenAD_aux_23 = (NDDRU%v+OMEGA%v-DR%v*UHAT%v)
      OpenAD_lin_143 = UHAT%v
      OpenAD_lin_144 = DR%v
      ALP5%v = (OpenAD_aux_23*5.0D-01)
      OpenAD_acc_26 = (OpenAD_lin_101*OpenAD_lin_99)
      OpenAD_acc_27 = (OpenAD_lin_102*OpenAD_lin_99)
      OpenAD_acc_28 = (OpenAD_lin_144*(-5.0D-01))
      OpenAD_acc_29 = (OpenAD_lin_112*5.0D-01)
      OpenAD_acc_30 = (INT((-1_w2f__i8))*OpenAD_lin_100)
      OpenAD_acc_31 = (OpenAD_lin_97*OpenAD_acc_30)
      OpenAD_acc_32 = (OpenAD_lin_98*OpenAD_acc_30)
      OpenAD_acc_33 = (OpenAD_lin_137*OpenAD_lin_136)
      OpenAD_acc_34 = (OpenAD_lin_138*OpenAD_lin_136+INT((-1_w2f__i8))*O
     +penAD_lin_140)
      OpenAD_acc_35 = (INT((-1_w2f__i8))*OpenAD_lin_136)
      OpenAD_acc_36 = (OpenAD_lin_127*OpenAD_lin_126)
      OpenAD_acc_37 = (OpenAD_lin_128*OpenAD_lin_126+INT((-1_w2f__i8))*O
     +penAD_lin_130)
      OpenAD_acc_38 = (INT((-1_w2f__i8))*OpenAD_lin_126)
      OpenAD_acc_39 = (OpenAD_lin_117*OpenAD_lin_116)
      OpenAD_acc_40 = (OpenAD_lin_118*OpenAD_lin_116+INT((-1_w2f__i8))*O
     +penAD_lin_120)
      OpenAD_acc_41 = (INT((-1_w2f__i8))*OpenAD_lin_116)
      OpenAD_acc_42 = (OpenAD_lin_103*OpenAD_lin_100)
      OpenAD_acc_43 = (OpenAD_lin_104*OpenAD_lin_100)
      OpenAD_acc_44 = (OpenAD_lin_143*(-5.0D-01)+OpenAD_acc_42*5.0D-01)
      OpenAD_acc_45 = (OpenAD_lin_135+OpenAD_acc_42*OpenAD_lin_139)
      OpenAD_acc_46 = (OpenAD_lin_125+OpenAD_acc_42*OpenAD_lin_129)
      OpenAD_acc_47 = (OpenAD_lin_115+OpenAD_acc_42*OpenAD_lin_119)
      OpenAD_acc_48 = (OpenAD_lin_111*5.0D-01+OpenAD_acc_42*5.0D-01)
      OpenAD_acc_49 = (OpenAD_lin_95*OpenAD_acc_30)
      OpenAD_acc_50 = (OpenAD_lin_96*OpenAD_acc_30)
      OpenAD_acc_51 = (OpenAD_lin_93*OpenAD_acc_30)
      OpenAD_acc_52 = (OpenAD_lin_94*OpenAD_acc_30)
      OpenAD_acc_53 = (OpenAD_lin_141*INT((-1_w2f__i8)))
      OpenAD_acc_54 = (OpenAD_lin_142*INT((-1_w2f__i8)))
      OpenAD_acc_55 = (OpenAD_lin_131*INT((-1_w2f__i8)))
      OpenAD_acc_56 = (OpenAD_lin_132*INT((-1_w2f__i8)))
      OpenAD_acc_57 = (OpenAD_lin_121*INT((-1_w2f__i8)))
      OpenAD_acc_58 = (OpenAD_lin_122*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_6,ALAMCM)
      CALL setderiv(OpenAD_prp_7,ALAMCP)
      CALL setderiv(OpenAD_prp_8,ALAMU)
      CALL setderiv(OpenAD_prp_9,LAMCM)
      CALL dec_deriv(OpenAD_prp_9,OpenAD_prp_6)
      CALL setderiv(OpenAD_prp_10,LAMCP)
      CALL dec_deriv(OpenAD_prp_10,OpenAD_prp_7)
      CALL setderiv(OpenAD_prp_11,LAMU)
      CALL dec_deriv(OpenAD_prp_11,OpenAD_prp_8)
      CALL sax(OpenAD_lin_77,PRIML2,RUL)
      CALL saxpy(OpenAD_lin_78,PRIML3,RUL)
      CALL sax(OpenAD_lin_79,PRIML2,RVL)
      CALL saxpy(OpenAD_lin_80,PRIML4,RVL)
      CALL sax(OpenAD_lin_81,PRIML2,RWL)
      CALL saxpy(OpenAD_lin_82,PRIML5,RWL)
      CALL setderiv(DR,PRIMR2)
      CALL dec_deriv(DR,PRIML2)
      CALL set_neg_deriv(DRU,RUL)
      CALL saxpy(OpenAD_lin_87,PRIMR2,DRU)
      CALL saxpy(OpenAD_lin_88,PRIMR3,DRU)
      CALL set_neg_deriv(DRV,RVL)
      CALL saxpy(OpenAD_lin_89,PRIMR2,DRV)
      CALL saxpy(OpenAD_lin_90,PRIMR4,DRV)
      CALL set_neg_deriv(DRW,RWL)
      CALL saxpy(OpenAD_lin_91,PRIMR2,DRW)
      CALL saxpy(OpenAD_lin_92,PRIMR5,DRW)
      CALL sax(5.0D-01,OpenAD_prp_9,ALAMCM)
      CALL sax(5.0D-01,OpenAD_prp_10,ALAMCP)
      CALL sax(5.0D-01,OpenAD_prp_11,ALAMU)
      CALL sax(OpenAD_lin_73,PRIML1,EL)
      CALL saxpy(OpenAD_lin_74,GM1INV,EL)
      CALL saxpy(OpenAD_lin_75,PRIML2,EL)
      CALL saxpy(OpenAD_lin_76,THETAL,EL)
      CALL set_neg_deriv(DE,EL)
      CALL saxpy(OpenAD_lin_83,PRIMR1,DE)
      CALL saxpy(OpenAD_lin_84,GM1INV,DE)
      CALL saxpy(OpenAD_lin_85,PRIMR2,DE)
      CALL saxpy(OpenAD_lin_86,THETAR,DE)
      CALL sax(OpenAD_lin_109,DRW,NDDRU)
      CALL saxpy(OpenAD_lin_110,NZHAT,NDDRU)
      CALL saxpy(OpenAD_lin_107,DRV,NDDRU)
      CALL saxpy(OpenAD_lin_108,NYHAT,NDDRU)
      CALL saxpy(OpenAD_lin_105,DRU,NDDRU)
      CALL saxpy(OpenAD_lin_106,NXHAT,NDDRU)
      CALL sax(OpenAD_acc_26,GM1,OMEGA)
      CALL saxpy(OpenAD_acc_27,CAVE,OMEGA)
      CALL saxpy(OpenAD_acc_31,DRW,OMEGA)
      CALL saxpy(OpenAD_acc_32,WAVE,OMEGA)
      CALL saxpy(OpenAD_lin_100,DE,OMEGA)
      CALL saxpy(OpenAD_acc_43,THTAVE,OMEGA)
      CALL saxpy(OpenAD_acc_49,DRV,OMEGA)
      CALL saxpy(OpenAD_acc_50,VAVE,OMEGA)
      CALL saxpy(OpenAD_acc_51,DRU,OMEGA)
      CALL saxpy(OpenAD_acc_52,UAVE,OMEGA)
      CALL sax(-5.0D-01,NDDRU,ALP1)
      CALL saxpy(5.0D-01,OMEGA,ALP1)
      CALL saxpy(OpenAD_acc_48,DR,ALP1)
      CALL saxpy(OpenAD_acc_29,UHAT,ALP1)
      CALL sax(5.0D-01,NDDRU,ALP5)
      CALL saxpy(5.0D-01,OMEGA,ALP5)
      CALL saxpy(OpenAD_acc_44,DR,ALP5)
      CALL saxpy(OpenAD_acc_28,UHAT,ALP5)
      CALL sax(OpenAD_lin_119,OMEGA,ALP2)
      CALL saxpy(OpenAD_acc_47,DR,ALP2)
      CALL saxpy(OpenAD_acc_39,CAVE,ALP2)
      CALL saxpy(OpenAD_acc_40,NXHAT,ALP2)
      CALL saxpy(OpenAD_acc_41,UTILDE,ALP2)
      CALL saxpy(OpenAD_lin_113,DRW,ALP2)
      CALL saxpy(OpenAD_lin_114,NYHAT,ALP2)
      CALL saxpy(OpenAD_acc_57,DRV,ALP2)
      CALL saxpy(OpenAD_acc_58,NZHAT,ALP2)
      CALL sax(OpenAD_lin_129,OMEGA,ALP3)
      CALL saxpy(OpenAD_acc_46,DR,ALP3)
      CALL saxpy(OpenAD_acc_36,CAVE,ALP3)
      CALL saxpy(OpenAD_acc_37,NYHAT,ALP3)
      CALL saxpy(OpenAD_acc_38,VTILDE,ALP3)
      CALL saxpy(OpenAD_lin_123,DRU,ALP3)
      CALL saxpy(OpenAD_lin_124,NZHAT,ALP3)
      CALL saxpy(OpenAD_acc_55,DRW,ALP3)
      CALL saxpy(OpenAD_acc_56,NXHAT,ALP3)
      CALL sax(OpenAD_lin_139,OMEGA,ALP4)
      CALL saxpy(OpenAD_acc_45,DR,ALP4)
      CALL saxpy(OpenAD_acc_33,CAVE,ALP4)
      CALL saxpy(OpenAD_acc_34,NZHAT,ALP4)
      CALL saxpy(OpenAD_acc_35,WTILDE,ALP4)
      CALL saxpy(OpenAD_lin_133,DRV,ALP4)
      CALL saxpy(OpenAD_lin_134,NXHAT,ALP4)
      CALL saxpy(OpenAD_acc_53,DRU,ALP4)
      CALL saxpy(OpenAD_acc_54,NYHAT,ALP4)
      OpenAD_lin_145 = ALP1%v
      OpenAD_lin_146 = ALAMCM%v
      ALP1%v = (ALAMCM%v*ALP1%v)
      OpenAD_lin_147 = ALP2%v
      OpenAD_lin_148 = ALAMU%v
      ALP2%v = (ALAMU%v*ALP2%v)
      OpenAD_lin_149 = ALP3%v
      OpenAD_lin_150 = ALAMU%v
      ALP3%v = (ALAMU%v*ALP3%v)
      OpenAD_lin_151 = ALP4%v
      OpenAD_lin_152 = ALAMU%v
      ALP4%v = (ALAMU%v*ALP4%v)
      OpenAD_lin_153 = ALP5%v
      OpenAD_lin_154 = ALAMCP%v
      ALP5%v = (ALAMCP%v*ALP5%v)
      ALP15P%v = (ALP1%v+ALP5%v)
      ALP15M%v = (ALP1%v-ALP5%v)
      OpenAD_lin_155 = (INT(1_w2f__i8)/CAVE%v)
      OpenAD_lin_156 = (-(ALP15P%v/(CAVE%v*CAVE%v)))
      OpenAD_lin_157 = NXHAT%v
      OpenAD_lin_158 = ALP2%v
      OpenAD_lin_159 = NYHAT%v
      OpenAD_lin_160 = ALP3%v
      OpenAD_lin_161 = NZHAT%v
      OpenAD_lin_162 = ALP4%v
      MU%v = ((ALP15P%v/CAVE%v)+ALP2%v*NXHAT%v+ALP3%v*NYHAT%v+ALP4%v*NZH
     +AT%v)
      OpenAD_aux_24 = (GM1INV%v*CAVE%v)
      OpenAD_lin_163 = THTAVE%v
      OpenAD_lin_164 = MU%v
      OpenAD_lin_165 = OpenAD_aux_24
      OpenAD_lin_167 = CAVE%v
      OpenAD_lin_168 = GM1INV%v
      OpenAD_lin_166 = ALP15P%v
      OpenAD_lin_169 = UHAT%v
      OpenAD_lin_170 = ALP15M%v
      OpenAD_lin_171 = UTILDE%v
      OpenAD_lin_172 = ALP2%v
      OpenAD_lin_173 = VTILDE%v
      OpenAD_lin_174 = ALP3%v
      OpenAD_lin_175 = WTILDE%v
      OpenAD_lin_176 = ALP4%v
      DSS1%v = (MU%v*THTAVE%v+ALP15P%v*OpenAD_aux_24-ALP15M%v*UHAT%v+ALP
     +2%v*UTILDE%v+ALP3%v*VTILDE%v+ALP4%v*WTILDE%v)
      DSS2%v = MU%v
      OpenAD_lin_177 = NZHAT%v
      OpenAD_lin_178 = ALP3%v
      OpenAD_lin_179 = UAVE%v
      OpenAD_lin_180 = MU%v
      OpenAD_lin_181 = NXHAT%v
      OpenAD_lin_182 = ALP15M%v
      OpenAD_lin_183 = NYHAT%v
      OpenAD_lin_184 = ALP4%v
      DSS3%v = (ALP3%v*NZHAT%v+MU%v*UAVE%v-ALP15M%v*NXHAT%v-ALP4%v*NYHAT
     +%v)
      OpenAD_lin_185 = NXHAT%v
      OpenAD_lin_186 = ALP4%v
      OpenAD_lin_187 = VAVE%v
      OpenAD_lin_188 = MU%v
      OpenAD_lin_189 = NYHAT%v
      OpenAD_lin_190 = ALP15M%v
      OpenAD_lin_191 = NZHAT%v
      OpenAD_lin_192 = ALP2%v
      DSS4%v = (ALP4%v*NXHAT%v+MU%v*VAVE%v-ALP15M%v*NYHAT%v-ALP2%v*NZHAT
     +%v)
      OpenAD_lin_193 = NYHAT%v
      OpenAD_lin_194 = ALP2%v
      OpenAD_lin_195 = WAVE%v
      OpenAD_lin_196 = MU%v
      OpenAD_lin_197 = NZHAT%v
      OpenAD_lin_198 = ALP15M%v
      OpenAD_lin_199 = NXHAT%v
      OpenAD_lin_200 = ALP3%v
      DSS5%v = (ALP2%v*NYHAT%v+MU%v*WAVE%v-ALP15M%v*NZHAT%v-ALP3%v*NXHAT
     +%v)
      OpenAD_lin_201 = NXHAT%v
      OpenAD_lin_202 = PRIML3%v
      OpenAD_lin_203 = NYHAT%v
      OpenAD_lin_204 = PRIML4%v
      OpenAD_lin_205 = NZHAT%v
      OpenAD_lin_206 = PRIML5%v
      UHATL%v = (PRIML3%v*NXHAT%v+PRIML4%v*NYHAT%v+PRIML5%v*NZHAT%v)
      OpenAD_aux_26 = (PRIML1%v+EL%v)
      OpenAD_aux_25 = (DSS1%v+UHATL%v*OpenAD_aux_26)
      OpenAD_lin_207 = OpenAD_aux_25
      OpenAD_lin_209 = OpenAD_aux_26
      OpenAD_lin_210 = UHATL%v
      OpenAD_lin_208 = NSIZE%v
      FLUX1%v = (NSIZE%v*OpenAD_aux_25)
      OpenAD_aux_27 = (DSS2%v+PRIML2%v*UHATL%v)
      OpenAD_lin_211 = OpenAD_aux_27
      OpenAD_lin_213 = UHATL%v
      OpenAD_lin_214 = PRIML2%v
      OpenAD_lin_212 = NSIZE%v
      FLUX2%v = (NSIZE%v*OpenAD_aux_27)
      OpenAD_aux_28 = (DSS3%v+PRIML1%v*NXHAT%v+RUL%v*UHATL%v)
      OpenAD_lin_215 = OpenAD_aux_28
      OpenAD_lin_217 = NXHAT%v
      OpenAD_lin_218 = PRIML1%v
      OpenAD_lin_219 = UHATL%v
      OpenAD_lin_220 = RUL%v
      OpenAD_lin_216 = NSIZE%v
      FLUX3%v = (NSIZE%v*OpenAD_aux_28)
      OpenAD_aux_29 = (DSS4%v+PRIML1%v*NYHAT%v+RVL%v*UHATL%v)
      OpenAD_lin_221 = OpenAD_aux_29
      OpenAD_lin_223 = NYHAT%v
      OpenAD_lin_224 = PRIML1%v
      OpenAD_lin_225 = UHATL%v
      OpenAD_lin_226 = RVL%v
      OpenAD_lin_222 = NSIZE%v
      FLUX4%v = (NSIZE%v*OpenAD_aux_29)
      OpenAD_aux_30 = (DSS5%v+PRIML1%v*NZHAT%v+RWL%v*UHATL%v)
      OpenAD_lin_227 = OpenAD_aux_30
      OpenAD_lin_229 = NZHAT%v
      OpenAD_lin_230 = PRIML1%v
      OpenAD_lin_231 = UHATL%v
      OpenAD_lin_232 = RWL%v
      OpenAD_lin_228 = NSIZE%v
      FLUX5%v = (NSIZE%v*OpenAD_aux_30)
      OpenAD_acc_59 = (OpenAD_lin_209*OpenAD_lin_208)
      OpenAD_acc_60 = (OpenAD_lin_210*OpenAD_lin_208)
      OpenAD_acc_61 = (OpenAD_lin_175*OpenAD_lin_208)
      OpenAD_acc_62 = (OpenAD_lin_176*OpenAD_lin_208)
      OpenAD_acc_63 = (INT((-1_w2f__i8))*OpenAD_lin_228)
      OpenAD_acc_64 = (OpenAD_lin_199*OpenAD_acc_63)
      OpenAD_acc_65 = (OpenAD_lin_200*OpenAD_acc_63)
      OpenAD_acc_66 = (INT((-1_w2f__i8))*OpenAD_lin_222)
      OpenAD_acc_67 = (OpenAD_lin_191*OpenAD_acc_66)
      OpenAD_acc_68 = (OpenAD_lin_192*OpenAD_acc_66)
      OpenAD_acc_69 = (INT((-1_w2f__i8))*OpenAD_lin_216)
      OpenAD_acc_70 = (OpenAD_lin_183*OpenAD_acc_69)
      OpenAD_acc_71 = (OpenAD_lin_184*OpenAD_acc_69)
      OpenAD_acc_72 = (OpenAD_lin_173*OpenAD_lin_208)
      OpenAD_acc_73 = (OpenAD_lin_174*OpenAD_lin_208)
      OpenAD_acc_74 = (OpenAD_lin_193*OpenAD_lin_228)
      OpenAD_acc_75 = (OpenAD_lin_194*OpenAD_lin_228)
      OpenAD_acc_76 = (OpenAD_lin_185*OpenAD_lin_222)
      OpenAD_acc_77 = (OpenAD_lin_186*OpenAD_lin_222)
      OpenAD_acc_78 = (OpenAD_lin_177*OpenAD_lin_216)
      OpenAD_acc_79 = (OpenAD_lin_178*OpenAD_lin_216)
      OpenAD_acc_80 = (OpenAD_lin_171*OpenAD_lin_208)
      OpenAD_acc_81 = (OpenAD_lin_172*OpenAD_lin_208)
      OpenAD_acc_82 = (OpenAD_lin_195*OpenAD_lin_228)
      OpenAD_acc_83 = (OpenAD_lin_196*OpenAD_lin_228)
      OpenAD_acc_84 = (INT((-1_w2f__i8))*OpenAD_lin_228)
      OpenAD_acc_85 = (OpenAD_lin_197*OpenAD_acc_84)
      OpenAD_acc_86 = (OpenAD_lin_187*OpenAD_lin_222)
      OpenAD_acc_87 = (OpenAD_lin_188*OpenAD_lin_222)
      OpenAD_acc_88 = (INT((-1_w2f__i8))*OpenAD_lin_222)
      OpenAD_acc_89 = (OpenAD_lin_189*OpenAD_acc_88)
      OpenAD_acc_90 = (OpenAD_lin_179*OpenAD_lin_216)
      OpenAD_acc_91 = (OpenAD_lin_180*OpenAD_lin_216)
      OpenAD_acc_92 = (INT((-1_w2f__i8))*OpenAD_lin_216)
      OpenAD_acc_93 = (OpenAD_lin_181*OpenAD_acc_92)
      OpenAD_acc_94 = (OpenAD_lin_163*OpenAD_lin_208)
      OpenAD_acc_95 = (OpenAD_lin_164*OpenAD_lin_208)
      OpenAD_acc_96 = (OpenAD_lin_231*OpenAD_lin_228)
      OpenAD_acc_97 = (OpenAD_lin_232*OpenAD_lin_228)
      OpenAD_acc_98 = (OpenAD_lin_198*OpenAD_acc_84+OpenAD_lin_230*OpenA
     +D_lin_228)
      OpenAD_acc_99 = (OpenAD_lin_229*OpenAD_lin_228)
      OpenAD_acc_100 = (OpenAD_lin_225*OpenAD_lin_222)
      OpenAD_acc_101 = (OpenAD_lin_226*OpenAD_lin_222)
      OpenAD_acc_102 = (OpenAD_lin_190*OpenAD_acc_88+OpenAD_lin_224*Open
     +AD_lin_222)
      OpenAD_acc_103 = (OpenAD_lin_223*OpenAD_lin_222)
      OpenAD_acc_104 = (OpenAD_lin_219*OpenAD_lin_216)
      OpenAD_acc_105 = (OpenAD_lin_220*OpenAD_lin_216)
      OpenAD_acc_106 = (OpenAD_lin_182*OpenAD_acc_92+OpenAD_lin_218*Open
     +AD_lin_216)
      OpenAD_acc_107 = (OpenAD_lin_217*OpenAD_lin_216)
      OpenAD_acc_108 = (OpenAD_lin_213*OpenAD_lin_212)
      OpenAD_acc_109 = (OpenAD_lin_214*OpenAD_lin_212)
      OpenAD_acc_110 = (OpenAD_lin_165*OpenAD_lin_208)
      OpenAD_acc_111 = (OpenAD_lin_167*OpenAD_lin_166*OpenAD_lin_208)
      OpenAD_acc_112 = (OpenAD_lin_168*OpenAD_lin_166*OpenAD_lin_208)
      OpenAD_acc_113 = (INT((-1_w2f__i8))*OpenAD_lin_208)
      OpenAD_acc_114 = (OpenAD_lin_169*OpenAD_acc_113)
      OpenAD_acc_115 = (OpenAD_lin_170*OpenAD_acc_113)
      CALL setderiv(OpenAD_prp_12,ALP1)
      CALL setderiv(OpenAD_prp_13,ALP2)
      CALL setderiv(OpenAD_prp_14,ALP3)
      CALL setderiv(OpenAD_prp_15,ALP4)
      CALL setderiv(OpenAD_prp_16,ALP5)
      CALL sax(OpenAD_lin_145,ALAMCM,ALP1)
      CALL saxpy(OpenAD_lin_146,OpenAD_prp_12,ALP1)
      CALL sax(OpenAD_lin_147,ALAMU,ALP2)
      CALL saxpy(OpenAD_lin_148,OpenAD_prp_13,ALP2)
      CALL sax(OpenAD_lin_149,ALAMU,ALP3)
      CALL saxpy(OpenAD_lin_150,OpenAD_prp_14,ALP3)
      CALL sax(OpenAD_lin_151,ALAMU,ALP4)
      CALL saxpy(OpenAD_lin_152,OpenAD_prp_15,ALP4)
      CALL sax(OpenAD_lin_153,ALAMCP,ALP5)
      CALL saxpy(OpenAD_lin_154,OpenAD_prp_16,ALP5)
      CALL setderiv(ALP15P,ALP1)
      CALL inc_deriv(ALP15P,ALP5)
      CALL setderiv(ALP15M,ALP1)
      CALL dec_deriv(ALP15M,ALP5)
      CALL setderiv(OpenAD_prp_17,PRIML1)
      CALL inc_deriv(OpenAD_prp_17,EL)
      CALL sax(OpenAD_lin_201,PRIML3,UHATL)
      CALL saxpy(OpenAD_lin_202,NXHAT,UHATL)
      CALL saxpy(OpenAD_lin_203,PRIML4,UHATL)
      CALL saxpy(OpenAD_lin_204,NYHAT,UHATL)
      CALL saxpy(OpenAD_lin_205,PRIML5,UHATL)
      CALL saxpy(OpenAD_lin_206,NZHAT,UHATL)
      CALL sax(OpenAD_lin_155,ALP15P,MU)
      CALL saxpy(OpenAD_lin_156,CAVE,MU)
      CALL saxpy(OpenAD_lin_157,ALP2,MU)
      CALL saxpy(OpenAD_lin_158,NXHAT,MU)
      CALL saxpy(OpenAD_lin_159,ALP3,MU)
      CALL saxpy(OpenAD_lin_160,NYHAT,MU)
      CALL saxpy(OpenAD_lin_161,ALP4,MU)
      CALL saxpy(OpenAD_lin_162,NZHAT,MU)
      CALL sax(OpenAD_lin_211,NSIZE,FLUX2)
      CALL saxpy(OpenAD_acc_108,PRIML2,FLUX2)
      CALL saxpy(OpenAD_acc_109,UHATL,FLUX2)
      CALL saxpy(OpenAD_lin_212,MU,FLUX2)
      CALL sax(OpenAD_lin_215,NSIZE,FLUX3)
      CALL saxpy(OpenAD_acc_70,ALP4,FLUX3)
      CALL saxpy(OpenAD_acc_71,NYHAT,FLUX3)
      CALL saxpy(OpenAD_acc_78,ALP3,FLUX3)
      CALL saxpy(OpenAD_acc_79,NZHAT,FLUX3)
      CALL saxpy(OpenAD_acc_90,MU,FLUX3)
      CALL saxpy(OpenAD_acc_91,UAVE,FLUX3)
      CALL saxpy(OpenAD_acc_93,ALP15M,FLUX3)
      CALL saxpy(OpenAD_acc_106,NXHAT,FLUX3)
      CALL saxpy(OpenAD_acc_104,RUL,FLUX3)
      CALL saxpy(OpenAD_acc_105,UHATL,FLUX3)
      CALL saxpy(OpenAD_acc_107,PRIML1,FLUX3)
      CALL sax(OpenAD_lin_221,NSIZE,FLUX4)
      CALL saxpy(OpenAD_acc_67,ALP2,FLUX4)
      CALL saxpy(OpenAD_acc_68,NZHAT,FLUX4)
      CALL saxpy(OpenAD_acc_76,ALP4,FLUX4)
      CALL saxpy(OpenAD_acc_77,NXHAT,FLUX4)
      CALL saxpy(OpenAD_acc_86,MU,FLUX4)
      CALL saxpy(OpenAD_acc_87,VAVE,FLUX4)
      CALL saxpy(OpenAD_acc_89,ALP15M,FLUX4)
      CALL saxpy(OpenAD_acc_102,NYHAT,FLUX4)
      CALL saxpy(OpenAD_acc_100,RVL,FLUX4)
      CALL saxpy(OpenAD_acc_101,UHATL,FLUX4)
      CALL saxpy(OpenAD_acc_103,PRIML1,FLUX4)
      CALL sax(OpenAD_lin_227,NSIZE,FLUX5)
      CALL saxpy(OpenAD_acc_64,ALP3,FLUX5)
      CALL saxpy(OpenAD_acc_65,NXHAT,FLUX5)
      CALL saxpy(OpenAD_acc_74,ALP2,FLUX5)
      CALL saxpy(OpenAD_acc_75,NYHAT,FLUX5)
      CALL saxpy(OpenAD_acc_82,MU,FLUX5)
      CALL saxpy(OpenAD_acc_83,WAVE,FLUX5)
      CALL saxpy(OpenAD_acc_85,ALP15M,FLUX5)
      CALL saxpy(OpenAD_acc_98,NZHAT,FLUX5)
      CALL saxpy(OpenAD_acc_96,RWL,FLUX5)
      CALL saxpy(OpenAD_acc_97,UHATL,FLUX5)
      CALL saxpy(OpenAD_acc_99,PRIML1,FLUX5)
      CALL sax(OpenAD_lin_207,NSIZE,FLUX1)
      CALL saxpy(OpenAD_acc_59,UHATL,FLUX1)
      CALL saxpy(OpenAD_acc_60,OpenAD_prp_17,FLUX1)
      CALL saxpy(OpenAD_acc_61,ALP4,FLUX1)
      CALL saxpy(OpenAD_acc_62,WTILDE,FLUX1)
      CALL saxpy(OpenAD_acc_72,ALP3,FLUX1)
      CALL saxpy(OpenAD_acc_73,VTILDE,FLUX1)
      CALL saxpy(OpenAD_acc_80,ALP2,FLUX1)
      CALL saxpy(OpenAD_acc_81,UTILDE,FLUX1)
      CALL saxpy(OpenAD_acc_94,MU,FLUX1)
      CALL saxpy(OpenAD_acc_95,THTAVE,FLUX1)
      CALL saxpy(OpenAD_acc_110,ALP15P,FLUX1)
      CALL saxpy(OpenAD_acc_111,GM1INV,FLUX1)
      CALL saxpy(OpenAD_acc_112,CAVE,FLUX1)
      CALL saxpy(OpenAD_acc_114,ALP15M,FLUX1)
      CALL saxpy(OpenAD_acc_115,UHAT,FLUX1)
      END SUBROUTINE
