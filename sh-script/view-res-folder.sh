#!/bin/sh

## Input an output management
export filePattern=/archive/bioinformatics/Danuser_lab/softwareDevelopment/analysis/proudot/cytodi-OF3D/raw/diaSLM/2015-09-10-MV3-EGFP-Tractin/Cell19/ch1/1_CAM01_%.6d.tif
sequence=$(seq 50 99)
export outputFolder=/archive/bioinformatics/Danuser_lab/softwareDevelopment/analysis/proudot/cytodi-OF3D/analysis/flowScopTestFolder/
export outputMagPattern=$outputFolder/1_CAM01_%.6d.tifmag.tif

## Render and view
rm $outputFolder/*overay.tif
rm $outputFolder/*mip.tif
parallel 'source=$(printf "$filePattern" {}); target=$(printf "$filePattern" $(({}+1))); mag=$(printf "$outputMagPattern" $(({}+1))); \
	  gmic -tiff3d $source -resize 50%,50%,200% -tiff3d $mag \
	       -orthoMIP -o[0] ${mag}_mip.tif  -o[1] ${mag}_overlay.tif' ::: $sequence

gmic  $outputFolder/*mip.tif -a z -n 0,256 \
      -l[] $outputFolder/*overlay.tif -blur z,10 -n 0,256 -a z -endl \
      -to_rgb[0] -map[-1] 3 -blend overlay,30% 




