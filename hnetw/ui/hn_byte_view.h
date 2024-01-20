#pragma once


class HnByteView : public QAbstractScrollArea {
public:
    HnByteView(QWidget* parent = nullptr);
    ~HnByteView();

    void setRawData(const uint8_t* rawData, size_t len);
    void printPacketBytes();
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void updateScrollBars();
    QSize fullSize() const;

    int getLastLineIndex(int firsLineIdx, int areaHeight);
    void drawLineAfterBytes(QPainter& painter, QPaintEvent* event);
    QPen createPen(int width, Qt::PenStyle style, const QColor& color);
    uint8_t* getDataToPrint(int firstLineIdx, int lastLineIdx, size_t* buffForLen);
    void printAddress(QPainter& painter, int lineIdx, int yPos);
    void printBytes(QPainter& painter, const uint8_t* data, size_t dataLen, int startByte, int yPos);
    void printAsciiChars(QPainter& painter, const uint8_t* data, size_t dataLen, int startByte, int yPos);

    uint8_t* rawData_ = nullptr;
    size_t rawDataLen_ = 0;

    const int addrCharsNum_ = 4;
    const int gapAfterAddr_ = 10;
    const int gapAfterBytes_ = 20;
    const int endGap_ = 32;
    int bytesInLine_ = 16;
    const int bytesCharsNum_ = 48;

    int charWidth_;
    int charHeight_;
    int spaceBetweenChars_;

    // Screen offsets in pixels
    int addrScreenOffsetX_ = 6;
    int bytesScreenOffsetX_;
    int asciiScreenOffsetX_;
    int lineScreenOffsetX_;
};

