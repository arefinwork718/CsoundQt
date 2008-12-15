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
#include "qutetext.h"
#include <math.h>

// Font point sizes equivalent for html
// This seems necessary since qt rich text
// only takes these values for font size
#define QUTE_XXSMALL 8
#define QUTE_XSMALL 10
#define QUTE_SMALL 12
#define QUTE_MEDIUM 16
#define QUTE_LARGE 20
#define QUTE_XLARGE 24
#define QUTE_XXLARGE 28

//number of decimal places shown on the widget
#define QUTESLIDER_PRECISION 4

QuteText::QuteText(QWidget *parent) : QuteWidget(parent)
{
  m_value = 0.0;
  m_widget = new LabelWidget(this);
  dynamic_cast<LabelWidget*>(m_widget)->setWordWrap (true);
  dynamic_cast<LabelWidget*>(m_widget)->setMargin (5);
  dynamic_cast<LabelWidget*>(m_widget)->setTextFormat(Qt::RichText);
  connect(dynamic_cast<LabelWidget*>(m_widget), SIGNAL(popUpMenu(QPoint)), this, SLOT(popUpMenu(QPoint)));
}

QuteText::~QuteText()
{
}

double QuteText::getValue()
{
  return m_value;
}

void QuteText::setValue(double value)
{
  if (m_type == "display")
    setText(QString::number(value, 'f', QUTESLIDER_PRECISION));
}

void QuteText::setValue(QString value)
{
  setText(value);
}

void QuteText::setType(QString type)
{
  m_type = type;
}

void QuteText::setResolution(double resolution)
{
  m_resolution = resolution;
}

void QuteText::setAlignment(int alignment)
{
//   qDebug("QuteText::setAlignment %i", alignment);
  Qt::Alignment align;
  switch (alignment) {
    case 0:
      align = Qt::AlignLeft|Qt::AlignTop;
      break;
    case 1:
      align = Qt::AlignHCenter|Qt::AlignTop;
      break;
    case 2:
      align = Qt::AlignRight|Qt::AlignTop;
      break;
    default:
      align = Qt::AlignLeft|Qt::AlignTop;
  }
  dynamic_cast<LabelWidget*>(m_widget)->setAlignment(align);
}

void QuteText::setFont(QString font)
{
  m_font = font;
}

void QuteText::setFontSize(int fontSize)
{
  m_fontSize = fontSize;
}

void QuteText::setTextColor(QColor textColor)
{
  QPalette palette = m_widget->palette();
  palette.setColor(QPalette::WindowText, textColor);
  m_widget->setPalette(palette);
}

void QuteText::setBgColor(QColor bgColor)
{
  QPalette palette = m_widget->palette();
  palette.setColor(QPalette::Window, bgColor);
  m_widget->setPalette(palette);
}

void QuteText::setBg(bool bg)
{
  m_widget->setAutoFillBackground(bg);
}

void QuteText::setBorder(bool border)
{
  if (border)
    dynamic_cast<QFrame*>(m_widget)->setFrameShape(QFrame::Box);
  else
    dynamic_cast<QFrame*>(m_widget)->setFrameShape(QFrame::NoFrame);
}

void QuteText::setText(QString text)
{
  m_text = text;
  int size;
  if (m_fontSize >= QUTE_XXLARGE)
    size = 7;
  else if (m_fontSize >= QUTE_XLARGE)
    size = 6;
  else if (m_fontSize >= QUTE_LARGE)
    size = 5;
  else if (m_fontSize >= QUTE_MEDIUM)
    size = 4;
  else if (m_fontSize >= QUTE_SMALL)
    size = 3;
  else if (m_fontSize >= QUTE_XSMALL)
    size = 2;
  else
    size = 1;
  text.prepend("<font face=\"" + m_font + "\" size=\"" + QString::number(size) + "\">");
  text.append("</font>");
  //TODO USE CORRECT CHARACTER for line break
//   text = text.replace("�", "\n");
  dynamic_cast<LabelWidget*>(m_widget)->setText(text);
}

QString QuteText::getWidgetLine()
{
  QString line = "ioText {" + QString::number(x()) + ", " + QString::number(y()) + "} ";
  line += "{"+ QString::number(width()) +", "+ QString::number(height()) +"} ";
  line += m_type + " ";
  line += QString::number(m_value, 'f', 6) + " 0.00100 \"" + m_name + "\" ";
  QString alignment = "";
  if (((LabelWidget *)m_widget)->alignment() & Qt::AlignLeft)
    alignment = "left";
  else if (((LabelWidget *)m_widget)->alignment() & Qt::AlignCenter)
      alignment = "center";
  else if (((LabelWidget *)m_widget)->alignment() & Qt::AlignRight)
    alignment = "right";
  line += alignment + " ";
  line += "\"" + m_font + "\" " + QString::number(m_fontSize) + " ";
  QColor color = m_widget->palette().color(QPalette::WindowText);
  line += "{" + QString::number(color.red() * 256)
      + ", " + QString::number(color.green() * 256)
      + ", " + QString::number(color.blue() * 256) + "} ";
  color = m_widget->palette().color(QPalette::Window);
  line += "{" + QString::number(color.red() * 256)
      + ", " + QString::number(color.green() * 256)
      + ", " + QString::number(color.blue() * 256) + "} ";
  line += m_widget->autoFillBackground()? "background ":"nobackground ";
  line += dynamic_cast<QFrame*>(m_widget)->frameShape()==QFrame::NoFrame ? "noborder ": "border ";
//   line += ((LabelWidget *)m_widget)->toPlainText();
  line += m_text;
//   qDebug("QuteText::getWidgetLine() %s", line.toStdString().c_str());
  return line;
}

void QuteText::createPropertiesDialog()
{
  QuteWidget::createPropertiesDialog();
  if (m_type == "label") {
    dialog->setWindowTitle("Label");
  }
  else if (m_type == "display") {
    dialog->setWindowTitle("Display");
  }
  QLabel *label = new QLabel(dialog);
  label->setText("Text:");
  layout->addWidget(label, 5, 0, Qt::AlignRight|Qt::AlignVCenter);
  text = new QLineEdit(dialog);
  text->setText(m_text);
  layout->addWidget(text, 5,1,1,3, Qt::AlignLeft|Qt::AlignVCenter);
  text->setMinimumWidth(320);
  label = new QLabel(dialog);
  label->setText("Text Color");
  layout->addWidget(label, 6, 0, Qt::AlignRight|Qt::AlignVCenter);
  textColor = new QPushButton(dialog);
  QPixmap pixmap(64,64);
  pixmap.fill(m_widget->palette().color(QPalette::WindowText));
  textColor->setIcon(pixmap);
  QPalette palette(m_widget->palette().color(QPalette::WindowText));
  textColor->setPalette(palette);
  palette.color(QPalette::Window);
  layout->addWidget(textColor, 6,1, Qt::AlignLeft|Qt::AlignVCenter);
  connect(textColor, SIGNAL(released()), this, SLOT(selectTextColor()));
  label = new QLabel(dialog);
  label->setText("Background Color");
  layout->addWidget(label, 6, 2, Qt::AlignRight|Qt::AlignVCenter);
  bgColor = new QPushButton(dialog);
//   QPixmap pixmap(64,64);
  pixmap.fill(((LabelWidget *) m_widget)->palette().color(QPalette::Window));
  bgColor->setIcon(pixmap);
  palette = QPalette(((LabelWidget *) m_widget)->palette().color(QPalette::Window));
  bgColor->setPalette(palette);
  palette.color(QPalette::Window);
  layout->addWidget(bgColor, 6,3, Qt::AlignLeft|Qt::AlignVCenter);
  bg = new QCheckBox("Background", dialog);
  bg->setChecked(((LabelWidget *)m_widget)->autoFillBackground());
  layout->addWidget(bg, 7,3, Qt::AlignLeft|Qt::AlignVCenter);
  border = new QCheckBox("Border", dialog);
  border->setChecked(((LabelWidget *)m_widget)->frameShape() != QFrame::NoFrame);
  layout->addWidget(border, 7,2, Qt::AlignLeft|Qt::AlignVCenter);
  label = new QLabel(dialog);
  label->setText("Font");
  layout->addWidget(label, 7, 0, Qt::AlignRight|Qt::AlignVCenter);
  font = new QFontComboBox(dialog);
  font->setCurrentFont(QFont(m_font));
  layout->addWidget(font, 7, 1, Qt::AlignLeft|Qt::AlignVCenter);
  label = new QLabel(dialog);
  label->setText("Font Size");
  layout->addWidget(label, 8, 0, Qt::AlignRight|Qt::AlignVCenter);
  fontSize = new QComboBox(dialog);
  fontSize->addItem("XX-Small", QVariant((int) QUTE_XXSMALL));
  fontSize->addItem("X-Small", QVariant((int) QUTE_XSMALL));
  fontSize->addItem("Small", QVariant((int) QUTE_SMALL));
  fontSize->addItem("Medium", QVariant((int) QUTE_MEDIUM));
  fontSize->addItem("Large", QVariant((int) QUTE_LARGE));
  fontSize->addItem("X-Large", QVariant((int) QUTE_XLARGE));
  fontSize->addItem("XX-Large", QVariant((int) QUTE_XXLARGE));
  int i = m_fontSize;
  int index = -1;
  while (i > 0 and index == -1) {
    index = fontSize->findData(i);
    i--;
  }
  if (index == -1)
    index = 0;
  fontSize->setCurrentIndex(index);
  layout->addWidget(fontSize,8, 1, Qt::AlignLeft|Qt::AlignVCenter);
  label = new QLabel(dialog);
  label->setText("Alignment");
  layout->addWidget(label, 8, 2, Qt::AlignRight|Qt::AlignVCenter);
  alignment = new QComboBox(dialog);
  alignment->addItem("Left");
  alignment->addItem("Center");
  alignment->addItem("Right");
  int align;
  Qt::Alignment currentAlignment = ((QLabel *)m_widget)->alignment();
  if (currentAlignment & Qt::AlignLeft) {
      align = 0;
  }
  else if (currentAlignment & Qt::AlignCenter) {
      align = 1;
  }
  else if (currentAlignment & Qt::AlignRight) {
      align = 2;
  }
  else
    align = 0;
  alignment->setCurrentIndex(align);
  layout->addWidget(alignment,8, 3, Qt::AlignLeft|Qt::AlignVCenter);
  connect(bgColor, SIGNAL(released()), this, SLOT(selectBgColor()));
}

void QuteText::applyProperties()
{
  m_font = font->currentFont().family();
  m_fontSize = fontSize->itemData(fontSize->currentIndex()).toInt();
  setText(text->text());
  m_widget->setAutoFillBackground(bg->isChecked());
  dynamic_cast<QFrame*>(m_widget)->setFrameShape(border->isChecked()?  QFrame::Box : QFrame::NoFrame);
  setAlignment(alignment->currentIndex());
  QuteWidget::applyProperties();  //Must be last to make sure the widgetsChanged signal is last
}

void QuteText::selectTextColor()
{
  QColor color = QColorDialog::getColor(m_widget->palette().color(QPalette::WindowText), this);
  if (color.isValid()) {
    setTextColor(color);
    textColor->setPalette(QPalette(m_widget->palette().color(QPalette::WindowText)));
    QPixmap pixmap(64,64);
    pixmap.fill(m_widget->palette().color(QPalette::WindowText));
    textColor->setIcon(pixmap);
  }
}

void QuteText::selectBgColor()
{
  QColor color = QColorDialog::getColor(m_widget->palette().color(QPalette::Window), this);
  if (color.isValid()) {
    setBgColor(color);
    bgColor->setPalette(QPalette(m_widget->palette().color(QPalette::Window)));
    QPixmap pixmap(64,64);
    pixmap.fill(m_widget->palette().color(QPalette::Window));
    bgColor->setIcon(pixmap);
  }
}

/* -----------------------------------------------------------------*/
/*               QuteLineEdit class                                 */
/* -----------------------------------------------------------------*/

QuteLineEdit::QuteLineEdit(QWidget* parent) : QuteText(parent)
{
  delete m_widget; //delete widget created by parent constructor
  m_widget = new LineEditWidget(this);
  connect(dynamic_cast<LineEditWidget*>(m_widget), SIGNAL(popUpMenu(QPoint)), this, SLOT(popUpMenu(QPoint)));
  m_type = "edit";
}

QuteLineEdit::~QuteLineEdit()
{
}

void QuteLineEdit::setAlignment(int alignment)
{
//   qDebug("QuteText::setAlignment %i", alignment);
  Qt::Alignment align;
  switch (alignment) {
    case 0:
      align = Qt::AlignLeft|Qt::AlignTop;
      break;
    case 1:
      align = Qt::AlignHCenter|Qt::AlignTop;
      break;
    case 2:
      align = Qt::AlignRight|Qt::AlignTop;
      break;
    default:
      align = Qt::AlignLeft|Qt::AlignTop;
  }
  static_cast<LineEditWidget*>(m_widget)->setAlignment(align);
}

void QuteLineEdit::setText(QString text)
{
  m_text = text;
//   int size;
//   if (m_fontSize >= QUTE_XXLARGE)
//     size = 7;
//   else if (m_fontSize >= QUTE_XLARGE)
//     size = 6;
//   else if (m_fontSize >= QUTE_LARGE)
//     size = 5;
//   else if (m_fontSize >= QUTE_MEDIUM)
//     size = 4;
//   else if (m_fontSize >= QUTE_SMALL)
//     size = 3;
//   else if (m_fontSize >= QUTE_XSMALL)
//     size = 2;
//   else
//     size = 1;
//   text.prepend("<font face=\"" + m_font + "\" size=\"" + QString::number(size) + "\">");
//   text.append("</font>");
  //TODO USE CORRECT CHARACTER for line break
//   text = text.replace("�", "\n");
  dynamic_cast<LineEditWidget*>(m_widget)->setText(text);
}

QString QuteLineEdit::getWidgetLine()
{
  QString line = "ioText {" + QString::number(x()) + ", " + QString::number(y()) + "} ";
  line += "{"+ QString::number(width()) +", "+ QString::number(height()) +"} ";
  line += m_type + " ";
  line += QString::number(m_value, 'f', 6) + " 0.00100 \"" + m_name + "\" ";
  QString alignment = "";
  int align = dynamic_cast<LineEditWidget *>(m_widget)->alignment();
  if (align & Qt::AlignLeft)
    alignment = "left";
  else if (align & Qt::AlignCenter)
    alignment = "center";
  else if (align & Qt::AlignRight)
    alignment = "right";
  line += alignment + " ";
  line += "\"" + m_font + "\" " + QString::number(m_fontSize) + " ";
  QColor color = m_widget->palette().color(QPalette::WindowText);
  line += "{" + QString::number(color.red() * 256)
      + ", " + QString::number(color.green() * 256)
      + ", " + QString::number(color.blue() * 256) + "} ";
  color = m_widget->palette().color(QPalette::Window);
  line += "{" + QString::number(color.red() * 256)
      + ", " + QString::number(color.green() * 256)
      + ", " + QString::number(color.blue() * 256) + "} ";
  line += m_widget->autoFillBackground()? "background ":"nobackground ";
  line += "noborder ";
  line += m_text;
//   qDebug("QuteText::getWidgetLine() %s", line.toStdString().c_str());
  return line;
}

QString QuteLineEdit::getStringValue()
{
  return static_cast<LineEditWidget *>(m_widget)->text();
}

void QuteLineEdit::createPropertiesDialog()
{
  QuteText::createPropertiesDialog();
  dialog->setWindowTitle("Line Edit");
  fontSize->hide();
  font->hide();
  border->hide();
  bg->hide();
  textColor->hide();
  bgColor->hide();
  text->setText(dynamic_cast<LineEditWidget *>(m_widget)->text());
}

void QuteLineEdit::applyProperties()
{
  setText(text->text());
  m_widget->setAutoFillBackground(bg->isChecked());
  setAlignment(alignment->currentIndex());
  QuteWidget::applyProperties();  //Must be last to make sure the widgetsChanged signal is last
}


/* -----------------------------------------------------------------*/
/*               QuteScrollNumber class                             */
/* -----------------------------------------------------------------*/

QuteScrollNumber::QuteScrollNumber(QWidget* parent) : QuteText(parent)
{
  delete m_widget; //delete widget created by parent constructor
  m_widget = new ScrollNumberWidget(this);
  connect(dynamic_cast<ScrollNumberWidget*>(m_widget), SIGNAL(popUpMenu(QPoint)), this, SLOT(popUpMenu(QPoint)));
  connect(dynamic_cast<ScrollNumberWidget*>(m_widget), SIGNAL(addValue(double)), this, SLOT(addValue(double)));
  connect(dynamic_cast<ScrollNumberWidget*>(m_widget), SIGNAL(setValue(double)), this, SLOT(setValue(double)));
  m_type = "scroll";
}

QuteScrollNumber::~QuteScrollNumber()
{
}

void QuteScrollNumber::setResolution(double resolution)
{
  m_resolution = resolution;
  int i;
  for (i = 0; i < 6; i++) {
//     Check for used decimal places.
    if ((m_resolution * pow(10, i)) == (int) (m_resolution * pow(10,i)) )
      break;
  }
  m_places = i;
  static_cast<ScrollNumberWidget*>(m_widget)->setResolution(resolution);
}

void QuteScrollNumber::setAlignment(int alignment)
{
//   qDebug("QuteText::setAlignment %i", alignment);
  Qt::Alignment align;
  switch (alignment) {
    case 0:
      align = Qt::AlignLeft|Qt::AlignTop;
      break;
    case 1:
      align = Qt::AlignHCenter|Qt::AlignTop;
      break;
    case 2:
      align = Qt::AlignRight|Qt::AlignTop;
      break;
    default:
      align = Qt::AlignLeft|Qt::AlignTop;
  }
  static_cast<ScrollNumberWidget*>(m_widget)->setAlignment(align);
}

void QuteScrollNumber::setText(QString text)
{
  m_text = text;
  int size;
  if (m_fontSize >= QUTE_XXLARGE)
    size = 7;
  else if (m_fontSize >= QUTE_XLARGE)
    size = 6;
  else if (m_fontSize >= QUTE_LARGE)
    size = 5;
  else if (m_fontSize >= QUTE_MEDIUM)
    size = 4;
  else if (m_fontSize >= QUTE_SMALL)
    size = 3;
  else if (m_fontSize >= QUTE_XSMALL)
    size = 2;
  else
    size = 1;
  text.prepend("<font face=\"" + m_font + "\" size=\"" + QString::number(size) + "\">");
  text.append("</font>");
  //TODO USE CORRECT CHARACTER for line break
//   text = text.replace("�", "\n");
  dynamic_cast<ScrollNumberWidget*>(m_widget)->setText(text);
}

QString QuteScrollNumber::getWidgetLine()
{
  QString line = "ioText {" + QString::number(x()) + ", " + QString::number(y()) + "} ";
  line += "{"+ QString::number(width()) +", "+ QString::number(height()) +"} ";
  line += m_type + " ";
  line += QString::number(m_value, 'f', 6) + " ";
  line += QString::number(dynamic_cast<ScrollNumberWidget*>(m_widget)->getResolution(), 'f', 6);
  line += + " \"" + m_name + "\" ";
  QString alignment = "";
  int align = dynamic_cast<ScrollNumberWidget *>(m_widget)->alignment();
  if (align & Qt::AlignLeft)
    alignment = "left";
  else if (align & Qt::AlignCenter)
    alignment = "center";
  else if (align & Qt::AlignRight)
    alignment = "right";
  line += alignment + " ";
  line += "\"" + m_font + "\" " + QString::number(m_fontSize) + " ";
  QColor color = m_widget->palette().color(QPalette::WindowText);
  line += "{" + QString::number(color.red() * 256)
      + ", " + QString::number(color.green() * 256)
      + ", " + QString::number(color.blue() * 256) + "} ";
  color = m_widget->palette().color(QPalette::Window);
  line += "{" + QString::number(color.red() * 256)
      + ", " + QString::number(color.green() * 256)
      + ", " + QString::number(color.blue() * 256) + "} ";
  line += m_widget->autoFillBackground()? "background ":"nobackground ";
  line += dynamic_cast<QFrame*>(m_widget)->frameShape()==QFrame::NoFrame ? "noborder ": "border ";
  line += m_text;
//   qDebug("QuteText::getWidgetLine() %s", line.toStdString().c_str());
  return line;
}

QString QuteScrollNumber::getStringValue()
{
  return static_cast<ScrollNumberWidget *>(m_widget)->text();
}

double QuteScrollNumber::getValue()
{
  return QuteText::getValue();
}

void QuteScrollNumber::createPropertiesDialog()
{
//   qDebug("QuteScrollNumber::createPropertiesDialog()");
  QuteText::createPropertiesDialog();
  dialog->setWindowTitle("Scroll Number");
  QLabel *label = new QLabel(dialog);
  label->setText("Resolution");
  layout->addWidget(label, 4, 0, Qt::AlignRight|Qt::AlignVCenter);
  resolutionSpinBox = new QDoubleSpinBox(dialog);
  resolutionSpinBox->setDecimals(6);
  resolutionSpinBox->setValue(dynamic_cast<ScrollNumberWidget*>(m_widget)->getResolution());
  layout->addWidget(resolutionSpinBox, 4, 1, Qt::AlignLeft|Qt::AlignVCenter);
}

void QuteScrollNumber::applyProperties()
{
  setResolution(resolutionSpinBox->value());
  bool ok;
  double value = text->text().toDouble(&ok);
  if (ok) {
    m_value = value;
    addValue(0);
  }
  QuteText::applyProperties();  //Must be last to make sure the widgetsChanged signal is last
}

void QuteScrollNumber::addValue(double delta)
{
  m_value +=delta;
  m_resolution = dynamic_cast<ScrollNumberWidget*>(m_widget)->getResolution();
//   qDebug("QuteScrollNumber::addValue places = %i resolution = %f", places, m_resolution);
  setText(QString::number(m_value, 'f', m_places));
  emit widgetChanged(this);
}

void QuteScrollNumber::setValue(double value)
{
  m_value = value;
//   qDebug("QuteScrollNumber::addValue places = %i resolution = %f", places, m_resolution);
  setText(QString::number(m_value, 'f', m_places));
  emit widgetChanged(this);
}
