gmic -tiff3d ../mv3blebbing/1_CAM01_000004.tif -tiff3d ../mv3blebbing/1_CAM01_000008.tif \
     -resize 50%,50%,200% \
     -tiff3d ../output/mag.tif \
     -n 0,256 -a c -crop -s c -orthoMIP -a[0,1] z \
     -resize[-1] [-2] -map[-1] 3 -to_rgb[-2] -blend overlay,30%