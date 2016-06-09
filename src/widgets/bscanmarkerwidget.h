#ifndef BSCANMARKERWIDGET_H
#define BSCANMARKERWIDGET_H

#include "cvimagewidget.h"

#include <QPoint>

class QWheelEvent;
class QMouseEvent;
class MarkerManager;

class BScanMarkerWidget : public CVImageWidget
{
	Q_OBJECT

	QPoint clickPos;
	QPoint mousePos;

	bool markerActiv = false;
	bool mouseInWidget = false;

	MarkerManager& markerManger;

	virtual void contextMenuEvent(QContextMenuEvent* event);
public:
	BScanMarkerWidget(MarkerManager& markerManger);

	virtual ~BScanMarkerWidget();

protected:
	void paintEvent(QPaintEvent* event);

	virtual void mouseMoveEvent   (QMouseEvent*);
	virtual void mousePressEvent  (QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	
	virtual void keyPressEvent    (QKeyEvent*  );
	virtual void leaveEvent       (QEvent*     );
	virtual void wheelEvent       (QWheelEvent*);

private slots:
	void bscanChanged(int);
	void cscanLoaded();

	void markersMethodChanged();

signals:
	void bscanChangeInkrement(int delta);
};

#endif // BSCANMARKERWIDGET_H
