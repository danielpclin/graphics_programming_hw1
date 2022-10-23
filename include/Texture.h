#ifndef TEXTURE_H
#define TEXTURE_H
class Texture
{
public:
    GLuint texture{};

    struct TextureData
    {
        TextureData():width(0), height(0), data(nullptr) {}

        int width;
        int height;
        unsigned char* data;
    };

    // load a png image and return a TextureData structure with raw data
    // not limited to png format. works with any image format that is RGBA-32bit
    static TextureData loadImg(const char* const imgFilePath)
    {
        TextureData textureData;
        int components;

        // load the textureData with stb image, force RGBA (4 components required)
        stbi_uc *data = stbi_load(imgFilePath, &textureData.width, &textureData.height, &components, 4);

        // is the image successfully loaded?
        if(data != nullptr)
        {
            // copy the raw data
            size_t dataSize = textureData.width * textureData.height * 4 * sizeof(unsigned char);
            textureData.data = new unsigned char[dataSize];
            memcpy(textureData.data, data, dataSize);

            // mirror the image vertically to comply with OpenGL convention
            for (size_t i = 0; i < textureData.width; ++i)
            {
                for (size_t j = 0; j < textureData.height / 2; ++j)
                {
                    for(size_t k = 0; k < 4; ++k)
                    {
                        size_t coord1 = (j * textureData.width + i) * 4 + k;
                        size_t coord2 = ((textureData.height - j - 1) * textureData.width + i) * 4 + k;
                        std::swap(textureData.data[coord1], textureData.data[coord2]);
                    }
                }
            }

            // release the loaded image
            stbi_image_free(data);
        }

        return textureData;
    }

    explicit Texture(const std::string& texture_filename)
    {

        TextureData textureData = loadImg(texture_filename.c_str());

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        delete textureData.data;
    }
};
#endif //TEXTURE_H
