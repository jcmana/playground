#import module.module_script
#import module.module_part.module_script


#from . import module_script
#from .module_part import module_script


import importlib
importlib.import_module(name='.module_script', package='module')
importlib.import_module(name='.module_part', package='module')
importlib.import_module(name='.module_part.module_script', package='module')
importlib.import_module(name='.module_part.module_part', package='module')
importlib.import_module(name='.module_part.module_part.module_script', package='module')
