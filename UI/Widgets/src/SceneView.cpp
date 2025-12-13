#include "SceneView.hpp"
#include "DataManagerWidget.h"
#include "DicomReader.hpp"
#include "IEventObserver.h"
#include "ImageNavigatorWidget.h"
#include "ImagePropertiesWidget.h"
#include "LogWidget.h"
#include "OpenGLViewerWidget.h"
#include "PixelValueWidget.h"
#include "SeriesSelectDialog.h"
#include "WidgetEvent.h"
#include "WidgetEventData.h"
#include "imgui_internal.h"
#include <imfilebrowser.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

struct SceneView::Impl final
{
  // create a file browser instance
  ImGui::FileBrowser mFileDialog;
  std::string mCurrentFile;
  ImGuiContext* mContext{};

  ImGuiID mDataManagerNode;
  ImGuiID mPixelValueNode;
  ImGuiID mImageNavigatorNode;
  ImGuiID mPropertiesNode;
  ImGuiID mLogNode;
  ImGuiID mViewNode;

  const char* UI_DOCK_WINDOW = "##ui.dock_window";
  const char* UI_DATA_MANAGER = "DataManager##ui.DataManager";
  const char* UI_PIXEL_VALUE_WINDOW = "Pixel Value##ui.PixelValue";
  const char* UI_IMAGE_NAVIGATOR_WINDOW = "Image Navigator##ui.ImageNavigator";
  const char* UI_PROPERTIES_WINDOW = "Properties##ui.Properties";
  const char* UI_LOG_BOX = "Logs##ui.log";
  const char* UI_VIEW_BOX = "View3D##ui.view";

  std::shared_ptr<DataManagerWidget> mDataManagerWidget;
  std::unique_ptr<PixelValueWidget> mPixelValueWidget;
  std::unique_ptr<ImageNavigatorWidget> mImageNavigatorWidget;
  std::unique_ptr<ImagePropertiesWidget> mImagePropertiesWidget;
  std::unique_ptr<LogWidget> mLogWidget;
  std::shared_ptr<OpenGLViewerWidget> mViewerWidget;
  std::shared_ptr<SeriesSelectDialog> mSeriesSelectDialog;
  using DicomReaderType = DicomReadReader<itk::Image<float, 3>>;
  itk::SmartPointer<DicomReaderType> mDicomReader;

  Impl()
    : mDataManagerNode(0)
    , mPixelValueNode(0)
    , mImageNavigatorNode(0)
    , mPropertiesNode(0)
    , mLogNode(0)
    , mViewNode(0)
  {
    mDataManagerWidget = std::make_shared<DataManagerWidget>(UI_DATA_MANAGER);
    mPixelValueWidget = std::make_unique<PixelValueWidget>(UI_PIXEL_VALUE_WINDOW);
    mImageNavigatorWidget = std::make_unique<ImageNavigatorWidget>(UI_IMAGE_NAVIGATOR_WINDOW);
    mImagePropertiesWidget = std::make_unique<ImagePropertiesWidget>(UI_PROPERTIES_WINDOW);
    mSeriesSelectDialog = std::make_shared<SeriesSelectDialog>("Pick Image##SeriesSelectDialog");
    mDicomReader = DicomReaderType::New();
    mLogWidget = std::make_unique<LogWidget>(UI_LOG_BOX);
    mViewerWidget = std::make_shared<OpenGLViewerWidget>(
      UI_VIEW_BOX, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);

    mSeriesSelectDialog->addObserver(mDataManagerWidget);
  }
};

SceneView::SceneView(ImGuiContext* context)
{
  mImpl = std::make_unique<Impl>();
  mImpl->mContext = context;
}

SceneView::~SceneView() = default;

void SceneView::render(SceneView* mScene) const
{
  // ImGui跨dll使用时需要共享ImGui::context，不然gImGui为空
  ImGui::SetCurrentContext(mImpl->mContext);
  const ImGuiID dockSpaceID = ImGui::GetID("##ui.dock_space");
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  constexpr int windowFlags =
    ImGuiWindowFlags_NoDecoration            // 无标题栏、不可改变大小、无滚动条、不可折叠
    | ImGuiWindowFlags_NoMove                // 不可移动
    | ImGuiWindowFlags_NoBackground          // 无背景（背景透明）
    | ImGuiWindowFlags_MenuBar               // 菜单栏
    | ImGuiWindowFlags_NoDocking             // 不可停靠
    | ImGuiWindowFlags_NoBringToFrontOnFocus // 无法设置前台和聚焦
    | ImGuiWindowFlags_NoNavFocus            // 无法通过键盘和手柄聚焦
    ;

  // 压入样式设置
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);            // 无边框
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // 无边界
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);              // 无圆角

  ImGui::Begin(mImpl->UI_DOCK_WINDOW, nullptr, windowFlags); // 开始停靠窗口
  ImGui::PopStyleVar(3);                                     // 弹出样式设置

  // 创建停靠空间
  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
  { // 判断是否开启停靠
    // 判断是否有根节点，防止一直重建
    if (!ImGui::DockBuilderGetNode(dockSpaceID))
    {
      // 移除根节点
      ImGui::DockBuilderRemoveNode(dockSpaceID);

      // 创建根节点
      ImGuiID root = ImGui::DockBuilderAddNode(dockSpaceID, ImGuiDockNodeFlags_None);

      // 设置根节点位置大小
      ImGui::DockBuilderSetNodePos(root, { 0.0f, 0.0f });
      ImGui::DockBuilderSetNodeSize(root, viewport->WorkSize);

      // 根节点分割左节点
      const ImGuiID leftNode =
        ImGui::DockBuilderSplitNode(root, ImGuiDir_Left, 0.25f, nullptr, &root);

      // 根节点分割右节点
      const ImGuiID rightNode =
        ImGui::DockBuilderSplitNode(root, ImGuiDir_Right, 0.25f / 0.75f, nullptr, &root);

      // 根节点分割下节点
      const ImGuiID bottomNode =
        ImGui::DockBuilderSplitNode(root, ImGuiDir_Down, 0.25f, nullptr, &root);

      // 左节点分割上下节点
      ImGuiID leftTopNode, leftBottomNode;
      ImGui::DockBuilderSplitNode(leftNode, ImGuiDir_Up, 0.5f, &leftTopNode, &leftBottomNode);

      // 设置分割到最后的根节点隐藏标签栏
      ImGui::DockBuilderGetNode(root)->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar;

      // 设置节点停靠窗口
      ImGui::DockBuilderDockWindow(mImpl->UI_DATA_MANAGER, leftTopNode);          // 左上节点
      ImGui::DockBuilderDockWindow(mImpl->UI_PIXEL_VALUE_WINDOW, leftBottomNode); // 左下节点
      ImGui::DockBuilderDockWindow(mImpl->UI_IMAGE_NAVIGATOR_WINDOW, leftBottomNode);
      ImGui::DockBuilderDockWindow(mImpl->UI_PROPERTIES_WINDOW, rightNode); // 右边节点

      ImGui::DockBuilderDockWindow(mImpl->UI_LOG_BOX, bottomNode);

      ImGui::DockBuilderDockWindow(
        mImpl->UI_VIEW_BOX, root); // 观察窗口平铺“客户区”，停靠的节点是 CentralNode

      // 结束停靠设置
      ImGui::DockBuilderFinish(dockSpaceID);

      // 设置焦点窗口
      ImGui::SetWindowFocus(mImpl->UI_VIEW_BOX);
    }

    // 创建停靠空间
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
  }
  ImGui::End(); // 结束停靠窗口

  mImpl->mDataManagerWidget->render();
  mImpl->mPixelValueWidget->render();
  mImpl->mImageNavigatorWidget->render();
  mImpl->mLogWidget->render();
  mImpl->mImagePropertiesWidget->render();
  mImpl->mViewerWidget->render();
  mImpl->mSeriesSelectDialog->render();
}

SceneView::SceneView(SceneView&&) noexcept = default;
SceneView& SceneView::operator=(SceneView&&) noexcept = default;

std::shared_ptr<OpenGLViewerWidget> SceneView::viewerWidget()
{
  return mImpl->mViewerWidget;
}

bool SceneView::handle(const EventObject& event)
{
  auto widgetEvent = dynamic_cast<const WidgetEvent*>(&event);
  if (widgetEvent)
  {
    if (widgetEvent->eventId() == EventId::WidgetResize)
    {
      auto resizeData = dynamic_cast<const WidgetResizeData*>(widgetEvent->eventData());
      mImpl->mViewerWidget->resize(resizeData->widgetSize().x, resizeData->widgetSize().y);
    }
    else if (widgetEvent->eventId() == EventId::FileOpened)
    {
      const auto fileOpenedData = dynamic_cast<const FileOpenedData*>(widgetEvent->eventData());
      mImpl->mDicomReader->SetDicomDirectory(fileOpenedData->filePath());
      mImpl->mDicomReader->GenerateData();
      mImpl->mSeriesSelectDialog->setDicomSeries(mImpl->mDicomReader->getSeries());
    }
  }
  return false;
}
