#include "stdafx.h"
#include "hn_byte_view.h"

HnByteView::HnByteView(QWidget* parent) : QAbstractScrollArea(parent)
{
    setStyleSheet("font: 10pt 'Cascadia Mono SemiLight';");

    charWidth_ = fontMetrics().horizontalAdvance('C');
    charHeight_ = fontMetrics().height();
    spaceBetweenChars_ = charWidth_ / 2;

    bytesScreenOffsetX_ = addrScreenOffsetX_ + (addrCharsNum_ * charWidth_) + gapAfterAddr_;
    asciiScreenOffsetX_ = bytesScreenOffsetX_ + (bytesCharsNum_ * charWidth_) + gapAfterBytes_;
    lineScreenOffsetX_ = asciiScreenOffsetX_ - (gapAfterBytes_ / 2);
}

HnByteView::~HnByteView()
{}

void HnByteView::setRawData(const uint8_t* rawData, int len)
{
    clear();
    rawData_ = rawData;
    rawDataLen_ = len;
}

void HnByteView::printPacketBytes()
{
    viewport()->update();
}

void HnByteView::clear()
{
    rawData_ = nullptr;
    verticalScrollBar()->setValue(0);
    horizontalScrollBar()->setValue(0);
    viewport()->update();
}

void HnByteView::paintEvent(QPaintEvent* event)
{
    if (!rawData_) return;

    QPainter painter(viewport());
    painter.translate(-horizontalScrollBar()->value() * charWidth_, 0);
    updateScrollBars();

    QSize areaSize = viewport()->size();
    int firstLineIdx = verticalScrollBar()->value();
    int lastLineIdx = getLastLineIndex(firstLineIdx, areaSize.height());

    QColor addressAreaColor = palette().color(QPalette::Window);
    painter.fillRect(QRect(addrScreenOffsetX_ - addrScreenOffsetX_, event->rect().top(),
                     bytesScreenOffsetX_ - gapAfterAddr_ + addrScreenOffsetX_, height()), addressAreaColor);

    drawLineAfterBytes(painter, event);

    // Get data that corresponds to the current view lines from the packet raw data to print
    int dataLen = 0;
    uint8_t* data = getDataToPrint(firstLineIdx, lastLineIdx, &dataLen);
    
    int yPosStart = charHeight_;
    QBrush defaultBrush = painter.brush();
    painter.setBackground(defaultBrush);

    for (int lineIdx = firstLineIdx, yPos = yPosStart; lineIdx < lastLineIdx; lineIdx += 1, yPos += charHeight_) {
        int startDataByte = (lineIdx - firstLineIdx) * bytesInLine_;
        printAddress(painter, lineIdx, yPos);
        printBytes(painter, data, dataLen, startDataByte, yPos);
        printAsciiChars(painter, data, dataLen, startDataByte, yPos);
    }

    delete[] data;
}

void HnByteView::updateScrollBars()
{
    QSize areaSize = viewport()->size();
    QSize widgetSize = fullSize();
    verticalScrollBar()->setPageStep(areaSize.height() / charHeight_);
    verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / charHeight_ + 1);
    horizontalScrollBar()->setPageStep(areaSize.width() / charWidth_);
    horizontalScrollBar()->setRange(0, (widgetSize.width() - areaSize.width()) / charWidth_ + 1);
}

QSize HnByteView::fullSize() const
{
    if (!rawData_)
        return QSize(0, 0);

    int width = asciiScreenOffsetX_ + (bytesInLine_ * charWidth_);
    int height = rawDataLen_ / bytesInLine_;
    if (rawDataLen_ % bytesInLine_)
        ++height;

    height *= charHeight_;

    return QSize(width, height);
}

int HnByteView::getLastLineIndex(int firsLineIdx, int areaHeight)
{
    int lastLineIdx = firsLineIdx + areaHeight / charHeight_;
    if (static_cast<unsigned int>(lastLineIdx) > rawDataLen_ / bytesInLine_) {
        lastLineIdx = rawDataLen_ / bytesInLine_;
        if (rawDataLen_ % bytesInLine_)
            ++lastLineIdx;
    }

    return lastLineIdx;
}

void HnByteView::drawLineAfterBytes(QPainter& painter, QPaintEvent* event)
{
    QColor lineColor = palette().color(QPalette::Window);
    QPen linePen = createPen(4, Qt::SolidLine, lineColor);
    painter.setPen(linePen);
    painter.drawLine(lineScreenOffsetX_, event->rect().top(), lineScreenOffsetX_, height());
    painter.setPen(palette().color(QPalette::Text));
}

QPen HnByteView::createPen(int width, Qt::PenStyle style, const QColor& color)
{
    QPen pen;
    pen.setWidth(width);
    pen.setStyle(style);
    pen.setColor(color);

    return pen;
}

uint8_t* HnByteView::getDataToPrint(int firstLineIdx, int lastLineIdx, int* buffForLen)
{
    int dataLen = (lastLineIdx - firstLineIdx) * bytesInLine_;
    if (dataLen > rawDataLen_) {
        dataLen -= dataLen - rawDataLen_;
    }
    uint8_t* data = new uint8_t[dataLen];
    std::memcpy(data, rawData_ + firstLineIdx * bytesInLine_, dataLen);
    *buffForLen = dataLen;

    return data;
}

void HnByteView::printAddress(QPainter& painter, int lineIdx, int yPos)
{
    QString address = QString("%1").arg(lineIdx * bytesInLine_, addrCharsNum_, 16, QChar('0'));
    QColor addrTextColor = palette().color(QPalette::PlaceholderText);
    painter.setPen(addrTextColor);
    painter.drawText(addrScreenOffsetX_, yPos, address);
    painter.setPen(palette().color(QPalette::Text));
}

void HnByteView::printBytes(QPainter& painter, const uint8_t* data, int dataLen, int startByte, int yPos)
{
    int currDataByte = startByte;
    for (int xPos = bytesScreenOffsetX_, i = 0;
         i < bytesInLine_ && currDataByte < dataLen;
         ++i, xPos += 3 * charWidth_, ++currDataByte
        ) {

        // Additional space after 8 bytes
        if (i != 0 && i % 8 == 0) {
            xPos += charWidth_;
        }

        QString fourBitVal = QString::number((data[currDataByte] & 0xF0) >> 4, 16);
        painter.drawText(xPos, yPos, fourBitVal);

        fourBitVal = QString::number((data[currDataByte] & 0x0F), 16);
        painter.drawText(xPos + charWidth_, yPos, fourBitVal);
    }
}

void HnByteView::printAsciiChars(QPainter& painter, const uint8_t* data, int dataLen, int startByte, int yPos)
{
    int currDataByte = startByte;
    for (int xPosAscii = asciiScreenOffsetX_, i = 0;
         currDataByte < dataLen && i < bytesInLine_;
         ++i, xPosAscii += charWidth_, ++currDataByte
        ) {

        // Additional space after 8 bytes
        if (i != 0 && i % 8 == 0) {
            xPosAscii += charWidth_;
        }

        char ch = data[currDataByte];
        if ((ch < 0x20) || (ch > 0x7e))
            ch = '.';

        painter.drawText(xPosAscii, yPos, QString(ch));
    }
}
