#!/bin/tcsh -f

source /home/heejong/sim/geant4/geant4.9.1.p01/env.csh

set loop = 0

while( $loop < 5)

cat >  test_$loop.mac << +EOF

/run/verbose 2
/control/verbose 2

# detector dimension
/detector/hall_x 50.0 mm
/detector/hall_y 50.0 mm
/detector/hall_z 50.0 mm

/detector/slab_x 1.0 mm
/detector/slab_y 1.0 mm
/detector/slab_z 5.0 mm

/detector/guide_x1 1.0 mm
/detector/guide_y1 1.0 mm
/detector/guide_x2 1.0 mm
/detector/guide_y2 1.0 mm
/detector/guide_z  0.1 mm

/detector/sipm_x 1.0 mm
/detector/sipm_y 1.0 mm
/detector/sipm_z 1.0 mm

/detector/distance 10.0 mm

/detector/update


# radiation source
/gps/energy        511 keV

/gps/pos/type      Volume
/gps/pos/shape     Cylinder
/gps/pos/centre    0. 0. 0. mm
/gps/pos/radius    1.0 mm
/gps/pos/halfz     2.0 mm

/gps/ang/type iso
/gps/ang/mintheta    0. deg
/gps/ang/maxtheta    1. deg
/gps/ang/minphi      0. deg
/gps/ang/maxphi    360. deg


# histogram file
/histo/file test_$loop

/run/beamOn 200


+EOF
    
single test_$loop.mac >> test_$loop.out
#echo "loop = $loop"
@ loop++
end
