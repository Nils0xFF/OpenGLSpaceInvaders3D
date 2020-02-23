#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include <math.h>

RGBImage::RGBImage( unsigned int Width, unsigned int Height):m_Width(Width),m_Height(Height)
{
    // TODO: add your code
	this->m_Image = new Color[Height * Width];
	// std::cout << "Size: " << sizeof(Color) * Height * Width << std::endl;
}

RGBImage::~RGBImage()
{
    delete[] this->m_Image;
	this->m_Image = NULL;
}

void RGBImage::setPixelColor( unsigned int x, unsigned int y, const Color& c)
{
    // TODO: add your code
    if(x < this->m_Width && y < this->m_Height){
        this->m_Image[y * this->m_Width  + x] = c;
    }
}

const Color& RGBImage::getPixelColor( unsigned int x, unsigned int y) const
{
    x < 0 ? x = 0 : x = x % this->m_Width;
    y < 0 ? y = 0 : y = y % this->m_Height;
    return this->m_Image[y * this->m_Width  + x];
}

unsigned int RGBImage::width() const
{
    return this->m_Width;
}
unsigned int RGBImage::height() const
{
    return m_Height;
}

unsigned char RGBImage::convertColorChannel( float v)
{
    if(v >= 1.0f) return 255;
    if(v <= 0) return 0;
    return (unsigned char)(v * 255);
}

// Quelle: https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
bool RGBImage::saveToDisk( const char* Filename)
{
    
    // TRY TO CREATE FILE
    FILE *f = fopen(Filename,"wb");
    if(f == NULL){
        return false;
    }
    
    unsigned char *img = NULL;
    
    // 54 Byte Header + heigth * width pixels with 3 Byte each
    int filesize = 54 + 3 * this->m_Height * this->m_Width;
    
    // allocate memory for the imageData 3Bytes per Pixel x width x height
    img = (unsigned char *)malloc(3*this->m_Width*this->m_Height);
    
    if(img == NULL) return false;
    
    memset(img,0,3*this->m_Width*this->m_Height);
    
    // 24bpp -> reverse RBG
    for(int unsigned y=0; y<this->m_Height; y++)
    {
        for(int unsigned x=0; x<this->m_Width; x++)
        {
            img[(x+y*this->m_Width)*3+2] = convertColorChannel(this->m_Image[y * this->m_Width  + x].R);
            img[(x+y*this->m_Width)*3+1] = convertColorChannel(this->m_Image[y * this->m_Width  + x].G);
            img[(x+y*this->m_Width)*3+0] = convertColorChannel(this->m_Image[y * this->m_Width  + x].B);
        }
    }
    
    // BMP FILE HEADER
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    
    // BMP INFO HEADER
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    
    // HELPER ARRAY USED TO FILL DATA LINES
    unsigned char bmppad[3] = {0,0,0};
    
    // WRITE FILE SIZE TO FILE HEADER
    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);
    
    // WRITE DIMENSIONS TO INFO HEADER
    bmpinfoheader[ 4] = (unsigned char)(       this->m_Width    );
    bmpinfoheader[ 5] = (unsigned char)(       this->m_Width>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       this->m_Width>>16);
    bmpinfoheader[ 7] = (unsigned char)(       this->m_Width>>24);
    bmpinfoheader[ 8] = (unsigned char)(       this->m_Height    );
    bmpinfoheader[ 9] = (unsigned char)(       this->m_Height>> 8);
    bmpinfoheader[10] = (unsigned char)(       this->m_Height>>16);
    bmpinfoheader[11] = (unsigned char)(       this->m_Height>>24);
    
    // WRITE HEADERS TO FILE
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    
    // iterate over each line in the image
    // last to first line
    for(unsigned int y = 0; y < this->m_Height; y++)
    {
        // Write Line to File
        fwrite(img + ( this->m_Width * (this->m_Height-y-1) * 3), 3, this->m_Width, f);
        // Fill line to multiple 4 Bytes (24bpp)
        fwrite(bmppad, 1 ,(4 - (this->m_Width * 3) % 4) % 4, f);
    }
    
    // free the memory for the image data
    free(img);
    // close the files
    fclose(f);
    
    return true;
    
}


RGBImage& RGBImage::SobelFilter(RGBImage& dst, const RGBImage& src, float factor){
    assert(dst.m_Height == src.m_Height && dst.m_Width == src.m_Width);
    
    RGBImage U(src.m_Width, src.m_Height);
    RGBImage V(src.m_Width, src.m_Height);
    float K[3][3] = {{1.0f,0.0f,-1.0f},{2.0f,0.0f,-2.0f},{1.0f,0.0f,-1.0f}};
    // int Kt[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

    for (unsigned int x = 0; x < src.m_Width; x++) {
        for (unsigned int y = 0; y < src.m_Width; y++) {
            for (unsigned int i = 0; i < 3; i++) {
                for (unsigned int j = 0; j < 3; j++) {
                    int offsetX = x-i-1;
                    int offsetY = y-j-1;
                    if(offsetX < 0) offsetX = 0;
                    if(offsetY < 0) offsetY = 0;
                    U.setPixelColor(x, y, Color(U.getPixelColor(x, y) + (src.getPixelColor(offsetX,offsetY) * K[i][j])));
                    V.setPixelColor(x, y, Color(V.getPixelColor(x, y) + (src.getPixelColor(offsetX,offsetY) * K[j][i])));
                }
            }
            dst.setPixelColor(x, y, (U.getPixelColor(x, y).powColor(2) + V.getPixelColor(x, y).powColor(2)).powColor(0.5f) * factor);
        }
    }
    

    
    return dst;
}

RGBImage& RGBImage::GaussFilter(RGBImage& dst, const RGBImage& src, float factor){
    assert(dst.m_Height == src.m_Height && dst.m_Width == src.m_Width);
    
    RGBImage G(src.m_Width, src.m_Height);
    RGBImage Gdash(src.m_Width, src.m_Height);
    float K[7] = {0.006f, 0.061f, 0.242f, 0.383f, 0.242f, 0.061f, 0.006f};

    for (unsigned int x = 0; x < src.m_Width; x++) {
        for (unsigned int y = 0; y < src.m_Width; y++) {
            for (unsigned int i = 0; i < 7; i++) {
                int offsetX = x-i-3;
                int offsetY = y-i-3;
                if(offsetX < 0) offsetX = 0;
                if(offsetY < 0) offsetY = 0;
                Gdash.setPixelColor(x, y, Color(Gdash.getPixelColor(x, y) + src.getPixelColor(offsetX, y) * K[i]));
                G.setPixelColor(x, y, Color(G.getPixelColor(x, y) + Gdash.getPixelColor(x, offsetY) * K[i]));
            }
            dst.setPixelColor(x, y, (G.getPixelColor(x, y) * factor));
        }
    }
    
    
    
    return dst;
}
