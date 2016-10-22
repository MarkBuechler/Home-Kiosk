/* */

#include "KioskJukeboxItem.h"

KioskJukeboxItem::KioskJukeboxItem(QWidget *parent)
    : QWidget(parent)
{
    QWidget::setFixedSize(215, 85);
}

void KioskJukeboxItem::setSelection(QString selection1, QString selection2)
{
    m_selection1 = selection1;
    m_selection2 = selection2;

    repaint();
}

void KioskJukeboxItem::setTitles(QString artist, QString title1, QString title2)
{
    m_artist = artist;
    m_title1 = title1;
    m_title2 = title2;

    repaint();
}

void KioskJukeboxItem::drawLabel(QPainter *painter)
{
    QRect rect = QWidget::rect();
    float centerY = rect.height() / 4;
    int font_size = int(rect.height() * .20);

	QFont font;
	font.setPointSize(font_size);
	font.setBold(true);

	QPen text_pen;
	text_pen.setBrush(Qt::red);

	painter->setPen(text_pen);
	painter->setFont(font);

	QSize size1, size2;

    if (!m_selection1.isEmpty())
        size1 = _textSize(painter, m_selection1);
    if (!m_selection2.isEmpty())
        size2 = _textSize(painter, m_selection2);

    QPointF center = _center(size1, !m_selection1.isEmpty());

    float mainX1 = rect.width() - size1.width();
    float mainX2 = rect.width() - size2.width();
    float mainY;

    if (!m_selection1.isEmpty())
        mainY = centerY + center.y();
    else
        mainY = centerY - center.y();

    QPointF point1 = QPointF(mainX1, mainY);
    QPointF point2 = QPointF(mainX2, mainY + (centerY * 2));

    if (!m_selection1.isEmpty())
        painter->drawText(point1, m_selection1);

    if (!m_selection2.isEmpty())
        painter->drawText(point2, m_selection2);
}

void KioskJukeboxItem::drawTitles(QPainter *painter)
{
    QRect rect = QWidget::rect().adjusted(0, 0, -45, 0);
    float centerX = rect.width() / 2;
    float centerY = rect.height() / 2;
    int font_size = int(rect.height() * .09);

    QFont font;
    font.setPointSize(font_size);
    font.setBold(true);

    QPen pen;
    pen.setBrush(Qt::black);

    painter->setPen(pen);
    painter->setFont(font);

    QSize a_size, t1_size, t2_size;

    if (!m_artist.isEmpty())
        a_size = _textSize(painter, m_artist);
    if (!m_title1.isEmpty())
        t1_size = _textSize(painter, m_title1);
    if (!m_title2.isEmpty())
        t2_size = _textSize(painter, m_title2);

    QPointF a_center = _center(a_size, !m_artist.isEmpty());
    QPointF t1_center = _center(t1_size, !m_title1.isEmpty());
    QPointF t2_center = _center(t2_size, !m_title2.isEmpty());

    float a_x = centerX - a_center.x();
    float a_y = centerY + a_center.y() + 2;

    float t1_x = centerX - t1_center.x();
    float t1_y = (centerY / 2) + t1_center.y();

    float t2_x = centerX - t2_center.x();
    float t2_y = ((centerY / 2) * 3) + t2_center.y();

    QPointF a_point = QPointF(a_x, a_y);
    QPointF t1_point = QPointF(t1_x, t1_y);
    QPointF t2_point = QPointF(t2_x, t2_y);

    if (!m_artist.isEmpty())
        painter->drawText(a_point, m_artist);

    if (!m_title1.isEmpty())
        painter->drawText(t1_point, m_title1);

    if (!m_title2.isEmpty())
        painter->drawText(t2_point, m_title2);
}

QSize KioskJukeboxItem::_textSize(QPainter *painter, QString text)
{
    return QSize(painter->fontMetrics().width(text, text.length()),
                 painter->fontMetrics().lineSpacing());
}

QPointF KioskJukeboxItem::_center(QSize size, bool is_text)
{
    float x = size.width() / 2;
    float y;

    if (is_text)
        y = size.height() / 4;
    else
        y = size.height() / 2;

    return QPointF(x, y);
}

void KioskJukeboxItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect rect = QWidget::rect();

    painter.setOpacity(0.5);
    painter.fillRect(rect, Qt::black);
    painter.setOpacity(1.0);

    QPixmap background(":/res/jukebox_label.png");

    QRect prect = rect.adjusted(0, 0, -45, 0);

    painter.drawPixmap(prect, background);

    drawLabel(&painter);
    drawTitles(&painter);
}
