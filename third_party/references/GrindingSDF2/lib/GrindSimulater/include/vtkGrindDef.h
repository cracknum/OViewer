#ifndef vtkGrindDef_h
#define vtkGrindDef_h

class vtkGrindDef {

public:
    enum GrindType {
        GT_Geom = 0,
        GT_Model,
        GT_Count,
    };

    enum GrindGeomType {
        GGT_Sphere = 0,
        GGT_Box,
        GGT_RoundBox,
        GGT_Capsule,
        GGT_Plane,
        GGT_2DAxisDilate,
        GGT_Round2DAxisDilate,
        GGT_Count,
    };

    enum ToolType {
        TT_Sphere = 0,
        TT_Semisphere,
        TT_Box,
        TT_Cylinder,
        TT_Capsule,
        TT_2DAxisDilate,
        TT_Round2DAxisDilate,
        TT_Count,
    };
};

#endif //vtkGrindDef_h