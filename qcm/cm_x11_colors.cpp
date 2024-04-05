/* external standar */
#include <string>
#include <iostream>
#include <regex>
#include <math.h>

/* Qt5 environment libraries */
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QSpinBox>


/* codemapper libraries */
#include <cm_x11_colors.hpp>

cm_qcolor::cm_qcolor(QWidget *parent) : QDialog(parent)
{   
    /* canvas */
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    QFormLayout *grid_layout = new QFormLayout;
    QHBoxLayout *bar_layout = new QHBoxLayout;

    /* wheels  */
    QLabel *title = new QLabel("X11 standard color:", this);
    colorcombo = new QComboBox(this);
    for (const auto& two : x11colors) 
    {    
        const std::string& x11name = two.first;
        colorcombo->addItem(QString::fromStdString(x11name));
        x11palette.push_back(two.second);
    }    
    red_spin = new QSpinBox(this);
    red_spin->setRange(0,255);
    green_spin = new QSpinBox(this);
    green_spin->setRange(0,255);
    blue_spin = new QSpinBox(this);
    blue_spin->setRange(0,255);    

    /* sample */
    sample = new QLabel(this);
    sample->setFixedSize(100, 100);
    sample->setAutoFillBackground(false);
    html.setFixedHeight(40);
    update_from_name();

    /* button OK/CANCEL */
    QPushButton *OK = new QPushButton("OK", this);
    QPushButton *CANCEL = new QPushButton("Cancel", this);

    // Adding widgets to layouts
    grid_layout->addRow(title, colorcombo);
    grid_layout->addRow("Red:", red_spin);
    grid_layout->addRow("Green:", green_spin);
    grid_layout->addRow("Blue:", blue_spin);
    grid_layout->addRow("Sample:", sample);
    grid_layout->addRow("HTML:", &html);
    bar_layout->addWidget(OK);
    bar_layout->addWidget(CANCEL);
    main_layout->addLayout(grid_layout);        
    main_layout->addLayout(bar_layout);        
    

    // Connect signals and slots
    connect(OK, &QPushButton::clicked, this, &QDialog::accept);
    connect(CANCEL, &QPushButton::clicked, this, &QDialog::reject);
    connect(colorcombo, QOverload<int>::of(&QComboBox::activated), this, &cm_qcolor::update_from_name);
    connect(red_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &cm_qcolor::update_from_wheel);
    connect(green_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &cm_qcolor::update_from_wheel);
    connect(blue_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &cm_qcolor::update_from_wheel);

    // Set window title
    setWindowTitle("Color Selector");
}

void cm_qcolor::update_from_name()
{
    /* sample */
    int s = colorcombo->currentIndex();
    std::string current = colorcombo->itemText(s).toStdString();    
    QColor color(x11colors[current].red,x11colors[current].green,x11colors[current].blue);    
    red_spin->setValue(x11colors[current].red);
    green_spin->setValue(x11colors[current].green);
    blue_spin->setValue(x11colors[current].blue);
    QString hex = color.name();    
    sample->setStyleSheet("QLabel { background-color: " + hex + "; color: white; }");
    html.setText(hex);
    std::cerr << "cm_qcolor: Update() <- from name " << std::endl;
}

void cm_qcolor::update_from_wheel()
{
    /* sample */    
    std::cerr << "cm_qcolor: Update() <- from wheels " << std::endl;
    cm_rgb_color selection;
    selection.red = red_spin->value();
    selection.green = green_spin->value();
    selection.blue = blue_spin->value();    
    int s = equal(selection);
    if (s==-1)
    {
        if (timeswheel>25)
        {
            timeswheel = 0;
            s = nearest(selection);
        }
        else
        {
            timeswheel++;
            return;
        }
    }
    std::cerr << "color " << s;
    colorcombo->setCurrentIndex(s);
    std::string current = colorcombo->itemText(s).toStdString();  
    std::cerr << current << std::endl;
    QColor color(x11colors[current].red,x11colors[current].green,x11colors[current].blue);    
    red_spin->setValue(x11colors[current].red);
    green_spin->setValue(x11colors[current].green);
    blue_spin->setValue(x11colors[current].blue);
    QString hex = color.name();    
    sample->setStyleSheet("QLabel { background-color: " + hex + "; color: white; }");
    html.setText(hex);
    std::cerr << "cm_qcolor: Update()" << std::endl;    
}

double cm_qcolor::distance(const cm_rgb_color& color1, const cm_rgb_color& color2) 
{
    return std::sqrt(std::pow(color1.red - color2.red, 2) + std::pow(color1.green - color2.green, 2) + std::pow(color1.blue - color2.blue, 2));
}

int cm_qcolor::nearest(cm_rgb_color selection)
{
    double cmin = std::numeric_limits<double>::max();
    int nearest = 0;
    int i = 0;
    for (const auto& color : x11palette) 
    {
        double dist = distance(selection, color);
        if (dist < cmin)                 
        {
            cmin = dist;
            nearest = i;
        }
        i++;
    }    
    return(nearest);
}

int cm_qcolor::equal(cm_rgb_color selection)
{
    int i = 0;
    for (const auto& color : x11palette) 
    {
        if ((selection.blue==color.blue)&&(selection.green==color.green)&&(selection.red==color.red))
        {
            return(i);
        }
        i++;
    }
    return(-1);
}
