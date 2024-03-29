#define RGB_NCHANNELS  3
#define RGBA_NCHANNELS 4

typedef struct {
  unsigned char r, g, b, a;
} RGBAPixel;

typedef struct {
  unsigned char r, g, b;
} RGBPixel;

class ImageIO {
  public:
    int width, height; // loaded image size, for draw image
    int nchannels;
    RGBAPixel *inPixmap;

    ImageIO();
    ~ImageIO();

    void loadImage(const std::string fileName);
    void exportImage(const std::string fileName);
    void drawImage();
    
  private:
    RGBAPixel *outPixmap;
    RGBPixel  *rgbInPixmap, *rgbOutPixmap;

    int getNChannelsByFormat(const std::string fileName);
    void buildMap();
    void convertRGBToRGBA(int iw, int ih);
    void convertRGBAToRGB(int iw, int ih);
    void convertPixmapToUpperLeft(unsigned char* pixmap, int nchannels, int width, int height);
};

