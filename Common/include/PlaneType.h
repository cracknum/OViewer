#ifndef PLANE_TYPE_H
#define PLANE_TYPE_H
#include <iostream>

enum class StandardPlane
{
  None,
  Axial,
  Sagittal,
  Coronal
};

inline std::ostream& operator<<(std::ostream& oss, const StandardPlane& plane)
{
  switch (plane)
  {
    case StandardPlane::None:
      oss << "None" << "\n";
      break;
    case StandardPlane::Axial:
      oss << "Axial" << "\n";
      break;
    case StandardPlane::Sagittal:
      oss << "Sagittal" << "\n";
      break;
    case StandardPlane::Coronal:
      oss << "Coronal" << "\n";
      break;
    default:
      oss << "Unknown type: " << static_cast<int>(plane) << "\n";
      break;
  }

  return oss;
}

#endif // PLANE_TYPE_H