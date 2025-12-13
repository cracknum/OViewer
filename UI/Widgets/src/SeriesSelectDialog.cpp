#include "SeriesSelectDialog.h"
#include "DicomReader.hpp"
#include "DicomSeries.h"
#include "WidgetEvent.h"

#include "DcmEvent.h"
#include "DcmEventData.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

struct SeriesSelectDialog::Private
{
  std::vector<itk::SmartPointer<DicomSeries>> mDicomSeries;
  bool mOpen;
  int mSelectRow;

  Private()
    : mOpen(false)
    , mSelectRow(-1)
  {
  }
};
SeriesSelectDialog::SeriesSelectDialog(const char* widgetName, int widgetFlags)
  : Widget(widgetName, widgetFlags)
{
  mPrivate = std::make_unique<Private>();
}
SeriesSelectDialog::~SeriesSelectDialog() = default;
bool SeriesSelectDialog::render()
{
  if (mPrivate->mDicomSeries.empty())
  {
    return true;
  }
  if (ImGui::BeginPopupModal(mWidgetName.c_str(), &mPrivate->mOpen, mWidgetFlags))
  {
    if (ImGui::BeginTable("##SeriesSelectTable", 4,
          ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersInnerH |
            ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV))
    {
      auto scale = ImGui::GetWindowDpiScale();
      ImGui::TableSetColumnWidth(0, scale * 60);
      ImGui::TableSetColumnWidth(1, scale * 60);
      ImGui::TableSetColumnWidth(2, scale * 60);
      ImGui::TableSetColumnWidth(3, scale * 60);
      ImGui::TableSetupColumn("Series Number");
      ImGui::TableSetupColumn("Description");
      ImGui::TableSetupColumn("Dimensions");
      ImGui::TableSetupColumn("number of images");
      ImGui::TableHeadersRow();
      for (int i = 0; i < mPrivate->mDicomSeries.size(); ++i)
      {
        const auto dicomSeries = mPrivate->mDicomSeries.at(i);
        ImGui::TableNextRow();
        const auto seriesInfo = dicomSeries->GetSeriesInfo();
        const auto imageInfo = dicomSeries->GetImageInfo();

        ImGui::TableSetColumnIndex(0);
        std::string rowId = "##Row_" + std::to_string(i);

        if (ImGui::Selectable(rowId.c_str(), mPrivate->mSelectRow == i,
              ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups))
        {
          mPrivate->mSelectRow = i;
        }
        ImGui::TableSetColumnIndex(0);
        ImGui::SameLine();
        auto id = seriesInfo->GetInstanceUID();
        if (id.empty())
        {
          id = seriesInfo->GetNumber();
        }
        ImGui::Text(id.c_str());

        ImGui::TableSetColumnIndex(1);
        ImGui::SameLine();
        ImGui::Text(seriesInfo->GetDescription().c_str());

        ImGui::TableSetColumnIndex(2);
        const auto dimensions = imageInfo->GetDimensions();
        ImGui::SameLine();
        ImGui::Text("%d x %d x %d", dimensions[0], dimensions[1], dimensions[2]);

        ImGui::TableSetColumnIndex(3);
        ImGui::SameLine();
        ImGui::Text("%d", dimensions[2]);
      }
      ImGui::EndTable();
    }
    ImGui::Spacing();

    const float button_width =
      (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) * 0.5f;

    if (ImGui::Button("OK", ImVec2(button_width, 0)))
    {
      auto eventData =
        std::make_unique<SeriesSelectedData>(mPrivate->mDicomSeries.at(mPrivate->mSelectRow));
      this->invokeEvent(DcmEvent(EventId::SeriesSelected, std::move(eventData)));
      mPrivate->mOpen = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(button_width, 0)))
    {
      mPrivate->mSelectRow = -1;
    }

    ImGui::EndPopup();
  }

  return true;
}
void SeriesSelectDialog::resize(int width, int height) {}
void SeriesSelectDialog::setDicomSeries(
  const std::vector<itk::SmartPointer<DicomSeries>>& dicomSeries)
{
  mPrivate->mDicomSeries = dicomSeries;
  if (!mPrivate->mOpen)
  {
    ImGui::OpenPopup(mWidgetName.c_str());
    mPrivate->mOpen = true;
    mPrivate->mSelectRow = -1;
  }
}
