
//////////////////////////////////////////////////////// 
/*
Author: Arrvind Venugopal
 RT Rendering 
 Implementation of IRIS scatterplot using VBO

*/
//////////////////////////////////////////////////////// 
#define _CRT_SECURE_NO_WARNINGS
#ifdef __APPLE__
#include <GLUT/glut.h> 
#include <OpenGL/gl.h> 
#else 
#include <GL/glew.h> 
#include <GL/glut.h> 
#include <GL/gl.h> 
#endif 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<math.h>
#include <fstream>
using std::ifstream;
#include <fstream>
#include <ostream>
using std::ofstream;
using std::endl;

const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 5;
const char* const DELIMITER = ",";


std::vector<float> v1, v2, v3, v4;

float max_v1 = -10000, min_v1 = 10000;
float max_v2 = -10000, min_v2 = 10000;
float max_v3 = -10000, min_v3 = 10000;
float max_v4 = -10000, min_v4 = 10000;
float scale_v1, scale_v2, scale_v3, scale_v4;
float *vertices; //Vertix pointer stack
float *colors;  //Color pointer stck
int count = 0; //counts the plot
GLubyte *pindices;


GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;


GLuint vboHandle[2];   // two VBOs, one for positions, one for colors 

GLuint indexVBO;

//////////////////////////////////////////////////////////////////////////////////
//
// create VBO objects and send the vertices/colors that contain ploted points, tick mark points and margin points to the graphics card
// 
void InitVBO()
{
	glGenBuffers(2, vboHandle);   // create two VBO handles, one position, one color handle

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);   // bind the first handle 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(count * 16 * 4 + 44 * 4 ), vertices, GL_STATIC_DRAW); // allocate space and copy the position data over

	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);   // bind the second handle 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(count * 16 * 4 + 44* 4 ), colors, GL_STATIC_DRAW); // allocate space and copy the color data over

	glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up 

	// by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time 


}

void display()
{
	
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(2);
	int const w = glutGet(GLUT_WINDOW_WIDTH);
	int const h = glutGet(GLUT_WINDOW_HEIGHT);
	// X and Y Cordinate Text
	char* text1 = "Sepal Length";
	char* text2 = "Sepal Width";
	char* text3 = "Petal Length";
	char* text4 = "Petal Width";
	int i;
	glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array on the client side
	glEnableClientState(GL_COLOR_ARRAY); // enable the color array on the client side

	// Tells OpenGL how to walk through the VBOs, i.e., how the data are packed 

	// the vertex color buffer object 
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[1]);
	glColorPointer(4, GL_FLOAT, 0, 0);

	// the vertex position buffer object 
	glBindBuffer(GL_ARRAY_BUFFER, vboHandle[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glVertexPointer(4, GL_FLOAT, 0, 0); //number of coordinates per vertex (4 here), type of the coordinates, 
	// stride between consecutive vertices, and pointers to the first coordinate
	
	
	for (i = 0; i < 16; i++)
	{
	
		glViewport(20+(w / 4 * (i % 4)), 20+(h / 4 * (i /4)), w / 4-(2*20) , h / 4 -(2*20));
		
	// X and Y coordinate text writing based on corresponding scatterplots
		glColor3f(0.0f, 0.0f, 0.0f);
		
		glRasterPos2f(-0.50f, -1.0f);
		
		char *text,*text0;
		if (i / 4 == 0)
		{
			text = text1;
			if (i == 0)
				text0 = text1;
			else if (i == 1)
				text0 = text2;
			else if (i == 2)
				text0 = text3;
			else 
				text0 = text4;
		}
		else if (i / 4 == 1)
		{
			text = text2;
			if (i == 4)
				text0 = text1;
			else if (i == 5)
				text0 = text2;
			else if (i == 6)
				text0 = text3;
			else
				text0 = text4;

		}
		else if (i / 4 == 2)
		{
			text = text3;
			if (i == 8)
				text0 = text1;
			else if (i == 9)
				text0 = text2;
			else if (i == 10)
				text0 = text3;
			else
				text0 = text4;
		
		}
		else
		{
			text = text4;
			if (i == 12)
				text0 = text1;
			else if (i == 13)
				text0 = text2;
			else if (i == 14)
				text0 = text3;
			else
				text0 = text4;

		}
		
		for (int j = 0; text[j] != '\0'; j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text[j]);
			glColor3f(0.0f, 0.0f, 0.0f);
			glRasterPos2f((-0.50 + .1*(j + 1)), -1.0f);// X coordinate- hozizontal writing
			
		}
		
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2f(-1.0f, 0.30f);
		for (int j = 0; text0[j] != '\0'; j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text0[j]);
			glColor3f(0.0f, 0.0f, 0.0f);
			
			glRasterPos2f(-1.0f,(0.30 -.1*(j + 1)));// Y coordinate- vertical writing

		}
	  // Border of the scaterplot
	   glDrawArrays(GL_LINE_LOOP, 2400, 4);
	  
	  // Ten equally spaced ticks on both  X and Y axis
	  glDrawArrays(GL_LINES, 2404, 40);
	   // Ploting the points 
	  glDrawArrays(GL_POINTS, i*count, 150);
		 
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glutSwapBuffers();

}

int main(int argc, char** argv)
{

	
	// create a file-reading object
	ifstream fin;
	
	fin.open("iris.csv"); // open a file

	if (!fin.good())
		return 1; // exit if file not found

	// read each line of the file
	while (!fin.eof())
	{
		// read an entire line into memory
		char buf[MAX_CHARS_PER_LINE];
		fin.getline(buf, MAX_CHARS_PER_LINE);

		// parse the line into blank-delimited tokens
		int n = 0; // a for-loop index

		// array to store memory addresses of the tokens in buf
		char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0

		// parse the line
		token[0] = strtok(buf, DELIMITER); // first token
		if (token[0]) // zero if line is blank
		{
			for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
			{
				token[n] = strtok(0, DELIMITER); // subsequent tokens
				if (!token[n]) break; // no more tokens
			}

			std::string fs(token[0]);

			if (fs.compare("setosa") == 0 || fs.compare("versicolor") == 0 || fs.compare("virginica") == 0)
			{
				//Converting the points from string to float after recieving the tokens

				if (atof(token[1]) < min_v1) min_v1 = atof(token[1]);
				if (atof(token[1]) > max_v1) max_v1 = atof(token[1]);

				if (atof(token[2]) < min_v2) min_v2 = atof(token[2]);
				if (atof(token[2]) > max_v2) max_v2 = atof(token[2]);

				if (atof(token[3]) < min_v3) min_v3 = atof(token[3]);
				if (atof(token[3]) > max_v3) max_v3 = atof(token[3]);

				if (atof(token[4]) < min_v4) min_v4 = atof(token[4]);
				if (atof(token[4]) > max_v4) max_v4 = atof(token[4]);


				v1.push_back(atof(token[1]));
				v2.push_back(atof(token[2]));
				v3.push_back(atof(token[3]));
				v4.push_back(atof(token[4]));
				count++;
			}
		}
		

	}
	
	// Range for each of theses vertices

	scale_v1 = max_v1 - min_v1;
	scale_v2 = max_v2 - min_v2;
	scale_v3 = max_v3 - min_v3;
	scale_v4 = max_v4 - min_v4;

	
	float a[4];// temp Varaible to store Normalized coordinates
	vertices = new float[count * 16 * 4 + 4 * 4 + 40*4];// total vertices + border points + points for tick marks 
	colors = new float[count * 4 * 16 + 4 * 4 + 40*4 ]; // total color vertices + color border points + color points for tick marks 

	//Setting the Border Points scaled to .86 so that text could be written beneath the border
	vertices[count * 16 * 4] = (-1)*.86;
	vertices[count * 16 * 4 + 1] = (-1)*.86;
	vertices[count * 16 * 4 + 2] = 0;
	vertices[count * 16 * 4 + 3] = 1;

	vertices[count * 16 * 4 + 4 * 1] = (-1)*.86;
	vertices[count * 16 * 4 + 4 * 1 + 1] = .86;
	vertices[count * 16 * 4 + 4 * 1 + 2] = 0;
	vertices[count * 16 * 4 + 4 * 1 + 3] = 1;

	vertices[count * 16 * 4 + 4 * 2] = .86;
	vertices[count * 16 * 4 + 4 * 2 + 1] = .86;
	vertices[count * 16 * 4 + 4 * 2 + 2] = 0;
	vertices[count * 16 * 4 + 4 * 2 + 3] = 1;

	vertices[count * 16 * 4 + 4 * 3] = .86;
	vertices[count * 16 * 4 + 4 * 3 + 1] = (-1)*.86;
	vertices[count * 16 * 4 + 4 * 3 + 2] = 0;
	vertices[count * 16 * 4 + 4 * 3 + 3] = 1;

	//Setting points for storing equally spaced  tick marks 

	for (int i = 0; i < 10; i++)
	{
		vertices[count * 4 * 16 + 4 * 4 + 16 * i] = -0.86 + (0.86 * 2 / 11)*(i + 1);
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 1] = -0.86;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 2] = 0;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 3] = 1;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 4] = -0.86 + (0.86 * 2 / 11)*(i + 1);
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 5] = -0.86 - .06;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 6] = 0;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 7] = 1;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 8] = -0.86;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 9] =- 0.86 + (0.86 * 2 / 11)*(i + 1);
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 10] = 0;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 11] = 1;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 12] = -0.86 - .06;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 13] = -0.86 + (0.86 * 2 / 11)*(i + 1);
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 14] = 0;
		vertices[count * 4 * 16 + 4 * 4 + 16 * i + 15] = 1;
		
	}
	
	//Storing color info of border and tick points
	for (int j = 0; j < 44; j++)
	{

		colors[count * 16 * 4 + 4 * j] = 0;
		colors[count * 16 * 4 + 4 * j + 1] = 0;
		colors[count * 16 * 4 + 4 * j + 2] = 0;
		colors[count * 16 * 4 + 4 * j + 3] = 1;


	}
	//Storing color info of Iris points
	for (int j = 0; j < 16; j++)
	{
		for (int i = 0; i < 3; i++)
			for (int y = 0; y < 50; y++)
			{
				colors[4 * (j * 150 + i * 50 + y)] = (1 - (i + 1) / 2);
				colors[4 * (j * 150 + i * 50 + y) + 1] = 1 - (i + 1) % 2;
				colors[4 * (j * 150 + i * 50 + y) + 2] = (i + 1) / 3;
				colors[4 * (j * 150 + i * 50 + y) + 3] = 1;

			}
	}

	//initialize unused scterplot points
	for (int j = 0; j < (count * 16); j++)
	{
		vertices[4 * j + 2] = 0;
		vertices[4 * j + 3] = 1;
	}
	
	//Convert the  points Normalized form   
	for (int i = 0; i < count; i++) { 
		a[0] = (v1[i] - min_v1) * 2 / scale_v1 - 1;
		a[1] = (v2[i] - min_v2) * 2 / scale_v2 - 1;
		a[2] = (v3[i] - min_v3) * 2 / scale_v3 - 1;
		a[3] = (v4[i] - min_v4) * 2 / scale_v4 - 1;
		int ct = 0;
		//Permute all 16 possibilites
		for (int j = 0; j < 4; j++)
			for (int z = 0; z < 4; z++)
			{
				vertices[4 * i + 4 * count*ct] = 0.85*a[j];//Scaling to plot inside the boundry 
				vertices[4 * i + 4 * count*ct + 1] = 0.85*a[z];//Scaling to plot inside the boundry
				
				ct++;
			}
	}
	
	fin.close();
	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900,900);

	glutCreateWindow("IRIS scatterplot : Red for Setosa, Green for Versicolor, and Blue for Virginica ");
	//Registering the display window
	glutDisplayFunc(display);
	
	
	glewInit();
	
	
	//Initializing the VBO
	InitVBO();
	
	glutMainLoop();
	return 0;
}