#include "dico_image_triple.h"
#include <iostream>
using namespace std;

Dico_image_triple::Dico_image_triple()
{

}

QImage* Dico_image_triple::find_nearest_image(QColor transfo) //here we will
                       //search the best image to represent the wanted color
{
//    transfo.toHsv();
/*    int Hdiff = 360;
    int diff = 360;
*/

    int totalDiff;
    int diff = 255*3;

    int it = 0;


    for(int i = 0 ; i < Image_Array.size() ; i++)
    {

        /*  A try with hsv but the weighted equation to include saturation and value is hard to manage
         *
        Hdiff = abs(transfo.hsvHue() - Image_Array[i].mean_color_image.hsvHue());
        if(Hdiff > 180)
        {
            Hdiff = abs(Hdiff - 360);
        }

        if(Hdiff < diff)
        {
            diff = Hdiff;
            it = i;
        }*/


        totalDiff = abs(transfo.red() - Image_Array[i].mean_color_image.red())  //absolute diff between the color
                    + abs(transfo.green() - Image_Array[i].mean_color_image.green()) //inside the image and the pixel
                    + abs(transfo.blue() - Image_Array[i].mean_color_image.blue());

        if(totalDiff <= diff)    //we keep the image with image which is the best
        {
            it = i;
            diff = totalDiff;
        }


    }
    return Image_Array[it].candy;

}








