

int mdivision;
int sdivPerDiv;
int nmeasures;
int nparts;
int* rangemin;
int* rangemax;
int* rangediff;
long pieceScore;
int verboseP;

int totalSubdivisions;

void initGlobals() {
    mdivision = 4;
    sdivPerDiv = 1;
    nmeasures = 6;

    totalSubdivisions = mdivision * sdivPerDiv * nmeasures;
}
