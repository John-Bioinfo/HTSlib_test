# HTSlib_test
test coding for htslib 1.9

compile(first install htslib and samtools)
reference : https://www.biostars.org/p/328831/

- gcc parseTest.c -o parse_test -I/root/samtools -L/root/samtools -lhts -std=c99
- gcc parseBam_seq_qual.c -L/root/htslib/ -o main -I/root/samtools -std=c99 -lhts
- g++ parseBam.cpp -o parseBAM -I/root/htslib/htslib -L/root/samtools -lhts -std=c++11
- g++ testhtslib.cpp -o testHTSlib -I/root/htslib/htslib -L/root/samtools -lhts -std=c++11

