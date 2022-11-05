#ifndef TEXTURE_H
#define TEXTURE_H
class Texture {
private:
    struct TextureData {
        TextureData(): width(0), height(0), data(nullptr) {}

        int width;
        int height;
        unsigned char* data;
    };

public:
    GLuint texture = 0;

    // load a png image and return a TextureData structure with raw data
    // not limited to png format. works with any image format that is RGBA-32bit
    static TextureData loadImg(const std::string& imgFilePath) {
        TextureData textureData;
        int components;

        // load the textureData with stb image, force RGBA (4 components required)
        stbi_uc *data = stbi_load(imgFilePath.c_str(), &textureData.width, &textureData.height, &components, 4);

        // is the image successfully loaded?
        if (data != nullptr) {
            // copy the raw data
            size_t dataSize = textureData.width * textureData.height * 4 * sizeof(unsigned char);
            textureData.data = new unsigned char[dataSize];
            memcpy(textureData.data, data, dataSize);

            // mirror the image vertically to comply with OpenGL convention
            for (size_t i = 0; i < textureData.width; ++i) {
                for (size_t j = 0; j < textureData.height / 2; ++j) {
                    for (size_t k = 0; k < 4; ++k) {
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

    explicit Texture(const std::string& filename) {
        TextureData textureData = loadImg(filename);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data);
//        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "Loaded texture \"" << filename << "\"" << std::endl;

        delete textureData.data;
    }
    ~Texture() {
        glDeleteTextures(1, &texture);
    }

    void bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    static void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
#endif //TEXTURE_H
