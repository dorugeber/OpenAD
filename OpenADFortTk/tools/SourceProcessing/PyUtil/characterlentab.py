from _Setup import *

import PyFort.fortExp as fe

class CharacterLenTab(object):
    '''class to keep track of character entry lengths for character entry types in type table'''
    def __init__(self):
        self.ids = dict() # string for the key and ArrayBoundsTabEntry for the value
        self.entry_counter=1

    def lookupCharLenId(self,charlen_id):
        '''get char len entry by charlen_id'''
        if charlen_id in self.ids:
            return self.ids[charlen_id]
        else:
            return None

    def lookupCharLen(self,charLenExp):
        '''get the chararcter length entry id by the character length expression, or return None'''
        for entry in self.ids.values():
            if entry.charLenExp==charLenExp:
                return entry.charLenId
        return None

    def __enterNewCharLen(self,charLenExp):
        '''create & add a new character len tab entry with the given char length expression; return id'''
        newEntry=CharacterLenTabEntry(charLenExp,self.entry_counter)
        self.ids[self.entry_counter]=newEntry
        self.entry_counter += 1
        return newEntry.charLenId

    def getCharLen(self,charLenExp):
        '''get the character length id for the entry corresponding to the given expression'''
        charLenId = self.lookupCharLen(charLenExp)
        if not charLenId:
            charLenId = self.__enterNewCharLen(charLenExp)
        return charLenId

class CharacterLenTabEntry(object):
    '''class to keep track of character entry lengths for character entry types in type table'''

    def __init__(self,charLenExp,charLenId):
        self.charLenExp=charLenExp    # expression for the character length
        self.charLenId=charLenId      # array bounds info id number in ArrayBoundsTab

    def getCharLenExp(self):
        return self.charLenExp

    def debug(self):
        return 'CharacterLenTabEntry; character lenght expression: ',self.charLenExp,'; character length ID:',self.charLenId
