#include "FileBrowserDialog.h"
// clang-format off
#include <imgui.h>
#include <imfilebrowser.h>
// clang-format on
#include "WidgetEvent.h"
#include "WidgetEventData.h"

#include <spdlog/spdlog.h>
struct FileBrowserDialog::Private
{
  ImGui::FileBrowser mFileBrowser;
  Private()
    : mFileBrowser(ImGuiFileBrowserFlags_SelectDirectory | ImGuiFileBrowserFlags_HideRegularFiles)
  {
    mFileBrowser.SetTitle("选择DCM目录");
  }
};

FileBrowserDialog::FileBrowserDialog(const char* widgetName, int widgetFlags /*= 0*/)
  : Widget(widgetName, widgetFlags)
{
  mPrivate = std::make_unique<Private>();
}
inline FileBrowserDialog::~FileBrowserDialog() = default;
bool FileBrowserDialog::render()
{
  // process MenuItemClicked event
  mPrivate->mFileBrowser.Display();

  if (mPrivate->mFileBrowser.HasSelected())
  {
    auto filePath = mPrivate->mFileBrowser.GetSelected().string();
    auto eventData = std::make_unique<FileOpenedData>(std::move(filePath));
    this->invokeEvent(WidgetEvent(EventId::FileOpened, std::move(eventData)));
    mPrivate->mFileBrowser.Close();
  }
  return true;
}
void FileBrowserDialog::resize(int width, int height) {}
bool FileBrowserDialog::handle(const EventObject& event)
{
  const auto itemEvent = dynamic_cast<const MenuItemClicked*>(&event);
  if (!itemEvent || itemEvent->eventId() != EventId::FileOpened)
  {
    return false;
  }
  mPrivate->mFileBrowser.Open();
  return false;
}