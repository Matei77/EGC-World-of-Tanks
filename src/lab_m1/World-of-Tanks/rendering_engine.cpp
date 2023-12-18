#include "rendering_engine.h"

#include <iostream>

#include "building.h"
#include "building.h"
#include "constants.h"
#include "utils.h"
#include "components/camera.h"
#include "components/camera.h"

using namespace world_of_tanks;

// ------------------ Initialization ------------------

void RenderingEngine::Init() {
    InitCamera();
    LoadMeshes();
    CreateShader();
    logic_engine_.Init();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderingEngine::InitCamera() {
    camera_ = new Camera(CAMERA_OFFSET, logic_engine_.GetPlayerTank().GetPosition(), ABSOLUTE_UP);
    camera_->SetProjectionMatrix(window->props.aspectRatio);
}

void RenderingEngine::LoadMeshes() {
    {
        Mesh *mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("tracks");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "tracks.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("terrain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("building");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("game_over");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "game-over.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}

void RenderingEngine::SetViewport() {
    // Sets the screen area where to draw
    const glm::ivec2 resolution = window->GetResolution();
    viewport_.width = resolution.x;
    viewport_.height = resolution.y;
    viewport_.x = 0;
    viewport_.y = 0;

    if (resolution.x * ASPECT_HEIGHT > resolution.y * ASPECT_WIDTH) {
        viewport_.width = resolution.y * ASPECT_WIDTH / ASPECT_HEIGHT;
        viewport_.x = (resolution.x - viewport_.width) / 2;
    }
    else if (resolution.x * ASPECT_HEIGHT < resolution.y * ASPECT_WIDTH) {
        viewport_.height = resolution.x * ASPECT_HEIGHT / ASPECT_WIDTH;
        viewport_.y = (resolution.y - viewport_.height) / 2;
    }

    glViewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height);

    glScissor(viewport_.x, viewport_.y, viewport_.width, viewport_.height);
}


// --------------------- Shaders ----------------------

void RenderingEngine::CreateShader() {
    Shader *shader = new Shader("world_of_tanks_shader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "World-of-Tanks", "shaders",
                                "vertex_shader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "World-of-Tanks", "shaders",
                                "fragment_shader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void RenderingEngine::CustomRenderMesh(const Mesh *mesh, const Shader *shader, const glm::mat4 &model_matrix,
                                       const glm::vec3 &color, const int hp = TANK_HEALTH,
                                       const float alpha = 1.0f) const {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    const int alpha_loc = glGetUniformLocation(shader->program, "alpha");
    glUniform1f(alpha_loc, alpha);

    const int object_color_loc = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_color_loc, 1, glm::value_ptr(color));

    const int hp_loc = glGetUniformLocation(shader->program, "hp");
    glUniform1i(hp_loc, hp);

    const int max_hp_loc = glGetUniformLocation(shader->program, "max_hp");
    glUniform1i(max_hp_loc, TANK_HEALTH);

    // Bind model matrix
    const int model_matrix_loc = glGetUniformLocation(shader->program, "model");
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    // Bind view matrix
    glm::mat4 view_matrix = camera_->GetViewMatrix();
    const int view_matrix_loc = glGetUniformLocation(shader->program, "view");
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix));

    // Bind projection matrix
    glm::mat4 projection_matrix = camera_->GetProjectionMatrix();
    const int projection_matrix_loc = glGetUniformLocation(shader->program, "projection");
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection_matrix));


    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


// ---------------------- Frame -----------------------

void RenderingEngine::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(SCREEN_CUT_COLOR.r, SCREEN_CUT_COLOR.g, SCREEN_CUT_COLOR.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetViewport();

    glClearColor(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, 1);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void RenderingEngine::Update(const float delta_time_seconds) {
    if (logic_engine_.IsGameOver() == true) {
        RenderGameOver();
        return;
    }
    RenderScene();
    logic_engine_.Update(delta_time_seconds, camera_);
}

void RenderingEngine::FrameEnd() {
    // DrawCoordinateSystem(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
}


// ---------------------- Inputs ----------------------

void RenderingEngine::OnInputUpdate(float delta_time, int mods) {
    // Player Movement - update player tank position and rotation
    Tank &player_tank = logic_engine_.GetPlayerTank();
    if (window->KeyHold(GLFW_KEY_W) && window->KeyHold(GLFW_KEY_SPACE)) {
        glm::vec3 camera_pos = camera_->GetPosition();
        glm::vec3 position = player_tank.GetPosition();

        position.x -= TANK_SPEED * 3 * delta_time * sin(player_tank.GetBodyRotation());
        position.z -= TANK_SPEED * 3 * delta_time * cos(player_tank.GetBodyRotation());
        player_tank.SetPosition(position);

        camera_pos.x -= TANK_SPEED * 3 * delta_time * sin(player_tank.GetBodyRotation());
        camera_pos.z -= TANK_SPEED * 3 * delta_time * cos(player_tank.GetBodyRotation());
        camera_->SetPosition(camera_pos);

        // const float distance = glm::distance(old_position,  position);
        // camera_->MoveForward(distance);
    }
    else if (window->KeyHold(GLFW_KEY_W)) {
        glm::vec3 camera_pos = camera_->GetPosition();
        glm::vec3 position = player_tank.GetPosition();

        position.x -= TANK_SPEED * delta_time * sin(player_tank.GetBodyRotation());
        position.z -= TANK_SPEED * delta_time * cos(player_tank.GetBodyRotation());
        player_tank.SetPosition(position);

        camera_pos.x -= TANK_SPEED * delta_time * sin(player_tank.GetBodyRotation());
        camera_pos.z -= TANK_SPEED * delta_time * cos(player_tank.GetBodyRotation());
        camera_->SetPosition(camera_pos);

        // const float distance = glm::distance(old_position,  position);
        // camera_->MoveForward(distance);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        glm::vec3 camera_pos = camera_->GetPosition();
        glm::vec3 position = player_tank.GetPosition();

        position.x += TANK_SPEED * delta_time * sin(player_tank.GetBodyRotation());
        position.z += TANK_SPEED * delta_time * cos(player_tank.GetBodyRotation());
        player_tank.SetPosition(position);

        camera_pos.x += TANK_SPEED * delta_time * sin(player_tank.GetBodyRotation());
        camera_pos.z += TANK_SPEED * delta_time * cos(player_tank.GetBodyRotation());
        camera_->SetPosition(camera_pos);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        player_tank.SetBodyRotation(player_tank.GetBodyRotation() + 1 * delta_time);
        camera_->RotateThirdOY(TANK_ROTATION_SPEED * delta_time, player_tank.GetPosition());
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        player_tank.SetBodyRotation(player_tank.GetBodyRotation() - 1 * delta_time);
        camera_->RotateThirdOY(-TANK_ROTATION_SPEED * delta_time, player_tank.GetPosition());
    }
}

void RenderingEngine::OnKeyPress(int key, int mods) {}

void RenderingEngine::OnKeyRelease(int key, int mods) {}

void RenderingEngine::OnMouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) {
    // std::cout << "-----------------------------\n";
    // std::cout << "mouse_x: " << mouse_x << '\n';
    // std::cout << "mouse_y: " << mouse_y << '\n';
    // std::cout << "delta_x: " << delta_x << '\n';
    // std::cout << "delta_y: " << delta_y << '\n';


    Tank &player_tank = logic_engine_.GetPlayerTank();

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        camera_->RotateThirdOY(-delta_x * MOUSE_SENSITIVITY, logic_engine_.GetPlayerTank().GetPosition());

        player_tank.SetTurretRotation(player_tank.GetTurretRotation() - delta_x * MOUSE_SENSITIVITY);

        camera_->SetRotationAngleOy(camera_->GetRotationAngleOy() - delta_x * MOUSE_SENSITIVITY);
    }
    else {
        float turret_rotation = utils::liner_conversion(mouse_x, viewport_.x, viewport_.x + viewport_.width,
                                                        RADIANS(90), RADIANS(-90));

        player_tank.SetTurretRotation(camera_->GetRotationAngleOy() + turret_rotation);
    }
}

void RenderingEngine::OnMouseBtnPress(int mouse_x, int mouse_y, int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_2) {
        // std::cout << logic_engine_.GetPlayerTank().GetReloadTimer() << "";
        if (logic_engine_.GetPlayerTank().GetReloadTimer().IsFinished()) {
            logic_engine_.GetPlayerTank().FireProjectile(logic_engine_.GetProjectiles(), PROJECTILE_SPEED);
            logic_engine_.GetPlayerTank().GetReloadTimer().ResetTimer();
        }

        // std::cout << "Projectiles UPTIME:\n";
        // for (auto projectile : logic_engine_.GetProjectiles())
        //     std::cout << projectile.GetTime() << " ";
        //
        // std::cout << "\n";
    }
}

void RenderingEngine::OnMouseBtnRelease(int mouse_x, int mouse_y, int button, int mods) {}

void RenderingEngine::OnMouseScroll(int mouse_x, int mouse_y, int offset_x, int offset_y) {}

void RenderingEngine::OnWindowResize(int width, int height) {
    // Tank &player_tank = logic_engine_.GetPlayerTank();
    // float turret_rotation = player_tank.GetTurretRotation();
    //
    // turret_rotation += -MOUSE_SENSITIVITY * (width - window->GetResolution().x);
    // player_tank.SetTurretRotation(turret_rotation);
}


// -------------------- Rendering ---------------------

void RenderingEngine::RenderScene() {
    RenderTanks();
    RenderProjectiles();
    RenderMap();
}

void RenderingEngine::RenderTanks() {
    // Render player tank
    Tank player_tank = logic_engine_.GetPlayerTank();
    {
        // Render body
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation(), glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["body"], shaders["world_of_tanks_shader"], model_matrix, TANK_BODY_COLOR,
                         player_tank.GetHealth());
    }

    {
        // Render tracks
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation(), glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["tracks"], shaders["world_of_tanks_shader"], model_matrix, TANK_TRACKS_COLOR,
                         player_tank.GetHealth());
    }

    {
        // Render turret
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation() + player_tank.GetTurretRotation(),
                                   glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["turret"], shaders["world_of_tanks_shader"], model_matrix, TANK_TURRET_COLOR,
                         player_tank.GetHealth());
    }

    {
        // Render cannon
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation() + player_tank.GetTurretRotation(),
                                   glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["cannon"], shaders["world_of_tanks_shader"], model_matrix, TANK_CANNON_COLOR,
                         player_tank.GetHealth());
    }

    // Render enemy tanks
    for (auto enemy_tank : logic_engine_.GetEnemyTanks()) {
        {
            // Render body
            glm::mat4 model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, enemy_tank.GetPosition());
            model_matrix = glm::rotate(model_matrix, enemy_tank.GetBodyRotation(), glm::vec3(0, 1, 0));
            CustomRenderMesh(meshes["body"], shaders["world_of_tanks_shader"], model_matrix, ENEMY_TANK_BODY_COLOR,
                             enemy_tank.GetHealth());
        }

        {
            // Render tracks
            glm::mat4 model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, enemy_tank.GetPosition());
            model_matrix = glm::rotate(model_matrix, enemy_tank.GetBodyRotation(), glm::vec3(0, 1, 0));
            CustomRenderMesh(meshes["tracks"], shaders["world_of_tanks_shader"], model_matrix, ENEMY_TANK_TRACKS_COLOR,
                             enemy_tank.GetHealth());
        }

        {
            // Render turret
            glm::mat4 model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, enemy_tank.GetPosition());
            model_matrix = glm::rotate(model_matrix, enemy_tank.GetBodyRotation() + enemy_tank.GetTurretRotation(),
                                       glm::vec3(0, 1, 0));
            CustomRenderMesh(meshes["turret"], shaders["world_of_tanks_shader"], model_matrix, ENEMY_TANK_TURRET_COLOR,
                             enemy_tank.GetHealth());
        }

        {
            // Render cannon
            glm::mat4 model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, enemy_tank.GetPosition());
            model_matrix = glm::rotate(model_matrix, enemy_tank.GetBodyRotation() + enemy_tank.GetTurretRotation(),
                                       glm::vec3(0, 1, 0));
            CustomRenderMesh(meshes["cannon"], shaders["world_of_tanks_shader"], model_matrix, ENEMY_TANK_CANNON_COLOR,
                             enemy_tank.GetHealth());
        }
    }
}

void RenderingEngine::RenderMap() {
    // Render ground
    for (int i = -GRID_NUMBER / 2; i <= GRID_NUMBER / 2; i++)
        for (int j = -GRID_NUMBER / 2; j <= GRID_NUMBER / 2; j++) {
            glm::mat4 model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, glm::vec3(4 * i, -0.51f, 4 * j));
            model_matrix = glm::scale(model_matrix, glm::vec3(0.08f, 1, 0.08f));
            if ((i + j) % 2 == 0) {
                CustomRenderMesh(meshes["terrain"], shaders["world_of_tanks_shader"], model_matrix, TERRAIN_COLOR_1);
            }
            else {
                CustomRenderMesh(meshes["terrain"], shaders["world_of_tanks_shader"], model_matrix, TERRAIN_COLOR_2);
            }
        }

    // Render buildings
    glm::vec3 camera_pos = camera_->GetPosition();
    std::vector<Building> buildings = logic_engine_.GetMap().GetBuildings();
    std::sort(buildings.begin(), buildings.end(),
              [&camera_pos](const Building &b1, const Building &b2) {
                  return glm::distance(b1.GetPosition(), camera_pos) > glm::distance(
                      b2.GetPosition(), camera_pos);
              });

    for (auto building : buildings) {
        float closest_x = std::max(building.GetPosition().x - building.GetHalfWidth(),
                                   std::min(camera_->GetPosition().x,
                                            building.GetPosition().x + building.GetHalfWidth()));
        float closest_z = std::max(building.GetPosition().z - building.GetHalfLength(),
                                   std::min(camera_->GetPosition().z,
                                            building.GetPosition().z + building.GetHalfLength()));

        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, building.GetPosition());
        model_matrix = glm::scale(model_matrix,
                                  glm::vec3(building.GetHalfWidth() * 2, building.GetHalfHeight() * 2,
                                            building.GetHalfLength() * 2));

        if (!(closest_x == camera_->GetPosition().x && closest_z == camera_->GetPosition().z)) {
            CustomRenderMesh(meshes["building"], shaders["world_of_tanks_shader"], model_matrix, BUILDING_COLOR);
        }
        else {
            CustomRenderMesh(meshes["building"], shaders["world_of_tanks_shader"], model_matrix, BUILDING_COLOR,
                             TANK_HEALTH, 0.5f);
        }
    }
}

void RenderingEngine::RenderProjectiles() {
    for (auto projectile : logic_engine_.GetProjectiles()) {
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, projectile.GetPosition());
        model_matrix = glm::scale(model_matrix, glm::vec3(0.2f, 0.2f, 0.2f));
        CustomRenderMesh(meshes["projectile"], shaders["world_of_tanks_shader"], model_matrix, PROJECTILE_COLOR);
    }
}

void RenderingEngine::RenderGameOver() {
    glm::mat4 model_matrix = glm::mat4(1);
    model_matrix = glm::translate(model_matrix, logic_engine_.GetPlayerTank().GetPosition());
    model_matrix = glm::rotate(model_matrix, logic_engine_.GetPlayerTank().GetBodyRotation(), glm::vec3(0, 1, 0));
    model_matrix = glm::rotate(model_matrix, RADIANS(25.0f), glm::vec3(1, 0, 0));
    CustomRenderMesh(meshes["game_over"], shaders["world_of_tanks_shader"], model_matrix, PROJECTILE_COLOR);
}
