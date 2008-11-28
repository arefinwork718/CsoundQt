/***************************************************************************
 *   Copyright (C) 2008 by Andres Cabrera                                  *
 *   mantaraya36@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/
#include "qutemeter.h"

QuteMeter::QuteMeter(QWidget *parent) : QuteWidget(parent)
{
  setGeometry(0,0, parent->width(), parent->height());
  m_widget = new MeterWidget(this);
  m_widget->setAutoFillBackground(true);
  connect(((MeterWidget *)m_widget), SIGNAL(popUpMenu(QPoint)), this, SLOT(popUpMenu(QPoint)));
  connect(((MeterWidget *)m_widget), SIGNAL(newValues(double, double)), this, SLOT(setValuesFromWidget(double,double)));
}

QuteMeter::~QuteMeter()
{
}

QString QuteMeter::getWidgetLine()
{
  QString line = "ioMeter {" + QString::number(x()) + ", " + QString::number(y()) + "} ";
  line += "{"+ QString::number(width()) +", "+ QString::number(height()) +"} ";
  line += "{" + QString::number(m_color.red() * 256)
      + ", " + QString::number(m_color.green() * 256)
      + ", " + QString::number(m_color.blue() * 256) + "} ";
  line += "\"" + m_name + "\" " + QString::number(((MeterWidget *)m_widget)->getValue(), 'f', 6) + " ";
  line += "\"" + m_name2 + "\" " + QString::number(((MeterWidget *)m_widget)->getValue2(), 'f', 6) + " ";
  line += ((MeterWidget *)m_widget)->getType() + " ";
  line += QString::number(((MeterWidget *)m_widget)->getPointSize()) + " ";
  line += QString::number(m_fadeSpeed) + " ";
  line += m_behavior;
  qDebug("QuteMeter::getWidgetLine() %s", line.toStdString().c_str());
  return line;
}

void QuteMeter::popUpMenu(QPoint pos)
{
  QuteWidget::popUpMenu(pos);
}

void QuteMeter::createPropertiesDialog()
{
  QuteWidget::createPropertiesDialog();
  QLabel *label = new QLabel(dialog);

  nameLineEdit->setText(getChannelName());

  label = new QLabel(dialog);
  label->setText("Channel 2 name =");
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  layout->addWidget(label, 4, 0, Qt::AlignRight|Qt::AlignVCenter);
  name2LineEdit = new QLineEdit(dialog);
  name2LineEdit->setText(getChannel2Name());
  name2LineEdit->setMinimumWidth(320);
  layout->addWidget(name2LineEdit, 4,1,1,3, Qt::AlignLeft|Qt::AlignVCenter);
  if (((MeterWidget *)m_widget)->getType() != "point" and ((MeterWidget *)m_widget)->getType() != "crosshair") {
    label->setEnabled(false);
    name2LineEdit->setEnabled(false);
  }

  label = new QLabel(dialog);
  label->setText("Color");
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  layout->addWidget(label, 5, 0, Qt::AlignRight|Qt::AlignVCenter);
  colorButton = new QPushButton(dialog);
  QPixmap pixmap(64,64);
  pixmap.fill(((MeterWidget *) m_widget)->getColor());
  colorButton->setIcon(pixmap);
  QPalette palette(((MeterWidget *) m_widget)->getColor());
  palette.color(QPalette::Window);
  colorButton->setPalette(palette);
  layout->addWidget(colorButton, 5,1, Qt::AlignLeft|Qt::AlignVCenter);
  connect(colorButton, SIGNAL(released()), this, SLOT(selectTextColor()));

  label->setText("Type");
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  layout->addWidget(label, 6, 0, Qt::AlignRight|Qt::AlignVCenter);
//   label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  typeComboBox = new QComboBox(dialog);
  typeComboBox->addItem("fill");
  typeComboBox->addItem("llif");
  typeComboBox->addItem("line");
  typeComboBox->addItem("crosshair");
  typeComboBox->addItem("point");
  typeComboBox->setCurrentIndex(typeComboBox->findText(((MeterWidget *)m_widget)->getType()));
  layout->addWidget(typeComboBox, 6, 1, Qt::AlignLeft|Qt::AlignVCenter);

  label = new QLabel(dialog);
  label->setText("Point size:");
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  layout->addWidget(label, 7, 0, Qt::AlignRight|Qt::AlignVCenter);
  pointSizeSpinBox = new QSpinBox(dialog);
  pointSizeSpinBox->setValue(((MeterWidget *)m_widget)->getPointSize());
  layout->addWidget(pointSizeSpinBox, 7,1, Qt::AlignLeft|Qt::AlignVCenter);
  if (((MeterWidget *)m_widget)->getType() != "point") {
    label->setEnabled(false);
    pointSizeSpinBox->setEnabled(false);
  }

  label = new QLabel(dialog);
  label->setText("Fade speed:");
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  layout->addWidget(label, 7, 2, Qt::AlignRight|Qt::AlignVCenter);
  fadeSpeedSpinBox = new QSpinBox(dialog);
  fadeSpeedSpinBox->setValue(m_fadeSpeed);
  fadeSpeedSpinBox->setRange(0, 1000);
  layout->addWidget(fadeSpeedSpinBox, 7,3, Qt::AlignLeft|Qt::AlignVCenter);
  label->setEnabled(false);
  fadeSpeedSpinBox->setEnabled(false);

  label = new QLabel(dialog);
  label->setText("Behavior");
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  layout->addWidget(label, 8, 0, Qt::AlignRight|Qt::AlignVCenter);
  behaviorComboBox = new QComboBox(dialog);
  behaviorComboBox->addItem("notrack");
  behaviorComboBox->addItem("mouse");
  behaviorComboBox->addItem("mousezero");
  behaviorComboBox->addItem("toggle");
  behaviorComboBox->addItem("togglezero");
  behaviorComboBox->setCurrentIndex(behaviorComboBox->findText(m_behavior));
  layout->addWidget(behaviorComboBox,8, 1, Qt::AlignLeft|Qt::AlignVCenter);
  label->setEnabled(false);
  behaviorComboBox->setEnabled(false);
}

void QuteMeter::applyProperties()
{
  m_name = nameLineEdit->text();
  m_name2 = name2LineEdit->text();
  setColor(colorButton->palette().color(QPalette::Window));
  ((MeterWidget *)m_widget)->setType(typeComboBox->currentText());
  ((MeterWidget *)m_widget)->setPointSize(pointSizeSpinBox->value());
  m_fadeSpeed = fadeSpeedSpinBox->value();
  m_behavior = behaviorComboBox->currentText();
  QuteWidget::applyProperties();  //Must be last to make sure the widgetsChanged signal is last
  setValue(m_value); // To update controller type on change from fill to llif
  qDebug("QuteMeter::applyProperties()");
}

void QuteMeter::setWidgetGeometry(int x,int y,int width,int height)
{
  QuteWidget::setWidgetGeometry(x,y,width, height);
  /* In MacCsound, meter widgets have an offset of about five pixels in every border
     This has proven problematic in Qt, as the graphicsScene*/
  ((MeterWidget *)m_widget)->setWidgetGeometry(0,0,width, height);
}

void QuteMeter::setValue(double value)
{
  if (value < 0.0)
    value = 0.0;
  else if (value > 1.0)
    value = 1.0;
  ((MeterWidget *)m_widget)->setValue(value);
//   m_value = value;
}

double QuteMeter::getValue()
{
  return ((MeterWidget *)m_widget)->getValue();
}

double QuteMeter::getValue2()
{
  return ((MeterWidget *)m_widget)->getValue2();
}

QString QuteMeter::getChannelName()
{
//   if (((MeterWidget *)m_widget)->getType() == "crosshair" or ((MeterWidget *)m_widget)->getType() == "point")
//     return m_name2;
//   else
    return m_name;
}

QString QuteMeter::getChannel2Name()
{
//   if (((MeterWidget *)m_widget)->getType() == "crosshair" or ((MeterWidget *)m_widget)->getType() == "point")
//     return m_name;
//   else
  if (((MeterWidget *)m_widget)->getType() == "crosshair" or ((MeterWidget *)m_widget)->getType() == "point")
    return m_name2;
  else
    return QString("");
}

void QuteMeter::setChannel2Name(QString name)
{
  m_name2 = name;
}

void QuteMeter::setValue2(double value)
{
  if (value < 0.0)
    value = 0.0;
  else if (value > 1.0)
    value = 1.0;
  ((MeterWidget *)m_widget)->setValue2(value);
//   m_value2 = value;
}

void QuteMeter::setColor(QColor color)
{
  ((MeterWidget *)m_widget)->setColor(color);
  m_color = color;
}

void QuteMeter::setType(QString type)
{
  ((MeterWidget *)m_widget)->setType(type);
}

void QuteMeter::setPointSize(int size)
{
  ((MeterWidget *)m_widget)->setPointSize(size);
}

void QuteMeter::setFadeSpeed(int speed)
{
  m_fadeSpeed = speed;
}

void QuteMeter::setBehavior(QString behavior)
{
  m_behavior = behavior;
}

void QuteMeter::selectTextColor()
{
  QColor color = QColorDialog::getColor(m_color, this);
  if (color.isValid()) {
    QPixmap pixmap(64,64);
    pixmap.fill(color);
    colorButton->setIcon(pixmap);
    QPalette palette(color);
    palette.color(QPalette::Window);
    colorButton->setPalette(palette);
  }
}

void QuteMeter::setValuesFromWidget(double value1, double value2)
{
  setValue(value1);
  setValue2(value2);
}

/* Meter Widget ----------------------------------------*/

MeterWidget::MeterWidget(QWidget *parent) : QGraphicsView(parent)
{
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setTransformationAnchor(QGraphicsView::NoAnchor);
  setInteractive (true);
  m_scene = new QGraphicsScene(5,5, 20, 20, this);
  m_scene->setBackgroundBrush(Qt::black);
  m_scene->setSceneRect(0,0, 20, 20);
  setScene(m_scene);
  m_mouseDown = false;
//   m_background = m_scene->addRect(0, 0, 0, 0, QPen() , QBrush(Qt::black));
  m_block = m_scene->addRect(0, 0, 0, 0, QPen() , QBrush(Qt::green));
  m_point = m_scene->addEllipse(0, 0, 0, 0, QPen(Qt::green) , QBrush(Qt::green));
  m_vline = m_scene->addLine(0, 0, 0, 0, QPen(Qt::green));
  m_hline = m_scene->addLine(0, 0, 0, 0, QPen(Qt::green));
}

MeterWidget::~MeterWidget()
{
}

QColor MeterWidget::getColor()
{
  return m_block->brush().color();
}

double MeterWidget::getValue()
{
  // For crosshair and point, the first value is x , the second y
//   if (m_type == "crosshair" or m_type == "point") {
//     return m_value2;
//   }
//   else
    return m_value;
}

double MeterWidget::getValue2()
{
//   if (m_type == "crosshair" or m_type == "point") {
//     return m_value;
//   }
//   else
    return m_value2;
}

void MeterWidget::setValue(double value)
{
  /*if (m_type == "crosshair" or m_type == "point") {
    m_value2 = value;
    m_hline->setLine(0, (1-m_value2)*height(), width(), (1-m_value2)*height());
    m_vline->setLine(m_value*width(), 0 ,m_value*width(), height());
    m_point->setRect(m_value*width()- (m_pointSize/2), (1-m_value2)*height()- (m_pointSize/2), m_pointSize, m_pointSize);
  }
  else  */{
    m_value = value;
    if (m_type == "fill") {
      m_block->setRect(0, (1-value)*height(), width(), height());
    }
    else if (m_type == "llif") {
      m_block->setRect(0, 0, width(), (1-value)*height());
    }
    m_hline->setLine(0, (1-value)*height(), width(), (1-value)*height());
    m_vline->setLine(m_value2*width(), 0 ,m_value2*width(), height());
    m_point->setRect(m_value2*width()- (m_pointSize/2), (1-value)*height()- (m_pointSize/2), m_pointSize, m_pointSize);
  }
}

void MeterWidget::setValue2(double value)
{
  if (m_type == "crosshair" or m_type == "point") {
    m_value2 = value;
//     m_hline->setLine(0, (1-m_value2)*height(), width(), (1-m_value2)*height());
//     m_vline->setLine(m_value*width(), 0 ,m_value*width(), height());
//     m_point->setRect(m_value*width()- (m_pointSize/2), (1-m_value2)*height()- (m_pointSize/2), m_pointSize, m_pointSize);
    setValue(m_value);
  }
}

void MeterWidget::setType(QString type)
{
  qDebug("MeterWidget::setType");
  if (type == "fill") {
    m_type = type;
    m_block->show();
    m_point->hide();
    m_vline->hide();
    m_hline->hide();
  }
  else if (type == "llif") {
    m_type = type;
    m_block->show();
    m_point->hide();
    m_vline->hide();
    m_hline->hide();
  }
  else if (type == "line") {
    m_type = type;
    m_block->hide();
    m_point->hide();
    m_vline->hide();
    m_hline->show();
  }
  else if (type == "crosshair") {
    m_type = type;
    m_block->hide();
    m_point->hide();
    m_vline->show();
    m_hline->show();
  }
  else if (type == "point") {
    m_type = type;
    m_block->hide();
    m_point->show();
    m_vline->hide();
    m_hline->hide();
  }
}

void MeterWidget::setPointSize(int size)
{
  m_pointSize = size;
  m_point->setRect(m_value2, m_value, size, size);
}

void MeterWidget::setColor(QColor color)
{
  qDebug("MeterWidget::setColor()");
  m_block->setBrush(QBrush(color));
  m_point->setPen(QPen(color));
  m_point->setBrush(QBrush(color));
  m_vline->setPen(QPen(color));
  m_hline->setPen(QPen(color));
}

void MeterWidget::setWidgetGeometry(int x,int y,int width,int height)
{
  m_scene->setSceneRect(0,0, width, height);
  setSceneRect(0,0, width, height);
//   m_background->setRect(0,0,width - 5, height - 5 );
  setGeometry(x,y,width, height);
  setMaximumSize(width, height);
}

void MeterWidget::mouseMoveEvent(QMouseEvent* event)
{
  if (m_mouseDown) {
    if (event->x() > 0 and event->x()< width() and
        event->y() > 0 and event->y()< height())
    emit newValues(1 - ((double)event->y()/ height()), (double)event->x()/ width());
  }
}

void MeterWidget::mousePressEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton) {
    m_mouseDown = true;
  }
}

void MeterWidget::mouseReleaseEvent(QMouseEvent* event)
{
  m_mouseDown = false;
}
