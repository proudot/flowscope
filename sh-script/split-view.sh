gmic /tmp/split.cimg -repeat $! -l[0] -s c -to_rgb[0,1] -a[0,1] z -map[1] 3 -resize[1] [0] -blend overlay,30% -endl -mv[0] -1  -done
