#include "ImageNavigatorWidget.h"
#include <imgui.h>
struct ImageNavigatorWidget::Private
{
};

ImageNavigatorWidget::ImageNavigatorWidget(const char* widgetName, int widgetFlags)
  : Widget(widgetName, widgetFlags)
{
}

ImageNavigatorWidget::~ImageNavigatorWidget() {}

bool ImageNavigatorWidget::render()
{
    if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
    {
        float value[3] = {};
        float axialValue = 0.0f;
        float sagittalValue = 0.0f;
        float coronalValue = 0.0f;
        
        ImGui::DragFloat3("Loc", value);
        ImGui::SliderFloat("Axial", &axialValue, 0, 100);
        ImGui::SliderFloat("Sagittal", &sagittalValue, 0, 100);
        ImGui::SliderFloat("Coronal", &coronalValue, 0, 100);
    }
    ImGui::End();
    
  return false;
}

void ImageNavigatorWidget::resize(int width, int height) {}
