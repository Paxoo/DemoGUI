#include "donutbreakdownchart.h"
#include "mainslice.h"
#include <QtCharts/QPieSlice>
#include <QtCharts/QPieLegendMarker>

QT_CHARTS_USE_NAMESPACE

DonutBreakdownChart::DonutBreakdownChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    // create the series for main center pie
    m_mainSeries = new QPieSeries();
    m_mainSeries->setPieSize(0.5);
    QChart::addSeries(m_mainSeries);

}

void DonutBreakdownChart::addBreakdownSeries(QPieSeries *breakdownSeries, QColor color)
{
    QFont font("Arial", 8);

    // add breakdown series as a slice to center pie
    MainSlice *mainSlice = new MainSlice(breakdownSeries);
    mainSlice->setName(breakdownSeries->name());
    mainSlice->setValue(breakdownSeries->sum());
    m_mainSeries->append(mainSlice);

    // customize the slice
    mainSlice->setBrush(color);
    mainSlice->setLabelVisible();
    mainSlice->setLabelColor(Qt::white);
    mainSlice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
    mainSlice->setLabelFont(font);

    // position and customize the breakdown series
    breakdownSeries->setPieSize(0.6);
    breakdownSeries->setHoleSize(0.5);
    breakdownSeries->setLabelsVisible();
    const auto slices = breakdownSeries->slices();
    for (QPieSlice *slice : slices) {
        color = color.lighter(105);
        slice->setBrush(color);
        slice->setLabelFont(font);
    }

    // add the series to the chart
    QChart::addSeries(breakdownSeries);

    // recalculate breakdown donut segments
    //recalculateAngles();

    // update customize legend markers
    //updateLegendMarkers();
}

void DonutBreakdownChart::recalculateAngles(QList<QColor> color)
{
    const auto slices = m_mainSeries->slices();

    // we need to recalculate the sum of the main slice and adjust the slice labels + color
    char i = 0;
    for (QPieSlice *slice : slices) {
        QPieSeries *breakdownSeries = qobject_cast<MainSlice *>(slice)->breakdownSeries();
        slice->setValue(breakdownSeries->sum());
        breakdownSeries->setLabelsVisible();

        QColor c = color.at(i);
        const auto slices = breakdownSeries->slices();
        for (QPieSlice *slice : slices) {
            slice->setLabelVisible();
            c = c.lighter(105);
            slice->setBrush(c);
            slice->setLabelFont(QFont("Arial", 8));
        }
        i = i + 1;
    }

    qreal angle = 0;
    for (QPieSlice *slice : slices) {
        QPieSeries *breakdownSeries = qobject_cast<MainSlice *>(slice)->breakdownSeries();     
        breakdownSeries->setPieStartAngle(angle);
        angle += slice->percentage() * 360.0; // full pie is 360.0
        breakdownSeries->setPieEndAngle(angle);
    }
}

void DonutBreakdownChart::updateLegendMarkers()
{
    // go through all markers
    const auto allseries = series();
    for (QAbstractSeries *series : allseries) {
        const auto markers = legend()->markers(series);
        for (QLegendMarker *marker : markers) {
            QPieLegendMarker *pieMarker = qobject_cast<QPieLegendMarker *>(marker);
            if (series == m_mainSeries) {
                // hide markers from main series
                pieMarker->setVisible(false);
            } else {
                // modify markers from breakdown series
                pieMarker->setLabel(QString("%1: %2")
                                    .arg(pieMarker->slice()->label())
                                    .arg(pieMarker->slice()->value()));
                pieMarker->setFont(QFont("Arial", 8));
            }
        }
    }
}
