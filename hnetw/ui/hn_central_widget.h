#pragma once

class HnCentralWidget : public QWidget {
public:
    HnCentralWidget(QWidget* parent = nullptr);
    ~HnCentralWidget();

    void addWidget(QWidget* widget);

private:
    QVBoxLayout* layout_ = nullptr;

};

