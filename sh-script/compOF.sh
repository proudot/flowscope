module load cmake/3.10.2
module load gcc/8.3.0
module load tiff/gcc/4.0.7
outputFolder=/archive/bioinformatics/Danuser_lab/softwareDevelopment/analysis/proudot/cytodi-OF3D/analysis/flowScopTest/
mkdir $outputFolder
#../flowscope/CSL2/build/CTvar -s ../mv3blebbing/1_CAM01_000004.tif -t ../mv3blebbing/1_CAM01_000008.tif -o $outputFolder
gmic $outputFolder/*soru.tif -a z -l[] $outputFolder/*sorv.tif -a z -endl -l[] $outputFolder/*soru.tif -a z -endl -a c \
     -normp 2 -o $outputFolder/mag.tif
