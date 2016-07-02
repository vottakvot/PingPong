#include "Main.h"

void Main::init() {
    // Polygons
    polygons = MakeShaders::createProgram(MakeShaders::v_main_shader, MakeShaders::f_main_shader);
    polygonsPositionAttr = glGetAttribLocation(polygons, "a_Position");
    checkGLError("Main::init - a_Position");
    polygonsTextureAttr = glGetAttribLocation(polygons, "a_Texture");
    checkGLError("Main::init - a_Texture");
    polygonsTransformationAttr = glGetUniformLocation(polygons, "u_Matrix");
    checkGLError("Main::init - u_Matrix");

    // Sprites
    sprites = MakeShaders::createProgram(MakeShaders::v_point_shader, MakeShaders::f_point_shader);
    spritesPosition = glGetAttribLocation(sprites, "a_Position");
    checkGLError("Main::init - a_Position");
    spritesDelta = glGetAttribLocation(sprites, "a_Delta");
    checkGLError("Main::init - a_Delta");
    spritesSize = glGetUniformLocation(sprites, "u_Size");
    checkGLError("Main::init - u_Size");
    spritesColorStart = glGetAttribLocation(sprites, "a_ColorStart");
    checkGLError("Main::init - a_ColorStart");
    spritesColorEnd = glGetAttribLocation(sprites, "a_ColorEnd");
    checkGLError("Main::init - a_ColorEnd");

    //On alfa-blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGLError("Main::init - glBlendFunc");
    glEnable(GL_BLEND);
    checkGLError("Main::init - glEnable");

    // Set viewport
    glViewport(0, 0, width, height);
    checkGLError("Main::init - glViewport");
}

void Main::createObjects(){

    matrix = new Matrix();
    textures = new ManageTexture(env, pngManager, assetManager);
    particles = new Particles(1000,
                              sprites,
                              textures->getTexturesPackIDs(ManageTexture::STAR),
                              spritesPosition,
                              spritesDelta,
                              spritesColorStart,
                              spritesColorEnd,
                              spritesSize);

    // Background image
    background = new View(textures->getTexturesPackIDs(ManageTexture::BACKGROUND),
                          polygons,
                          polygonsPositionAttr,
                          polygonsTextureAttr,
                          polygonsTransformationAttr,
                          matrix->getDefaultVerticesCoords(),
                          matrix->getDefaultTextureCoord(),
                          matrix->getDefaultMatrix4x4());

    // Background center image
    center = new View(textures->getTexturesPackIDs(ManageTexture::BACKGROUND_CENTER),
                      polygons,
                      polygonsPositionAttr,
                      polygonsTextureAttr,
                      polygonsTransformationAttr,
                      matrix->getDefaultVerticesCoords(),
                      matrix->getDefaultTextureCoord(),
                      matrix->getDefaultMatrix4x4());

    // Bord down
    bordDown = new View(textures->getTexturesPackIDs(ManageTexture::BORD_DOWN),
                        polygons,
                        polygonsPositionAttr,
                        polygonsTextureAttr,
                        polygonsTransformationAttr,
                        setBordDownPosition(matrix->getDefaultVerticesCoords()),
                        matrix->getDefaultTextureCoord(),
                        matrix->getDefaultMatrix4x4());



    // Bord up
    bordUp = new View(textures->getTexturesPackIDs(ManageTexture::BORD_UP),
                        polygons,
                        polygonsPositionAttr,
                        polygonsTextureAttr,
                        polygonsTransformationAttr,
                        setBordUpPosition(matrix->getDefaultVerticesCoords()),
                        matrix->getDefaultTextureCoord(),
                        matrix->getDefaultMatrix4x4());

    // Left button
    left = new Button(true,
                      -0.95f, -0.75f, 0.7f, 0.2f,
                      textures->getTexturesPackIDs(ManageTexture::BUTTONS),
                      polygons,
                      polygonsPositionAttr,
                      polygonsTextureAttr,
                      polygonsTransformationAttr,
                      matrix->getDefaultVerticesCoords(),
                      Matrix::setTextureCoords(matrix->getDefaultTextureCoord(), 2, 2, Matrix::ONE),
                      matrix->getDefaultMatrix4x4());

    // Right button
    right = new Button(true,
                       0.3f, -0.75f, 0.7f, 0.2f,
                       textures->getTexturesPackIDs(ManageTexture::BUTTONS),
                       polygons,
                       polygonsPositionAttr,
                       polygonsTextureAttr,
                       polygonsTransformationAttr,
                       matrix->getDefaultVerticesCoords(),
                       Matrix::setTextureCoords(matrix->getDefaultTextureCoord(),  2, 2, Matrix::ONE),
                       matrix->getDefaultMatrix4x4());

    // Play pause button
    playPause = new Button(false,
                           -0.22f, -0.75f, 0.5f, 0.2f,
                           textures->getTexturesPackIDs(ManageTexture::BUTTONS),
                           polygons,
                           polygonsPositionAttr,
                           polygonsTextureAttr,
                           polygonsTransformationAttr,
                           matrix->getDefaultVerticesCoords(),
                           Matrix::setTextureCoords(matrix->getDefaultTextureCoord(), 2, 2, Matrix::THREE),
                           matrix->getDefaultMatrix4x4());

    // Label of speed
    speed = new Label(  matrix,
                        textures->getTexturesPackIDs(ManageTexture::NUMBERS),
                        polygons,
                        polygonsPositionAttr,
                        polygonsTextureAttr,
                        polygonsTransformationAttr,
                        "000",
                        -0.5f, 0.95f, 1.0f, 0.1f);

    // Field
    field = new Platform( deltaStepPlatforms,
                           -1.0f, 0.8f, 2.0f, 1.6f,
                           textures->getTexturesPackIDs(ManageTexture::OBJECTS),
                           polygons,
                           polygonsPositionAttr,
                           polygonsTextureAttr,
                           polygonsTransformationAttr,
                           matrix->getDefaultVerticesCoords(),
                           matrix->getDefaultTextureCoord(),
                           matrix->getDefaultMatrix4x4());

    field->setVisible(false);


    // Player platform
    player = new Platform( deltaStepPlatforms,
                           -0.25, -0.4f, 0.7f, 0.1f,
                           textures->getTexturesPackIDs(ManageTexture::OBJECTS),
                           polygons,
                           polygonsPositionAttr,
                           polygonsTextureAttr,
                           polygonsTransformationAttr,
                           matrix->getDefaultVerticesCoords(),
                           Matrix::setTextureCoords(matrix->getDefaultTextureCoord(), 2, 2, Matrix::ONE),
                           matrix->getDefaultMatrix4x4());

    // Bot platform
    bot = new Platform( deltaStepPlatforms,
                           -0.25, 0.7f, 0.7f, 0.1f,
                           textures->getTexturesPackIDs(ManageTexture::OBJECTS),
                           polygons,
                           polygonsPositionAttr,
                           polygonsTextureAttr,
                           polygonsTransformationAttr,
                           matrix->getDefaultVerticesCoords(),
                           Matrix::setTextureCoords(matrix->getDefaultTextureCoord(), 2, 2, Matrix::TWO),
                           matrix->getDefaultMatrix4x4());

    // Bot platform
    ball = new Ball( deltaStepBall,
                    0.0, 0.0f, 0.2f, 0.2f,
                    textures->getTexturesPackIDs(ManageTexture::OBJECTS),
                    polygons,
                    polygonsPositionAttr,
                    polygonsTextureAttr,
                    polygonsTransformationAttr,
                    matrix->getDefaultVerticesCoords(),
                    Matrix::setTextureCoords(matrix->getDefaultTextureCoord(), 2, 2, Matrix::THREE),
                    matrix->getDefaultMatrix4x4());
}

void Main::step(){
    // Clear screen
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    checkGLError("Main::step - glClearColor");
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGLError("Main::step - glClear");

    // Logic
    logic();

    // Draw
    graphicInterface();
}

void Main::graphicInterface(){
    // Draw something under
    // Background
    background->render();

    // Rotate background
    rotateBackground();

    // Particles
    particles->render();

    // Bord down
    bordDown->render();

    // Bord up
    bordUp->render();

    // Speed
    speed->render();

    // Button left
    left->render();

    // Button right
    right->render();

    // Button playPause
    playPause->render();

    // Player
    player->render();

    // Bot
    bot->render();

    // Ball
    ball->render();
}

void Main::logic(){
    if(left->getState() && !player->collision(field)){
        player->setDx(-1.0f * player->getStep());
    } else if(right->getState() && !player->collision(field)){
                player->setDx(player->getStep());
            } else {
                    player->setDx(0.0f);
                    player->getCrossPoints()->clear();
                }

    ball->collision(player);
    ball->collision(bot);
    ball->collision(field);
    ball->move();
}

void Main::rotateBackground(){
    if(deltaRotate >= 3.14f)
        deltaRotate = -3.14f;

    center->setTransformationMatrix(Matrix::setRotateMatrix4x4(Matrix::setRotateMatrix4x4(center->getTransformationMatrix(), deltaRotate += 0.01f, Matrix::X), deltaRotate, Matrix::Z));
    center->render();
}

GLfloat * Main::setBordDownPosition(GLfloat * positionCoords){
    positionCoords[1] = -0.6f;
    positionCoords[7] = -0.6f;
    return positionCoords;
}

GLfloat * Main::setBordUpPosition(GLfloat * positionCoords){
    positionCoords[3] = 0.8f;
    positionCoords[5] = 0.8f;
    return positionCoords;
}

