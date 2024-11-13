#include "Line.h"

Line::Line() {}

Line::Line(Vec3 start, Vec3 end) {
    startPoint = start;
    endPoint = end;

    vertices = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Line::draw()  {
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);


}

Line::~Line() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
