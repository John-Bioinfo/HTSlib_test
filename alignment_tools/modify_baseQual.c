#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sam.h"
//#include "hts.h"

int main(int argc, char *argv[]){
	samFile *fp_in = hts_open(argv[1],"r");   //open bam file
	bam_hdr_t *bamHdr = sam_hdr_read(fp_in);  //read header
	bam1_t *aln = bam_init1();                //initialize an alignment
	int readsNum = 0 ;

        char* outf;
        if (argv[2] != NULL){
            outf = argv[2];
        }
        else {
            outf = strcat(argv[1],".mod");
        }

        printf("%s\n", outf);
        printf("USAGE: ./modify_qual bamfile\n");

        samFile *fp = hts_open(outf,"wb");

        //write header
        if (bamHdr == NULL) {
                sam_close(fp);
                return 0;
        }

        //This will force the header to be parsed.
        if (sam_hdr_write(fp, bamHdr) != 0) {
                bam_hdr_destroy(bamHdr);
                sam_close(fp);
                return 0;
        }

	while(sam_read1(fp_in,bamHdr,aln) > 0){
                readsNum += 1;
		int32_t pos = aln->core.pos +1;   //left most position of alignment in zero based coordianate (+1)
		char *chr = bamHdr->target_name[aln->core.tid] ; //contig name (chromosome)
		uint32_t len = aln->core.l_qseq; //length of the read.
		uint8_t *s = bam1_seq(aln); 
		uint8_t *q = bam_get_qual(aln) ; //quality string start.
		char q2[200]; //quality string
                strcpy(q2, q);
		
		char *seq = (char *)malloc(len);
		//for(int i=0; i< len ; i++){
                      //gets nucleotide id and converts them into IUPAC id.
		     //	seq[i] = seq_nt16_str[bam_seqi(s,i)]; 
		// }

		for(int i=0; i< len ; i++){
		        //if(q2[i] >= 83){
		        if(q2[i] >= 72){
		                //gets quality of eachnucleotide base.
			        q[i] =  q2[i]/2; 
	          	}
                }
		//printf("%s\t%d\t%d\n",chr,pos,len);
                int ret = sam_write1(fp,bamHdr, aln);
                //printf("Return value of writing process: %d\n", ret);
                //bam_destroy1(aln);
	}
        bam_destroy1(aln);
	sam_close(fp_in);
        printf("Total reads number:\t%d\n", readsNum);

	return 0;
}


// Ref
// https://github.com/samtools/htslib/blob/d2b4c2c273de27f2544717dbafbb8e016b557130/test/test_realn.c 
// https://docs.rs/rust-htslib/0.24.0/rust_htslib/index.html
// https://rdrr.io/cran/rbamtools/
// https://github.com/samtools/htslib/blob/develop/htslib/sam.h
// http://118.31.76.100:100/python/py_module/pysam-bam-sam-to-read-and-write-files/
// http://ecogenomics.github.io/BamM/manual/BamM_manual.pdf
// 
