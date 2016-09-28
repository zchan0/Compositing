/**
 * Homework 3 for CS604
 * Get full sepcs on https://people.cs.clemson.edu/~dhouse/courses/404/hw/hw3/hw3.html
 *
 * Program function:
 * - Generate alpha mask for image
 * - Display generated image
 * - Save generated image
 *
 * Usage:
 * - alphamask input.img output.img
 *
 * author: Cencen Zheng 9/27/2016
 * 
 */

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

#include "ImageIO.h"

#define maximum(x, y, z) ((x) > (y) ? ((x) > (z) ? (x) : (z)) : ((y) > (z) ? (y) : (z))) 
#define minimum(x, y, z) ((x) < (y) ? ((x) < (z) ? (x) : (z)) : ((y) < (z) ? (y) : (z)))

static std::string input, output; 
static ImageIO imageIO = ImageIO();

void RGBtoHSV(unsigned char r, unsigned char g, unsigned char b, double &h, double &s, double &v) {

  double red, green, blue; 
  double max, min, delta;

  red = r / 255.0; green = g / 255.0; blue = b / 255.0;  /* r, g, b to 0 - 1 scale */

  max = maximum(red, green, blue); 
  min = minimum(red, green, blue);

  v = max;	/* value is maximum of r, g, b */

  /* saturation and hue 0 if value is 0 */ 
  if(max == 0) {    
     s = 0; 
     h = 0; 
  } else { 
  	/* saturation is color purity on scale 0 - 1 */
    s = (max - min) / max;           

    delta = max - min; 
    if(delta == 0) { /* hue doesn't matter if saturation is 0 */ 
       h = 0;
    } else { 
      if(red == max) /* otherwise, determine hue on scale 0 - 360 */ 
          h = (green - blue) / delta; 
      else if(green == max) 
          h = 2.0 + (blue - red) / delta; 
      else /* (green == max) */ 
          h = 4.0 + (red - green) / delta; 
      h = h * 60.0;	/* change hue to degrees */
      if(h < 0) 
          h = h + 360.0; /* negative hue rotated to equivalent positive around color wheel */
    } 
  } 
} 

void getFileNameFromCommandLine(int argc, char* argv[], std::string &input, std::string &output) {
  switch (argc) {
  case 3:
    input  = argv[1];
    output = argv[2];
    break;
  default:
      std::cerr << "Usage: alphamask input.img output.img" << std:: endl;
      exit(1);
    break;
  }
}

/**
 * split string by delimiter
 * credits to http://stackoverflow.com/a/236803
 * @param s     string to be splited
 * @param delim delimiter
 * @param elems splited values
 */
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

void parseParameters(double &screenValue, double &hueThreshold, double &thresholdS, double &thresholdV) {
	std::ifstream file("params.txt");
	std::string line;
	std::vector<std::string> v;

	if (file.is_open()) {
		while(getline(file, line)) {
			split(line, ':', v);
		}
		file.close();
	}

	double minH, maxH;
	for (int i = 0; i < v.size(); ++i) {
		switch(i) {
			case 1: screenValue = stod(v[i]); break;
			case 3: minH = stod(v[i]); break;
			case 5: maxH = stod(v[i]); break;
			case 7: thresholdS = stod(v[i]); break;
			case 9: thresholdV = stod(v[i]); break;
			default: break;
		}
	}
	hueThreshold = fmin(fabs(screenValue - minH), fabs(screenValue - maxH));
}

void spillSupression() {
	unsigned char min = 255;
	for (int i = 0; i < imageIO.width * imageIO.height; ++i) {
		min = minimum(imageIO.inPixmap[i].r, imageIO.inPixmap[i].g, imageIO.inPixmap[i].b);
		imageIO.inPixmap[i].g = min;
	}
}

void associatedColorImage() {
  	unsigned char alpha = 0;
  	for (size_t i = 0; i < imageIO.width * imageIO.height; ++i) {
  		alpha = imageIO.inPixmap[i].a;
  		imageIO.inPixmap[i].r *= alpha / 255.0; 
  		imageIO.inPixmap[i].g *= alpha / 255.0; 
  		imageIO.inPixmap[i].b *= alpha / 255.0; 
  	}
}

void createAlphaMask() {
	double h, s, v;
	double screenValue, thresholdH, thresholdS, thresholdV;
	parseParameters(screenValue, thresholdH, thresholdS, thresholdV);

	for (int i = 0; i < imageIO.width * imageIO.height; ++i) {	
		RGBtoHSV(imageIO.inPixmap[i].r, imageIO.inPixmap[i].g, imageIO.inPixmap[i].b, h, s, v);

		if (fabs(h - screenValue) <= thresholdH && s > thresholdS && v > thresholdV) {
			imageIO.inPixmap[i].a = 0;
		} else {
			imageIO.inPixmap[i].a = 255;
		}
	}

	spillSupression();
	associatedColorImage();

	glutPostRedisplay();
}

void handleKeyboard(unsigned char key, int x, int y) {
	switch(key) {
    case 'w': case 'W': imageIO.exportImage(output); break;
    case 'm': case 'M': createAlphaMask(); break;
    case 'q': case 'Q': exit(0); break;
  }	
}

void handleReshape(int width, int height) {
  	glViewport(0, 0, width, height);

  	glMatrixMode(GL_PROJECTION);
	// Clear the projection matrix 
	glLoadIdentity();
  	gluOrtho2D(0,  width, 0, height);
	glMatrixMode(GL_MODELVIEW);
}

void display() {
	imageIO.drawImage();
}

int main(int argc, char* argv[]) {
	// handle command line input
	getFileNameFromCommandLine(argc, argv, input, output);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	if (!input.empty()) {			
		imageIO.loadImage(input);

	  	glutInitWindowSize(imageIO.width, imageIO.height);
	}

	glutCreateWindow("Green Screen");
	glutDisplayFunc(display);
	glutKeyboardFunc(handleKeyboard);
	glutReshapeFunc(handleReshape);

		glutMainLoop();

	return 0;
}