#ifndef MPR_SLICE_LINE_REPRESENTATION
#define MPR_SLICE_LINE_REPRESENTATION
#include "MPRSliceRepresentation.h"

class MPRSliceLineRepresentation: public MPRSliceRepresentation
{
public:
static MPRSliceLineRepresentation* New();
vtkTypeMacro(MPRSliceLineRepresentation, MPRSliceRepresentation);

protected:
MPRSliceLineRepresentation();
~MPRSliceLineRepresentation() override;
};

#endif // MPR_SLICE_LINE_REPRESENTATION