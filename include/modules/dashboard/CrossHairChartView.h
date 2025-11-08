//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_CROSS_HAIR_CHART_VIEW_H
#define AWSMOCK_QT_UI_CROSS_HAIR_CHART_VIEW_H

#include <QApplication>
#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QPen>
#include <QValueAxis>
#include <QGraphicsProxyWidget>

// 1. Define a custom QChartView to handle mouse events for the crosshair
class CrosshairChartView final : public QChartView {
    Q_OBJECT

public:
    explicit CrosshairChartView(QChart *chart, QWidget *parent = nullptr) : QChartView(chart, parent) {
        // Enable mouse tracking so mouseMoveEvent fires even without a button pressed
        setMouseTracking(true);

        // Create the vertical and horizontal lines
        vLine = new QGraphicsLineItem(chart);
        vLine->setPen(QPen(Qt::red, 1, Qt::DashLine));
        vLine->setVisible(false);

        hLine = new QGraphicsLineItem(chart);
        hLine->setPen(QPen(Qt::red, 1, Qt::DashLine));
        hLine->setVisible(false);

        // Create the value label (QLabel)
        valueLabel = new QLabel();
        valueLabel->setStyleSheet("color: black; background-color: rgba(255, 255, 255, 200); border: 1px solid red; padding: 3px;");
        valueLabel->setText("X: -- | Y: --");

        // Embed the QLabel into the chart's QGraphicsScene
        labelProxy = chart->scene()->addWidget(valueLabel);
        labelProxy->setZValue(99); // Ensure it's rendered on top
        labelProxy->setVisible(false);
    }

protected:
    // 2. Override mouseMoveEvent to update the crosshair position
    void mouseMoveEvent(QMouseEvent *event) override {
        // Get the chart's plot area (where the data is drawn)
        const QRectF plotArea = chart()->plotArea();

        // Check if the mouse is within the plot area
        if (const QPointF mousePos = event->pos(); plotArea.contains(mousePos)) {
            if (!vLine->isVisible()) {
                vLine->setVisible(true);
                hLine->setVisible(true);
                labelProxy->setVisible(true);
            }

            // Update Graphics
            vLine->setLine(mousePos.x(), plotArea.top(), mousePos.x(), plotArea.bottom());
            hLine->setLine(plotArea.left(), mousePos.y(), plotArea.right(), mousePos.y());

            // Map the screen coordinates (mousePos) to chart data values
            const QPointF chartValue = chart()->mapToValue(mousePos);
            const QDateTime xDateTime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(chartValue.x()));

            // Update the label text with formatted values
            const QString text = QString("X: %1 | Y: %2").arg(xDateTime.toString("hh:mm:ss")).arg(chartValue.y(), 0, 'f', 3);
            valueLabel->setText(text);
            valueLabel->setStyleSheet("color:black; background-color: rgba(200, 220, 255, 220); border: 1px solid blue; padding: 3px; font-weight: bold;");

            // Position the label near the crosshair intersection
            // We offset the label slightly to the right/down so it doesn't cover the intersection
            labelProxy->setPos(mousePos.x() + 5, mousePos.y() + 5);

            // Calculate the required size of the label (it changes based on text)
            valueLabel->adjustSize();
            const qreal labelWidth = valueLabel->width();
            const qreal labelHeight = valueLabel->height();
            constexpr qreal padding = 5.0; // The fixed offset distance

            qreal newX = mousePos.x() + padding;
            qreal newY = mousePos.y() + padding;

            // Check Right Edge: If the label's right edge exceeds the plot area boundary, flip the X offset.
            if (newX + labelWidth > plotArea.right()) {
                newX = mousePos.x() - labelWidth - padding;
            }

            // Check Bottom Edge: If the label's bottom edge exceeds the plot area boundary, flip the Y offset.
            if (newY + labelHeight > plotArea.bottom()) {
                newY = mousePos.y() - labelHeight - padding;
            }

            // Position the label using the calculated coordinates
            labelProxy->setPos(newX, newY);
        } else {
            // Hide the crosshair and label if the mouse leaves the plot area
            if (vLine->isVisible()) {
                vLine->setVisible(false);
                hLine->setVisible(false);
                labelProxy->setVisible(false);
            }
        }

        QChartView::mouseMoveEvent(event);
    }

    // Override leaveEvent for explicit control
    void leaveEvent(QEvent *event) override {
        vLine->setVisible(false);
        hLine->setVisible(false);
        labelProxy->setVisible(false);
        QChartView::leaveEvent(event);
    }

    void enterEvent(QEnterEvent *event) override {
        // We handle visibility primarily in mouseMoveEvent, but this ensures it is ready
        QChartView::enterEvent(event);
    }

private:
    QGraphicsLineItem *vLine = nullptr;
    QGraphicsLineItem *hLine = nullptr;
    QLabel *valueLabel = nullptr;
    QGraphicsProxyWidget *labelProxy = nullptr;
};
#endif //AWSMOCK_QT_UI_CROSS_HAIR_CHART_VIEW_H
