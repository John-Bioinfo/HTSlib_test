#include <iostream>
#include <stdio.h>
#include <stdexcept>

#include <sam.h>
#include <hts.h>

using namespace std;

int usage() {
    cerr << "./parse_bam example.bam";
    return 0;

}


int parse_bam(int argc, char* argv[]) {
    if(argc < 2) {
        return usage();
    }

    string bam = string(argv[1]);
    string region_ = ".";
    if(argc > 2) {
        region_ = string(argv[2]);
    }
    if(!bam.empty()) {
        //open BAM for reading
        samFile *in = sam_open(bam.c_str(), "r");
        if(in == NULL) {
            throw runtime_error("Unable to open BAM/SAM file.");
        }
        //Load the index
        hts_idx_t *idx = sam_index_load(in, bam.c_str());
        if(idx == NULL) {
            throw runtime_error("Unable to open BAM/SAM index."
                                " Make sure alignments are indexed");
        }
        //Get the header
        bam_hdr_t *header = sam_hdr_read(in);
        //Initialize iterator
        hts_itr_t *iter = NULL;
        //Move the iterator to the region we are interested in
        iter  = sam_itr_querys(idx, header, region_.c_str());
        if(header == NULL || iter == NULL) {
            sam_close(in);
            throw runtime_error("Unable to iterate to region within BAM.");
        }

        //Initiate the alignment record
        bam1_t *aln = bam_init1();
        while(sam_itr_next(in, iter, aln) >= 0) {
            cout << "Read Chr: " << header->target_name[aln->core.tid];
            cout << "\tPos: " << aln->core.pos;
            string seq, qual;
            uint8_t *quali = bam_get_qual(aln);
            uint8_t *seqi = bam_get_seq(aln);
            for (int i = 0; i < aln->core.l_qseq; i++) {
                seq += seq_nt16_str[bam_seqi(seqi, i)];
                qual += 33 + quali[i];
            }

            cout << "\tSeq: " << seq << "\tQual: " << qual;
            cout << endl;
        }

        hts_itr_destroy(iter);
        hts_idx_destroy(idx);
        bam_destroy1(aln);
        bam_hdr_destroy(header);
        sam_close(in);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    try {
        parse_bam(argc, argv);
    } catch (const runtime_error& e) {
        cerr << e.what();
    }
}

