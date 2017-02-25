#ifndef WGSEGNN_H
#define WGSEGNN_H

#ifdef ML_SUPPORT

#include <QObject>

#include <ui_marker_segmentation_nn.h>

class BScanSegLocalOpNN;
class WGSegmentation;
class BScanSegmentation;
class WindowNNInOut;

namespace BScanSegmentationMarker
{
	struct NNTrainData;
}

class WgSegNN : public QWidget, Ui::BScanSegWidgetNN
{
	Q_OBJECT


	BScanSegmentation* segmentation = nullptr;
	BScanSegLocalOpNN* localOpNN    = nullptr;

	WindowNNInOut* inOutWindow = nullptr;

	void createConnections();

	void setNNData(const BScanSegmentationMarker::NNTrainData& data);
	void getNNData(      BScanSegmentationMarker::NNTrainData& data);

	void updateActLayerInfo();
	void updateExampleInfo();

public:
	WgSegNN(WGSegmentation* parent, BScanSegmentation* seg);
	virtual ~WgSegNN();


private slots:

	// void slotLearnBScan();
	void slotTrain();

	void slotAddBscanExampels();

	void slotSave();
	void slotLoad();

	void slotLoadSaveButtonBoxClicked(QAbstractButton* button);

	void changeNNConfig();

	void showInOutWindow(bool show);

};

#endif

#endif // WGSEGNN_H
