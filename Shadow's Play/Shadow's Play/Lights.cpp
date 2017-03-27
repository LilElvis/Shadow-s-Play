#include "Lights.h"
#include "Shader.h"

namespace ENG
{
	void PointLight::render(Mesh* meshPtr, Shader* shaderPtr)
	{
		transform.setMatrix(glm::mat4(
			transform.getMatrix()[0][0], transform.getMatrix()[0][1], transform.getMatrix()[0][2], position.x,
			transform.getMatrix()[1][0], transform.getMatrix()[1][1], transform.getMatrix()[1][2], position.y,
			transform.getMatrix()[2][0], transform.getMatrix()[2][1], transform.getMatrix()[2][2], position.z,
			0.0f, 0.0f, 0.0f, 1.0f));

		glBindVertexArray(renderVolume);

		shaderPtr->sendUniformMat4("uModel", transform.getFirstElement(), true);

		glDrawArrays(GL_TRIANGLES, 0, (meshPtr->listOfMeshes[meshName]->getNumberOfVerticies()));
		glBindVertexArray(0);
	}
}