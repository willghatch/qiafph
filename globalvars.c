

int mdivision;
int sdivPerDiv;
int nmeasures;
int nparts;
int* rangemin;
int* rangemax;
int* rangediff;
int pieceScore;

int totalSubdivisions;

void initGlobals() {
    mdivision = 4;
    sdivPerDiv = 2;
    nmeasures = 8;
    nparts = 4;

    totalSubdivisions = mdivision * sdivPerDiv * nmeasures;
}
