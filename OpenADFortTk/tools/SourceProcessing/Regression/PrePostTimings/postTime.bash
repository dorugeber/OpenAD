#! /bin/bash
echo -n "$0 "
../../postProcess.py --timing --infoUnitFile ../Extras/w2f__types.f90 --noWarnings -m r --width 4  postInput.f 
