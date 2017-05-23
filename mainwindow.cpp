#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dico_image_triple.h"

#include <QFileDialog>
#include <QDirIterator>

#include <cassert>

#include <QImage>
#include <QColor>
#include <iostream>

#include <QPaintEvent>
#include <QPainter>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    paint = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_Load_clicked()
{
    ui->label_path_image->clear();
    QString image_user = QFileDialog::getOpenFileName(this,("Selectionner une image à CandyPixeliser"),"", ("Images(*.png *jpg *jpeg)"));
    //Fonction qui recherche les fichier (avec "this" obligatoire / tr("..")= Nom de la fenetre / ""=Repertoire préprogrammé (pas dans notre cas / tr("..("..)")= Types de fichiers (Une image de ce types de formats dans notre cas)

    if (QString::compare(image_user, QString()) != 0){ //Si un fichier (image) à été selectionné

        origin_image .load(image_user); //Enregistre l'image dans la variable "image" et permet de valider si l'image a ete chargé avec succes
        origin_image = origin_image .scaled( ui->label_image_container->size(), Qt::KeepAspectRatio, Qt::FastTransformation); //Permet d'aficher l'image de la taille du label sans changer son ratio
    }   //Fonction permettant de mettre l'image  dans le label

    ui -> label_image_container -> setPixmap(QPixmap::fromImage(origin_image));       // afficher l'image entrée par l'utilisateur
    ui->button_pixelise->setEnabled(true);      //we allow to pixelise

    ui->Label_maxX->setText(QString::number(origin_image.width()) + " pixels max on X");    //we display the size of the
    ui->Label_maxY->setText(QString::number(origin_image.height()) + " pixels max on Y");   //image

    ui->XPixel_new->setMaximum(origin_image.width());   //we change the maximum value of QSpinBox
    ui->YPixel_new->setMaximum(origin_image.height());

    ui->XPixel_size->setMaximum(origin_image.width());
    ui->YPixel_size->setMaximum(origin_image.height());

    ui->XPixel_new->setValue(origin_image.width()); //we change the value of the number of pixel groups
    ui->YPixel_new->setValue(origin_image.height());
}


bool MainWindow :: LoadAllPixmaps (const QDir &DirPath, bool verbose) {

    //this function is almost your functions to load all pixmap, I just did some little changes to adapt
    //it on my software

    Dico_image.Image_Array.resize(0);
    Dico_image.Image_Array.clear();

    if (verbose) {
        cout << "FILES LOADED FROM " << DirPath.path().toStdString() << endl;
        cout << " MinesweeperGame ::  LoadAllPixmaps..." << endl;
    }

    //assert the directory and all the contained files
    assert( DirPath.exists() );

    //we now are sure that the Directory exists, so we are going to explore it

    bool ok = true;
    QDirIterator it( DirPath );//, QDirIterator::Subdirectories);

    //list all the elements in the directory
    while ( ok ) {

       //if (verbose) cout <<"\t CURRENTLY TRYING TO LOAD: "<<it.fileName().toStdString()<<endl;

       //if we encounter a file with the appropriate extension
       if ( it.fileInfo().suffix().toStdString() == "png" )
        {
            if (verbose) cout <<"\t processing "<<it.fileName().toStdString()<<endl;

            QImage *ptr = new QImage;

            //QPixmap *ptr = new QPixmap;
            assert (ptr->load(it.filePath()) == true);

            Dico_image.Image_Array.push_back(ptr); //we add the pointer to the image library
        }

        if (it.hasNext()) it.next();
        else ok = false;
    }

   //when this line is reached, all the data have been properly loaded and we have no error
   cout<<Dico_image.Image_Array.size()<<endl;
   return false;
}



void MainWindow::on_button_pixelise_clicked()
{

    ui->button_Save->setEnabled(true);  //we set enable the save button
    ui->button_pixelise->setEnabled(false); //we disable the pixelise button

    pixel_matrix.clear();   // we clear the matrix to be sure that no data would disrubt
    pixel_matrix.resize(origin_image.width(), vector<pixel>(origin_image.height()));   //we resize the bidimensional matrix

    for(int i = 0; i < origin_image.width(); i++)
    {   //we go through the image
        for(int j = 0; j < origin_image.height(); j++)
        {
            pixel_matrix[i][j].setColor(origin_image.pixel(i, j));
            //we set the matrix cell to the color of the image pixel
        }
    }
    find_color();   //we will find the mean color of big pixels
}

void MainWindow::transform_pixel()  //repaint the window
{

    if(ui->radioButton_noImage->isChecked() == true)    //we are checking the radio button used
    {
        paint = false;  //if we don't need to repaint except the label
        ui -> label_image_pixel -> setPixmap(QPixmap::fromImage(new_image));  //we load the image inside the label
    }
    else    //if we need to repaint the window
    {
        paint = true;
        repaint();  //we repaint the window
    }
}

void MainWindow::find_color()   //this function will find the mean color of each big pixels
{
    new_image = origin_image;   //we copy the original image to be sure to not modify it

    QColor new_color; //we create a new color to contain the value inside a pixel

    //here I think I can optimize this part for the number of actions done but i'm not sure i will dicrease
    //the complexity of this part

    int mean_red; //this variables will contain the value of the color component mean color
    int mean_blue;
    int mean_green;

    int total_red; //this variables will contain the amount of each color component found inside a group of pixels
    int total_blue;
    int total_green;

    int it; //this integer will count the number of pixels inside a group

    for(int i = 0 ; i < origin_image.width() ; i++) //we will go through the image
    {
        for(int j = 0 ; j < origin_image.height() ; j++)
        {
            if( pixel_matrix[i][j].getBool() == false ) //if the pixel hadn't been processed
            {
                total_red = 0;  //we reset the value of the amount of color
                total_blue = 0;
                total_green = 0;

                it = 0; //we reset the number of pixels done

                for(int k = 0 ; k < ui->XPixel_size->text().toInt() ; k++)
                {   //we are processing "big pixel"

                    for(int l = 0 ; l < ui->YPixel_size->text().toInt() ; l++)
                    {

                        if(i+k < origin_image.width() && j+l < origin_image.height())
                        {   //if we are still inside the vector

                            new_color = pixel_matrix[i+k][j+l].getColor(); //we copy the color inside the QColor

                            total_red += new_color.red();   //we add each component to the total of a big pixel
                            total_blue += new_color.blue();
                            total_green += new_color.green();

                            it += 1;    //we increment the count of pixel done

                            pixel_matrix[i+k][j+l].setBool(true); //we set the boolean to true when we are done with it
                        }
                    }
                }

                mean_red = total_red / it;  //we compute the mean value of the color
                mean_blue = total_blue / it;
                mean_green = total_green / it;

                for(int k = 0 ; k < ui->XPixel_size->text().toInt() ; k++)
                {
                    //We are going again through the groupe of pixel

                    for(int l = 0 ; l < ui->YPixel_size->text().toInt() ; l++)
                    {
                        if(i+k < origin_image.width() && j+l < origin_image.height())
                        {
                            //if we are staying inside the vector

                            pixel_matrix[i+k][j+l].setColor(QColor(mean_red, mean_green, mean_blue)); //the pixel take the new color
                            QColor color (mean_red, mean_green, mean_blue); //we are creating a color with the 3 components we calculate before
                            new_image.setPixel(i+k, j+l, color.rgba()); //we are changing the new image pixels
                            //new_image.setPixelColor( i+k, j+l, pixel_matrix[i][j].getColor()); //depending on Qt version
                            //we could need to comment the line before and uncomment this one
                        }
                    }
                }
            }
        }
    }
    transform_pixel(); // we call the function to change the display of the window
}

void MainWindow::paintEvent(QPaintEvent *)  //the paint function
{
    if(paint == true)   //if we need to repaint the window
    {
        QPainter painter(this); //we create our tool to paint

        int sizeX = ui->XPixel_size->value(); //we give the size of big pixel wanted
        int sizeY = ui->YPixel_size->value();

        QImage *image = new QImage; //we create a pointer to an image
        QRectF image_space(0 , 0 , image->width() , image->height());   //I take the full image
        QRectF rect; //I prepare a rectangular space

        for(int i = 0; i < ui->XPixel_new->value() ; i++)
        {
            //the loop will go during the number of big pixels
            for(int j = 0; j < ui->YPixel_new ->value(); j++)
            {

                QColor new_color = pixel_matrix[i * sizeX][j * sizeY].getColor(); //I get the color of the pixel

                image = Dico_image.find_nearest_image(new_color); //I'm using the function to point to the best image
                rect = QRectF(i * sizeX + 10 , j * sizeY + 391 , sizeX , sizeY);
                //the zone is pointing to the area of the big pixel

                painter.drawImage(rect, *image, image_space);
                // I draw the best images possible inside the space wanted

            }
        }
    }
    paint = false; //I block the repaint
}


void MainWindow::on_Button_load_images_clicked()
{
    //this event load every images depending of the choice we do

    bool ok = true;
    if(ui->radioButton_custom->isChecked() == true) //if we want a custom directory of pictures
    {
        //this function had been taken on Ellias project

        image_user = QFileDialog::getExistingDirectory(this,("Select an image folder "),"");
    //Fonction qui recherche les fichier (avec "this" obligatoire / tr("..")= Nom de la fenetre / ""=Repertoire préprogrammé (pas dans notre cas / tr("..("..)")= Types de fichiers (Une image de ce types de formats dans notre cas)
        ok = LoadAllPixmaps(image_user);
    }
    else if(ui->radioButton_standard->isChecked() == true)//if we want to use the standart image library
    {


        //image_user = QDir("../Image_Library/"); Not working

        //here we don't need to search our directory
        image_user = QDir("C:/Users/Romain/Desktop/CandyXel/Image_Library");    //our directory
        if(image_user.exists() == true) //if the folder exist
        {
            ok = LoadAllPixmaps(image_user); //we load all images inside the folder
        }
    }
}


void MainWindow::on_button_Save_clicked() //here we save the image
{   //I took this function on Ellias project, I modified the window grab to adapt it


    QPixmap pixmap = QPixmap(); //we create an image
    pixmap = QPixmap::grabWindow(QWidget::winId(), 10, 391,
                                 ui->XPixel_new->value() * ui->XPixel_size->value(),
                                 ui->YPixel_new->value() * ui->YPixel_size->value());
    //the image will be a copy of the display part we want, here the pixelised part

    //to save the picture
    QString format = "png"; //we decide the format of the image
    QString filePath = QFileDialog::getSaveFileName(this,("Save File"),"new image",("Images(*.png)"));
    //we take the path and the name of the pixmap image

    pixmap.save(filePath, format. toLatin1()); //we save the image using the path and format
    repaint();
}


//--------------------------------------------------------------------------

//this part is almost recurrent so I will explain only one each function

void MainWindow::on_XPixel_new_editingFinished()        //when we change the value by hand
{
    ui->button_pixelise->setEnabled(true);              //we permit the user to pixelise the image
    int X_Big_Pixel = ui->XPixel_new->text().toInt();   //we set the variable to the value of the number of big pixel wanted
    ui->XPixel_size->setValue(origin_image.width() / X_Big_Pixel); //we change the value of the size of pixels
}

void MainWindow::on_XPixel_new_valueChanged()   //when we change the value using arrows
{
    ui->button_pixelise->setEnabled(true);
    ui->XPixel_size->setValue(origin_image.width() / ui->XPixel_new->text().toInt());
}

void MainWindow::on_YPixel_new_editingFinished()
{
    ui->button_pixelise->setEnabled(true);
    ui->YPixel_size->setValue(origin_image.height() / ui->YPixel_new->text().toInt());
}

void MainWindow::on_YPixel_new_valueChanged()
{
    ui->button_pixelise->setEnabled(true);
    ui->YPixel_size->setValue(origin_image.height() / ui->YPixel_new->text().toInt());
}

//--------------------------------------------------------------------------

void MainWindow::on_XPixel_size_editingFinished()
{
    ui->button_pixelise->setEnabled(true);
    ui->XPixel_new->setValue(origin_image.width() / ui->XPixel_size->text().toInt());
}

void MainWindow::on_XPixel_size_valueChanged()
{
    ui->button_pixelise->setEnabled(true);
    ui->XPixel_new->setValue(origin_image.width() / ui->XPixel_size->text().toInt());
}

void MainWindow::on_YPixel_size_editingFinished()
{
    ui->button_pixelise->setEnabled(true);
    ui->YPixel_new->setValue(origin_image.height() / ui->YPixel_size->text().toInt());
}

void MainWindow::on_YPixel_size_valueChanged()
{
    ui->button_pixelise->setEnabled(true);
    ui->YPixel_new->setValue(origin_image.height() / ui->YPixel_size->text().toInt());
}

//-------------------------------------------------------------------------------

//if we touch a button we change the type of pixelisation we want, so we allow the user to pixelise again
void MainWindow::on_radioButton_standard_clicked()
{
    ui->button_pixelise->setEnabled(true); //we let the user pixelise the image
}

void MainWindow::on_radioButton_custom_clicked()
{
    ui->button_pixelise->setEnabled(true);
}

void MainWindow::on_radioButton_noImage_clicked()
{
    ui->button_pixelise->setEnabled(true);
}
