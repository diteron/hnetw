#pragma once

class HnCentralWidget : public QWidget {
public:
    HnCentralWidget(QWidget* parent = nullptr);
    ~HnCentralWidget();
    void setupUi();

private:
    QVBoxLayout* layout_ = nullptr;

};

