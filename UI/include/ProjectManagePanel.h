#ifndef PROJECT_MANAGE_PANEL
#define PROJECT_MANAGE_PANEL
#include <QWidget>
#include <memory>
#include "UIExport.h"
#include <itkSmartPointer.h>

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
protected:
	void initUI();
	void initConnects();
	void initParams();
private:
	struct Impl;
	std::unique_ptr<Impl> m_Impl;
};

#endif // PROJECT_MANAGE_PANEL
