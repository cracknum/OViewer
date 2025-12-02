#ifndef PROJECT_MANAGE_PANEL
#define PROJECT_MANAGE_PANEL
#include "UIExport.h"
#include <QWidget>
#include <itkSmartPointer.h>
#include <memory>

class DicomSeries;
class UI_API ProjectManagePanel final : public QWidget
{
  Q_OBJECT

public:
  explicit ProjectManagePanel(QWidget* parent = nullptr);
  ~ProjectManagePanel();
  using SeriesVector = std::vector<itk::SmartPointer<DicomSeries>>;
public slots:
  void slotSetImageTable(const SeriesVector& seriesVector);

signals:
  void signalSelectedSeries(const QString& seriesId);

protected:
  void initUI();
  void initConnects();
  void initParams();

private:
  struct Impl;
  std::unique_ptr<Impl> m_Impl;
};

#endif // PROJECT_MANAGE_PANEL
