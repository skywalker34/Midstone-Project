#include "Line.h"
#include "QMath.h"
#include "Constants.h"
Line::Line() {}

Line::Line(Vec3 start, Vec3 end) {
 
	//set teh line's pos to teh starting oint of the libe
	transform.setPos(start);

	//get the length of the vector between the two points (line)
	float num = VMath::mag(start - end);

	//our line has a world space length of 1, so scaling by a value will set its length to said value
	transform.setScale(num, num, num);

	Vec3 direction = VMath::normalize(end - start); //temp vec to figure out which way the line is pointing
	Quaternion q = QMath::lookAt(direction, UP); //temp quaternion to rotate the line to its correct orientation

	//rotate the scaled and transformed line to face the right orientation and so its graphical end pos is the listed end pos
	transform.setOrientation(q);

	
	//our line mesh is not flyweight, as the buffer is only 1 vec and isn't required for the scene  ( we were using them for debugging mainly)

	// 1. bind Vertex Array Object
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Line::RecalculateLine(Vec3 start, Vec3 end)
{//does the constructor logic againw ithout creating the mesh 


	transform.setPos(start);

	float num = VMath::mag(start - end);

	transform.setScale(num, num, num);


	Quaternion q = Quaternion();
	Vec3 direction = VMath::normalize(end - start);

	if (direction.x < VERY_SMALL && direction.y < VERY_SMALL && direction.z < VERY_SMALL) {
		q = Quaternion();
	}
	else {
		q = QMath::lookAt(direction, UP);
	}



	transform.setOrientation(q);
}


void Line::draw() {

	//use openGL to draw the line
	glLineWidth(lineThickness); // Set the line width defaulted to 5 pixels (adjust as needed)
	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 3);
	glBindVertexArray(0);


}

Line::~Line() {
	//clear the line out of the gpu
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

