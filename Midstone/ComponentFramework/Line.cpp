#include "Line.h"
#include "QMath.h"
#include "Constants.h"
Line::Line() {}

Line::Line(Vec3 start, Vec3 end) {


	transform.setPos(start);

	float num = VMath::mag(start - end);

	transform.setScale(num, num, num);

	Vec3 foward = Vec3(0, 0, -1);

	start.print("Starting line point: ");
	end.print("Ending line point: ");
	Vec3 direction = VMath::normalize(end - start);

	Quaternion q = QMath::lookAt(direction, UP);


	transform.setOrientation(q);





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
{
	transform.setPos(start);

	float num = VMath::mag(start - end);

	transform.setScale(num, num, num);

	Vec3 foward = Vec3(0, 0, -1);

	start.print("Starting line point: ");
	end.print("Ending line point: ");
	Vec3 direction = VMath::normalize(end - start);

	Quaternion q = QMath::lookAt(direction, UP);


	transform.setOrientation(q);
}

void Line::draw() {


	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);


}

Line::~Line() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}