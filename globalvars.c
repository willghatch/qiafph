

int mdivision;
int sdivPerDiv;
int nmeasures;
int nparts;
int* rangemin;
int* rangemax;
int* rangediff;
long pieceScore;

int totalSubdivisions;

void initGlobals() {
    mdivision = 4;
    sdivPerDiv = 1;
    nmeasures = 12;
    nparts = 4; // Ok, so this is a variable because I thought to make it configurable,
        // but various parts I've since hard coded to be 4 parts.  So... unless I change
        // that this needs to stay 4.

    totalSubdivisions = mdivision * sdivPerDiv * nmeasures;
}
