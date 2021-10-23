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
    LoadTexture((char*)"Textures\\Ship Afire.bmp", this->ShipAfireTextureID);
    LoadTexture((char*)"Textures\\Ship Afire aim.bmp", this->ShipAfireAimTextureID);

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

    LoadTexture((char*)"Textures\\WaterAim.bmp", this->WaterAimTextureID);

    LoadTexture((char*)"Textures\\ButtonFieldFire.bmp", this->ButtonFieldFireTextureID);
    LoadTexture((char*)"Textures\\ButtonFieldDeploy.bmp", this->ButtonFieldDeployTextureID);

    LoadTexture((char*)"Textures\\BTN_DISCONNECT.bmp", this->Btn_DisconnectTextureID);
    LoadTexture((char*)"Textures\\BTN_CONNECT.bmp", this->Btn_ConnectTextureID);
    LoadTexture((char*)"Textures\\BTN_CANCEL.bmp", this->Btn_CancelTextureID);
    
    LoadTexture((char*)"Textures\\New Game Label.bmp", this->Label_NewGame);
    LoadTexture((char*)"Textures\\New Game PVE.bmp", this->Btn_NewGame_PVE);
    LoadTexture((char*)"Textures\\New Game PVP.bmp", this->Btn_NewGame_PVP);

    LoadTexture((char*)"Textures\\StatisticField.bmp", this->StatisticFieldTextureID);

    LoadTexture((char*)"Textures\\sea.bmp", this->WaterTextureID);
    LoadTexture((char*)"Textures\\Rocket Missed.bmp", this->MissedTextureID);
    LoadTexture((char*)"Textures\\Rocket Missed Aim.bmp", this->MissedAimTextureID);

    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number0.bmp", this->TimeNumber0TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number1.bmp", this->TimeNumber1TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number2.bmp", this->TimeNumber2TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number3.bmp", this->TimeNumber3TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number4.bmp", this->TimeNumber4TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number5.bmp", this->TimeNumber5TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number6.bmp", this->TimeNumber6TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number7.bmp", this->TimeNumber7TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number8.bmp", this->TimeNumber8TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\Number9.bmp", this->TimeNumber9TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\TimeTextures\\colon.bmp", this->TimeColonTextureID);
   
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber0.bmp", this->AliveDeadNumber0TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber1.bmp", this->AliveDeadNumber1TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber2.bmp", this->AliveDeadNumber2TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber3.bmp", this->AliveDeadNumber3TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber4.bmp", this->AliveDeadNumber4TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber5.bmp", this->AliveDeadNumber5TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber6.bmp", this->AliveDeadNumber6TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber7.bmp", this->AliveDeadNumber7TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber8.bmp", this->AliveDeadNumber8TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber9.bmp", this->AliveDeadNumber9TextureID);
    LoadTexture((char*)"Textures\\SymbolTextures\\AliveDeadTextures\\BigNumber10.bmp", this->AliveDeadNumber10TextureID);
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
