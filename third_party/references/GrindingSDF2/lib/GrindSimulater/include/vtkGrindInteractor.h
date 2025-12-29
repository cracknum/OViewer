#ifndef vtkGrindInteractor_h
#define vtkGrindInteractor_h

#ifdef GrindSimulater_EXPORTS
#define GrindSimulater_EXPORT __declspec(dllexport)
#else
#define GrindSimulater_EXPORT __declspec(dllimport)
#endif

#include "vtkGrindDef.h"
#include <vector>

class vtkInformationStringKey;
class vtkTextureObject;
class vtkMatrix4x4;
class GrindSimulater_EXPORT vtkGrindInteractor {
public:
    vtkGrindInteractor();
    virtual ~vtkGrindInteractor();

    static vtkInformationStringKey * GrindRequest();

    // Prepare
    virtual void AllocateToolTrack(int count);
    virtual void SetToolTrackSpacing(float spacing);
    virtual void ActiveToolTrack(int id);
    virtual void ToggleToolTrack();
    virtual void RecreateToolTrack();
    virtual void ResetToolTrack();
    
    // Grind
    virtual void SetGrindType(vtkGrindDef::GrindType type);
    virtual void SetGrindBaseColor(float clr[3]);
    virtual void SetGrindResectPos(float pos);
    virtual void SetGrindResectColor(float clr[3]);
    virtual void SetGrindExpandDefaultColor(float clr[3]);
    virtual void AddGrindExpand(double len, float clr[3]);
    virtual void ClearGrindExpands();

    // Geom
    virtual void SetSphereGrindResect(double radius, vtkMatrix4x4 * mat);
    virtual void SetBoxGrindResect(double xl, double yl, double zl, vtkMatrix4x4 * mat);
    virtual void SetRoundBoxGrindResect(double xl, double yl, double zl, double radius, vtkMatrix4x4 * mat);
    virtual void SetCapsuleGrindResect(double radius, double height, vtkMatrix4x4 * mat);
    virtual void SetPlaneGrindResect(double origin[3], double normal[3]);
    virtual void Set2DAxisDilateGrindResect(const std::vector<double> & points, double height, int axis, vtkMatrix4x4 * mat); // axis only: 0-x, 1-y, 2-z
    virtual void SetRound2DAxisDilateGrindResect(const std::vector<double> & points, double radius, double height, int axis, vtkMatrix4x4 * mat); // axis only: 0-x, 1-y, 2-z

    // Model
    virtual vtkTextureObject * CreateGrindResect(const char * vti);
    virtual vtkTextureObject * CreateGrindResect(void* data, int * dims, double * origin, double * spacing);
    virtual void DeleteGrindResect(vtkTextureObject *& tex);
    virtual void SetGrindResect(vtkTextureObject * tex, vtkMatrix4x4 * mat);

    // Tool
    virtual void SetSphereTool(double radius);
    virtual void SetSemisphereTool(double radius);
    virtual void SetBoxTool(double xl, double yl, double zl);
    virtual void SetCylinderTool(double radius, double height);
    virtual void SetCapsuleTool(double radius, double height);
    virtual void Set2DAxisDilateTool(const std::vector<double> & points, double height, int axis);
    virtual void SetRound2DAxisDilateTool(const std::vector<double> & points, double radius, double height, int axis);
    virtual void AddToolSnapshot(vtkMatrix4x4 * mat);
};

#endif //vtkGrindInteractor_h