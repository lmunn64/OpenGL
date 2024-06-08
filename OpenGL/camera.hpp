#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

class Camera {

public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float cameraSpeed;

	Camera() { //constructor for camera positions
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	void setCameraSpeed(float speed) {
		cameraSpeed = speed;
	}
};

#endif