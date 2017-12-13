'''functions related to type/shape/generic inference'''

import re
import copy

from _Setup import *

from PyUtil.debugManager import DebugManager
from PyUtil.symtab import SymtabEntry,globalTypeTable
from PyUtil.typetab import TypetabEntry
import fortStmts
from fortExp import App,NamedParam,Sel,Unary,Ops,isConstantExpression,is_const,is_id,_id_re,_flonum_re,_int_re,_logicon_set,_quote_set,Slice,Zslice,Lslice,Rslice,ArrayConstructor
from intrinsic import is_intrinsic, is_inquiry, getNonStandard

class InferenceError(Exception):
   '''exception for ...'''
   def __init__(self,msg,lineNumber=None):
      self.msg  = msg
      self.lineNumber=lineNumber

   def __str__(self):
      errString='\nERROR: InferenceError: '
      if self.lineNumber is not None:
         errString+='at line '+str(self.lineNumber)+':'
      if self.msg: errString+=str(self.msg)
      return (errString)


def _kw2type(s): return(fortStmts.kwtbl[s.lower()])
def _lenfn(n): return fortStmts._F77Len(str(n))

_modhash = { fortStmts._Prec     : 0,
             fortStmts._Kind     : 1,
             fortStmts._ExplKind : 2,
             }

class _TypeContext:
   def __init__(self,lineNumber,localSymtab):
      self.lineNumber=lineNumber,
      self.localSymtab=localSymtab


   def __modCompare(self,m1,m2,addLength):
      'compare type modifiers'
      if not m1: return m2
      if not m2: return m1
      mm1 = m1[0]
      mm2 = m2[0]
      if (mm1.__class__ == mm2.__class__) and isinstance(mm1,fortStmts._TypeMod) :
         if mm1.mod >= mm2.mod: return m1
         return m2
      if isinstance(mm2,fortStmts._FLenMod) and isinstance(mm1,fortStmts._FLenMod) :
         # could have different ways of specifying length - easiest case is identical modifiers
         if mm1.len==mm2.len:
            return m1
         # one of them could have a "*" so we have to use that one
         if (mm1.len=='*'):
            return m1
         if (mm2.len=='*'):
            return m2
         # they could be integers
         try :
            le1=mm1.len
            if (isinstance(mm1.len,NamedParam) and mm1.len.myId.lower()=='len'):
               le1=mm1.len.myRHS
            le2=mm2.len
            if (isinstance(mm2.len,NamedParam) and mm2.len.myId.lower()=='len'):
               le2=mm2.len.myRHS
            if (addLength):
               return [fortStmts._F90Len(Ops('+',le1,le2))] 
            l1=int(le1)
            l2=int(le2)
            if l1>l2:
               return m1
            else :
               return m2
         # if they are not both integers there could be some parameter name etc.
         except ValueError:
            raise InferenceError(sys._getframe().f_code.co_name+': Cannot compare length specifiers '+str(mm1.len)+' and '+str(mm2.len))
      if (isinstance(mm1,fortStmts._KindTypeMod) and isinstance(mm2,fortStmts._KindTypeMod)) :
         k1=mm1.mod
         if (is_id(k1)):
             symtabEntry=self.localSymtab.lookup_name(k1)
             if (symtabEntry and symtabEntry.constInit):
                k1=symtabEntry.constInit
         k2=mm2.mod
         if (is_id(k2)):
             symtabEntry=self.localSymtab.lookup_name(k2)
             if (symtabEntry and symtabEntry.constInit):
                k2=symtabEntry.constInit
         if (k1==k2):
            return m1
      if (str(m1) == str(m2)):
         return m1 
      raise InferenceError(sys._getframe().f_code.co_name+': Do not know how to compare modification specifiers '+str(m1)+' and '+str(m2))

   def __typeCompare(self,t1,t2,addLength):
      DebugManager.debug(sys._getframe().f_code.co_name+' called on t1 = "'+str(t1)+'\tt2 = "'+str(t2)+'"')
      mergeit = dict(character=0,
                     logical=1,
                     integer=2,
                     real=3,
                     doubleprecision=4,
                     complex=5,
                     doublecomplex=6,
                     )

      if t1[0] == t2[0]:
         return(t1[0],self.__modCompare(t1[1],t2[1],addLength))

      if mergeit[t1[0].kw] > mergeit[t2[0].kw]: return t1

      return t2

   def __typeCompare(self,t1,t2,addLength):
      DebugManager.debug(sys._getframe().f_code.co_name+' called on t1 = "'+t1.debug()+'\tt2 = "'+t2.debug()+'"')
      mergeit = dict(character=0,
                     logical=1,
                     integer_4=2,
                     real_4=3,
                     real_8=4,
                     complex_8=5,
                     complex_16=6,
                     )
      if globalTypeTable.equivalence(t1,t2):
         return t1
      if isinstance(t1.entryKind,TypetabEntry.ArrayEntryKind):
         # get type name of built in type
         t1_type=globalTypeTable.lookupTypeId(t1.entryKind.typetab_id).entryKind.type_name
      else: t1_type=t1.getBaseTypeEntry().entryKind.type_name
      if isinstance(t2.entryKind,TypetabEntry.ArrayEntryKind):
         # get type name of built in type
         t2_type=globalTypeTable.lookupTypeId(t2.entryKind.typetab_id).entryKind.type_name
      else: t2_type=t2.getBaseTypeEntry().entryKind.type_name
      if mergeit[t1_type] > mergeit[t2_type]: return t1
      return t2

   # each item of list should be a typetab entry
   def _typemerge(self,lst,default,addLength=False):
      DebugManager.debug(sys._getframe().f_code.co_name+' called on '+str(lst)+'...',newLine=False)
      if not lst: return default
      if len(lst) == 1: return lst[0]
      mergedEntry = self.__typeCompare(lst[0],lst[1],addLength)
      for entry in lst[2:]:
         mergedEntry = self.__typeCompare(mergedEntry,entry,addLength)
      DebugManager.debug(' result is '+str(mergedEntry))
      return mergedEntry

   def _constantType(self,e):
      kind_re = re.compile(r'_(\w+)')
      if _flonum_re.match(e):
         sep_re = re.compile(r'([^_]+)(_(\w+))?')
         v      = sep_re.match(e)
         ty     = 'd' in v.group(1).lower() and _kw2type('doubleprecision') \
                                             or _kw2type('real')
         kind   = v.group(2) and [fortStmts._Kind(v.group(3))] \
                              or []
         if ty==fortStmts.DoubleStmt:
            typeName='real'
         elif ty==fortStmts.DoubleCplexStmt:
            typeName='complex'
         else: typeName=ty.kw
         if len(kind)>0:
            [kindexp] = kind
            numBytes = self.__guessBytesFromKind(kindexp.mod)
         else: 
            numBytes = self._guessBytes((ty,kind))
         typeName=typeName+'_'+str(numBytes)
         return globalTypeTable.intrinsicTypeNameToEntry(typeName)
      if _int_re.match(e):
         ty   = _kw2type('integer')
         kind = kind_re.search(e)
         kind = kind and [fortStmts._Kind(kind.group(1))] \
                      or []
         if (len(kind)>0):
            [kindexp]=kind
            numBytes = self.__guessBytesFromKind(kindexp.mod)
            typeName = ty.kw+'_'+str(numBytes)
         elif ty.kw=='real' or ty.kw=='integer':
            typeName=ty.kw+'_4'
         else:
            typeName=ty.kw
         return globalTypeTable.intrinsicTypeNameToEntry(typeName)
      if e.lower() in _logicon_set:
         return globalTypeTable.intrinsicTypeNameToEntry('logical')
      if e[0] in _quote_set:
         l=max(len(e)-2,1)
         return globalTypeTable.getTypeEntry(fortStmts.CharacterStmt([_lenfn(l)],[],[]),self.localSymtab)
         #return (_kw2type('character'),_lenfn(len(e)-2))

   def __identifierType(self,anId):
      (symtabEntry,containingSymtab) = self.localSymtab.lookup_name_level(anId)
      # a type is known -> return it
      if symtabEntry and symtabEntry.typetab_id:
         returnType = globalTypeTable.lookupTypeId(symtabEntry.typetab_id)
         DebugManager.debug('with symtab entry '+symtabEntry.debug(anId)+' -> returning type '+str(returnType))
      # an entry exists with no type -> try to type implicitly
      elif symtabEntry and isinstance(symtabEntry.entryKind,SymtabEntry.InterfaceEntryKind):
         DebugManager.debug('with symtab entry'+symtabEntry.debug(anId)+' (is an interface name).')
         return None
      elif symtabEntry:
         #try local implicit typing
         implicitLocalType=containingSymtab.implicit[anId[0]]
         if implicitLocalType: # we handle the error condition below
            symtabEntry.enterType(containingSymtab.implicit[anId[0]],containingSymtab)
            DebugManager.warning(sys._getframe().f_code.co_name+' implicit typing for symbol >'+anId+'< to type: '+symtabEntry.typePrint(),self.lineNumber,DebugManager.WarnType.implicit)
            returnType = globalTypeTable.getTypeEntry(implicitLocalType[0](implicitLocalType[1],[],[]),self.localSymtab)
         else:
            DebugManager.warning('unimplemented type inference; returning None for '+sys._getframe().f_code.co_name,self.lineNumber,DebugManager.WarnType.implicit)
            returnType=None
      else: # no symtab entry -> try local implicit typing
         implicitType = self.localSymtab.implicit[anId[0]]
         if implicitType:
            (theType,theMods)=implicitType
            typeName = theType.kw
            if len(theMods)>0:
               [kindexp]=theMods
               numBytes=self.__guessBytesFromKind(kindexp.mod)
               if numBytes:
                  typeName=typeName+'_'+numBytes
            elif typeName=='real' or typeName=='integer':
               typeName=typeName+'_4'
            elif typeName=='doubleprecision':
               typeName='real_8'
            returnType=globalTypeTable.intrinsicTypeNameToEntry(typeName)
         else:
            returnType=None
      if not returnType:
         raise InferenceError(sys._getframe().f_code.co_name+': No type could be determined for identifier "'+anId+'"',self.lineNumber)
      return returnType

   def __intrinsicType(self,anIntrinsicApp):
      if anIntrinsicApp.head.lower() in ['aimag','alog']:
         return globalTypeTable.intrinsicTypeNameToEntry('real_4')
      elif anIntrinsicApp.head.lower() == 'real':
         typeName='real'
         typeMod=[]
         if (len(anIntrinsicApp.args)==2): # the second argument would be the kind parameter
            if isinstance(anIntrinsicApp.args[1],NamedParam):
               kindExp=fortStmts._Kind(anIntrinsicApp.args[1].myRHS)
               typeMod.append(kindExp)
               numBytes=self.__guessBytesFromKind(kindExp)
               typeName=typeName+'_'+str(numBytes)
            else:
               typeMod.append(fortStmts._Kind(anIntrinsicApp.args[1]))
               numBytes=self.__guessBytesFromKind(fortStmts._Kind(anIntrinsicApp.args[1]))
               typeName=typeName+'_'+str(numBytes)
         else:
            typeName=typeName+'_4'
         return globalTypeTable.intrinsicTypeNameToEntry(typeName)
      elif anIntrinsicApp.head.lower() in ['iachar','ichar','idint','index','int','lbound','maxloc','scan','shape','size','ubound']:
         return globalTypeTable.intrinsicTypeNameToEntry('integer_4')
      elif anIntrinsicApp.head.lower() in ['dble','dfloat','dabs','dexp','dlog','dsqrt','dmod']:
         return globalTypeTable.intrinsicTypeNameToEntry('real_8')
      elif anIntrinsicApp.head.lower() == 'cmplx':
         return globalTypeTable.intrinsicTypeNameToEntry('complex_8')
      elif anIntrinsicApp.head.lower() in ['achar','repeat']:
         return globalTypeTable.intrinsicTypeNameToEntry('character')
      elif anIntrinsicApp.head.lower() in ['all','iand','ior','lge','lgt','lle','llt','present']:
         return globalTypeTable.intrinsicTypeNameToEntry('logical')
      elif anIntrinsicApp.head.lower() in ['transfer']:
         return self._expressionType(anIntrinsicApp.args[1]) # the type of the second argument
      elif anIntrinsicApp.head.lower() in ['reshape']:
         return self._expressionType(anIntrinsicApp.args[0]) # the type of the first argument
      #nonstandard ones: we check is_intrinsic before this is called.
      elif anIntrinsicApp.head.lower() in getNonStandard():
         return globalTypeTable.intrinsicTypeNameToEntry('integer_4')
      else:
         return self._typemerge([self._expressionType(anArg) for anArg in anIntrinsicApp.args],
                                (None,None))
      
   def __guessBytesFromKind(self,aKindExpressionMod) :
      kindList=['kind','selected_real_kind','selected_int_kind']
      if isinstance(aKindExpressionMod,fortStmts._KindTypeMod):
         theExpr=aKindExpressionMod
         return self.__guessBytesFromKind(aKindExpressionMod.mod)
      if (isinstance(aKindExpressionMod,App) and (aKindExpressionMod.head.lower() in kindList)):
         theExpr=aKindExpressionMod.args[0]
         if isinstance(theExpr,NamedParam):
            theExpr=theExpr.myRHS
         if is_const(theExpr):
            if (_flonum_re.match(theExpr)) :
               if ('d' in theExpr.lower()):
                  return 8
               else:
                  return 4
            elif (_int_re.match(theExpr)) :
               if aKindExpressionMod.head.lower()=='selected_int_kind':
                  if int(theExpr)<2:
                     return 1
                  elif int(theExpr)<5:
                     return 2
                  elif int(theExpr)<10:
                     return 4
                  else:
                     return 8
               elif aKindExpressionMod.head.lower()=='selected_real_kind':
                  if int(theExpr)<7:
                     return 4
                  elif int(theExpr)<16:
                     return 8
                  else:
                     return 16
               return 4
      if (_int_re.match(aKindExpressionMod)):
         return int(aKindExpressionMod)
      if (is_id(aKindExpressionMod)): # for example some module variable declared as parameter and initialized with a kind expression
         symtabEntry=self.localSymtab.lookup_name(aKindExpressionMod)
         if symtabEntry and (symtabEntry.constInit):
            return self.__guessBytesFromKind(symtabEntry.constInit)
      raise InferenceError(sys._getframe().f_code.co_name+': cannot guess bytes for KIND expression "'+str(aKindExpressionMod)+'"',self.lineNumber)

   def _guessBytes(self,aType):
      if (aType[0]==fortStmts.DoubleStmt) :
         return 8
      if (aType[0]==fortStmts.RealStmt and not aType[1]) :
         return 4
      if (aType[0]==fortStmts.IntegerStmt and not aType[1]) :
         return 4
      if (aType[0]==fortStmts.ComplexStmt and not aType[1]):
         return 8
      if (aType[0]==fortStmts.DoubleCplexStmt and not aType[1]):
         return 16
      if (len(aType[1])==1):
         theMod=aType[1][0]
         if (isinstance(theMod,fortStmts._Prec)):
            return int(theMod.mod)
         if (isinstance(theMod,fortStmts._KindTypeMod)):
            return self.__guessBytesFromKind(theMod.mod)
      raise InferenceError(sys._getframe().f_code.co_name+': cannot guess bytes for  "'+str(aType)+'"',self.lineNumber)

   def __matchTypeMod(self,aTypeMod,otherIndexList,otherTypeModList):
      for otherIndex in otherIndexList:
         otherTypeMod=otherTypeModList[otherIndex]
         if (isinstance(aTypeMod, type(otherTypeMod))):
            if (str(aTypeMod)==str(otherTypeMod)):
               otherIndexList.pop(otherIndex)
               return True;
         elif (isInstance(aTypeMod,_KindTypeMod)
               and
               isInstance(otherTypeMod,_KindTypeMod)) :
            if (self._guessBytes(aTypeMod) == self._guessBytes(otherTypeMod)):
               return True;
                   

   def __matchTypeModList(self,modListPair):
      indexListPair=map(lambda l:range(len(a)),modListPair)
      while (indexListPair!=[[],[]]):
         if indexListPair[0] :
            if (self.__matchTupeMod(modListPair[0][indexListPair[0][0]],indexListPair[1],modListPair[1])):
               indexListPair[0].pop(0)
            else :
               return False
         if indexListPair[1] :
            if (self.__matchTupeMod(modListPair[1][indexListPair[1][0]],indexListPair[0],modListPair[0])):
               indexListPair[1].pop(0)
            else :
               return False
      return True

   def matchTypes(self,aTypePair):
      if aTypePair[0]==aTypePair[1]:
         return True
      else:
         return False
      aTypeClassPair=map(lambda l: l[0],aTypePair)
      if (all(map(lambda l: l in [fortStmts.DoubleStmt,fortStmts.RealStmt],aTypeClassPair))) :
         if (self._guessBytes(aTypePair[0])==self._guessBytes(aTypePair[1])):
            return True
      if (all(map(lambda l: l==fortStmts.IntegerStmt,aTypeClassPair))) :
         if (self._guessBytes(aTypePair[0])==self._guessBytes(aTypePair[1])):
            return True
      return False

   def __genericFunctionType(self,aFunctionApp):
      specInfo=_genericResolve(aFunctionApp,self.localSymtab,self.lineNumber)
      if specInfo:
         return globalTypeTable.lookupTypeId(specInfo[1].typetab_id)

   def _appType(self,anApp):
      DebugManager.debug(sys._getframe().f_code.co_name+' called on '+str(anApp)+'...',newLine=False)
      if isinstance(anApp.head,Sel):  # example type%member(1)
         expType=self._expressionType(anApp.head)
         if isinstance(expType.entryKind,TypetabEntry.ArrayEntryKind) or \
                isinstance(expType.entryKind,TypetabEntry.ArrayPointerEntryKind) or \
                isinstance(expType.entryKind,TypetabEntry.AllocatableEntryKind):
            arraySlice=False
            for anArg in anApp.args:
               if isinstance(anArg,Ops) and anArg.op==':':
                  # arg is a slice
                  arraySlice=True
            if arraySlice:
               newType=globalTypeTable.enterArrayType(expType.getBaseTypeId(),anApp.args,expType.entryKind)
               return newType
            # if the args are all scalar, then the type is a scalar which is the base type of expType
            return expType.getBaseTypeEntry()
      if isinstance(anApp.head,App): # example: matrix(3)(2:14)
         return self._appType(anApp.head)
      returnType = None
      # intrinsics: do a type merge
      if (not isinstance(anApp.head,Sel)) and is_intrinsic(anApp.head):
         returnType = self.__intrinsicType(anApp)
         DebugManager.debug(' It is an INTRINSIC of type '+str(returnType))
      # nonintrinsics: Look for it in the symbol table or for implicit type
      else:
         returnType = self.__identifierType(anApp.head)
         if (returnType is None) :
            # this must be a generic
            returnType=self.__genericFunctionType(anApp)
         if (returnType is None) :
            raise InferenceError(sys._getframe().f_code.co_name+': no return type for generic call to "'+str(anApp)+'"',self.lineNumber)
         if isinstance(returnType.entryKind,TypetabEntry.ArrayEntryKind):
            arraySlice=False
            for anArg in anApp.args:
               if isinstance(anArg,Ops) and anArg.op==':':
                  # arg is a slice
                  arraySlice=True
            if arraySlice:
               newType=globalTypeTable.enterArrayType(returnType.getBaseTypeId(),anApp.args,returnType.entryKind)
               return newType
            # if the args are all scalar, then the type is a scalar which is the base type of expType
            return returnType.getBaseTypeEntry()
         if (returnType.isExternal()) :
            # this must be an implicitly typed external function
            # update the type
            (symtabEntry,containingSymtab) = self.localSymtab.lookup_name_level(anApp.head)
            implicitLocalType=containingSymtab.implicit[anApp.head[0]]
            if implicitLocalType: 
               symtabEntry.enterType(implicitLocalType,containingSymtab)
               DebugManager.warning(sys._getframe().f_code.co_name+' implicit typing for external symbol >'+anApp.head+'< to type: '+symtabEntry.typePrint(),self.lineNumber,DebugManager.WarnType.implicit)
               returnType = globalTypeTable.getTypeEntry(implicitLocalType[0](implicitLocalType[1],[],[]),self.localSymtab)
         DebugManager.debug(' It is an NONINTRINSIC of type '+str(returnType))
      return returnType

   def __selectionType(self,aSelectionExpression):
      DebugManager.debug(sys._getframe().f_code.co_name+' determining type of selection expression '+str(aSelectionExpression)+' using symtab '+str(self.localSymtab))
      # lookup type of head
      dType=self._expressionType(aSelectionExpression.head)
      if isinstance(dType.entryKind,TypetabEntry.ArrayEntryKind) or isinstance(dType.entryKind,TypetabEntry.AllocatableEntryKind) \
             or isinstance(dType.entryKind,TypetabEntry.ArrayPointerEntryKind):
         baseType=globalTypeTable.lookupTypeId(dType.getBaseTypeId())
         lookupName=baseType.entryKind.symbolName+":"+aSelectionExpression.proj
         (projSymtabEntry,projSymtab)=baseType.entryKind.localSymtab.lookup_name_level(lookupName)
      else:
         lookupName=dType.entryKind.symbolName+":"+aSelectionExpression.proj
         (projSymtabEntry,projSymtab)=dType.entryKind.localSymtab.lookup_name_level(lookupName)
      if projSymtabEntry is not None:
         # lookup the projection type
         if isinstance(aSelectionExpression.proj,App):
            return self.__appType(aSelectionExpression.proj)
         pType=globalTypeTable.lookupTypeId(projSymtabEntry.typetab_id)
         return pType
      else:
         raise InferenceError(sys._getframe().f_code.co_name+': Selection expression projection does not have a symtab entry "'+str(aSelectionExpression)+'"',self.lineNumber)

   def _expressionType(self,anExpression):
      DebugManager.debug(sys._getframe().f_code.co_name+' determining type of expression '+str(anExpression)+'...',newLine=False)
      if isinstance(anExpression,str) and is_const(anExpression):
         rType=self._constantType(anExpression)
         DebugManager.debug(' it\'s a '+str(rType)+' constant')
         return rType
      elif isinstance(anExpression,str) and _id_re.match(anExpression):
         DebugManager.debug(' it\'s an IDENTIFIER')
         return self.__identifierType(anExpression)
      elif isinstance(anExpression,Unary):
         DebugManager.debug(' it\'s a UNARY EXPRESSION')
         return self._expressionType(anExpression.exp)
      elif isinstance(anExpression,Ops):
         if (anExpression.op =='//'):
            return self._typemerge([self._expressionType(anExpression.a1),
                                    self._expressionType(anExpression.a2)],
                                   (None,None),True)
         else: 
            DebugManager.debug(' it\'s a BINARY EXPRESSION')
            return self._typemerge([self._expressionType(anExpression.a1),
                                    self._expressionType(anExpression.a2)],
                                   (None,None))
      elif isinstance(anExpression,App):
         DebugManager.debug(' it\'s an APPLICATION')
         return self._appType(anExpression)
      elif isinstance(anExpression,NamedParam):
         DebugManager.debug(' it\'s a NAMED PARAMETER')
         return self._expressionType(anExpression.myRHS)
      elif isinstance(anExpression,Sel):
         DebugManager.debug(' it\'s a SELECTION EXPRESSION')
         return self.__selectionType(anExpression)
      elif isinstance(anExpression,Slice) :
         DebugManager.debug(' it\'s a SLICE EXPRESSION')
         return globalTypeTable.intrinsicTypeNameToEntry('integer_4')
      elif isinstance(anExpression,fortStmts._NoInit):
         DebugManager.debug(' it\'s a NO INIT EXPRESSION')
         return self._expressionType(anExpression.lhs)
      elif isinstance(anExpression,fortStmts._ImplicitDoConstruct):
         DebugManager.debug(' it\'s a implicit DO')
         return self._expressionType(anExpression.object)
      elif isinstance(anExpression,ArrayConstructor): # look at the first element in the list, they have to be consistent
         DebugManager.debug(' it\'s an ARRAY CONSTRUCTOR')
         return self._expressionType(anExpression.valueList[0])
      else:
         raise InferenceError(sys._getframe().f_code.co_name+': No type could be determined for expression "'+str(anExpression)+'" (represented as '+repr(anExpression)+' )',self.lineNumber)

def guessBytes(typePair,localSymtab,lineNumber):
   return _TypeContext(lineNumber,localSymtab)._guessBytes(typePair)

def appType(anApp,localSymtab,lineNumber):
   return _TypeContext(lineNumber,localSymtab)._appType(anApp)

def expressionType(anExpression,localSymtab,lineNumber):
   return _TypeContext(lineNumber,localSymtab)._expressionType(anExpression)

def canonicalTypeClass(typeClass,modList):
   if modList:
      if (typeClass==fortStmts.RealStmt):
         if (isinstance(modList[0],fortStmts._Kind)):
            if (modList[0].mod=='8'):
               return (True,fortStmts.DoubleStmt)
            if (modList[0].mod=='4'):
               return (True,fortStmts.RealStmt)
   else:
      return (True,typeClass)
   return (False,None)

def __shapecompare(d1,d2):
   DebugManager.debug(sys._getframe().f_code.co_name+' called on d1 = "'+str(d1)+'\td2 = "'+str(d2)+'"')
   if d1 is None :
      return d2
   return d1

def __shapemerge(lst,default):
   DebugManager.debug(sys._getframe().f_code.co_name+' called on '+str(lst)+'...',newLine=False)
   if not lst: return default
   if len(lst) == 1: return lst[0]
   d1 = __shapecompare(lst[0],lst[1])
   for l in lst[2:]:
      d1 = __shapecompare(d1,l)
   DebugManager.debug(' result is '+str(d1))
   return d1

def __constantShape(e,lineNumber):
   kind_re = re.compile(r'_(\w+)')
   if _flonum_re.match(e):
      return None
   if _int_re.match(e):
      return None
   if e.lower() in _logicon_set:
      return None
   if e[0] in _quote_set:
      return None
   raise InferenceError(sys._getframe().f_code.co_name+': No shape could be determined for "'+e+'"',lineNumber)

def __identifierShape(anId,localSymtab,lineNumber):
   returnShape=None
   (symtabEntry,containingSymtab) = localSymtab.lookup_name_level(anId)
   if symtabEntry:
      if symtabEntry.dimensions : 
         returnShape=[]
         for (r,dim) in enumerate(symtabEntry.dimensions):
            if (isinstance(dim, Zslice)):
               returnShape.append(App("size",[anId,str(r+1)]))
            elif (isinstance(dim, Rslice)):
               returnShape.append(Op(":",App("lbound",[anId,str(r+1)]),dim.arg))
            elif (isinstance(dim, Lslice)):
               returnShape.append(Op(":",dim.arg,App("ubound",[anId,str(r+1)])))
            else : 
               returnShape.append(dim)
      dbgStr='with symtab entry '+symtabEntry.debug(anId)+' -> returning shape '
      if returnShape:
         dbgStr+=str(returnShape)
      else:
         dbgStr+='None'
      DebugManager.debug(dbgStr)
   return returnShape

def __arrayReferenceShape(arrRefApp,localSymtab,lineNumber):
   DebugManager.debug(sys._getframe().f_code.co_name+' called on '+repr(arrRefApp)+'...',newLine=False)
   symtabEntry=None
   if isinstance(arrRefApp.head,Sel):
      theSel=arrRefApp.head
      # lookup type of head
      dType=expressionType(theSel.head,localSymtab,lineNumber)
      # lookup the projection
      (symtabEntry,containingSymtab)=dType.entryKind.localSymtab.lookup_name_level(dType.entryKind.symbolName+":"+theSel.proj)
      typeEntry=globalTypeTable.lookupTypeId(symtabEntry.typetab_id)
   else: 
      (symtabEntry,containingSymtab) = localSymtab.lookup_name_level(arrRefApp.head)
      typeEntry=globalTypeTable.lookupTypeId(symtabEntry.typetab_id)
   if typeEntry and isinstance(typeEntry.entryKind,TypetabEntry.ArrayEntryKind):
      arrayBounds=typeEntry.entryKind.getArrayBounds()
   dimensions=[]
   symDimIndex=0
   for index in arrRefApp.args:
      if isinstance(index,Zslice):
         dimensions.append(symtabEntry.dimensions[symDimIndex])
      if isinstance(index,Lslice):
         if (isinstance(symtabEntry.dimensions[symDimIndex],Ops)):
            uBound=symtabEntry.dimensions[symDimIndex].a2
         else:
            uBound=symtabEntry.dimensions[symDimIndex]
         dimensions.append(Ops(':',index.arg,uBound))
      if isinstance(index,Rslice):
         if (isinstance(symtabEntry.dimensions[symDimIndex],Ops)):
            lBound=symtabEntry.dimensions[symDimIndex].a1
         else:
            lBound='1'
         dimensions.append(Ops(':',lBound,index.arg))
      elif (__isRangeExpression(index)):
         dimensions.append(index)
      symDimIndex+=1
   while symDimIndex<len(symtabEntry.dimensions):
      dimensions.append(symtabEntry.dimensions[symDimIndex])
   if (dimensions!=[]): 
      return dimensions
   return None

def __intrinsicShape(anIntrinsicApp,localSymtab,lineNumber):
   if anIntrinsicApp.head.lower() in ['matmul']:
      raise InferenceError(sys._getframe().f_code.co_name+': Not implemented for "'+anIntrinsicApp+'"',lineNumber)
   if anIntrinsicApp.head.lower() in ['ichar','maxval','minval','lge','lgt','lle','llt','scan','size','time']:
      return None
   if anIntrinsicApp.head.lower() in ['maxloc']:
      if (len(anIntrinsicApp.args)>1): 
         raise InferenceError(sys._getframe().f_code.co_name+': Not implemented for "'+anIntrinsicApp+'" with more than 1 argument',lineNumber)
      return [Ops(':','1',str(len(expressionShape(anIntrinsicApp.args[0],localSymtab,lineNumber))))]
   if anIntrinsicApp.head.lower() in ['reshape']:
      if (len(anIntrinsicApp.args)>2): 
         raise InferenceError(sys._getframe().f_code.co_name+': Not implemented for "'+anIntrinsicApp+'" with more than 2 arguments',lineNumber)
      return self._expressionShape(anIntrinsicApp.args[1]) # the shape of the second argument
   else:
      return __shapemerge([expressionShape(anArg,localSymtab,lineNumber) for anArg in anIntrinsicApp.args],
                       (None,None))

__ourMissingFuncDefs=set()
def __functionShape(aFunctionApp,localSymtab,lineNumber):
   class MapParams:

      def __init__(self,aFunctionApp,aSymtabEntry,lineNumber):
         self.argDict={}          
         for pos,arg in enumerate(aFunctionApp.args):
            if isinstance(arg,NamedParam):
               if (arg.myId in self.argDict.keys()):
                  raise InferenceError(sys._getframe().f_code.co_name+': Formal parameter for actual named parameter: '+str(arg)+
                                       ' is already in the  mapping dictionary for positional parameter '+
                                       str(argDict[arg.myId][1])+' at position '+str(argDict[arg.myId][0]),lineNumber)
               namedParmFormalPosition=symtabEntry.specificFormalArgs.positionOf(arg.myId)
               if (namedParmFormalPosition in map(lambda l: l[0],self.argDict.values())):
                  raise InferenceError(sys._getframe().f_code.co_name+': Formal parameter position '+namedParmFormalPosition+' for actual named parameter: '+str(arg)+
                                       ' is already in the  mapping dictionary as '+
                                       str(argDict[arg.myId][1]),lineNumber)
               self.argDict[arg.myId]=(symtabEntry.specificFormalArgs.positionOf(arg.myId),arg.myRHS)
            else:
               formalParm=symtabEntry.specificFormalArgs.nameByPosition(pos)
               if (formalParm in self.argDict.keys() or pos in map(lambda l: l[0],self.argDict.values())):
                  raise InferenceError(sys._getframe().f_code.co_name+': Formal parameter: '+formalParm+' for position '+str(pos)+
                                       ' is already in the  mapping dictionary with actual parameter value '+str(self.argDict[arg.myId][1]),lineNumber)
               self.argDict[formalParm]=(pos,arg)

      def replaceFormalWithActual(self,dimExpr):
         ''' this modifies dimExpr'''
         if  isinstance(dimExpr,Unary):
            dimExpr.exp=self.replaceFormalWithActual(dimExpr.exp)
         elif isinstance(dimExpr,Ops):
            dimExpr.a1=self.replaceFormalWithActual(dimExpr.a1)
            dimExpr.a2=self.replaceFormalWithActual(dimExpr.a2)
         elif isinstance(dimExpr,App):
            args=[]
            for arg in dimExpr.args:
               args.append(self.replaceFormalWithActual(arg))
            dimExpr.args=args
         elif isinstance(dimExpr,str) and is_id(dimExpr) and dimExpr in self.argDict.keys():
            dimExpr=copy.deepcopy(self.argDict[dimExpr][1])
         return dimExpr

   DebugManager.debug(sys._getframe().f_code.co_name+' called on '+str(aFunctionApp)+'...',newLine=False)
   # example: bbb(3)(2:14)
   if isinstance(aFunctionApp.head,App):
      return __functionShape(aFunctionApp.head,localSymtab,lineNumber)
   returnShape = None
   # intrinsics: do a shape merge
   if is_intrinsic(aFunctionApp.head):
      returnShape = __intrinsicShape(aFunctionApp,localSymtab,lineNumber)
      DebugManager.debug(' It is an INTRINSIC of shape '+str(returnShape))
   # non-intrinsics: Look for it in the symbol table
   else:
      (symtabEntry,containingSymtab) = localSymtab.lookup_name_level(aFunctionApp.head)
      if symtabEntry:
         returnShape = symtabEntry.dimensions
      if (returnShape is None) :
         # this may be a generic
         DebugManager.debug(sys._getframe().f_code.co_name+' called on '+str(aFunctionApp)+'...',newLine=False)
         specInfo=_genericResolve(aFunctionApp,localSymtab,lineNumber)
         if specInfo:
            symtabEntry=specInfo[1]
            returnShape=specInfo[1].dimensions
      DebugManager.debug(' It is an NONINTRINSIC of shape '+str(returnShape))
      if (aFunctionApp.args and symtabEntry is None):
         global __ourMissingFuncDefs
         if (not (aFunctionApp.head.lower() in __ourMissingFuncDefs)) :
            __ourMissingFuncDefs.add(aFunctionApp.head.lower())
            DebugManager.warning('no explicit definition for function "'+str(aFunctionApp.head)+'" called as "'+str(aFunctionApp)+'" found within the current compile unit.',lineNumber,DebugManager.WarnType.noDefinition)
      if (returnShape and aFunctionApp.args and symtabEntry and symtabEntry.specificFormalArgs):
         # the dimensions need to be checked for references to formal parameters
         aMapParams=MapParams(aFunctionApp,symtabEntry,lineNumber)
         returnShape=copy.deepcopy(returnShape) # don't want to modify the original
         for dim in returnShape:
            aMapParams.replaceFormalWithActual(dim)
      elif (aFunctionApp.args and symtabEntry and symtabEntry.specificFormalArgs):
         aMapParams=MapParams(aFunctionApp,symtabEntry,lineNumber)
         returnShape=copy.deepcopy(returnShape) # don't want to modify the original
   return returnShape

def __selectionShape(aSelectionExpression,localSymtab,lineNumber):
   DebugManager.debug(sys._getframe().f_code.co_name+' determining shape of selection expression '+str(aSelectionExpression)+' using symtab '+str(localSymtab))
   # if not a scalar, that's the shape. Can stop & return it now.
   # if a scalar, recur on projection shape, if projection is a selection
   # lookup type of head
   dType=expressionType(aSelectionExpression.head,localSymtab,lineNumber)
   if isinstance(dType.entryKind,TypetabEntry.ArrayEntryKind):
      if isinstance(aSelectionExpression.head,App):
         if (dType.entryKind.getArrayRank()==len(aSelectionExpression.head.args)):
            # constant
            return None
      # this is the shape, so we can return now.
      return dType.entryKind.getArrayBounds()
   elif isinstance(aSelectionExpression.proj,Sel):
      return __selectionShape(aSelectionExpression.proj,dType.entryKind.localSymtab,lineNumber)
   else:
      projSymtabEntry=dType.entryKind.localSymtab.lookup_name_local(dType.entryKind.symbolName+":"+aSelectionExpression.proj)
      projType=globalTypeTable.lookupTypeId(projSymtabEntry.typetab_id)
      if isinstance(projType.entryKind,TypetabEntry.ArrayEntryKind):
         return projType.getArrayBounds()
      else:
         return __identifierShape(aSelectionExpression.proj,dType.entryKind.localSymtab,lineNumber)

def expressionShape(anExpression,localSymtab,lineNumber):
   DebugManager.debug(sys._getframe().f_code.co_name+' determining shape of expression '+str(anExpression)+'...',newLine=False)
   if isinstance(anExpression,str) and is_const(anExpression):
      rShape=__constantShape(anExpression,lineNumber)
      DebugManager.debug(' it\'s a '+str(rShape)+' constant')
      return rShape
   elif isinstance(anExpression,str) and _id_re.match(anExpression):
      DebugManager.debug(' it\'s an IDENTIFIER')
      return __identifierShape(anExpression,localSymtab,lineNumber)
   elif isinstance(anExpression,Unary):
      DebugManager.debug(' it\'s a UNARY EXPRESSION')
      return expressionShape(anExpression.exp,localSymtab,lineNumber)
   elif isinstance(anExpression,Ops):
      DebugManager.debug(' it\'s a BINARY EXPRESSION')
      return __shapemerge([expressionShape(anExpression.a1,localSymtab,lineNumber),
                        expressionShape(anExpression.a2,localSymtab,lineNumber)],
                                 (None,None))
   elif isinstance(anExpression,App):
      DebugManager.debug(' it\'s an APPLICATION')
      if isArrayReference(anExpression,localSymtab,lineNumber):
         return __arrayReferenceShape(anExpression,localSymtab,lineNumber)
      else : 
         return __functionShape(anExpression,localSymtab,lineNumber)
   elif isinstance(anExpression,NamedParam):
      DebugManager.debug(' it\'s a NAMED PARAMETER')
      return expressionShape(anExpression.myRHS,localSymtab,lineNumber)
   elif isinstance(anExpression,Sel):
      DebugManager.debug(' it\'s a SELECTION EXPRESSION')
      return __selectionShape(anExpression,localSymtab,lineNumber)
   elif isinstance(anExpression,ArrayConstructor):  
      DebugManager.debug(' it\'s an ARRAY CONSTRUCTOR')
      return [len(anExpression.valueList)]
   else:
      raise InferenceError(sys._getframe().f_code.co_name+': No shape could be determined for expression "'+str(anExpression)+'"',lineNumber)

def _genericResolve(aFunctionApp,localSymtab,lineNumber):
   ''' returns tuple (<specificName>,<symtabEntry>) for generic aFunctionApp '''
   DebugManager.debug(sys._getframe().f_code.co_name+' entered for '+str(aFunctionApp))
   # find the symbol:
   sName=aFunctionApp.head
   symtabEntry=localSymtab.lookup_name(aFunctionApp.head)
   if symtabEntry is None:
      # f77 style call, doesn't have to be in the symboltable
      DebugManager.debug(sys._getframe().f_code.co_name+' no symtab entry, f77 style call '+
                         aFunctionApp.head+'('+','.join([str(arg) for arg in aFunctionApp.args]))
      return None
   if (symtabEntry.genericInfo is None
       or
       len(symtabEntry.genericInfo.resolvableTo)==0) : # not overloaded
      DebugManager.debug(sys._getframe().f_code.co_name+' returning specific call info '+sName+' because of '+str(symtabEntry.debug())) 
      return (sName,symtabEntry)
   # find a match for the signature:
   matchedName=""
   matchCount=0
   for sName in symtabEntry.genericInfo.resolvableTo.keys():
      specificFormalArgs=symtabEntry.genericInfo.resolvableTo[sName].specificFormalArgs
      # we don't cover optional arguments here - yet
      if len(specificFormalArgs.args)!=len(aFunctionApp.args):
         DebugManager.debug(sys._getframe().f_code.co_name+' signature length mismatch at specific '+
                            str(sName)+'('+','.join(specificFormalArgs.args.keys())+
                            ') for generic call '+
                            aFunctionApp.head+'('+','.join([str(arg) for arg in aFunctionApp.args]))
         continue
      matched=True
      aTypeContext=_TypeContext(lineNumber,localSymtab)
      for formal,actual in zip(specificFormalArgs.args.keys(),aFunctionApp.args):
         theExpressionType=expressionType(actual,localSymtab,lineNumber)
         sigType=globalTypeTable.lookupTypeId(specificFormalArgs.args[formal][1].typetab_id)
         if (not globalTypeTable.equivalence(theExpressionType,sigType)):
            DebugManager.debug(sys._getframe().f_code.co_name+' argument type mismatch for specific "'+
                               str(sName)+'" at formal "'+
                               str(formal)+'"('+str(specificFormalArgs.args[formal][0])+')'
                               ' for call to generic "'+
                               aFunctionApp.head+'" at actual "'+
                               str(actual)+'"('+ str(expressionType(actual,localSymtab,lineNumber))+')')
            matched=False
            break
         formalRank=0
         if isinstance(sigType.entryKind,TypetabEntry.ArrayEntryKind):
            formalRank=globalTypeTable.lookupTypeId(specificFormalArgs.args[formal][1].typetab_id).entryKind.getArrayRank()
         actualRank=0
         actualShape=expressionShape(actual,localSymtab,lineNumber)
         if (actualShape):
            actualRank=len(actualShape)
         if formalRank!=actualRank:
            DebugManager.debug(sys._getframe().f_code.co_name+' argument rank mismatch for specific "'+
                               str(sName)+'" at formal "'+
                               str(formal)+'"('+str(formalRank)+')'
                               ' for call to generic "'+
                               aFunctionApp.head+'" at actual "'+
                               str(actual)+'"('+ str(actualRank)+')')
            matched=False
            break
      if (matched):
         matchCount+=1
         matchedName=sName
   if (matchCount==1): 
      return (matchedName,localSymtab.lookup_name(matchedName))
   # collect error info:
   reason=""
   if (matchCount==0):
      reason="Could not match"
   else:
      reason="Ambiguous matches for" 
   actualSignature="("+".".join(map(lambda l: expressionType(l,localSymtab,lineNumber).debug(), aFunctionApp.args))+")"
   specifics=[]
   for sName in symtabEntry.genericInfo.resolvableTo.keys():
      signature=symtabEntry.genericInfo.resolvableTo[sName].specificFormalArgs.args
      specifics.append("\t"+sName+"("+",".join(map(lambda l: globalTypeTable.lookupTypeId(l[1].typetab_id).debug(),signature.values()))+")")
   raise InferenceError(sys._getframe().f_code.co_name+': '+reason+' call to generic '+aFunctionApp.head+actualSignature+' to specific implemantation(s):\n '+'\n'.join(specifics),lineNumber)

def __isRangeExpression(theExpression):
   return (isinstance(theExpression,Ops) and theExpression.op==':')

def __selSymtabName(aSel,localSymtab,lineNumber):
   ''' for aSel the name to be used for a symbol table lookup is "<type_name>:<member name>" '''
   # lookup type of head
   dType=expressionType(aSel.head,localSymtab,lineNumber)
   if (not dType):
      raise InferenceError(sys._getframe().f_code.co_name+': Did not correctly resolve type of "'+aSel.head+'"',lineNumber)
   return dType[1][0]+":"+aSel.proj

def __ntArrayReference(namedType,localSymtab,lineNumber):
   DebugManager.debug(sys._getframe().f_code.co_name+' Named Type Expression "'+str(namedType))
   if isinstance(namedType,App):
      return __ntArrayReference(namedType.head,localSymtab,lineNumber)
   elif isinstance(namedType,Sel):
      nType=expressionType(namedType.head,localSymtab,lineNumber)
      if isinstance(nType.entryKind,TypetabEntry.NamedTypeEntryKind):
         (ntSymtabEntry,ntSymtab)=nType.entryKind.localSymtab.lookup_name_level(nType.entryKind.symbolName+":"+namedType.proj)
         if ntSymtabEntry is None:
            return False
         ntTypeEntry=globalTypeTable.lookupTypeId(ntSymtabEntry.typetab_id)
         if isinstance(ntTypeEntry.entryKind,TypetabEntry.AllocatableEntryKind) or \
                isinstance(ntTypeEntry.entryKind,TypetabEntry.ArrayEntryKind) or \
                isinstance(ntTypeEntry.entryKind,TypetabEntry.ArrayPointerEntryKind) or \
                isinstance(ntTypeEntry.entryKind,TypetabEntry.CharacterEntryKind) or \
                (isinstance(ntTypeEntry.entryKind,TypetabEntry.BuiltInEntryKind) and ntTypeEntry.entryKind.type_name=='character'):
            return True
         return __ntArrayReference(ntTypeEntry.entryKind.symbolName,ntTypeEntry.entryKind.localSymtab,lineNumber)
      else:
         baseType=nType.getBaseTypeEntry()
         (ntSymtabEntry,ntSymtab)=baseType.entryKind.localSymtab.lookup_name_level(baseType.entryKind.symbolName+":"+namedType.proj)
         if ntSymtabEntry is None:
            return False
         ntTypeEntry=globalTypeTable.lookupTypeId(ntSymtabEntry.typetab_id)
         if isinstance(ntTypeEntry.entryKind,TypetabEntry.AllocatableEntryKind) or \
                isinstance(ntTypeEntry.entryKind,TypetabEntry.ArrayEntryKind) or \
                isinstance(ntTypeEntry.entryKind,TypetabEntry.ArrayPointerEntryKind) or \
                isinstance(ntTypeEntry.entryKind,TypetabEntry.CharacterEntryKind) or \
                (isinstance(ntTypeEntry.entryKind,TypetabEntry.BuiltInEntryKind) and ntTypeEntry.entryKind.type_name=='character'):
            return True
         elif isinstance(ntTypeEntry.entryKind,TypetabEntry.NamedTypeEntryKind):
            return __ntArrayReference(ntTypeEntry.entryKind.symbolName,ntTypeEntry.entryKind.localSymtab,lineNumber)
   else:
      typeEntry=expressionType(namedType,localSymtab,lineNumber)
      DebugManager.debug(sys._getframe().f_code.co_name+' typetab entry is '+typeEntry.debug())
      if isinstance(typeEntry.entryKind,TypetabEntry.AllocatableEntryKind) or \
             isinstance(typeEntry.entryKind,TypetabEntry.ArrayEntryKind) or \
             isinstance(theTypeEntry.entryKind,TypetabEntry.ArrayPointerEntryKind) or \
             isinstance(typeEntry.entryKind,TypetabEntry.CharacterEntryKind) or \
             (isinstance(typeEntry.entryKind,TypetabEntry.BuiltInEntryKind) and typeEntry.entryKind.type_name=='character'):
         return True
      return False

def isArrayReference(theApp,localSymtab,lineNumber):
   if not isinstance(theApp,App):
      raise InferenceError(sys._getframe().f_code.co_name+': Called on non-App object '+str(theApp),lineNumber)
   DebugManager.debug(sys._getframe().f_code.co_name+' Application Expression "'+str(theApp))
   lookupName=""
   if isinstance(theApp.head,Sel): # example type%member(1)
      return __ntArrayReference(theApp.head,localSymtab,lineNumber)
   else:
      theSymtabEntry=localSymtab.lookup_name(theApp.head)
   DebugManager.debug(sys._getframe().f_code.co_name+' symtab entry is '+str(theSymtabEntry))
   if not theSymtabEntry:
      return False
   # there has to be a symbol table entry for a variable
   DebugManager.debug(sys._getframe().f_code.co_name+' for '+theSymtabEntry.debug(lookupName))
   if isinstance(theSymtabEntry.entryKind,SymtabEntry.ProcedureEntryKind) or \
          isinstance(theSymtabEntry.entryKind,SymtabEntry.InterfaceEntryKind):
      return False
   theTypeEntry=globalTypeTable.lookupTypeId(theSymtabEntry.typetab_id)
   if theTypeEntry is not None and (isinstance(theTypeEntry.entryKind,TypetabEntry.ArrayEntryKind) or \
                                       isinstance(theTypeEntry.entryKind,TypetabEntry.AllocatableEntryKind) or \
                                       isinstance(theTypeEntry.entryKind,TypetabEntry.ArrayPointerEntryKind) or \
                                       isinstance(theTypeEntry.entryKind,TypetabEntry.CharacterEntryKind) or \
                                       (isinstance(theTypeEntry.entryKind,TypetabEntry.BuiltInEntryKind) and \
                                           theTypeEntry.entryKind.type_name=='character')):
      return True
   elif theTypeEntry is not None and isinstance(theTypeEntry.entryKind,TypetabEntry.NamedTypeEntryKind):
      # named type initialization
      return False
   else:
      #  now we know that its NOT a scalar variable, but rather a function.  so we update the symbol table with this information.
      if (not theSymtabEntry.entryKind.__class__ in [SymtabEntry.InterfaceEntryKind,SymtabEntry.StatementFunctionEntryKind]) : 
         DebugManager.debug(sys._getframe().f_code.co_name+' Application Expression "'+str(theApp)+\
                            '" for something that we thought was a scalar variable => assuming it\'s a function and updating the symbol table to reflect this')
         theSymtabEntry.enterEntryKind(SymtabEntry.FunctionEntryKind())
      return False

def isSpecExpression(theExp,localSymtab,lineNumber):
   ''' true of theExp could be used as a specification expression in a declaration, conservative (incomplete) logic'''
   retVal=False
   if isConstantExpression(theExp):
      retVal=True
   elif (isinstance(theExp,Zslice)):
       retVal=True
   elif (isinstance(theExp,NamedParam)):
      retVal=isSpecExpression(theExp.myRHS,localSymtab,lineNumber)
   elif (isinstance(theExp,App)):
      if (is_inquiry(theExp.head)):
          # first argument is special:
          if (isinstance(theExp.args[0],str) and is_id(theExp.args[0])):
             retVal=True
          else:
             retVal=isSpecExpression(theExp.args[0],localSymtab,lineNumber)
          if (retVal and len(theExp.args)>1): 
             retVal=all(map(lambda l: isSpecExpression(l,localSymtab,lineNumber),theExp.args))
      elif (isArrayReference(theExp,localSymtab,lineNumber)
            and
            all(map(lambda l: isSpecExpression(l,localSymtab,lineNumber),theExp.args))):
         retVal=True
   elif (isinstance(theExp,str) and is_id(theExp)):
      symtabEntry=localSymtab.lookup_name(theExp)
      if (symtabEntry and (symtabEntry.constInit or symtabEntry.origin!="local")):
         retVal=True
   elif (isinstance(theExp,Unary)):
      retVal=isSpecExpression(theExp.exp,localSymtab,lineNumber)
   elif (isinstance(theExp,Ops)):
      retVal=(isSpecExpression(theExp.a1,localSymtab,lineNumber) and isSpecExpression(theExp.a2,localSymtab,lineNumber))
   return retVal

def makeDeclFromName(symtab,name,lineNumber):
        expTypeEntry=expressionType(name,symtab,lineNumber)
        varShape=expressionShape(name,symtab,lineNumber)
        modList=[];attrList=[]
        if varShape:
            declDimArgs=[]
            for dim in varShape:
                if (not isSpecExpression(dim,symtab,lineNumber)):
                    raise InferenceError('cannot find dimension for '+name, lineNumber)
                else:
                    declDimArgs.append(dim)
            attrList.append(App('dimension',declDimArgs))
        if (isinstance( expTypeEntry.entryKind,TypetabEntry.ArrayEntryKind)):
            expTypeEntry=globalTypeTable.lookupTypeId(expTypeEntry.getBaseTypeId())
        if (isinstance(expTypeEntry.entryKind,TypetabEntry.NamedTypeEntryKind)):
            varTypeClass=fortStmts.DrvdTypeDecl
            modList.insert(0,expTypeEntry.entryKind.symbolName)
        elif (isinstance(expTypeEntry.entryKind,TypetabEntry.CharacterEntryKind)):
            charLenEntry=globalTypeTable.charLenTab.lookupCharLenId(expTypeEntry.entryKind.charLenId)
            varTypeClass=fortStmts.CharacterStmt
            modList.append(charLenEntry.getCharLenExp())
        else: 
            varTypeClass=globalTypeTable.intrinsicIdToTypeMap[expTypeEntry.getBaseTypeId()][0]
        if (not attrList):
            attrList=None
        decl=varTypeClass(modList,attrList,[name])
        return decl
