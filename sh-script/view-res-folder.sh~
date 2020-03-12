#!/bin/sh
outputFolder=/archive/bioinformatics/Danuser_lab/softwareDevelopment/analysis/proudot/cytodi-OF3D/analysis/flowScopTest/
gmic -tiff3d ../mv3blebbing/1_CAM01_000004.tif -tiff3d ../mv3blebbing/1_CAM01_000008.tif -resize 50%,50%,200% -tiff3d $outputFolder/mag.tif \
     -n 0,256 -orthoMIP -a[0,1] z -resize[-1] [0] -to_rgb[0] -map[-1] 3 blend overlay,30%
