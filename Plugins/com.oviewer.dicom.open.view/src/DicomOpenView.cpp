#include "DicomOpenView.h"
#include <QIcon>
#include <QWidget>

DicomOpenView::DicomOpenView() {}

DicomOpenView::~DicomOpenView() {}

QString DicomOpenView::name()
{
  return "DicomOpenView";
}

QString DicomOpenView::categoryName()
{
  return "Files";
}

QString DicomOpenView::panelName()
{
  return "File";
}

QString DicomOpenView::actionName()
{
  return "Open";
}

QIcon DicomOpenView::actionIcon()
{
  return QIcon(":/icon/openFolder.svg");
}

QWidget* DicomOpenView::viewWidget()
{
  return new QWidget();
}
