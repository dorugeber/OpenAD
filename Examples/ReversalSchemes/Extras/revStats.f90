module RevStats

public :: revStatsInit, revStatsUpdateTape, revStatsUpdateCP, revStatsDump
public :: revStatsRevolveCPcount

type fCountersType   
integer :: & 
       box_forward=0, &
       box_final_state=0, &
       box_ini_fields=0, &
       box_ini_params=0, &
       box_model_body=0, &
       box_model_loop=0, &
       box_density=0, &
       box_transport=0, &
       box_robert_filter=0, &  
       box_cycle_fields=0, &
       box_update=0, &
       box_timestep=0
end type   

type(fCountersType), save, public :: revStatsFCounters

integer :: maxCP, maxTape, revStatsRevolveCPcount=0

contains

  subroutine revStatsInit()
    maxCP=0
    maxTape=0
    revStatsFCounters%box_forward=0
    revStatsFCounters%box_final_state=0
    revStatsFCounters%box_ini_fields=0
    revStatsFCounters%box_ini_params=0
    revStatsFCounters%box_model_body=0
    revStatsFCounters%box_model_loop=0
    revStatsFCounters%box_density=0
    revStatsFCounters%box_transport=0
    revStatsFCounters%box_robert_filter=0
    revStatsFCounters%box_cycle_fields=0
    revStatsFCounters%box_update=0
    revStatsFCounters%box_timestep=0
  end subroutine revStatsInit
  
  subroutine revStatsUpdateCP(cpPointer) 
    integer :: cpPointer
    if (cpPointer>maxCP) maxCP=cpPointer
  end subroutine revStatsUpdateCP
  
  subroutine revStatsupdateTape( tapePointer) 
    implicit none
    integer :: tapePointer
    if (tapePointer>maxTape) maxTape=tapePointer
  end subroutine revStatsupdateTape
  
  subroutine revStatsDump()
    print *,'revStats: '
    print *," maxCP=",maxCP, " maxTape=", maxTape
    print *," box_forward:      ",revStatsFCounters%box_forward
    print *," box_final_state:  ",revStatsFCounters%box_final_state
    print *," box_ini_fields:   ",revStatsFCounters%box_ini_fields
    print *," box_ini_params:   ",revStatsFCounters%box_ini_params
    print *," box_model_body:   ",revStatsFCounters%box_model_body
    print *," box_model_loop:   ",revStatsFCounters%box_model_loop
    print *," box_density:      ",revStatsFCounters%box_density
    print *," box_transport:    ",revStatsFCounters%box_transport
    print *," box_robert_filter:",revStatsFCounters%box_robert_filter
    print *," box_cycle_fields: ",revStatsFCounters%box_cycle_fields
    print *," box_update:       ",revStatsFCounters%box_update
    print *," box_timestep:     ",revStatsFCounters%box_timestep
  end subroutine revStatsDump

end module RevStats
