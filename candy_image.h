#ifndef CANDY_IMAGE_H
#define CANDY_IMAGE_H
#include <QImage>
#include <QColor>

class Candy_Image
{
public:
    Candy_Image();
    Candy_Image(QImage* );  //we allow to create an image with a pointer

    ~Candy_Image() { }
    QImage *candy;    //The image of the class
    QColor mean_color_image;    //the mean color of the image
    double price;   //actually I can't register the price of a candy

    void setupColor();  //to find the color of the image

    QColor getMean_color_image() const {return mean_color_image; }              //mutator and accessorfor the color
    void setMean_color_image(const QColor &value) {mean_color_image = value; }

    int getPrice() const { return price; }  //mutator and accessor for the price
    void setPrice(int value) {price = value; }
};

#endif // CANDY_IMAGE_H
