#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QPixmap>
#include <QLCDNumber>
#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QImage>
#include <QFrame>
#include<QHBoxLayout>
#include<QGridLayout>
#include<QMouseEvent>
#include<QEvent>
#include<QDebug>

class Interface : public QWidget
{
    Q_OBJECT

public:
    explicit Interface();
    void loadImage();
    void addIconToLabel(QString fileName,QLabel* label);
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void leaveEvent();
    // checks if mouse located where i want (in label)
    bool inLabel(QLabel* label, int xPositionMouse, int yPositionMouse);
    //reset all border and back-ground color of labels except one label (QString in use).
    // this method allows me to reset all label to they original color except one label.
    // and the Qstring InUse allows me to know which label I do not want to reset (its background color and border).
    void resetAllLabel(QString inUse);
    // this method will set up the sliders for the light "button" (button with sun icon) in the right position
    void setUpLightSliders();
    // this method will hide sliders sliders in the light panel.
    void hideSliderLights();
    // show sliders in the light panel
    void showSliderLights();


signals:
    // the following signal checks if mouse is pressed.
    void Mouse_Pressed();
    // the following signal will keep track of the mouse location.
    void Mouse_Pos();
    void Mouse_Left();


private:   
    // this label will hold the image that the user will upload and edit.
    // note this will hold the image in the that is in the center of the screen. this image is the image on which
    // which we are applying the zoom, rotation, and so on.
    QLabel *editImage;
    // this label will hold the open folder icon (decoration label)
    QLabel *open;
    //this label will also hold the save icon (decoration label)
    QLabel *save;
    // decoration label as well
    QLabel *about;
    //decoration label hold icon of sun
    QLabel *brightness;
    //this label will hold zoom in icon
    QLabel *zoomInIcon;
    QLabel *zoomOutIcon;
    // this label will display the percentage of the zoom level (ie: 100%, 70%)
    QLabel* percentageZoom;
    // the following label will hold the rotate Left icon
    QLabel *rotateLeftIcon;
    // the following label will hold the rotate right icon
    QLabel *rotateRightIcon;
    //will hold palet icon
    QLabel *palet;
    // will hold effect icon. decoration label
    QLabel* effect;
    // will hold undo icon. decoration label
    QLabel* undo;
    // this label will print the word "Light" that's it. it is a decoration label
    QLabel *txtLight;
    // this label will write the name of the slider1, the top one.
    QLabel* writeTxtSlider1;
    QLabel* writeTxtSlider2;
    QLabel* writeTxtSlider3;
    QLabel* writeTxtSlider4;
    QLabel* writeTxtSlider5;

    //the following label poiters will display the numbers when the sliders are scrolled.
    QLabel* displayNumberSlider1;
    QLabel* displayNumberSlider2;
    QLabel* displayNumberSlider3;
    QLabel* displayNumberSlider4;
    QLabel* displayNumberSlider5;


    // the following pointers are for the sliders
    QSlider *sliderBrightness;
    QSlider *sliderContrast;
    QSlider *sliderExposure;
    QSlider *sliderHighLight;
    QSlider *sliderShadows;
    QLCDNumber *numSlider; // box with numbers.    
    // will hold a resized Image
    QImage img2;
    QFrame* w1;
    QFrame* w2;
    QFrame* w3;
    QFrame* w4;
    // this frame will contain the color sliders.
    QFrame* frameColor;
    QGridLayout* gridLayout;
    QHBoxLayout* horizontalLayout;
    // this integer pointer zoom level i will use it to compute and determine the zoom level(ie 20%, 30%, 100%) and so on.
    int zoomLevel;
    // this string zoomLevelString = int zoomLevel. it is a duplicate. Using it for the sake of diplaying
     QString zoomLevelString;

     //mouse position
     int mouseX,mouseY;     



public slots:
    //void addImage(); // add image and return true or false depending on whether or not it added image
    //void calculateBrightness(int factor);

    // change reading zoom level
    // method incharge of changing the zoom in level display (ie:100% 200% ....)
    // method called when zoom button is clicked. note max zoom level is the zoom limit(ie cannot zoom past 1000%)
    // int increaseBy will determine by how much i need I will add the zoom.
     //(ie default zoom = 100% and increaseBy = 20%. new zoom is 120%). Increment Increments zoom by a number entered
    void adjustZoomInDisplay(QLabel* label, int increaseBy, int maxZoomLevel);

    // change reading zoom level
    // method incharge of changing the zoom out level display (ie:100% 75% ....)
    // same logic as adjustZoomInDisplay but it just decrement and cannot decrement past minZoomLevel
    void adjustZoomOutDisplay(QLabel* label, int decrementBy, int minZoomLevel);
};

#endif // INTERFACE_H
