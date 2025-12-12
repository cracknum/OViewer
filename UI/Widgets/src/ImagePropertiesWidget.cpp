#include "ImagePropertiesWidget.h"
#include <imgui.h>

struct ImagePropertiesWidget::Private
{

};

ImagePropertiesWidget::ImagePropertiesWidget(const char* widgetName, int widgetFlags)
  : AbstractPropertiesWidget(widgetName, widgetFlags)
{
}

ImagePropertiesWidget::~ImagePropertiesWidget() {}

bool ImagePropertiesWidget::render()
{
  if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
  {
    /* code */
  }
  ImGui::End();
  return true;
}

void ImagePropertiesWidget::resize(int width, int height) {}
