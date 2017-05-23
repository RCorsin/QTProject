#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dico_image_triple.h"
#include <QColor>
#include <QDir>
#include <QPixmap>
#include "pixel.h"


using namespace std;


#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void paintEvent(QPaintEvent *); //the function which will paint on the window

    ~MainWindow();

private slots:              //every widget events
    void on_Button_Load_clicked();
    void on_button_Save_clicked();

    void on_button_pixelise_clicked();

    void on_XPixel_new_editingFinished();
    void on_XPixel_new_valueChanged();

    void on_YPixel_new_editingFinished();
    void on_YPixel_new_valueChanged();

    void on_XPixel_size_editingFinished();
    void on_XPixel_size_valueChanged();

    void on_YPixel_size_editingFinished();
    void on_YPixel_size_valueChanged();

    void on_Button_load_images_clicked();

    void on_radioButton_standard_clicked();

    void on_radioButton_custom_clicked();

    void on_radioButton_noImage_clicked();

private:
    Ui::MainWindow *ui;

    QDir image_user;    //here we create a directory path

    QImage origin_image;    //the image loaded, we will never change this one
    QImage new_image;       //this image is the changed one, we will modify the color of pixels

    void find_color();      //the function used to modify the color of the new image
    void transform_pixel(); //this function will be used to modify the rendering, to show and image or to display inside the label


    std::vector < std::vector < pixel > > pixel_matrix; //this is a vector of pixels class
    Dico_image_triple Dico_image;   //this variable will contain our images

    bool paint; //this variable will be used to repaint or not our windows

    bool LoadAllPixmaps ( const QDir& = QDir("Image_Library"), bool verbose = true);    //this function will load
            //every pixmap inside a folder

};

#endif // MAINWINDOW_H
