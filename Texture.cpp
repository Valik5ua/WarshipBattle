#include <GL\glew.h>
#include <fstream>
#include "Texture.h"
#include <string>

GLuint	ShipFrontTextureID;
GLuint	ShipMiddleTextureID;
GLuint	ShipBackTextureID;

GLuint	ShipFrontAimTextureID;
GLuint	ShipMiddleAimTextureID;
GLuint	ShipBackAimTextureID;

GLuint	ShipFrontAfireTextureID;
GLuint	ShipMiddleAfireTextureID;
GLuint	ShipBackAfireTextureID;

GLuint	ShipFrontAfireAimTextureID;
GLuint	ShipMiddleAfireAimTextureID;
GLuint	ShipBackAfireAimTextureID;

GLuint	ShipFrontCrackedTextureID;
GLuint	ShipMiddleCrackedTextureID;
GLuint	ShipBackCrackedTextureID;

GLuint	ShipFrontCrackedAimTextureID;
GLuint	ShipMiddleCrackedAimTextureID;
GLuint	ShipBackCrackedAimTextureID;

GLuint   SingleShipTextureID;
GLuint   SingleShipAimTextureID;
GLuint   SingleShipCrackedTextureID;
GLuint   SingleShipCrackedAimTextureID;

GLuint  Btn_RotateTextureID;
GLuint	Btn_RandomAimTextureID;
GLuint	WaterAimTextureID;

GLuint	Btn_DownTextureID;
GLuint	Btn_UpTextureID;
GLuint	Btn_LeftTextureID;
GLuint	Btn_RightTextureID;
GLuint	Btn_FireTextureID;
GLuint	Btn_DeployTextureID;
GLuint	Btn_ConnectTextureID;
GLuint	Btn_CancelTextureID;
GLuint	Btn_DisconnectTextureID;
GLuint	WaterTextureID;

bool LoadTexture(char* FileName, GLuint& Texture_ID)
{
   // uint8_t* datBuff[2] = { nullptr, nullptr }; // Header buffers
   // uint8_t* pixels = nullptr; // Pixels
    BITMAPFILEHEADER* bmpHeader = nullptr; // Header
    BITMAPINFOHEADER* bmpInfo = nullptr; // Info 
    uint8_t* BMPHeaderBuffer = new uint8_t[sizeof(BITMAPFILEHEADER)];
    uint8_t* BMPInfoBuffer = new uint8_t[sizeof(BITMAPINFOHEADER)];
    
    std::ifstream file(FileName, std::ios::binary);
    if (!file)
    {
        MessageBox(NULL, L"Failure to open bitmap file.", L"Texture failure", MB_OK);
        return false;
    }

    file.read((char*)BMPHeaderBuffer, sizeof(BITMAPFILEHEADER));
    file.read((char*)BMPInfoBuffer, sizeof(BITMAPINFOHEADER));

    bmpHeader = (BITMAPFILEHEADER*)BMPHeaderBuffer;
    bmpInfo = (BITMAPINFOHEADER*)BMPInfoBuffer;

    if (bmpHeader->bfType != 0x4D42)
    {
        std::string FailureMSG="File \"";
        FailureMSG += FileName;
        FailureMSG += "\" isn't a bitmap file\n";
        MessageBoxA(NULL, FailureMSG.c_str(), "Texture Failure", MB_OK);
        return false;
    }

    uint8_t* BMPPixelsBuffer = new uint8_t[bmpInfo->biSizeImage];

    // Go to where image data starts, then read in image data
    file.seekg(bmpHeader->bfOffBits);
    file.read((char*)BMPPixelsBuffer, bmpInfo->biSizeImage);

    GLuint _width = bmpInfo->biWidth;
    GLuint _height = bmpInfo->biHeight;

    glGenTextures(1, &Texture_ID);             // Generate a texture
    glBindTexture(GL_TEXTURE_2D, Texture_ID); // Bind that texture temporarily

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // (Actually, this one is the default)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Create the texture. We get the offsets from the image, then we use it with the image's
    // pixel data to create it.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, BMPPixelsBuffer);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, NULL);

    // Delete the two buffers.
    delete[] BMPHeaderBuffer;
    delete[] BMPInfoBuffer;
    delete[] BMPPixelsBuffer;

    return true;
}
