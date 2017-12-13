//
// POV-Ray(tm) 3.0 tutorial example scene.
// Copyright 1996 by the POV-Ray Team
//

//
// Note: image.pov has to be traced before this scene can be used.
//
 
#include "colors.inc"
#include "finish.inc"
#include "metals.inc"
#include "woods.inc"

camera{
  orthographic
location <.5,1.1,.5>
  look_at <.5,0,.5>
}

height_field {
  tga "title.tga"
  water_level 0.15
  smooth
  texture { 
    pigment { P_Silver1 }
    finish { F_MetalC }
  }  	
  scale <1, 0.04, 1>
}

light_source{ <-10,20,+10>  color red .8 green .8 blue 0.6 }
light_source{ <10,20,-10> color red .8 green .8 blue .6}

