
C$OPENAD XXX File_start [head.f]
      SUBROUTINE head(X01, X02, X03, X04, X05, X06, X07, X08, X09, X10, 
     +X11, X12, X13, X14, X15, X16, X17, X18, X19, Y1, Y2, Y3, Y4, Y5 )
      use OAD_active
      use w2f__types
      use oad_intrinsics
      IMPLICIT NONE
C
C     **** Parameters and Result ****
C
      type(active) :: X01
      INTENT(IN) X01
      type(active) :: X02
      INTENT(IN) X02
      type(active) :: X03
      INTENT(IN) X03
      type(active) :: X04
      INTENT(IN) X04
      type(active) :: X05
      INTENT(IN) X05
      type(active) :: X06
      INTENT(IN) X06
      type(active) :: X07
      INTENT(IN) X07
      type(active) :: X08
      INTENT(IN) X08
      type(active) :: X09
      INTENT(IN) X09
      type(active) :: X10
      INTENT(IN) X10
      type(active) :: X11
      INTENT(IN) X11
      type(active) :: X12
      INTENT(IN) X12
      type(active) :: X13
      INTENT(IN) X13
      type(active) :: X14
      INTENT(IN) X14
      type(active) :: X15
      INTENT(IN) X15
      type(active) :: X16
      INTENT(IN) X16
      REAL(w2f__8) X17
      INTENT(IN) X17
      REAL(w2f__8) X18
      INTENT(IN) X18
      REAL(w2f__8) X19
      INTENT(IN) X19
      type(active) :: Y1
      INTENT(OUT) Y1
      type(active) :: Y2
      INTENT(OUT) Y2
      type(active) :: Y3
      INTENT(OUT) Y3
      type(active) :: Y4
      INTENT(OUT) Y4
      type(active) :: Y5
      INTENT(OUT) Y5
C
C     **** Local Variables and Functions ****
C
      REAL(w2f__4) HALF
      PARAMETER ( HALF = 5.0E-01)
      REAL(w2f__4) ONE
      PARAMETER ( ONE = 1.0)
      type(active) :: T01
      type(active) :: T02
      type(active) :: T03
      type(active) :: T04
      type(active) :: T05
      type(active) :: T06
      type(active) :: T07
      type(active) :: T08
      type(active) :: T09
      type(active) :: T10
      type(active) :: T11
      type(active) :: T12
      type(active) :: T13
      type(active) :: T14
      type(active) :: T15
      type(active) :: T16
      type(active) :: T17
      type(active) :: T18
      type(active) :: T19
      type(active) :: T20
      type(active) :: T21
      type(active) :: T22
      type(active) :: T23
      type(active) :: T24
      type(active) :: T25
      type(active) :: T26
      type(active) :: T27
      type(active) :: T28
      type(active) :: T29
      type(active) :: T30
      type(active) :: T31
      type(active) :: T32
      type(active) :: T33
      type(active) :: T34
      type(active) :: T35
      type(active) :: T36
      type(active) :: T37
      type(active) :: T38
      type(active) :: T39
      type(active) :: T40
      type(active) :: T41
      type(active) :: T42
      type(active) :: T43
      type(active) :: T44
      type(active) :: T45
      type(active) :: T46
      type(active) :: T47
      type(active) :: T48
      type(active) :: T49
      type(active) :: T50
      type(active) :: T51
      type(active) :: T52
      type(active) :: T53
      type(active) :: T54
      type(active) :: T55
      type(active) :: T56
      type(active) :: T57
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
      OpenAD_aux_0 = ((X01%v**2)+(X02%v**2)+(X03%v**2))
      OpenAD_lin_1 = (2*(X01%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_2 = (2*(X02%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_3 = (2*(X03%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_0 = COS(OpenAD_aux_0)
      T01%v = SIN(OpenAD_aux_0)
      OpenAD_lin_4 = (-(1.0D00/(T01%v*T01%v)))
      T02%v = (1.0D00/T01%v)
      OpenAD_lin_5 = T02%v
      OpenAD_lin_6 = X01%v
      T03%v = (X01%v*T02%v)
      OpenAD_lin_7 = T02%v
      OpenAD_lin_8 = X02%v
      T04%v = (X02%v*T02%v)
      OpenAD_lin_9 = T02%v
      OpenAD_lin_10 = X03%v
      T05%v = (X03%v*T02%v)
      OpenAD_aux_2 = (X10%v/X05%v)
      OpenAD_aux_1 = (SIN(OpenAD_aux_2)+1.0D00)
      OpenAD_lin_13 = (INT(1_w2f__i8)/X05%v)
      OpenAD_lin_14 = (-(X10%v/(X05%v*X05%v)))
      OpenAD_lin_12 = COS(OpenAD_aux_2)
      OpenAD_lin_11 = (-(1.0D00/(OpenAD_aux_1*OpenAD_aux_1)))
      T06%v = (1.0D00/OpenAD_aux_1)
      T07%v = (1.0D00-T06%v)
      OpenAD_aux_3 = ((X06%v**2)+(X07%v**2)+(X08%v**2))
      OpenAD_lin_15 = (2*(X06%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_16 = (2*(X07%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_17 = (2*(X08%v**(2-INT(1_w2f__i8))))
      T08%v = (OpenAD_aux_3*5.0D-01)
      OpenAD_aux_4 = ((X11%v**2)+(X12%v**2)+(X13%v**2))
      OpenAD_lin_18 = (2*(X11%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_19 = (2*(X12%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_20 = (2*(X13%v**(2-INT(1_w2f__i8))))
      T09%v = (OpenAD_aux_4*5.0D-01)
      OpenAD_aux_6 = (X14%v*X16%v)
      OpenAD_aux_5 = (X04%v*OpenAD_aux_6)
      OpenAD_lin_23 = OpenAD_aux_6
      OpenAD_lin_25 = X16%v
      OpenAD_lin_26 = X14%v
      OpenAD_lin_24 = X04%v
      OpenAD_lin_21 = (INT(1_w2f__i8)/X05%v)
      OpenAD_lin_22 = (-(OpenAD_aux_5/(X05%v*X05%v)))
      T10%v = (T08%v+(OpenAD_aux_5/X05%v))
      OpenAD_aux_8 = (X14%v*X16%v)
      OpenAD_aux_7 = (X09%v*OpenAD_aux_8)
      OpenAD_lin_29 = OpenAD_aux_8
      OpenAD_lin_31 = X16%v
      OpenAD_lin_32 = X14%v
      OpenAD_lin_30 = X09%v
      OpenAD_lin_27 = (INT(1_w2f__i8)/X10%v)
      OpenAD_lin_28 = (-(OpenAD_aux_7/(X10%v*X10%v)))
      T11%v = (T09%v+(OpenAD_aux_7/X10%v))
      OpenAD_lin_33 = T06%v
      OpenAD_lin_34 = X06%v
      OpenAD_lin_35 = T07%v
      OpenAD_lin_36 = X11%v
      T12%v = (X06%v*T06%v+X11%v*T07%v)
      OpenAD_lin_37 = T06%v
      OpenAD_lin_38 = X07%v
      OpenAD_lin_39 = T07%v
      OpenAD_lin_40 = X12%v
      T13%v = (X07%v*T06%v+X12%v*T07%v)
      OpenAD_lin_41 = T06%v
      OpenAD_lin_42 = X08%v
      OpenAD_lin_43 = T07%v
      OpenAD_lin_44 = X13%v
      T14%v = (X08%v*T06%v+X13%v*T07%v)
      OpenAD_lin_45 = T10%v
      OpenAD_lin_46 = T06%v
      OpenAD_lin_47 = T11%v
      OpenAD_lin_48 = T07%v
      T15%v = (T06%v*T10%v+T07%v*T11%v)
      OpenAD_aux_9 = ((T12%v**2)+(T13%v**2)+(T14%v**2))
      OpenAD_lin_49 = (2*(T12%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_50 = (2*(T13%v**(2-INT(1_w2f__i8))))
      OpenAD_lin_51 = (2*(T14%v**(2-INT(1_w2f__i8))))
      T18%v = (OpenAD_aux_9*5.0D-01)
      OpenAD_aux_10 = (T15%v-T18%v)
      OpenAD_lin_52 = OpenAD_aux_10
      OpenAD_lin_53 = X15%v
      T16%v = (X15%v*OpenAD_aux_10)
      OpenAD_acc_0 = (OpenAD_lin_30*OpenAD_lin_27)
      OpenAD_acc_1 = (OpenAD_lin_24*OpenAD_lin_21)
      OpenAD_acc_2 = (OpenAD_lin_4*OpenAD_lin_6)
      OpenAD_acc_3 = (OpenAD_lin_4*OpenAD_lin_8)
      OpenAD_acc_4 = (OpenAD_lin_4*OpenAD_lin_10)
      OpenAD_acc_5 = (5.0D-01*INT((-1_w2f__i8))*OpenAD_lin_53)
      OpenAD_acc_6 = (OpenAD_lin_48*OpenAD_lin_53)
      OpenAD_acc_7 = (OpenAD_lin_46*OpenAD_lin_53)
      OpenAD_acc_8 = (OpenAD_lin_11*(OpenAD_lin_45*OpenAD_lin_53+INT((-1
     +_w2f__i8))*OpenAD_lin_47*OpenAD_lin_53))
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
      CALL sax(OpenAD_lin_3,X03,OpenAD_prp_0)
      CALL saxpy(OpenAD_lin_1,X01,OpenAD_prp_0)
      CALL saxpy(OpenAD_lin_2,X02,OpenAD_prp_0)
      CALL sax(OpenAD_acc_12,X10,OpenAD_prp_1)
      CALL saxpy(OpenAD_acc_13,X05,OpenAD_prp_1)
      CALL sax(OpenAD_lin_17,X08,OpenAD_prp_2)
      CALL saxpy(OpenAD_lin_15,X06,OpenAD_prp_2)
      CALL saxpy(OpenAD_lin_16,X07,OpenAD_prp_2)
      CALL sax(OpenAD_lin_20,X13,OpenAD_prp_3)
      CALL saxpy(OpenAD_lin_18,X11,OpenAD_prp_3)
      CALL saxpy(OpenAD_lin_19,X12,OpenAD_prp_3)
      CALL sax(OpenAD_lin_0,OpenAD_prp_0,T01)
      CALL sax(OpenAD_lin_5,X01,T03)
      CALL saxpy(OpenAD_acc_2,T01,T03)
      CALL sax(OpenAD_lin_7,X02,T04)
      CALL saxpy(OpenAD_acc_3,T01,T04)
      CALL sax(OpenAD_lin_9,X03,T05)
      CALL saxpy(OpenAD_acc_4,T01,T05)
      CALL sax(5.0D-01,OpenAD_prp_2,T08)
      CALL sax(5.0D-01,OpenAD_prp_3,T09)
      CALL setderiv(T10,T08)
      CALL saxpy(OpenAD_lin_22,X05,T10)
      CALL saxpy(OpenAD_acc_14,X04,T10)
      CALL saxpy(OpenAD_acc_15,X14,T10)
      CALL saxpy(OpenAD_acc_16,X16,T10)
      CALL setderiv(T11,T09)
      CALL saxpy(OpenAD_lin_28,X10,T11)
      CALL saxpy(OpenAD_acc_17,X09,T11)
      CALL saxpy(OpenAD_acc_18,X14,T11)
      CALL saxpy(OpenAD_acc_19,X16,T11)
      CALL sax(OpenAD_lin_35,X11,T12)
      CALL saxpy(OpenAD_acc_11,OpenAD_prp_1,T12)
      CALL saxpy(OpenAD_lin_33,X06,T12)
      CALL sax(OpenAD_lin_39,X12,T13)
      CALL saxpy(OpenAD_lin_37,X07,T13)
      CALL saxpy(OpenAD_acc_10,OpenAD_prp_1,T13)
      CALL sax(OpenAD_lin_43,X13,T14)
      CALL saxpy(OpenAD_lin_41,X08,T14)
      CALL saxpy(OpenAD_acc_9,OpenAD_prp_1,T14)
      CALL sax(OpenAD_lin_51,T14,OpenAD_prp_4)
      CALL saxpy(OpenAD_lin_50,T13,OpenAD_prp_4)
      CALL saxpy(OpenAD_lin_49,T12,OpenAD_prp_4)
      CALL sax(5.0D-01,OpenAD_prp_4,T18)
      CALL sax(OpenAD_lin_52,X15,T16)
      CALL saxpy(OpenAD_acc_5,OpenAD_prp_4,T16)
      CALL saxpy(OpenAD_acc_6,T11,T16)
      CALL saxpy(OpenAD_acc_7,T10,T16)
      CALL saxpy(OpenAD_acc_8,OpenAD_prp_1,T16)
      OpenAD_lin_54 = COS(T16%v)
      T16%v = SIN(T16%v)
      OpenAD_lin_55 = T12%v
      OpenAD_lin_56 = T03%v
      OpenAD_lin_57 = T13%v
      OpenAD_lin_58 = T04%v
      OpenAD_lin_59 = T14%v
      OpenAD_lin_60 = T05%v
      T17%v = (T03%v*T12%v+T04%v*T13%v+T05%v*T14%v)
      OpenAD_lin_61 = T14%v
      OpenAD_lin_62 = T04%v
      OpenAD_lin_63 = T13%v
      OpenAD_lin_64 = T05%v
      T19%v = (T04%v*T14%v-T05%v*T13%v)
      OpenAD_lin_65 = T12%v
      OpenAD_lin_66 = T05%v
      OpenAD_lin_67 = T14%v
      OpenAD_lin_68 = T03%v
      T20%v = (T05%v*T12%v-T03%v*T14%v)
      OpenAD_lin_69 = T13%v
      OpenAD_lin_70 = T03%v
      OpenAD_lin_71 = T12%v
      OpenAD_lin_72 = T04%v
      T21%v = (T03%v*T13%v-T04%v*T12%v)
      T22%v = (T17%v-T16%v)
      T24%v = T17%v
      T23%v = (T16%v+T17%v)
      T25%v = T22%v
      T26%v = T23%v
      T27%v = T24%v
      OpenAD_acc_20 = (OpenAD_lin_63*INT((-1_w2f__i8)))
      OpenAD_acc_21 = (OpenAD_lin_64*INT((-1_w2f__i8)))
      OpenAD_acc_22 = (OpenAD_lin_67*INT((-1_w2f__i8)))
      OpenAD_acc_23 = (OpenAD_lin_68*INT((-1_w2f__i8)))
      OpenAD_acc_24 = (OpenAD_lin_71*INT((-1_w2f__i8)))
      OpenAD_acc_25 = (OpenAD_lin_72*INT((-1_w2f__i8)))
      CALL setderiv(OpenAD_prp_5,T16)
      CALL sax(OpenAD_lin_54,OpenAD_prp_5,T16)
      CALL sax(OpenAD_lin_61,T04,T19)
      CALL saxpy(OpenAD_lin_62,T14,T19)
      CALL saxpy(OpenAD_acc_20,T05,T19)
      CALL saxpy(OpenAD_acc_21,T13,T19)
      CALL sax(OpenAD_lin_65,T05,T20)
      CALL saxpy(OpenAD_lin_66,T12,T20)
      CALL saxpy(OpenAD_acc_22,T03,T20)
      CALL saxpy(OpenAD_acc_23,T14,T20)
      CALL sax(OpenAD_lin_69,T03,T21)
      CALL saxpy(OpenAD_lin_70,T13,T21)
      CALL saxpy(OpenAD_acc_24,T04,T21)
      CALL saxpy(OpenAD_acc_25,T12,T21)
      CALL sax(OpenAD_lin_55,T03,T17)
      CALL saxpy(OpenAD_lin_56,T12,T17)
      CALL saxpy(OpenAD_lin_57,T04,T17)
      CALL saxpy(OpenAD_lin_58,T13,T17)
      CALL saxpy(OpenAD_lin_59,T05,T17)
      CALL saxpy(OpenAD_lin_60,T14,T17)
      CALL setderiv(T22,T17)
      CALL dec_deriv(T22,T16)
      CALL setderiv(T24,T17)
      CALL setderiv(T23,T16)
      CALL inc_deriv(T23,T17)
      CALL setderiv(T25,T22)
      CALL setderiv(T26,T23)
      CALL setderiv(T27,T24)
      OpenAD_aux_11 = (T22%v-T25%v)
      T25%v = (OpenAD_aux_11*5.0D-01)
      OpenAD_aux_12 = (T23%v-T26%v)
      T26%v = (OpenAD_aux_12*5.0D-01)
      OpenAD_aux_13 = (T24%v-T27%v)
      T27%v = (OpenAD_aux_13*5.0D-01)
      OpenAD_lin_73 = X16%v
      OpenAD_lin_74 = X04%v
      OpenAD_lin_75 = T08%v
      OpenAD_lin_76 = X05%v
      T28%v = (X04%v*X16%v+X05%v*T08%v)
      OpenAD_lin_77 = X06%v
      OpenAD_lin_78 = X05%v
      T29%v = (X05%v*X06%v)
      OpenAD_lin_79 = X07%v
      OpenAD_lin_80 = X05%v
      T30%v = (X05%v*X07%v)
      OpenAD_lin_81 = X08%v
      OpenAD_lin_82 = X05%v
      T31%v = (X05%v*X08%v)
      OpenAD_lin_83 = X16%v
      OpenAD_lin_84 = X09%v
      OpenAD_lin_85 = T09%v
      OpenAD_lin_86 = X10%v
      T32%v = (X09%v*X16%v+X10%v*T09%v)
      OpenAD_lin_87 = X11%v
      OpenAD_lin_88 = X10%v
      T33%v = (X10%v*X11%v)
      OpenAD_lin_89 = X12%v
      OpenAD_lin_90 = X10%v
      T34%v = (X10%v*X12%v)
      OpenAD_lin_91 = X13%v
      OpenAD_lin_92 = X10%v
      T35%v = (X10%v*X13%v)
      T36%v = (T32%v-T28%v)
      T37%v = (X10%v-X05%v)
      T38%v = (T33%v-T29%v)
      T39%v = (T34%v-T30%v)
      T40%v = (T35%v-T31%v)
      OpenAD_lin_93 = T38%v
      OpenAD_lin_94 = T12%v
      OpenAD_lin_95 = T39%v
      OpenAD_lin_96 = T13%v
      OpenAD_lin_97 = T40%v
      OpenAD_lin_98 = T14%v
      T46%v = (T12%v*T38%v+T13%v*T39%v+T14%v*T40%v)
      OpenAD_aux_14 = (X15%v/T16%v)
      OpenAD_aux_15 = (T36%v+T18%v*T37%v-T46%v)
      OpenAD_lin_101 = (INT(1_w2f__i8)/T16%v)
      OpenAD_lin_102 = (-(X15%v/(T16%v*T16%v)))
      OpenAD_lin_99 = OpenAD_aux_15
      OpenAD_lin_103 = T37%v
      OpenAD_lin_104 = T18%v
      OpenAD_lin_100 = OpenAD_aux_14
      T47%v = (OpenAD_aux_14*OpenAD_aux_15)
      OpenAD_lin_105 = T38%v
      OpenAD_lin_106 = T03%v
      OpenAD_lin_107 = T39%v
      OpenAD_lin_108 = T04%v
      OpenAD_lin_109 = T40%v
      OpenAD_lin_110 = T05%v
      T48%v = (T03%v*T38%v+T04%v*T39%v+T05%v*T40%v)
      OpenAD_aux_16 = (T47%v+T17%v*T37%v-T48%v)
      OpenAD_lin_111 = T37%v
      OpenAD_lin_112 = T17%v
      T41%v = (OpenAD_aux_16*5.0D-01)
      OpenAD_aux_17 = (T03%v*T16%v-T19%v)
      OpenAD_aux_18 = (-T03%v)
      OpenAD_lin_113 = T40%v
      OpenAD_lin_114 = T04%v
      OpenAD_lin_115 = OpenAD_aux_17
      OpenAD_lin_117 = T16%v
      OpenAD_lin_118 = T03%v
      OpenAD_lin_116 = T37%v
      OpenAD_lin_119 = OpenAD_aux_18
      OpenAD_lin_120 = T47%v
      OpenAD_lin_121 = T39%v
      OpenAD_lin_122 = T05%v
      T42%v = (T04%v*T40%v+T37%v*OpenAD_aux_17+T47%v*OpenAD_aux_18-T05%v
     +*T39%v)
      OpenAD_aux_19 = (T04%v*T16%v-T20%v)
      OpenAD_aux_20 = (-T04%v)
      OpenAD_lin_123 = T38%v
      OpenAD_lin_124 = T05%v
      OpenAD_lin_125 = OpenAD_aux_19
      OpenAD_lin_127 = T16%v
      OpenAD_lin_128 = T04%v
      OpenAD_lin_126 = T37%v
      OpenAD_lin_129 = OpenAD_aux_20
      OpenAD_lin_130 = T47%v
      OpenAD_lin_131 = T40%v
      OpenAD_lin_132 = T03%v
      T43%v = (T05%v*T38%v+T37%v*OpenAD_aux_19+T47%v*OpenAD_aux_20-T03%v
     +*T40%v)
      OpenAD_aux_21 = (T05%v*T16%v-T21%v)
      OpenAD_aux_22 = (-T05%v)
      OpenAD_lin_133 = T39%v
      OpenAD_lin_134 = T03%v
      OpenAD_lin_135 = OpenAD_aux_21
      OpenAD_lin_137 = T16%v
      OpenAD_lin_138 = T05%v
      OpenAD_lin_136 = T37%v
      OpenAD_lin_139 = OpenAD_aux_22
      OpenAD_lin_140 = T47%v
      OpenAD_lin_141 = T38%v
      OpenAD_lin_142 = T04%v
      T44%v = (T03%v*T39%v+T37%v*OpenAD_aux_21+T47%v*OpenAD_aux_22-T04%v
     +*T38%v)
      OpenAD_aux_23 = (T48%v+T47%v-T17%v*T37%v)
      OpenAD_lin_143 = T37%v
      OpenAD_lin_144 = T17%v
      T45%v = (OpenAD_aux_23*5.0D-01)
      OpenAD_acc_26 = (OpenAD_lin_101*OpenAD_lin_99)
      OpenAD_acc_27 = (OpenAD_lin_102*OpenAD_lin_99)
      OpenAD_acc_28 = (OpenAD_lin_143*(-5.0D-01))
      OpenAD_acc_29 = (OpenAD_lin_111*5.0D-01)
      OpenAD_acc_30 = (INT((-1_w2f__i8))*OpenAD_lin_100)
      OpenAD_acc_31 = (OpenAD_lin_97*OpenAD_acc_30)
      OpenAD_acc_32 = (OpenAD_lin_98*OpenAD_acc_30)
      OpenAD_acc_33 = (OpenAD_lin_138*OpenAD_lin_136)
      OpenAD_acc_34 = (OpenAD_lin_137*OpenAD_lin_136+INT((-1_w2f__i8))*O
     +penAD_lin_140)
      OpenAD_acc_35 = (INT((-1_w2f__i8))*OpenAD_lin_136)
      OpenAD_acc_36 = (OpenAD_lin_128*OpenAD_lin_126)
      OpenAD_acc_37 = (OpenAD_lin_127*OpenAD_lin_126+INT((-1_w2f__i8))*O
     +penAD_lin_130)
      OpenAD_acc_38 = (INT((-1_w2f__i8))*OpenAD_lin_126)
      OpenAD_acc_39 = (OpenAD_lin_118*OpenAD_lin_116)
      OpenAD_acc_40 = (OpenAD_lin_117*OpenAD_lin_116+INT((-1_w2f__i8))*O
     +penAD_lin_120)
      OpenAD_acc_41 = (INT((-1_w2f__i8))*OpenAD_lin_116)
      OpenAD_acc_42 = (OpenAD_lin_103*OpenAD_lin_100)
      OpenAD_acc_43 = (OpenAD_lin_104*OpenAD_lin_100)
      OpenAD_acc_44 = (OpenAD_lin_144*(-5.0D-01)+OpenAD_acc_43*5.0D-01)
      OpenAD_acc_45 = (OpenAD_lin_135+OpenAD_acc_43*OpenAD_lin_139)
      OpenAD_acc_46 = (OpenAD_lin_125+OpenAD_acc_43*OpenAD_lin_129)
      OpenAD_acc_47 = (OpenAD_lin_115+OpenAD_acc_43*OpenAD_lin_119)
      OpenAD_acc_48 = (OpenAD_lin_112*5.0D-01+OpenAD_acc_43*5.0D-01)
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
      CALL setderiv(OpenAD_prp_6,T25)
      CALL setderiv(OpenAD_prp_7,T26)
      CALL setderiv(OpenAD_prp_8,T27)
      CALL setderiv(OpenAD_prp_9,T22)
      CALL dec_deriv(OpenAD_prp_9,OpenAD_prp_6)
      CALL setderiv(OpenAD_prp_10,T23)
      CALL dec_deriv(OpenAD_prp_10,OpenAD_prp_7)
      CALL setderiv(OpenAD_prp_11,T24)
      CALL dec_deriv(OpenAD_prp_11,OpenAD_prp_8)
      CALL sax(OpenAD_lin_77,X05,T29)
      CALL saxpy(OpenAD_lin_78,X06,T29)
      CALL sax(OpenAD_lin_79,X05,T30)
      CALL saxpy(OpenAD_lin_80,X07,T30)
      CALL sax(OpenAD_lin_81,X05,T31)
      CALL saxpy(OpenAD_lin_82,X08,T31)
      CALL setderiv(T37,X10)
      CALL dec_deriv(T37,X05)
      CALL set_neg_deriv(T38,T29)
      CALL saxpy(OpenAD_lin_87,X10,T38)
      CALL saxpy(OpenAD_lin_88,X11,T38)
      CALL set_neg_deriv(T39,T30)
      CALL saxpy(OpenAD_lin_89,X10,T39)
      CALL saxpy(OpenAD_lin_90,X12,T39)
      CALL set_neg_deriv(T40,T31)
      CALL saxpy(OpenAD_lin_91,X10,T40)
      CALL saxpy(OpenAD_lin_92,X13,T40)
      CALL sax(5.0D-01,OpenAD_prp_9,T25)
      CALL sax(5.0D-01,OpenAD_prp_10,T26)
      CALL sax(5.0D-01,OpenAD_prp_11,T27)
      CALL sax(OpenAD_lin_73,X04,T28)
      CALL saxpy(OpenAD_lin_74,X16,T28)
      CALL saxpy(OpenAD_lin_75,X05,T28)
      CALL saxpy(OpenAD_lin_76,T08,T28)
      CALL set_neg_deriv(T36,T28)
      CALL saxpy(OpenAD_lin_83,X09,T36)
      CALL saxpy(OpenAD_lin_84,X16,T36)
      CALL saxpy(OpenAD_lin_85,X10,T36)
      CALL saxpy(OpenAD_lin_86,T09,T36)
      CALL sax(OpenAD_lin_109,T05,T48)
      CALL saxpy(OpenAD_lin_110,T40,T48)
      CALL saxpy(OpenAD_lin_107,T04,T48)
      CALL saxpy(OpenAD_lin_108,T39,T48)
      CALL saxpy(OpenAD_lin_105,T03,T48)
      CALL saxpy(OpenAD_lin_106,T38,T48)
      CALL sax(OpenAD_acc_26,X15,T47)
      CALL saxpy(OpenAD_acc_27,T16,T47)
      CALL saxpy(OpenAD_acc_31,T14,T47)
      CALL saxpy(OpenAD_acc_32,T40,T47)
      CALL saxpy(OpenAD_lin_100,T36,T47)
      CALL saxpy(OpenAD_acc_42,T18,T47)
      CALL saxpy(OpenAD_acc_49,T13,T47)
      CALL saxpy(OpenAD_acc_50,T39,T47)
      CALL saxpy(OpenAD_acc_51,T12,T47)
      CALL saxpy(OpenAD_acc_52,T38,T47)
      CALL sax(-5.0D-01,T48,T41)
      CALL saxpy(5.0D-01,T47,T41)
      CALL saxpy(OpenAD_acc_29,T17,T41)
      CALL saxpy(OpenAD_acc_48,T37,T41)
      CALL sax(5.0D-01,T48,T45)
      CALL saxpy(5.0D-01,T47,T45)
      CALL saxpy(OpenAD_acc_28,T17,T45)
      CALL saxpy(OpenAD_acc_44,T37,T45)
      CALL sax(OpenAD_lin_119,T47,T42)
      CALL saxpy(OpenAD_acc_47,T37,T42)
      CALL saxpy(OpenAD_acc_40,T03,T42)
      CALL saxpy(OpenAD_acc_39,T16,T42)
      CALL saxpy(OpenAD_acc_41,T19,T42)
      CALL saxpy(OpenAD_lin_113,T04,T42)
      CALL saxpy(OpenAD_lin_114,T40,T42)
      CALL saxpy(OpenAD_acc_57,T05,T42)
      CALL saxpy(OpenAD_acc_58,T39,T42)
      CALL sax(OpenAD_lin_129,T47,T43)
      CALL saxpy(OpenAD_acc_46,T37,T43)
      CALL saxpy(OpenAD_acc_37,T04,T43)
      CALL saxpy(OpenAD_acc_36,T16,T43)
      CALL saxpy(OpenAD_acc_38,T20,T43)
      CALL saxpy(OpenAD_lin_123,T05,T43)
      CALL saxpy(OpenAD_lin_124,T38,T43)
      CALL saxpy(OpenAD_acc_55,T03,T43)
      CALL saxpy(OpenAD_acc_56,T40,T43)
      CALL sax(OpenAD_lin_139,T47,T44)
      CALL saxpy(OpenAD_acc_45,T37,T44)
      CALL saxpy(OpenAD_acc_34,T05,T44)
      CALL saxpy(OpenAD_acc_33,T16,T44)
      CALL saxpy(OpenAD_acc_35,T21,T44)
      CALL saxpy(OpenAD_lin_133,T03,T44)
      CALL saxpy(OpenAD_lin_134,T39,T44)
      CALL saxpy(OpenAD_acc_53,T04,T44)
      CALL saxpy(OpenAD_acc_54,T38,T44)
      OpenAD_lin_145 = T41%v
      OpenAD_lin_146 = T25%v
      T41%v = (T25%v*T41%v)
      OpenAD_lin_147 = T42%v
      OpenAD_lin_148 = T27%v
      T42%v = (T27%v*T42%v)
      OpenAD_lin_149 = T43%v
      OpenAD_lin_150 = T27%v
      T43%v = (T27%v*T43%v)
      OpenAD_lin_151 = T44%v
      OpenAD_lin_152 = T27%v
      T44%v = (T27%v*T44%v)
      OpenAD_lin_153 = T45%v
      OpenAD_lin_154 = T26%v
      T45%v = (T26%v*T45%v)
      T49%v = (T41%v+T45%v)
      T50%v = (T41%v-T45%v)
      OpenAD_lin_155 = (INT(1_w2f__i8)/T16%v)
      OpenAD_lin_156 = (-(T49%v/(T16%v*T16%v)))
      OpenAD_lin_157 = T42%v
      OpenAD_lin_158 = T03%v
      OpenAD_lin_159 = T43%v
      OpenAD_lin_160 = T04%v
      OpenAD_lin_161 = T44%v
      OpenAD_lin_162 = T05%v
      T56%v = ((T49%v/T16%v)+T03%v*T42%v+T04%v*T43%v+T05%v*T44%v)
      OpenAD_aux_24 = (X16%v*T16%v)
      OpenAD_lin_163 = T56%v
      OpenAD_lin_164 = T18%v
      OpenAD_lin_165 = OpenAD_aux_24
      OpenAD_lin_167 = T16%v
      OpenAD_lin_168 = X16%v
      OpenAD_lin_166 = T49%v
      OpenAD_lin_169 = T50%v
      OpenAD_lin_170 = T17%v
      OpenAD_lin_171 = T42%v
      OpenAD_lin_172 = T19%v
      OpenAD_lin_173 = T43%v
      OpenAD_lin_174 = T20%v
      OpenAD_lin_175 = T44%v
      OpenAD_lin_176 = T21%v
      T51%v = (T18%v*T56%v+T49%v*OpenAD_aux_24-T17%v*T50%v+T19%v*T42%v+T
     +20%v*T43%v+T21%v*T44%v)
      T52%v = T56%v
      OpenAD_lin_177 = T43%v
      OpenAD_lin_178 = T05%v
      OpenAD_lin_179 = T56%v
      OpenAD_lin_180 = T12%v
      OpenAD_lin_181 = T50%v
      OpenAD_lin_182 = T03%v
      OpenAD_lin_183 = T44%v
      OpenAD_lin_184 = T04%v
      T53%v = (T05%v*T43%v+T12%v*T56%v-T03%v*T50%v-T04%v*T44%v)
      OpenAD_lin_185 = T44%v
      OpenAD_lin_186 = T03%v
      OpenAD_lin_187 = T56%v
      OpenAD_lin_188 = T13%v
      OpenAD_lin_189 = T50%v
      OpenAD_lin_190 = T04%v
      OpenAD_lin_191 = T42%v
      OpenAD_lin_192 = T05%v
      T54%v = (T03%v*T44%v+T13%v*T56%v-T04%v*T50%v-T05%v*T42%v)
      OpenAD_lin_193 = T42%v
      OpenAD_lin_194 = T04%v
      OpenAD_lin_195 = T56%v
      OpenAD_lin_196 = T14%v
      OpenAD_lin_197 = T50%v
      OpenAD_lin_198 = T05%v
      OpenAD_lin_199 = T43%v
      OpenAD_lin_200 = T03%v
      T55%v = (T04%v*T42%v+T14%v*T56%v-T05%v*T50%v-T03%v*T43%v)
      OpenAD_lin_201 = T03%v
      OpenAD_lin_202 = X06%v
      OpenAD_lin_203 = T04%v
      OpenAD_lin_204 = X07%v
      OpenAD_lin_205 = T05%v
      OpenAD_lin_206 = X08%v
      T57%v = (X06%v*T03%v+X07%v*T04%v+X08%v*T05%v)
      OpenAD_aux_26 = (X04%v+T28%v)
      OpenAD_aux_25 = (T51%v+T57%v*OpenAD_aux_26)
      OpenAD_lin_207 = OpenAD_aux_25
      OpenAD_lin_209 = OpenAD_aux_26
      OpenAD_lin_210 = T57%v
      OpenAD_lin_208 = T01%v
      Y1%v = (T01%v*OpenAD_aux_25)
      OpenAD_aux_27 = (T52%v+X05%v*T57%v)
      OpenAD_lin_211 = OpenAD_aux_27
      OpenAD_lin_213 = T57%v
      OpenAD_lin_214 = X05%v
      OpenAD_lin_212 = T01%v
      Y2%v = (T01%v*OpenAD_aux_27)
      OpenAD_aux_28 = (T53%v+X04%v*T03%v+T29%v*T57%v)
      OpenAD_lin_215 = OpenAD_aux_28
      OpenAD_lin_217 = T03%v
      OpenAD_lin_218 = X04%v
      OpenAD_lin_219 = T57%v
      OpenAD_lin_220 = T29%v
      OpenAD_lin_216 = T01%v
      Y3%v = (T01%v*OpenAD_aux_28)
      OpenAD_aux_29 = (T54%v+X04%v*T04%v+T30%v*T57%v)
      OpenAD_lin_221 = OpenAD_aux_29
      OpenAD_lin_223 = T04%v
      OpenAD_lin_224 = X04%v
      OpenAD_lin_225 = T57%v
      OpenAD_lin_226 = T30%v
      OpenAD_lin_222 = T01%v
      Y4%v = (T01%v*OpenAD_aux_29)
      OpenAD_aux_30 = (T55%v+X04%v*T05%v+T31%v*T57%v)
      OpenAD_lin_227 = OpenAD_aux_30
      OpenAD_lin_229 = T05%v
      OpenAD_lin_230 = X04%v
      OpenAD_lin_231 = T57%v
      OpenAD_lin_232 = T31%v
      OpenAD_lin_228 = T01%v
      Y5%v = (T01%v*OpenAD_aux_30)
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
      OpenAD_acc_85 = (OpenAD_lin_198*OpenAD_acc_84)
      OpenAD_acc_86 = (OpenAD_lin_187*OpenAD_lin_222)
      OpenAD_acc_87 = (OpenAD_lin_188*OpenAD_lin_222)
      OpenAD_acc_88 = (INT((-1_w2f__i8))*OpenAD_lin_222)
      OpenAD_acc_89 = (OpenAD_lin_190*OpenAD_acc_88)
      OpenAD_acc_90 = (OpenAD_lin_179*OpenAD_lin_216)
      OpenAD_acc_91 = (OpenAD_lin_180*OpenAD_lin_216)
      OpenAD_acc_92 = (INT((-1_w2f__i8))*OpenAD_lin_216)
      OpenAD_acc_93 = (OpenAD_lin_182*OpenAD_acc_92)
      OpenAD_acc_94 = (OpenAD_lin_163*OpenAD_lin_208)
      OpenAD_acc_95 = (OpenAD_lin_164*OpenAD_lin_208)
      OpenAD_acc_96 = (OpenAD_lin_231*OpenAD_lin_228)
      OpenAD_acc_97 = (OpenAD_lin_232*OpenAD_lin_228)
      OpenAD_acc_98 = (OpenAD_lin_197*OpenAD_acc_84+OpenAD_lin_230*OpenA
     +D_lin_228)
      OpenAD_acc_99 = (OpenAD_lin_229*OpenAD_lin_228)
      OpenAD_acc_100 = (OpenAD_lin_225*OpenAD_lin_222)
      OpenAD_acc_101 = (OpenAD_lin_226*OpenAD_lin_222)
      OpenAD_acc_102 = (OpenAD_lin_189*OpenAD_acc_88+OpenAD_lin_224*Open
     +AD_lin_222)
      OpenAD_acc_103 = (OpenAD_lin_223*OpenAD_lin_222)
      OpenAD_acc_104 = (OpenAD_lin_219*OpenAD_lin_216)
      OpenAD_acc_105 = (OpenAD_lin_220*OpenAD_lin_216)
      OpenAD_acc_106 = (OpenAD_lin_181*OpenAD_acc_92+OpenAD_lin_218*Open
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
      CALL setderiv(OpenAD_prp_12,T41)
      CALL setderiv(OpenAD_prp_13,T42)
      CALL setderiv(OpenAD_prp_14,T43)
      CALL setderiv(OpenAD_prp_15,T44)
      CALL setderiv(OpenAD_prp_16,T45)
      CALL sax(OpenAD_lin_145,T25,T41)
      CALL saxpy(OpenAD_lin_146,OpenAD_prp_12,T41)
      CALL sax(OpenAD_lin_147,T27,T42)
      CALL saxpy(OpenAD_lin_148,OpenAD_prp_13,T42)
      CALL sax(OpenAD_lin_149,T27,T43)
      CALL saxpy(OpenAD_lin_150,OpenAD_prp_14,T43)
      CALL sax(OpenAD_lin_151,T27,T44)
      CALL saxpy(OpenAD_lin_152,OpenAD_prp_15,T44)
      CALL sax(OpenAD_lin_153,T26,T45)
      CALL saxpy(OpenAD_lin_154,OpenAD_prp_16,T45)
      CALL setderiv(T49,T41)
      CALL inc_deriv(T49,T45)
      CALL setderiv(T50,T41)
      CALL dec_deriv(T50,T45)
      CALL setderiv(OpenAD_prp_17,X04)
      CALL inc_deriv(OpenAD_prp_17,T28)
      CALL sax(OpenAD_lin_201,X06,T57)
      CALL saxpy(OpenAD_lin_202,T03,T57)
      CALL saxpy(OpenAD_lin_203,X07,T57)
      CALL saxpy(OpenAD_lin_204,T04,T57)
      CALL saxpy(OpenAD_lin_205,X08,T57)
      CALL saxpy(OpenAD_lin_206,T05,T57)
      CALL sax(OpenAD_lin_155,T49,T56)
      CALL saxpy(OpenAD_lin_156,T16,T56)
      CALL saxpy(OpenAD_lin_157,T03,T56)
      CALL saxpy(OpenAD_lin_158,T42,T56)
      CALL saxpy(OpenAD_lin_159,T04,T56)
      CALL saxpy(OpenAD_lin_160,T43,T56)
      CALL saxpy(OpenAD_lin_161,T05,T56)
      CALL saxpy(OpenAD_lin_162,T44,T56)
      CALL sax(OpenAD_lin_211,T01,Y2)
      CALL saxpy(OpenAD_acc_108,X05,Y2)
      CALL saxpy(OpenAD_acc_109,T57,Y2)
      CALL saxpy(OpenAD_lin_212,T56,Y2)
      CALL sax(OpenAD_lin_215,T01,Y3)
      CALL saxpy(OpenAD_acc_70,T04,Y3)
      CALL saxpy(OpenAD_acc_71,T44,Y3)
      CALL saxpy(OpenAD_acc_78,T05,Y3)
      CALL saxpy(OpenAD_acc_79,T43,Y3)
      CALL saxpy(OpenAD_acc_90,T12,Y3)
      CALL saxpy(OpenAD_acc_91,T56,Y3)
      CALL saxpy(OpenAD_acc_106,T03,Y3)
      CALL saxpy(OpenAD_acc_93,T50,Y3)
      CALL saxpy(OpenAD_acc_104,T29,Y3)
      CALL saxpy(OpenAD_acc_105,T57,Y3)
      CALL saxpy(OpenAD_acc_107,X04,Y3)
      CALL sax(OpenAD_lin_221,T01,Y4)
      CALL saxpy(OpenAD_acc_67,T05,Y4)
      CALL saxpy(OpenAD_acc_68,T42,Y4)
      CALL saxpy(OpenAD_acc_76,T03,Y4)
      CALL saxpy(OpenAD_acc_77,T44,Y4)
      CALL saxpy(OpenAD_acc_86,T13,Y4)
      CALL saxpy(OpenAD_acc_87,T56,Y4)
      CALL saxpy(OpenAD_acc_102,T04,Y4)
      CALL saxpy(OpenAD_acc_89,T50,Y4)
      CALL saxpy(OpenAD_acc_100,T30,Y4)
      CALL saxpy(OpenAD_acc_101,T57,Y4)
      CALL saxpy(OpenAD_acc_103,X04,Y4)
      CALL sax(OpenAD_lin_227,T01,Y5)
      CALL saxpy(OpenAD_acc_64,T03,Y5)
      CALL saxpy(OpenAD_acc_65,T43,Y5)
      CALL saxpy(OpenAD_acc_74,T04,Y5)
      CALL saxpy(OpenAD_acc_75,T42,Y5)
      CALL saxpy(OpenAD_acc_82,T14,Y5)
      CALL saxpy(OpenAD_acc_83,T56,Y5)
      CALL saxpy(OpenAD_acc_98,T05,Y5)
      CALL saxpy(OpenAD_acc_85,T50,Y5)
      CALL saxpy(OpenAD_acc_96,T31,Y5)
      CALL saxpy(OpenAD_acc_97,T57,Y5)
      CALL saxpy(OpenAD_acc_99,X04,Y5)
      CALL sax(OpenAD_lin_207,T01,Y1)
      CALL saxpy(OpenAD_acc_59,T57,Y1)
      CALL saxpy(OpenAD_acc_60,OpenAD_prp_17,Y1)
      CALL saxpy(OpenAD_acc_61,T21,Y1)
      CALL saxpy(OpenAD_acc_62,T44,Y1)
      CALL saxpy(OpenAD_acc_72,T20,Y1)
      CALL saxpy(OpenAD_acc_73,T43,Y1)
      CALL saxpy(OpenAD_acc_80,T19,Y1)
      CALL saxpy(OpenAD_acc_81,T42,Y1)
      CALL saxpy(OpenAD_acc_94,T18,Y1)
      CALL saxpy(OpenAD_acc_95,T56,Y1)
      CALL saxpy(OpenAD_acc_110,T49,Y1)
      CALL saxpy(OpenAD_acc_111,X16,Y1)
      CALL saxpy(OpenAD_acc_112,T16,Y1)
      CALL saxpy(OpenAD_acc_114,T17,Y1)
      CALL saxpy(OpenAD_acc_115,T50,Y1)
      END SUBROUTINE
