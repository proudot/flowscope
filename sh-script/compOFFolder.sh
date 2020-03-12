module load cmake/3.10.2
module load gcc/8.3.0
module load tiff/gcc/4.0.7
module load parallel

export filePattern=/archive/bioinformatics/Danuser_lab/softwareDevelopment/analysis/proudot/cytodi-OF3D/raw/diaSLM/2015-09-10-MV3-EGFP-Tractin/Cell19/ch1/1_CAM01_%.6d.tif
sequence=$(seq 50 99)
export outputFolder=/archive/bioinformatics/Danuser_lab/softwareDevelopment/analysis/proudot/cytodi-OF3D/analysis/flowScopTestFolder/

mkdir $outputFolder

# rm $outputFolder/*
# parallel 'source=$(printf "$filePattern" {}); target=$(printf "$filePattern" $(({}+1))); \
#  	  ../flowscope/CSL2/build/CTvar -s $source  -t $target -o $outputFolder ' ::: $sequence

export outputFilePattern=$outputFolder/1_CAM01_%.6d.tif
export fileBase=1_CAM01_%.6d.tif
parallel 'source=$(printf "$outputFilePattern" {}); target=$(printf "$fileBase" $(({}+1))); \
	  gmic  $source\_${target}soru.tif -a z -l[]  $source\_${target}sorv.tif -a z -endl -l[]  $source\_${target}sorw.tif -a z -endl \
	   -a c -normp 2 -o $outputFolder${target}mag.tif' ::: $sequence

# Compute magnitude
#gmic $outputFolder/*soru.tif -a z -l[] $outputFolder/*sorv.tif -a z -endl -l[] $outputFolder/*soru.tif -a z -endl -a c \
 #    -normp 2 -o $outputFolder/mag.tif



