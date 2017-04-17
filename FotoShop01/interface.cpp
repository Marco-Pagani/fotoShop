#include "interface.h"
#include<QPalette>
#include<QBrush>
#include <QImage>
#include "math.h"
#include<QSizePolicy>



Interface::Interface():QWidget()
{
    //initialze int zoomLevel to 100. the reson why default zoom level is 100%
    zoomLevel = 100;
    //initialize zoomLevelString
    zoomLevelString = "";

    //setting up UI
    this->setFixedSize(1320,800-32);
    this->setStyleSheet(" background-color: rgb(58, 58, 58);");

    open = new QLabel(this);
    open->setCursor(Qt::ClosedHandCursor);
    open->move(10,35);
    open->resize(160,110 - 32);

    save = new QLabel(this);
    save->setCursor(Qt::ClosedHandCursor);
    save->move(10,145 - 32);
    save->resize(160,110);

    about = new QLabel(this);
    about->setCursor(Qt::ClosedHandCursor);
    about->move(10,560 - 32);
    about->resize(150,150);




    //setting up the icons into a lbl. method created by me (look below)
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/open1.png" , open);
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/save.png" , save);
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/About.png" , about);



    // Black frame at bottom
    w1 = new QFrame(this);
    w1->move(0,710 - 32);
    w1->resize(1320,150 - 32);
    w1->setStyleSheet("background-color:rgb(20,37,23);");
    w1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    // the mid frame
    w2 = new QFrame(this);
    w2->move(180,0);
    w2->resize(825,710 - 32);
    w2->setStyleSheet("background-color:rgb(70,70,70);");
    w2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //last frame from the left
    w3 = new QFrame(this);
    w3->move(1005,0);
    w3->resize(200,710 - 32);
    w3->setStyleSheet("background-color:rgb(103,103,103);");
    w3->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //frame that contains "LIGHT" text.
    w4 = new QFrame(this);
    w4->move(1005,35);
    w4->resize(200,100- 32);
    w4->setStyleSheet("background-color:rgb(131,131,131);");
    w4->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


    horizontalLayout = new QHBoxLayout;
    // building the label pointer txtLight and setting text
    txtLight = new QLabel(w4);
    txtLight->setText(" LIGHT");
    txtLight->setStyleSheet("color: rgb(198,198,198)");

    //set font
    QFont font = txtLight->font();
    font.setPointSize(13);
    font.setBold(true);
    txtLight->setFont(font);

    // setting horizontal layout to frame w4
    w4->setLayout(horizontalLayout);
    // add horizontal layout to txtLight
    horizontalLayout->addWidget(txtLight);

    //
    brightness = new QLabel(this);
    brightness->move(1205,195 - 32);
    brightness->resize(120,120 - 32);

    // change background color of brightness label
    brightness->setStyleSheet("QLabel { background-color : rgb(103,103,103); }");

    // I am setting the sun icon onto the label brightness
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/sun.png" , brightness);

    //
    palet = new QLabel(this);
    palet->move(1205,320 - 32);
    palet->resize(120,120 - 32);

    //
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/palette.png" , palet);

    //constructing label zoomInIcon pointer. placing it in proper location
    zoomInIcon = new QLabel(this);
    zoomInIcon->move(210,730 - 48);
    zoomInIcon->resize(50,50 - 12);
    // changing background color
    zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");


    //method declared below. Will add Icon to lbl given a path.
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/zoomIn.png",zoomInIcon);

    //contructing label zoom out pointer. placing it in proper location
    zoomOutIcon = new QLabel(this);
    zoomOutIcon->move(280,730 - 48);
    zoomOutIcon ->resize(50,50 - 12);
    // changing background color
    zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");

    // method declare below add icon to label
    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/zoomOut.png",zoomOutIcon);

    // initialize the label pointer that will display the zoom percentage. and set it's default zoom percentage to 100%
    percentageZoom = new QLabel(this);
    percentageZoom->move(350,730 - 32);
    percentageZoom->resize(100,50 - 32);
    //set font percentage zoom
    QFont font1 = percentageZoom->font();
    font1.setPointSize(10);
    font1.setBold(true);
    percentageZoom->setFont(font1);
    percentageZoom->setText("100%");
    // changing background color
    percentageZoom->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");

    // intializing rotate left Qlabel and placing it into the frame
    rotateLeftIcon = new QLabel(this);
    rotateLeftIcon->move(475,730 - 48);
    rotateLeftIcon->resize(100,50 - 12);
    rotateLeftIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23);}");

    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/rotateLeft.png",rotateLeftIcon);

    // intializing rotate right Qlabel and placing it into the frame
    rotateRightIcon = new QLabel(this);
    rotateRightIcon->move(545,730 - 48);
    rotateRightIcon->resize(100,50 - 12);
    rotateRightIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23);}");

    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/rotateRight.png",rotateRightIcon);

    // intializing undo Qlabel and placing it into the frame
    undo = new QLabel(this);
    undo->move(875,720 - 38);
    undo->resize(215,70 - 32);
    undo->setStyleSheet("QLabel { background-color : rgb(20,37,23);}");

    addIconToLabel("/home/josh/fotoShop term project/FotoShop01/undo.png",undo);

    //Image loaded by user located in the middle of the frame. This is label will contain the image we
    //will edit
    editImage = new QLabel(this);
    editImage->move(228,20);
    editImage->resize(725,640 - 32);
    editImage->setStyleSheet("QLabel { background-color:rgb(70,70,70); color: white;}");


    // by default i want to display the light sliders because it is the window opened by default
    setUpLightSliders();

    // construct pointer slider1
    writeTxtSlider1 = new QLabel(this);
    writeTxtSlider1->move(1025,160 - 32);
    writeTxtSlider1->resize(150,45);
    writeTxtSlider1->setStyleSheet("background-color:rgb(103,103,103); color: white;");
    writeTxtSlider1->setText("Brightness");
    QFont fontwriteTxtSlider1 = writeTxtSlider1->font();
    fontwriteTxtSlider1.setPointSize(10);
    fontwriteTxtSlider1.setBold(true);
    writeTxtSlider1->setFont(fontwriteTxtSlider1);

    //construct pointer label slider2. decoration label. Set location.
    writeTxtSlider2 = new QLabel(this);
    writeTxtSlider2->move(1025,260 - 32);
    writeTxtSlider2->resize(150,45);
    writeTxtSlider2->setStyleSheet("background-color:rgb(103,103,103); color: white;");
    writeTxtSlider2->setText("Contrast");
    QFont fontwriteTxtSlider2 = writeTxtSlider2->font();
    fontwriteTxtSlider2.setPointSize(10);
    fontwriteTxtSlider2.setBold(true);
    writeTxtSlider2->setFont(fontwriteTxtSlider2);

    //construct pointer label slider3. decoration label with text and set location.
    writeTxtSlider3 = new QLabel(this);
    writeTxtSlider3->move(1025,360 - 32);
    writeTxtSlider3->resize(150,45);
    writeTxtSlider3->setStyleSheet("background-color:rgb(103,103,103); color: white;");
    writeTxtSlider3->setText("Exposure");
    QFont fontwriteTxtSlider3 = writeTxtSlider3->font();
    fontwriteTxtSlider3.setPointSize(10);
    fontwriteTxtSlider3.setBold(true);
    writeTxtSlider3->setFont(fontwriteTxtSlider3);

    //construct pointer label slider4. decoration label with text and set location.
    writeTxtSlider4 = new QLabel(this);
    writeTxtSlider4->move(1025,460 - 32);
    writeTxtSlider4->resize(150,45);
    writeTxtSlider4->setStyleSheet("background-color:rgb(103,103,103); color: white;");
    writeTxtSlider4->setText("Highlights");
    QFont fontwriteTxtSlider4 = writeTxtSlider4->font();
    fontwriteTxtSlider4.setPointSize(10);
    fontwriteTxtSlider4.setBold(true);
    writeTxtSlider4->setFont(fontwriteTxtSlider4);

    //construct pointer label slider4. decoration label with text and set location.
    writeTxtSlider5 = new QLabel(this);
    writeTxtSlider5->move(1025,560 - 32);
    writeTxtSlider5->resize(150,45);
    writeTxtSlider5->setStyleSheet("background-color:rgb(103,103,103); color: white;");
    writeTxtSlider5->setText("Shadows");
    QFont fontwriteTxtSlider5 = writeTxtSlider5->font();
    fontwriteTxtSlider5.setPointSize(10);
    fontwriteTxtSlider5.setBold(true);
    writeTxtSlider5->setFont(fontwriteTxtSlider5);



}

void Interface::resetAllSliders(){
    sliderBrightness->setValue(0);
    sliderContrast->setValue(0);
    sliderExposure->setValue(0);
    sliderHighLight->setValue(0);
    sliderShadows->setValue(0);
}

void Interface::showSliderLights(){
    sliderBrightness->setVisible(true);

    sliderContrast->setVisible(true);

    sliderExposure->setVisible(true);

    sliderHighLight->setVisible(true);

    sliderShadows->setVisible(true);

}

void Interface::setRangeBrightnessSliders(){
    sliderShadows->setRange(-100,100);
    sliderBrightness->setRange(-100,100);
    sliderHighLight->setRange(-100,100);
    sliderExposure->setRange(-100,100);
    sliderContrast->setRange(-255,255);
}

void Interface::setAllSlidersVisible(){
    sliderShadows->setVisible(true);
    sliderBrightness->setVisible(true);
    sliderHighLight->setVisible(true);
    sliderContrast->setVisible(true);
    sliderExposure->setVisible(true);

}

void Interface::hideAllSliders(){
    sliderShadows->setVisible(false);
    sliderBrightness->setVisible(false);
    sliderHighLight->setVisible(false);
    sliderContrast->setVisible(false);
    sliderExposure->setVisible(false);
}

void Interface::setRangeColorSliders(){

    sliderContrast->setVisible(true);
    sliderBrightness->setVisible(true);
    sliderExposure->setVisible(true);

    sliderBrightness->setRange(-359,359);
    sliderContrast->setRange(-100,100);
    sliderExposure->setRange(-255,255);

}

void Interface::hideSliderLights(){
    sliderBrightness->setVisible(false);
    sliderContrast->setVisible(false);
    sliderExposure->setVisible(false);
    sliderHighLight->setVisible(false);
    sliderShadows->setVisible(false);
}

void Interface::setUpLightSliders(){
    sliderBrightness = new QSlider(Qt::Horizontal,this);
    sliderBrightness->move(1025,210 - 32);
    sliderBrightness->resize(150,50);
    sliderBrightness->setStyleSheet("background-color:rgb(103,103,103);");

    sliderContrast = new QSlider(Qt::Horizontal,this);
    sliderContrast->move(1025,310 - 32);
    sliderContrast->resize(150,50);
    sliderContrast->setStyleSheet("background-color:rgb(103,103,103);");

    sliderExposure = new QSlider(Qt::Horizontal,this);
    sliderExposure->move(1025,410 - 32);
    sliderExposure->resize(150,50);
    sliderExposure->setStyleSheet("background-color:rgb(103,103,103);");

    sliderHighLight = new QSlider(Qt::Horizontal,this);
    sliderHighLight->move(1025,510 - 32);
    sliderHighLight->resize(150,50);
    sliderHighLight->setStyleSheet("background-color:rgb(103,103,103);");

    sliderShadows = new QSlider(Qt::Horizontal,this);
    sliderShadows->move(1025,610 - 32);
    sliderShadows->resize(150,50);
    sliderShadows->setStyleSheet("background-color:rgb(103,103,103);");

    setRangeBrightnessSliders();

    // Initialize the label pointers that will display the numbers when the sliders are scrolled.
    // contructing the label and than assigning it a location
    displayNumberSlider1 = new QLCDNumber(this);
    displayNumberSlider1->move(1180, 240 - 32);
    displayNumberSlider1->resize(70,40);
    displayNumberSlider1->setStyleSheet("background-color:rgb(103,103,103); color: white;");


    displayNumberSlider2 = new QLCDNumber(this);
    displayNumberSlider2->move(1180,340 - 32);
    displayNumberSlider2->resize(70,40);
    displayNumberSlider2->setStyleSheet("background-color:rgb(103,103,103); color: white;");


    displayNumberSlider3 = new QLCDNumber(this);
    displayNumberSlider3->move(1180,440 - 32);
    displayNumberSlider3->resize(70,40);
    displayNumberSlider3->setStyleSheet("background-color:rgb(103,103,103); color: white;");


    displayNumberSlider4 = new QLCDNumber(this);
    displayNumberSlider4->move(1180,540 - 32);
    displayNumberSlider4->resize(70,40);
    displayNumberSlider4->setStyleSheet("background-color:rgb(103,103,103); color: white;");

    displayNumberSlider5 = new QLCDNumber(this);
    displayNumberSlider5->move(1180,640 - 32);
    displayNumberSlider5->resize(70,40);
    displayNumberSlider5->setStyleSheet("background-color:rgb(103,103,103); color: white;");


    QApplication::connect(sliderBrightness,SIGNAL(valueChanged(int)), displayNumberSlider1,SLOT(display(int)));
    QApplication::connect(sliderContrast,SIGNAL(valueChanged(int)), displayNumberSlider2,SLOT(display(int)));
     QApplication::connect(sliderExposure,SIGNAL(valueChanged(int)), displayNumberSlider3,SLOT(display(int)));
      QApplication::connect(sliderHighLight,SIGNAL(valueChanged(int)), displayNumberSlider4,SLOT(display(int)));
       QApplication::connect(sliderShadows,SIGNAL(valueChanged(int)), displayNumberSlider5,SLOT(display(int)));


       // i am setting this bool to true because initially i will display the light window
       lightButtonClicked = true;


}

void Interface::writeImage(){

    if(lightButtonClicked == true){
        // populate brightness, contrast, exposure, heighlight, and shadow. with QLCD box.
        intBrightness = displayNumberSlider1->intValue();
        intContrast = displayNumberSlider2->intValue();
        intExposure = displayNumberSlider3->intValue();
        intHighlight = displayNumberSlider4->intValue();
        intShadow = displayNumberSlider5->intValue();

        changeBrightness(intBrightness);
        changeContrast(intContrast);
        changeExposure(intExposure);
        changeHighlights(intHighlight);
        changeShadows(intShadow);
    }


    if(lightButtonClicked == false){
        intHue = displayNumberSlider1->intValue();
        intSaturation = displayNumberSlider2->intValue();
        intTemperature = displayNumberSlider3->intValue();

        changeHue(intHue);
        changeSaturation(intSaturation);
        changeTemperature(intTemperature);
    }






    // need to create the methods for the remaining of the methods(ie highlight, shadow...)

    writePNGFile();


    // path(Qstring) == fileName (string)
    // display the image
    QImage img4(fileName);
    editImage->setPixmap(QPixmap::fromImage(img4));

  //  read_png_file();

    // reset all the slider to zero
    sliderBrightness->setValue(0);
    sliderContrast->setValue(0);
    sliderExposure->setValue(0);
    sliderHighLight->setValue(0);
    sliderShadows->setValue(0);


}

void Interface::displayQLCDnumbers(){
    displayNumberSlider1->setVisible(true);
}

void Interface::loadImage()
{
    // exception handling. if file loaded is not a JPG or png image.
    try
    {
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.jpg)"));
        QImage img(fileName);
        img2 = img.scaled(editImage->width(), editImage->height(), Qt::KeepAspectRatio);
        editImage->setPixmap(QPixmap::fromImage(img2));

        // reset default boder color of label open (to make it blend with the interface).
        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        if(!img2.isNull()){
            path = fileName.toUtf8().constData();
            set_char(path);
            readPNGFile();
        }




    }
    catch(...)
    {
        QMessageBox::critical(this,"Error", "The file selected is not an image. Please select an image");


    }
}

// it take xPosition which is the x position of the mouse, and yPosition which is the y position of the mouse
bool Interface::inLabel(QLabel* label, int xPositionMouse, int yPositionMouse){
   // keeping track of the start and end point of the label horizontaly
    int startXPosition;
    int endXPosition;

    // keeping track of the start and end point of the label verticaly.
    int startYPosition;
    int endYPosition;

    startXPosition = label->x();
    endXPosition = startXPosition + label->width();

    startYPosition = label->y();
    endYPosition = startYPosition + label->height();

    if(xPositionMouse >= startXPosition && xPositionMouse <= endXPosition &&
            yPositionMouse >= startYPosition && yPositionMouse <= endYPosition)
    {
        return true;
    }
    else{
        return false;
    }


}

// this function will add an icon  to a label. It takes a label pointer and a QString fileName which
// is the path to the file.
void Interface::addIconToLabel(QString fileName,QLabel* label)
{

    QImage img(fileName);
    img = img.scaled(label->width(), label->height(), Qt::KeepAspectRatio);
    label->setPixmap(QPixmap::fromImage(img));
}

// if mouse is pressed i perform the necessary actions here.
void Interface::mousePressEvent(QMouseEvent *ev)
{
    this->mouseX = ev ->x();
    this->mouseY = ev->y();
    emit Mouse_Pressed();
    // checks if mouse is in desired location and return a boolean.if yes perform action below(methode declared above)
    if(inLabel(zoomInIcon,mouseX, mouseY) ==  true){
        // change reading zoom level
        adjustZoomInDisplay(zoomInIcon,50,300);
        resetAllLabel("zoomInIcon");

    }
    // checks if mouse is in desired location and return a boolean.if yes perform action below(methode declared above)
    else if(inLabel(zoomOutIcon,mouseX,mouseY) == true){
        // change reading zoom level
        adjustZoomOutDisplay(zoomOutIcon,50,-100);
        resetAllLabel("zoomOutIcon");

    }
    // checks if mouse is in desired location and return a boolean.if yes perform action below(methode declared above)
    else if(inLabel(open,mouseX,mouseY) == true){
        open->setStyleSheet("border: 2px solid white");
        resetAllLabel("open");
        loadImage();

    }
    // checks if mouse is in desired location and return a boolean.if yes perform action below(methode declared above)
    else if(inLabel(brightness,mouseX,mouseY)){
        lightButtonClicked = true;
        hideAllSliders();
        resetAllSliders();
        setRangeBrightnessSliders();
        writeTxtSlider1->setText("Brightness");
        writeTxtSlider2->setText("Contrast");
        writeTxtSlider3->setText("Exposure");
        writeTxtSlider4->setText("Highlight");
        writeTxtSlider5->setText("Shadows");
        brightness->setStyleSheet("QLabel { background-color : rgb(103,103,103); }");
        resetAllLabel("brightness");
        txtLight->setText("Light");
        showSliderLights();

    }
    // checks if mouse is in desired location and return a boolean.if yes perform action below(methode declared above)
    else if(inLabel(palet,mouseX,mouseY)){
        lightButtonClicked = false;
        hideAllSliders();
        resetAllSliders();
        setRangeColorSliders();
        writeTxtSlider1->setText("Hue");
        writeTxtSlider2->setText("Saturation");
        writeTxtSlider3->setText("warmth");
        writeTxtSlider4->setText("");
        writeTxtSlider5->setText("");
        palet->setStyleSheet("QLabel { background-color : rgb(103,103,103); }");
        txtLight->setText("Color");
        resetAllLabel("palet");

    }

    // the undo button
    else if(inLabel(undo,mouseX,mouseY)){
        writeImage();
        undo->setStyleSheet("border: 2px solid rgb(58, 58, 58)");
        resetAllLabel("undo");
    }

    // reset All labels background color and border to default color
    else{


        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        //brightness->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        //brightness->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        //palet->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
       // palet->setStyleSheet("border: 2px solid rgb(58, 58, 58)");
/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

*/


    }
}

// this will keep track of mouse position
void Interface::mouseMoveEvent(QMouseEvent *ev)
{
    // keep track of mouse position
    this->mouseX = ev ->x();
    this->mouseY = ev->y();
    emit Mouse_Pos();
}

void Interface::leaveEvent()
{
    emit Mouse_Left();
}

// method incharge of changing the zoom in level display (ie:100% 200% ....)
// method called when zoom button is clicked. note max zoom level is the zoom limit(ie cannot zoom past 1000%)
// int increaseBy will determine by how much i need I will add the zoom.(ie increase = 20%, new zoom is 120%)
void Interface::adjustZoomInDisplay(QLabel* label, int increaseBy, int maxZoomLevel){

    resetAllLabel("zoomOutIcon");

    zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
    zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

    label->setStyleSheet("QLabel { background-color : white; color : white; }");
    // set border line for label
    label->setStyleSheet("border: 2px solid white");

    // this if condition checks if the zoom level is less than the maximum zoom level allowed
    // note zoom level is by default 100%.For example when ever this method is called i will increase zoom level by 25%
    if(zoomLevel < maxZoomLevel){
        zoomLevel += increaseBy;
        //convert int to string
        zoomLevelString = QString::number(zoomLevel);
        //update zoom display
        percentageZoom->setText(zoomLevelString + "%");

    }
    else{
        QMessageBox::critical(NULL,"Error", "Max zoom level reached. Cannot maximize image any further");

    }

}

//reset all border and back-ground color of labels except one label (QString in use).
// this method allows me to reset all label to they original color except one label.
// and the Qstring InUse allows me to know which label I do not want to reset (its background color and border).
void Interface::resetAllLabel(QString inUse){

    // for example this will set default back-ground color of all labels except zoomOutIcon label pointer

    if(inUse == "zoomOutIcon"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

       /*

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
*/

    }
    else if(inUse == "zoomInIcon"){
        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");


/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
*/
    }

    else if(inUse == "open"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");


        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");


/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
        */
    }
    else if(inUse == "save"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");


/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
  */
    }

    else if(inUse == "about"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");


/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
  */
    }

    else if(inUse == "brightness"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        palet->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        palet->setStyleSheet("border: 2px solid rgb(58, 58, 58)");
/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
  */
    }

    else if(inUse == "palet"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        brightness->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        brightness->setStyleSheet("border: 2px solid rgb(58, 58, 58)");
/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
 */
    }

    else if(inUse == "effect"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        brightness->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        brightness->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        palet->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        palet->setStyleSheet("border: 2px solid rgb(58, 58, 58)");
/*
        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
  */
    }

    else if(inUse == "rotateLeftIcon"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");


/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateRightIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");
 */
    }

    else if(inUse == "rotateRightIcon"){
        zoomInIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomInIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        zoomOutIcon->setStyleSheet("QLabel { background-color : rgb(20,37,23); color : white; }");
        zoomOutIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

        open->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        save->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        about->setStyleSheet("border: 2px solid rgb(58, 58, 58)");


/*
        effect->setStyleSheet("QLabel { background-color : rgb(58, 58, 58); }");
        effect->setStyleSheet("border: 2px solid rgb(58, 58, 58)");

        rotateLeftIcon->setStyleSheet("border: 2px solid rgb(20,37,23)");

   */
    }

}

void Interface::adjustZoomOutDisplay(QLabel* label, int decrementBy, int minZoomLevel){

    resetAllLabel("zoomOutIcon");

    label->setStyleSheet("QLabel { background-color : white; color : white; }");
    // set border line for label
    label->setStyleSheet("border: 2px solid white");

    // this if condition checks if the zoom level is greater than the minimum zoom level allowed
    // note zoom level is by default 100%.for example when ever this method is called i will decrease zoom level by 25%
    if(zoomLevel > minZoomLevel){
        zoomLevel -= decrementBy;
        //convert int to string
        zoomLevelString = QString::number(zoomLevel);
        //update zoom display
        percentageZoom->setText(zoomLevelString + "%");
    }
    else{
        QMessageBox::critical(NULL,"Error", "Minimum zoom level reached. Cannot reduce image any further");
    }

}

