//Cameron van Velzen - 100591663
#include "States.h"

void Initialize()
{
	//SETUP
	Settings.depthBits = 24;
	Settings.stencilBits = 8;
	Settings.antialiasingLevel = 2;

	//CREATE WINDOW
	gameWindow->GetSFMLWindow()->create(
		sf::VideoMode(windowWidth, windowHeight), "Shadow's Play", sf::Style::Close, Settings);

	//CHECK IF GLEW INITIALIZED SUCCESSFULLY
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW could not be initialized.\n";
		system("PAUSE");
		exit(0);
	}

	//LOAD SHADER PROGRAMS
	defaultShader.load("Default", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/BasicLighting.frag");
	passThrough.load("PassThrough", "../assets/shaders/PassThrough.vert", "../assets/shaders/PassThrough.frag");
	GBuffer.load("GBuffer", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/GBuffer.frag");
	lightingComposite.load("lightComp", "../assets/shaders/PassThrough.vert", "../assets/shaders/LightComposite.frag");
	deferredLighting.load("lighting", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/DeferredLighting.frag");
	bloomHighPass.load("highPass", "../assets/shaders/PassThrough.vert", "../assets/shaders/HighPass.frag");
	bloomHorizontalBlur.load("horBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/HorizontalBlur.frag");
	bloomVerticalBlur.load("vertBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/VerticalBlur.frag");
	bloomComposite.load("bloomComp", "../assets/shaders/PassThrough.vert", "../assets/shaders/BloomComposite.frag");
	UVScrolling.load("UVScroll", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/UVScrolling.frag");
	motionBlur.load("PassThrough", "../assets/shaders/PassThrough.vert", "../assets/shaders/MotionBlur.frag");
	colorCorrect.load("ColorCorrect", "../assets/shaders/PassThrough.vert", "../assets/shaders/LUT.frag");
	bokehBlur.load("BokehBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/bokeh.frag");
	bokehComposite.load("BokehComposite", "../assets/shaders/PassThrough.vert", "../assets/shaders/Composite.frag");
	
	//LOAD THINGS ONLY NECESSARY FOR LOADING SCREEN
	defaultTexture->LoadFromFile("Logo", "../assets/textures/Logo.png");
	defaultTexture->LoadFromFile("Credits", "../assets/textures/credits.png");
	defaultTexture->LoadFromFile("White", "../assets/textures/white.png");
	defaultTexture->LoadFromFile("PyramidColour", "../assets/textures/pyramidcolour.png");
	defaultTexture->LoadFromFile("CubeColour", "../assets/textures/cgcubecolour.png");
	
	defaultMesh->LoadFromFile("Quad", "../assets/objects/Quad.obj");
	static ENG::SceneObject Quad3("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["Credits"], *defaultTexture->listOfTextures["Logo"], *defaultTexture->listOfTextures["Logo"], *defaultTexture->listOfTextures["Logo"], geometryBuffer.getLayerNumber());
	sceneObjects["Quad3"] = &Quad3;

	passThrough.bind();
	defaultTexture->listOfTextures["Logo"]->Bind(GL_TEXTURE0);
	passThrough.sendUniform("uTex", 0);

	glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	
	passThrough.unBind();

	gameWindow->GetSFMLWindow()->display();

	//SET CAMERA
	cameraPos = glm::vec3(0.0f, 25.0f, 50.0f);

	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	down = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	cameraUp = glm::cross(cameraDirection, cameraRight);

	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	
	view.setMatrix(glm::lookAt(cameraPos, (cameraPos + cameraFront), up));
	view.rotateX(-0.436f);
	
	persp = glm::perspective(45.0f, 16.0f / 9.0f, 10.0f, 100.0f);

	//orthoView = glm::mat4();
	float orthoSize = 5.0f;
	orthoPersp = glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, -100.0f, 100.0f);

	glm::mat4 ProjBiasMatrix = glm::mat4(
		2.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 2.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 2.0f, -1.0f,
		0.0f, 0.0f, 0.0f,  1.0f);

	//LOAD DEFAULT TEXTURES
	defaultTexture->LoadFromFile("Emissive", "../assets/textures/default/Emissive.png");
	defaultTexture->LoadFromFile("Specular", "../assets/textures/default/Specular.png");
	defaultTexture->LoadFromFile("Normal", "../assets/textures/default/Normal.png");

	//LOAD OBJECTS

	defaultMesh->LoadFromFile("Floor", "../assets/objects/QuadLight.obj");
	static ENG::SceneObject Floor("Floor", defaultMesh->listOfMeshes["Floor"]->VAO, *defaultTexture->listOfTextures["White"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["Floor"] = &Floor;
	
	defaultMesh->LoadFromFile("CGCube", "../assets/objects/CGCUBE.obj");
	static ENG::SceneObject CGCube("CGCube", defaultMesh->listOfMeshes["CGCube"]->VAO, *defaultTexture->listOfTextures["CubeColour"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["CGCube"] = &CGCube;

	defaultMesh->LoadFromFile("Pyramid", "../assets/objects/light_volumes/PyramidLight.obj");
	static ENG::SceneObject Pyramid("Pyramid", defaultMesh->listOfMeshes["Pyramid"]->VAO, *defaultTexture->listOfTextures["PyramidColour"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["Pyramid"] = &Pyramid;

	//LIGHT OBJECTS
	defaultMesh->LoadFromFile("Sphere", "../assets/objects/light_volumes/sphere.obj");
	defaultMesh->LoadFromFile("Cube", "../assets/objects/light_volumes/cube.obj");
	defaultMesh->LoadFromFile("Cone", "../assets/objects/light_volumes/ConeLight.obj");
	defaultMesh->LoadFromFile("Plane", "../assets/objects/light_volumes/plane.obj");

	roomLight.renderVolume = defaultMesh->listOfMeshes["Plane"]->VAO;
	roomLight2.renderVolume = defaultMesh->listOfMeshes["Plane"]->VAO;
	roomLight3.renderVolume = defaultMesh->listOfMeshes["Plane"]->VAO;

	defaultLightPos = glm::vec4(0.0f, 20.0f, 0.0f, 1.0f);

	//INITIALiZE FBOS
	geometryBuffer.Init(windowWidth, windowHeight, 5);
	geometryBuffer.initColorTexture(0, GL_RGBA8);
	geometryBuffer.initColorTexture(1, GL_RGB8);
	geometryBuffer.initColorTexture(2, GL_RGB8);
	geometryBuffer.initColorTexture(3, GL_RGB8);
	geometryBuffer.initColorTexture(4, GL_RGB8);
	geometryBuffer.initDepthTexture();

	finalSceneFBO1.Init(windowWidth, windowHeight, 1);
	finalSceneFBO1.initColorTexture(0, GL_RGB8);
	finalSceneFBO2.Init(windowWidth, windowHeight, 1);
	finalSceneFBO2.initColorTexture(0, GL_RGB8);

	bloomFBO1.Init(windowWidth/2, windowHeight/2, 1);
	bloomFBO1.initColorTexture(0);

	bloomFBO2.Init(windowWidth / 2, windowHeight / 2, 1);
	bloomFBO2.initColorTexture(0);

	motionBlurFBO.Init(windowWidth, windowHeight, 1);
	motionBlurFBO.initColorTexture(0);

	lightingStencil.Init(windowWidth, windowHeight, 1);
	lightingStencil.initColorTexture(0);
	lightingStencil.initDepthTexture();

	bokehA.Init(windowWidth, windowHeight, 1);
	bokehA.initColorTexture(0);

	bokehB.Init(windowWidth, windowHeight, 1);
	bokehB.initColorTexture(0);

	bokehHorBlur.Init(windowWidth, windowHeight, 1);
	bokehHorBlur.initColorTexture(0);
}

void Reset()
{
	//RESET ALL OBJECTS BEFORE REAPPLYING TRANSLATIONS AND BOOLEANS

	for (sceneObjects_iterator = sceneObjects.begin(); sceneObjects_iterator != sceneObjects.end(); sceneObjects_iterator++)
	{
		sceneObjects_iterator->second->reset();
	}

}

//FIRST LEVEL MAIN LOOP
void GameLevel::Update()
{
	if (!weBePausing)
	{
		if (hasBeenInitialized == false)
		{
			enter();
			hasBeenInitialized = true;
		}

	
		globalT += rampValue;

		totalTime += 1 / 60.0f;
		sessionTime += 1 / 60.0f;

		deltaTime = (totalTime - previousTime);

		previousTime = totalTime;

		defaultShader.sendUniform("uTime", sessionTime);

			//Check Window Status
			gameWindow->check();
			
			//Drawing Geometry and Player Character
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			geometryBuffer.Bind();
			GBuffer.bind();

			GBuffer.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
			GBuffer.sendUniformMat4("uProj", &persp[0][0], false);
			GBuffer.sendUniform("LightPosition", defaultLightPos);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

			for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
				itr != itrEnd; itr++)
			{
				(*itr)->update(deltaTime, totalTime);
			}

			for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
				itr != itrEnd; itr++)
			{
				(*itr)->render(defaultMesh, &GBuffer, TEXTURE_TOGGLE);
			}

			//Lighting Prepass-Deferred Lighting

			geometryBuffer.BlitDepthToTargetBuffer(&lightingStencil);
			
			lightingStencil.Bind();
			
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			
			geometryBuffer.BindColorAsTexture(GL_TEXTURE0, 0);
			geometryBuffer.BindColorAsTexture(GL_TEXTURE1, 1);
			geometryBuffer.BindColorAsTexture(GL_TEXTURE2, 2);
			geometryBuffer.BindColorAsTexture(GL_TEXTURE3, 3);
			geometryBuffer.BindColorAsTexture(GL_TEXTURE4, 4);
			geometryBuffer.BindDepthAsTexture(GL_TEXTURE5);


			deferredLighting.bind();

			viewInverse = glm::inverse(view.getMatrix());

			deferredLighting.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
			deferredLighting.sendUniformMat4("uProj", &persp[0][0], false);
			deferredLighting.sendUniformMat4("uinverseViewMatrix", &viewInverse[0][0], false);
			deferredLighting.sendUniformMat4("uinversePerspectiveMatrix", &glm::inverse(persp)[0][0], false);
			deferredLighting.sendUniform("uWindowWidth", windowWidth);
			deferredLighting.sendUniform("uWindowHeight", windowHeight);

			updatedCamPos = viewInverse[3];
			deferredLighting.sendUniform("uCameraPos", updatedCamPos);

			glDepthMask(GL_FALSE);

			for (auto itr = lightObjects.begin(), itrEnd = lightObjects.end();
				itr != itrEnd; itr++)
			{
				deferredLighting.sendUniformPointLight("uCurrentLight", (*itr));

				(*itr)->render(defaultMesh, &deferredLighting);
			}

			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);

			deferredLighting.unBind();
			lightingStencil.Unbind();

			ENG::Texture::UnBind(GL_TEXTURE0);
			ENG::Texture::UnBind(GL_TEXTURE1);
			ENG::Texture::UnBind(GL_TEXTURE2);
			ENG::Texture::UnBind(GL_TEXTURE3);
			ENG::Texture::UnBind(GL_TEXTURE4);
			ENG::Texture::UnBind(GL_TEXTURE5);
			
			//Lighting Composite

			finalSceneFBO1.Bind();
			lightingComposite.bind();

			viewInverse = glm::inverse(view.getMatrix());

			lightingComposite.sendUniform("uWindowWidth", windowWidth);
			lightingComposite.sendUniform("uWindowHeight", windowHeight);

			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

			geometryBuffer.BindColorAsTexture(GL_TEXTURE0, 0);
			if (LIGHTING_TOGGLE)
			{
				lightingStencil.BindColorAsTexture(GL_TEXTURE1, 0);
				geometryBuffer.BindColorAsTexture(GL_TEXTURE2, 3);
			}
			else
				geometryBuffer.BindColorAsTexture(GL_TEXTURE2, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);

			lightingComposite.unBind();
			glViewport(0, 0, windowWidth / 2, windowHeight / 2);
			finalSceneFBO1.Unbind();

			bloomFBO1.Bind();
			bloomHighPass.bind();
			
			bloomHighPass.sendUniform("uScene", 0);
			
			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
			
			finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
			
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			glBindVertexArray(0);
			
			bloomHighPass.unBind();
			bloomFBO1.Unbind();
			
			//Looping Blur Passes
			if (BLUR_TOGGLE)
			{
				for (int i = 0; i < int(sinf(totalTime *2.0f) * 10 + 11); i++)
				{
					bloomFBO2.Bind();
					bloomHorizontalBlur.bind();

					bloomHorizontalBlur.sendUniform("texelWidth", (2.0f / windowWidth));

					glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

					bloomFBO1.BindColorAsTexture(GL_TEXTURE0, 0);

					glDrawArrays(GL_TRIANGLES, 0, 6);

					glBindVertexArray(0);

					bloomHorizontalBlur.unBind();
					bloomFBO2.Unbind();

					bloomFBO1.Bind();
					bloomVerticalBlur.bind();

					bloomVerticalBlur.sendUniform("texelHeight", (2.0f / windowHeight));

					glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

					bloomFBO2.BindColorAsTexture(GL_TEXTURE0, 0);

					glDrawArrays(GL_TRIANGLES, 0, 6);

					glBindVertexArray(0);

					bloomVerticalBlur.unBind();
					bloomFBO1.Unbind();
				}
			}

			glViewport(0, 0, windowWidth, windowHeight);
			finalSceneFBO2.Bind();
			bloomComposite.bind();
			bloomComposite.sendUniform("uScene", 0);
			
			finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
			
			bloomComposite.sendUniform("uBloomScene", 1);
			
			bloomFBO1.BindColorAsTexture(GL_TEXTURE1, 0);
			
			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
			
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			glBindVertexArray(0);
			
			bloomComposite.unBind();
			finalSceneFBO2.Unbind();
		
			//Copy Scene for next use
			motionBlurFBO.Bind();
			passThrough.bind();
			finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
			passThrough.sendUniform("uTex", 0);

			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			passThrough.unBind();
			motionBlurFBO.Unbind();

			//Drawing HUD
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			UVScrolling.bind();
			finalSceneFBO1.Bind();

			UVScrolling.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
			UVScrolling.sendUniformMat4("uProj", &orthoPersp[0][0], false);
			UVScrolling.sendUniform("LightPosition", down);

			UVScrolling.unBind();

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			finalSceneFBO1.Unbind();

			finalSceneFBO2.Bind();
			colorCorrect.bind();

			finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);

			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			colorCorrect.unBind();
			finalSceneFBO2.Unbind();
			
			finalSceneFBO1.Bind();
			colorCorrect.bind();

			finalSceneFBO2.BindColorAsTexture(GL_TEXTURE0, 0);
			colorCorrect.sendUniform("t", 0.0f);

			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			colorCorrect.unBind();
			finalSceneFBO1.Unbind();
			
			finalSceneFBO1.DrawToBackBuffer();
		}

	gameWindow->GetSFMLWindow()->display();

	ENG::Input::ResetKeys();
}

GameLevel::GameLevel()
{
	hasBeenInitialized = false;
}

void GameLevel::enter()
{

	seconds = 0.0f;
	minute = 0.0f;

	//POSITION OBJECTS
	collidables.clear();
	lightObjects.clear();
	Reset();

	gObjects.push_back(sceneObjects["Floor"]);
	sceneObjects["Floor"]->setPosition(glm::vec3(0.0f, 0.0f, -20.0f));

	gObjects.push_back(sceneObjects["CGCube"]);
	sceneObjects["CGCube"]->setPosition(glm::vec3(0.0f, 3.0f, -10.0f));
	sceneObjects["CGCube"]->uScale = glm::vec3(7.0f);

	gObjects.push_back(sceneObjects["Pyramid"]);
	sceneObjects["Pyramid"]->setPosition(glm::vec3(0.0f, 10.0f, -15.0f));
	sceneObjects["Pyramid"]->uScale = glm::vec3(0.2f, 0.5f, 0.2f);

	lightObjects.push_back(&roomLight);
	lightObjects.push_back(&roomLight2);
	lightObjects.push_back(&roomLight3);
	
	if (!hasLoadedOnce)
	{
		roomLight.position = glm::vec3(0.0f, 25.0f, 0.0f);
		roomLight.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		roomLight.aConstant = roomLight.aConstant * 0.1f;
		roomLight.aLinear = roomLight.aLinear * 0.1f;
		roomLight.aQuadratic = roomLight.aQuadratic * 0.1f;
		roomLight.meshName = "Pyramid";

		roomLight2.position = glm::vec3(35.0f, 25.0f, 0.0f);
		roomLight2.color = glm::vec4(0.0f, 1.0f, 1.0f, 2.0f);
		roomLight2.aConstant = roomLight2.aConstant * 0.1f;
		roomLight2.aLinear = roomLight2.aLinear * 0.1f;
		roomLight2.aQuadratic = roomLight2.aQuadratic * 0.1f;
		roomLight2.meshName = "Pyramid";

		roomLight3.position = glm::vec3(-35.0f, 25.0f, 0.0f);
		roomLight3.color = glm::vec4(1.0f, 0.0f, 0.0f, 2.0f);
		roomLight3.aConstant = roomLight3.aConstant * 0.1f;
		roomLight3.aLinear = roomLight3.aLinear * 0.1f;
		roomLight3.aQuadratic = roomLight3.aQuadratic * 0.1f;
		roomLight3.meshName = "Pyramid";

		hasLoadedOnce = true;
	}
}

void removeGameObjects()
{
	gObjects.clear();
	transparentGObjects.clear();
}