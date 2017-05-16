#include "mousecoordstatus.h"

#include "bscanmarkerwidget.h"

MouseCoordStatus::MouseCoordStatus(BScanMarkerWidget* bscanWidget)
: bscanWidget(bscanWidget)
{
	connect(bscanWidget, &BScanMarkerWidget::mouseLeaveImage, this, &MouseCoordStatus::mouseLeaveImage);
	connect(bscanWidget, &BScanMarkerWidget::mousePosInImage, this, &MouseCoordStatus::mousePosInImage);
}

MouseCoordStatus::~MouseCoordStatus()
{
}


void MouseCoordStatus::mouseLeaveImage()
{
	setText("");
}

void MouseCoordStatus::mousePosInImage(int x, int y)
{
	setText(QString("%1 %2").arg(x).arg(y));
}

