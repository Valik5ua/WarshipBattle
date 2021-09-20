#include "TextureManager.h"
#include "GL\freeglut.h"
#include <fstream>
#include <string>

/// <summary>
/// Loads all the textures used in the program into memory.
/// </summary>
void TextureManager::LoadAllTextures()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // (Actually, this one is the default)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    LoadTexture((char*)"Textures\\Ship front.bmp", this->ShipFrontTextureID);
    LoadTexture((char*)"Textures\\Ship middle.bmp", this->ShipMiddleTextureID);
    LoadTexture((char*)"Textures\\Ship Back.bmp", this->ShipBackTextureID);

    LoadTexture((char*)"Textures\\Ship front Deployable.bmp", this->ShipFrontDeployableTextureID);
    LoadTexture((char*)"Textures\\Ship middle Deployable.bmp", this->ShipMiddleDeployableTextureID);
    LoadTexture((char*)"Textures\\Ship Back Deployable.bmp", this->ShipBackDeployableTextureID);

    LoadTexture((char*)"Textures\\Ship front Undeployable.bmp", this->ShipFrontUndeployableTextureID);
    LoadTexture((char*)"Textures\\Ship middle Undeployable.bmp", this->ShipMiddleUndeployableTextureID);
    LoadTexture((char*)"Textures\\Ship Back Undeployable.bmp", this->ShipBackUndeployableTextureID);

    LoadTexture((char*)"Textures\\Ship front Afire.bmp", this->ShipFrontAfireTextureID);
    LoadTexture((char*)"Textures\\Ship middle Afire.bmp", this->ShipMiddleAfireTextureID);
    LoadTexture((char*)"Textures\\Ship Back Afire.bmp", this->ShipBackAfireTextureID);

    LoadTexture((char*)"Textures\\Ship front Cracked.bmp", this->ShipFrontCrackedTextureID);
    LoadTexture((char*)"Textures\\Ship middle Cracked.bmp", this->ShipMiddleCrackedTextureID);
    LoadTexture((char*)"Textures\\Ship Back Cracked.bmp", this->ShipBackCrackedTextureID);

    LoadTexture((char*)"Textures\\Ship front Cracked Aim.bmp", this->ShipFrontCrackedAimTextureID);
    LoadTexture((char*)"Textures\\Ship middle Cracked Aim.bmp", this->ShipMiddleCrackedAimTextureID);
    LoadTexture((char*)"Textures\\Ship Back Cracked Aim.bmp", this->ShipBackCrackedAimTextureID);

    LoadTexture((char*)"Textures\\Single Ship.bmp", this->SingleShipTextureID);
    LoadTexture((char*)"Textures\\Single Ship Aim.bmp", this->SingleShipAimTextureID);
    LoadTexture((char*)"Textures\\Single Ship Cracked.bmp", this->SingleShipCrackedTextureID);
    LoadTexture((char*)"Textures\\Single Ship Cracked Aim.bmp", this->SingleShipCrackedAimTextureID);
    LoadTexture((char*)"Textures\\Single Ship Deployable.bmp", this->SingleShipDeployableTextureID);
    LoadTexture((char*)"Textures\\Single Ship Undeployable.bmp", this->SingleShipUndeployableTextureID);

    LoadTexture((char*)"Textures\\BTN_ROTATE.bmp", this->Btn_RotateTextureID);
    LoadTexture((char*)"Textures\\BTN_RANDOMAIM.bmp", this->Btn_RandomAimTextureID);
    LoadTexture((char*)"Textures\\WaterAim.bmp", this->WaterAimTextureID);

    LoadTexture((char*)"Textures\\BTN_DOWN.bmp", this->Btn_DownTextureID);
    LoadTexture((char*)"Textures\\BTN_UP.bmp", this->Btn_UpTextureID);
    LoadTexture((char*)"Textures\\BTN_LEFT.bmp", this->Btn_LeftTextureID);
    LoadTexture((char*)"Textures\\BTN_RIGHT.bmp", this->Btn_RightTextureID);

    LoadTexture((char*)"Textures\\BTN_FIRE.bmp", this->Btn_FireTextureID);
    LoadTexture((char*)"Textures\\BTN_DEPLOY.bmp", this->Btn_DeployTextureID);

    LoadTexture((char*)"Textures\\BTN_DISCONNECT.bmp", this->Btn_DisconnectTextureID);
    LoadTexture((char*)"Textures\\BTN_CONNECT.bmp", this->Btn_ConnectTextureID);
    LoadTexture((char*)"Textures\\BTN_CANCEL.bmp", this->Btn_CancelTextureID);

    LoadTexture((char*)"Textures\\sea.bmp", this->WaterTextureID);
    LoadTexture((char*)"Textures\\Rocket Missed.bmp", this->MissedTextureID);
    LoadTexture((char*)"Textures\\Rocket Missed Aim.bmp", this->MissedAimTextureID);
}

/// <summary>
/// Loads a texture into memory.
/// </summary>
/// <param name="FileName: ">The path of the texture to be loaded.</param>
/// <param name="Texture_ID: ">The ID of the texture to be binded to.</param>
/// <returns>True if successful load, otherwise false.</returns>
bool TextureManager::LoadTexture(char* FileName, GLuint& Texture_ID)
{
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
        std::string FailureMSG = "File \"";
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

    glGenTextures(1, &Texture_ID);            // Generate a texture
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
