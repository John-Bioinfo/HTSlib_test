
export LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH}

gcc modify_baseQual.c -L/root/htslib/ -o modify_qual -I/root/samtools -std=c99 -lhts

