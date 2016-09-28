/**
 *
 * CS604 Homework 3,
 * Get full specification on https://people.cs.clemson.edu/~dhouse/courses/404/hw/hw3/hw3.html
 *
 * Program function:
 * - Read two associated color images and composite
 * - Display composited image
 * - Write composited image
 *
 * Usage:
 * - > composite A.img B.img // make A over B
 * - c/C to trigger composite
 * - w/W to save composited image, default name is "composite.png"
 *
 * Other:
 * - default position of A is (0, 0)
 * - composited image store in B.img
 * - for reuse, use imgs to store image names from command line, can modify some code to composite multiple images
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
#include <vector>

#include "ImageIO.h"

static const int Width  = 640;
static const int Height = 480;
static unsigned int startX = 0;
static unsigned int startY = 0;

static ImageIO AImageIO = ImageIO();
static ImageIO BImageIO = ImageIO();
static std::vector<std::string> imgs;	// store image names from command line

bool getImageNameFromCommandLine(int argc, char* argv[]) {
  if (argc <= 1) {	
      std::cerr << "Usage:composite A.img B.img" << std:: endl;
      exit(1);
      return false;
  }

  for (size_t i = 1; i < argc; ++i) {
  	imgs.push_back(argv[i]);	
  }
  return true;
}

void loadImages() {
	AImageIO.loadImage(imgs[0]);
	BImageIO.loadImage(imgs[1]);
}

bool setFgImagePostion(int x, int y) {
	if (x < 0 || y < 0) {
		std::cerr << "Invalid position, check x " << x << " and y " << y << std::endl;	
		return false;
	}

	if (AImageIO.inPixmap == NULL || BImageIO.inPixmap == NULL) {
		std::cerr << "Load image first" << std::endl;
		return false;
	}

	if (BImageIO.width < AImageIO.width || BImageIO.height < AImageIO.height) {
		std::cerr << "Cannot Composite: Background image size is smaller than foreground" << std::endl;
		return false;
	}

	startX = x;
	startY = y;

	return true;
}

bool compositImage() {
	int Aw = AImageIO.width;
	int Ah = AImageIO.height;
	int Bw = BImageIO.width;
	int Bh = BImageIO.height;

	if(!setFgImagePostion(0, 0)) { return false; }

	int aIndex, bIndex;
	unsigned char alpha = 0;
	for (size_t i = 0; i < Ah; ++i) {
		for (size_t j = 0; j < Aw; ++j) {
			// left-bottom, A (j, i) -> B (j + startX, i + startY)
			aIndex = j + i * Aw;
			bIndex = startX + j + (startY + i) * Bw;
			alpha  = AImageIO.inPixmap[aIndex].a;
			// A and B are both assocated color images, 
			// so Cp = CA + (1 - alphaA) * CB
			BImageIO.inPixmap[bIndex].r = AImageIO.inPixmap[aIndex].r + (255 - alpha) / 255.0 * BImageIO.inPixmap[bIndex].r;
			BImageIO.inPixmap[bIndex].g = AImageIO.inPixmap[aIndex].g + (255 - alpha) / 255.0 * BImageIO.inPixmap[bIndex].g;
			BImageIO.inPixmap[bIndex].b = AImageIO.inPixmap[aIndex].b + (255 - alpha) / 255.0 * BImageIO.inPixmap[bIndex].b;
		}
	}

	return true;
}

void handleKeyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'c': case 'C': if(compositImage()) { glutPostRedisplay(); }; break;
    	case 'w': case 'W': BImageIO.exportImage("composite.png"); break;
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
	BImageIO.drawImage();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  	glutInitWindowSize(Width, Height);
  	glutCreateWindow("Composite");

	if(getImageNameFromCommandLine(argc, argv)) {
		loadImages();
	}

	glutDisplayFunc(display);
	glutReshapeFunc(handleReshape);
	glutKeyboardFunc(handleKeyboard);

	glutMainLoop();

	return 0;
}