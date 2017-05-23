#include "candy_image.h"
#include <iostream>

using namespace std;

Candy_Image::Candy_Image()
{
    candy = new QImage; //candy is a pointer to a QImage
}

Candy_Image::Candy_Image(QImage* path)
{
    candy = new QImage;     //candy is a pointer to a QImage
    candy = path;           //candy is pointing the entered image
    setupColor();
}

void Candy_Image::setupColor()  //this function will find the mean color of an image
{
        //price = candy_price;
    

        //Here I opted to a classical way to compute it, we could bypass the creation of 3 variables
        //and skip 3 calculus at this end but I think the other calculus could be more long to process.

        int total_red = 0;      //this three variables will contain the amount value of red blue and green of the image
        int total_green = 0;
        int total_blue = 0;

        QColor pixel_color; //we create a QColor which contain the color of the pixel

        for(int i = 0; i < candy->width() ; i++)    //we are going through the width of the image
        {
            for(int j = 0 ; j < candy->height() ; j++)  //we are going through the height of the image
            {
                pixel_color = candy->pixel(i, j);   //pixel color change the QColor
                                                    //We need to pass by a temporary variable because pixel return a
                                                    //qRgb instead of a QColor
                total_red += pixel_color.red();  //we add the value of this pixel to the total
                total_blue += pixel_color.blue();
                total_green += pixel_color.green();
            }
        }
    
        mean_color_image = QColor(total_red / (candy->width() * candy->height()),    //we create a color with the value
                                  total_green / (candy->width() * candy->height()),  //total of each component divided
                                  total_blue / (candy->width() * candy->height()));  //by the size of the image
}
