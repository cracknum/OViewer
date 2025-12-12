#include "PixelValueWidget.h"
#include <imgui.h>

namespace 
{
  bool InputTextStd(const char* label, std::string& str, ImGuiInputTextFlags flags = 0)
{
    flags |= ImGuiInputTextFlags_CallbackResize;

    return ImGui::InputText(
        label,
        str.data(),
        str.capacity() + 1,
        flags,
        [](ImGuiInputTextCallbackData* data) -> int {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
            {
                std::string* str = (std::string*)data->UserData;
                str->resize(data->BufTextLen);
                data->Buf = (char*)str->c_str();
            }
            return 0;
        },
        (void*)&str
    );
}
}

struct PixelValueWidget::Private
{
  std::string mImageNameValue;
  std::string mPixelValue;
};

PixelValueWidget::PixelValueWidget(const char* widgetName, int widgetFlags)
  : Widget(widgetName, widgetFlags)
{
  mPrivate = std::make_unique<Private>();
}

PixelValueWidget::~PixelValueWidget() {}

bool PixelValueWidget::render()
{
  if (ImGui::Begin(mWidgetName.c_str(), false, mWidgetFlags))
  {
    InputTextStd("Image name", mPrivate->mImageNameValue,ImGuiInputTextFlags_CallbackResize);
    InputTextStd("Pixel Value", mPrivate->mPixelValue,ImGuiInputTextFlags_CallbackResize);
  }

  ImGui::End();

  return false;
}

void PixelValueWidget::resize(int width, int height) {}
