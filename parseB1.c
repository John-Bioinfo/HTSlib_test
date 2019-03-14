#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sam.h"

int main(int argc, char *argv[]){
	samFile *fp_in = hts_open(argv[1],"r"); //open bam file
	bam_hdr_t *bamHdr = sam_hdr_read(fp_in); //read header
	bam1_t *aln = bam_init1(); //initialize an alignment
	char *chrom = argv[2];
	int locus = atoi(argv[3]);
	int comp ;

	printf("%s\t%d\n", chrom, locus);
	while(sam_read1(fp_in,bamHdr,aln) > 0){

		int32_t pos = aln->core.pos +1; //left most position of alignment in zero based coordianate (+1)
		char *chr = bamHdr->target_name[aln->core.tid] ; //contig name (chromosome)
		uint32_t len = aln->core.l_qseq; //length of the read.
		uint8_t *q = bam1_seq(aln); 
                // *q = bam_get_qual(aln); //quality string
		char qua[200]; //quality string start.
                //strcpy(q2seq, bam_get_qual(aln) );
		uint8_t *q2seq = bam_get_qual(aln) ; //quality string start.
                strcpy(qua, q2seq);
		
		char *qseq = (char *)malloc(len);
		// char *qseq = bam_get_seq(aln);
		for(int i=0; i< len ; i++){
			qseq[i] = seq_nt16_str[bam_seqi(q,i)]; //gets nucleotide id and converts them into IUPAC id.
		}
		for(int i=0; i< len ; i++){
			qua[i] = 33 + q2seq[i]; //gets nucleotide id and converts them into IUPAC id.
		}
                 

		if(strcmp(chrom, chr) == 0){
			 if(locus > pos+len){
				 printf("%s\t%d\t%d\t%s\n",chr,pos,len,qseq);
                                 for(int i=0; i< len ; i++){
                                      printf("%d,", qua[i]);
                                 }
                                 printf("\n");
			 }
		 }
	}
	bam_destroy1(aln);
	sam_close(fp_in);
	return 0;
}


// Ref http://www.howtobuildsoftware.com/index.php/how-do/bAiC/c-arrays-string-pointers-uint8t-conversion-of-uint8-t-to-a-string-c
// https://github.com/samtools/htslib/blob/6e86e386bed5c80c5222bd2e8eb2fd8c99234909/htslib/sam.h
// https://github.com/gatoravi/bam-parser-tutorial/blob/master/parse_bam.cc
// https://github.com/samtools/htslib/blob/5cfc3ede097f20ccea9504b757a402ea5062bb3d/sam.c
// http://118.31.76.100:100/python/py_module/pysam-bam-sam-to-read-and-write-files/
// https://github.com/samtools/htslib/blob/develop/htslib/sam.h
// https://nvlabs.github.io/nvbio/sam_8c_source.html
