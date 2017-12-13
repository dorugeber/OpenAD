'''
Save and get module units
Currently only a thin veneer over caseless dict
'''
import _Setup
from PyUtil.caselessDict import caselessDict as cDict

class ModuleHandler(object):
    def __init__(self):
        self.modules = cDict() # for key module name contains as value the module unit 
    def add_module(self,name,moduleUnit):
        self.modules[name] = moduleUnit
    def get_module(self,name):
        return self.modules.get(name)

ourModuleHandler = ModuleHandler()

