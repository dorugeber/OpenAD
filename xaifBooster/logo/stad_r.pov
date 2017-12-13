// POV-Ray 3.6 Scene File "a_geo_sph1.pov"
// Author: Friedrich A. Lohmueller, 2005
// email: Friedrich.Lohmueller_at_t-online.de 
// homepage: http://wwww.f-lohmueller.de
//
//------------------------------------------------------------------------
global_settings {  assumed_gamma 1.0 }
//------------------------------------------------------------------------
#include "colors.inc"
#include "textures.inc"
#include "glass.inc"
#include "metals.inc"
#include "golds.inc"
#include "stones.inc"
#include "woods.inc"
#include "shapes.inc"
#include "shapes2.inc"
#include "functions.inc"
#include "math.inc"          
#include "transforms.inc"
//------------------------------------------------------------------------

camera{
  orthographic
  location <.5,5,.5>
  look_at <.5,0,.5>
}


light_source{<3000,3000,-1000> color White}
light_source{<-3000,3000,+1000> color White}


sky_sphere{ pigment{ gradient <0,1,0>
                     color_map{ [0   color rgb<1,1,1>         ]//White
                                [0.4 color rgb<1,1,1>]//~Navy
                                [0.6 color rgb<1,1,1>]//<0.14,0.14,0.56>]//~Navy
                                [1.0 color rgb<1,1,1>         ]//White
                              }
                     scale 2 }
           } // end of sky_sphere 

plane { <0,1,0>, 0    // plane with layered textures
        texture { pigment{color rgbt<1,1,1,0.7>*1.1}
                  finish {ambient 0.45 diffuse 0.85}}
        rotate<0,0,0>
        no_shadow
      }
#default{ finish {ambient 0.15 diffuse 0.85} } // 

#declare Rx  = 2.00;
#declare Ry  = 2.00;
#declare Rz  = 2.00;
 
union{ 
sphere{ <.5,0,-.5>, 1   pigment{ color Cyan transmit 0.0 } finish {phong .9 phong_size 160 }} 
sphere{ <.5,0,+.5>, 1   pigment{ color Cyan transmit 0.0 } finish {phong .9 phong_size 160 }} 
cone{<.5,0,-.5> ,1, <.5,0,.5>, 1 pigment{ color Cyan transmit 0.0 } finish {phong .9 phong_size 160 }}
}
