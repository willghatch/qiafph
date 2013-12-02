

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
    nparts = 4;

    totalSubdivisions = mdivision * sdivPerDiv * nmeasures;
}
