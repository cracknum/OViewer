#include "CubeOrientationMarker.h"
#include <vtkOrientationMarkerWidget.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>

CubeOrientationMarkerBuilder::CubeOrientationMarkerBuilder(vtkRenderWindowInteractor* interactor)
{
    // 默认颜色初始化
    std::pair<PLANE_DESCRIPTION, std::array<float, 3>> XPlusColor{ PLANE_DESCRIPTION::X_PLUS,
        std::array<float, 3>{ 0.0f, 0.0f, 0.0f } };
    std::pair<PLANE_DESCRIPTION, std::array<float, 3>> XMinusColor{ PLANE_DESCRIPTION::X_MINUS,
        std::array<float, 3>{ 0.0f, 0.0f, 0.0f } };
    std::pair<PLANE_DESCRIPTION, std::array<float, 3>> YPlusColor{ PLANE_DESCRIPTION::Y_PLUS,
        std::array<float, 3>{ 0.0f, 0.0f, 0.0f } };
    std::pair<PLANE_DESCRIPTION, std::array<float, 3>> YMinusColor{ PLANE_DESCRIPTION::Y_MINUS,
        std::array<float, 3>{ 0.0f, 0.0f, 0.0f } };
    std::pair<PLANE_DESCRIPTION, std::array<float, 3>> ZPlusColor{ PLANE_DESCRIPTION::Z_PLUS,
        std::array<float, 3>{ 0.0f, 0.0f, 0.0f } };
    std::pair<PLANE_DESCRIPTION, std::array<float, 3>> ZMinusColor{ PLANE_DESCRIPTION::Z_MINUS,
        std::array<float, 3>{ 0.0f, 0.0f, 0.0f } };
    mMapColor = std::unordered_map<PLANE_DESCRIPTION, std::array<float, 3>>{ XPlusColor,
        XMinusColor, YPlusColor, YMinusColor, ZPlusColor, ZMinusColor };

    // 默认显示内容初始化
    mMapText = std::unordered_map<PLANE_DESCRIPTION, std::string>{
        std::pair<PLANE_DESCRIPTION, std::string>{ PLANE_DESCRIPTION::X_PLUS, "L" },
        std::pair<PLANE_DESCRIPTION, std::string>{ PLANE_DESCRIPTION::X_MINUS, "R" },
        std::pair<PLANE_DESCRIPTION, std::string>{ PLANE_DESCRIPTION::Y_PLUS, "P" },
        std::pair<PLANE_DESCRIPTION, std::string>{ PLANE_DESCRIPTION::Y_MINUS, "A" },
        std::pair<PLANE_DESCRIPTION, std::string>{ PLANE_DESCRIPTION::Z_PLUS, "H" },
        std::pair<PLANE_DESCRIPTION, std::string>{ PLANE_DESCRIPTION::Z_MINUS, "F" }
    };

    mScale = 0.3;
    mTextColor[0] = 1.0f;
    mTextColor[1] = 1.0f;
    mTextColor[2] = 1.0f;
    mTextLineWidth = 1.0f;

    mInteractor = interactor;
}

void CubeOrientationMarkerBuilder::SetNamedColor(
  std::unordered_map<PLANE_DESCRIPTION, std::array<float, 3>>& mapColor)
{
    for (auto& pair : mapColor)
    {
        mMapColor[pair.first] = pair.second;
    }
}

void CubeOrientationMarkerBuilder::SetPlaneFaces(
  std::unordered_map<PLANE_DESCRIPTION, std::string>& mapText)
{
    for (auto& pair : mMapText)
    {
        mMapText[pair.first] = pair.second;
    }
}

void CubeOrientationMarkerBuilder::SetTextScale(float scale)
{
    mScale = scale;
}

void CubeOrientationMarkerBuilder::SetTextColor(float* color)
{
    try
    {
        mTextColor[0] = color[0];
        mTextColor[1] = color[1];
        mTextColor[2] = color[2];
    }
    catch (const std::exception& e)
    {
        abort();
    }
}

void CubeOrientationMarkerBuilder::SetTextLineWidth(float width)
{
    mTextLineWidth = width;
}

vtkSmartPointer<vtkOrientationMarkerWidget> CubeOrientationMarkerBuilder::Build()
{
#ifdef DIRECTION_INDICATOR_USE_HEAD_3D
    // 左下方指示器换成自定义的stl模型
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(uStatus::mSceneModelDir.toStdString().c_str());
    reader->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->RotateX(90);
#else
    vtkNew<vtkAnnotatedCubeActor> actor;
    actor->SetFaceTextScale(mScale);

    actor->SetXPlusFaceText(mMapText[PLANE_DESCRIPTION::X_PLUS].c_str());
    actor->SetXMinusFaceText(mMapText[PLANE_DESCRIPTION::X_MINUS].c_str());
    actor->SetYPlusFaceText(mMapText[PLANE_DESCRIPTION::Y_PLUS].c_str());
    actor->SetYMinusFaceText(mMapText[PLANE_DESCRIPTION::Y_MINUS].c_str());
    actor->SetZPlusFaceText(mMapText[PLANE_DESCRIPTION::Z_PLUS].c_str());
    actor->SetZMinusFaceText(mMapText[PLANE_DESCRIPTION::Z_MINUS].c_str());

    actor->GetTextEdgesProperty()->SetColor(mTextColor[0], mTextColor[1], mTextColor[2]);
    actor->GetTextEdgesProperty()->SetLineWidth(mTextLineWidth);

    auto& xPlusColor = mMapColor[PLANE_DESCRIPTION::X_PLUS];
    actor->GetXPlusFaceProperty()->SetColor(xPlusColor[0], xPlusColor[1], xPlusColor[2]);

    auto& xMinusColor = mMapColor[PLANE_DESCRIPTION::X_MINUS];
    actor->GetXMinusFaceProperty()->SetColor(xMinusColor[0], xMinusColor[1], xMinusColor[2]);

    auto& yPlusColor = mMapColor[PLANE_DESCRIPTION::Y_PLUS];
    actor->GetYPlusFaceProperty()->SetColor(yPlusColor[0], yPlusColor[1], yPlusColor[2]);

    auto& yMinusColor = mMapColor[PLANE_DESCRIPTION::Y_MINUS];
    actor->GetYMinusFaceProperty()->SetColor(yMinusColor[0], yMinusColor[1], yMinusColor[2]);

    auto& zPlusColor = mMapColor[PLANE_DESCRIPTION::Z_PLUS];
    actor->GetZPlusFaceProperty()->SetColor(zPlusColor[0], zPlusColor[1], zPlusColor[2]);
    auto& zMinusColor = mMapColor[PLANE_DESCRIPTION::Z_MINUS];
    actor->GetZMinusFaceProperty()->SetColor(zMinusColor[0], zMinusColor[1], zMinusColor[2]);

    actor->SetFaceTextScale(0.8);

    actor->SetZFaceTextRotation(-90);
#endif

    vtkNew<vtkOrientationMarkerWidget> orientationMarker;
    orientationMarker->SetOrientationMarker(actor);
    orientationMarker->SetInteractor(mInteractor);
    orientationMarker->EnabledOn();
    orientationMarker->SetInteractive(false);

    return orientationMarker;
}
