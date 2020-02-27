gmic -tiff3d ../mv3blebbing/1_CAM01_000004.tif -tiff3d ../mv3blebbing/1_CAM01_000008.tif -resize 50%,50%,200% -tiff3d ../output/mag.tif \
     -n 0,256 -orthoMIP -a[0,1] z -resize[-1] [0] -to_rgb[0] -map[-1] 3 blend overlay,30%
