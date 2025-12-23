#ifndef MPR_SLICE_THICK_LINE_REPRESENTATION
#define MPR_SLICE_THICK_LINE_REPRESENTATION
#include "MPRSliceRepresentation.h"

class MPRSliceThickLineRepresentation final : public MPRSliceRepresentation
{
public:
  static MPRSliceThickLineRepresentation* New();
  vtkTypeMacro(MPRSliceThickLineRepresentation, MPRSliceRepresentation);

protected:
  MPRSliceThickLineRepresentation();
  ~MPRSliceThickLineRepresentation() override;
};
#endif // MPR_SLICE_THICK_LINE_REPRESENTATION