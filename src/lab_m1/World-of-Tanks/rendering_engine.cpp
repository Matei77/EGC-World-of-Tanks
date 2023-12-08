#include "rendering_engine.h"

#include <iostream>

#include "constants.h"

using namespace world_of_tanks;

// ------------------ Initialization ------------------

void RenderingEngine::Init() {
    InitCamera();
    LoadMeshes();
    CreateShader();
}

void RenderingEngine::InitCamera() {
    camera_ = new Camera(CAMERA_OFFSET, logic_engine_.GetPlayerTank().GetPosition(), ABSOLUTE_UP);
    camera_->SetProjectionMatrix(window->props.aspectRatio);
}

void RenderingEngine::LoadMeshes() {
    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "body.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("tracks");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "tracks.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cannon");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "world_of_tanks"), "cannon.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("terrain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh *mesh = new Mesh("projectile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}


// --------------------- Shaders ----------------------

void RenderingEngine::CreateShader() {
    Shader *shader = new Shader("world_of_tanks_shader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "World-of-Tanks", "shaders", "vertex_shader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "World-of-Tanks", "shaders", "fragment_shader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void RenderingEngine::CustomRenderMesh(const Mesh *mesh, const Shader *shader, const glm::mat4 &model_matrix,
    const glm::vec3 &color) const {
    
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);
    
    const int object_color_loc = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_color_loc, 1, glm::value_ptr(color));

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
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    const glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void RenderingEngine::Update(const float delta_time_seconds) {
    RenderScene();
    logic_engine_.Update(delta_time_seconds);
}

void RenderingEngine::FrameEnd() {
    // DrawCoordinateSystem(camera_->GetViewMatrix(), camera_->GetProjectionMatrix());
}


// ---------------------- Inputs ----------------------

void RenderingEngine::OnInputUpdate(float delta_time, int mods) {
    // Player Movement - update player tank position and rotation
    Tank &player_tank = logic_engine_.GetPlayerTank();
    if (window->KeyHold(GLFW_KEY_W)) {
        const glm::vec3 old_position = player_tank.GetPosition();
        glm::vec3 position = player_tank.GetPosition();
        
        position.x -= TANK_SPEED * delta_time * sin(player_tank.GetBodyRotation());
        position.z -= TANK_SPEED * delta_time * cos(player_tank.GetBodyRotation());
        player_tank.SetPosition(position);
        
        const float distance = glm::distance(old_position,  position);
        camera_->MoveForward(distance);
    }
    
    if (window->KeyHold(GLFW_KEY_S)) {
        const glm::vec3 old_position = player_tank.GetPosition();
        glm::vec3 position = player_tank.GetPosition();
        
        position.x += TANK_SPEED * delta_time * sin(player_tank.GetBodyRotation());
        position.z += TANK_SPEED * delta_time * cos(player_tank.GetBodyRotation());
        player_tank.SetPosition(position);
        
        const float distance = glm::distance(old_position,  position);
        camera_->MoveForward(-distance);
    }
    
    if (window->KeyHold(GLFW_KEY_A)) {
        player_tank.SetBodyRotation(player_tank.GetBodyRotation() + 1 * delta_time);
        camera_->RotateThirdOY(1 * delta_time, player_tank.GetPosition());
    }
    
    if (window->KeyHold(GLFW_KEY_D)) {
        player_tank.SetBodyRotation(player_tank.GetBodyRotation() - 1 * delta_time);
        camera_->RotateThirdOY(-1 * delta_time, player_tank.GetPosition());
    }
}

void RenderingEngine::OnKeyPress(int key, int mods) {}

void RenderingEngine::OnKeyRelease(int key, int mods) {}

void RenderingEngine::OnMouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) {}

void RenderingEngine::OnMouseBtnPress(int mouse_x, int mouse_y, int button, int mods) {
    if (button == GLFW_MOUSE_BUTTON_2) {
        // std::cout << logic_engine_.GetPlayerTank().GetReloadTimer() << "";
        if (logic_engine_.GetPlayerTank().GetReloadTimer() <= 0.0f) {
            logic_engine_.GetPlayerTank().FireProjectile(logic_engine_.GetProjectiles());
            logic_engine_.GetPlayerTank().SetReloadTimer(DEFAULT_RELOAD_TIMER);
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

void RenderingEngine::OnWindowResize(int width, int height) {}


// -------------------- Rendering ---------------------

void RenderingEngine::RenderScene() {
    RenderMap();
    RenderTanks();
    RenderProjectiles();
}

void RenderingEngine::RenderTanks() {
    Tank player_tank = logic_engine_.GetPlayerTank();

    {
        // Render body
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation(), glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["body"], shaders["world_of_tanks_shader"], model_matrix, TANK_BODY_COLOR);
    }
    
    {
        // Render tracks
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation(), glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["tracks"], shaders["world_of_tanks_shader"], model_matrix, TANK_TRACKS_COLOR);
    }

    {
        // Render turret
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation() + player_tank.GetTurretRotation(), glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["turret"], shaders["world_of_tanks_shader"], model_matrix, TANK_TURRET_COLOR);
    }

    {
        // Render cannon
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, player_tank.GetPosition());
        model_matrix = glm::rotate(model_matrix, player_tank.GetBodyRotation() + player_tank.GetTurretRotation(), glm::vec3(0, 1, 0));
        CustomRenderMesh(meshes["cannon"], shaders["world_of_tanks_shader"], model_matrix, TANK_CANNON_COLOR);
    }
}

void RenderingEngine::RenderMap() {
    // Render ground
    for (int i = -10; i <= 10; i++)
        for (int j = -10; j <= 10; j++) {
            glm::mat4 model_matrix = glm::mat4(1);
            model_matrix = glm::translate(model_matrix, glm::vec3(4 * i, -0.51f, 4 * j));
            model_matrix = glm::scale(model_matrix, glm::vec3(0.08f, 1, 0.08f));
            if ((i + j) % 2 == 0) {
                CustomRenderMesh(meshes["terrain"], shaders["world_of_tanks_shader"], model_matrix, TERRAIN_COLOR_1);
            } else {
                CustomRenderMesh(meshes["terrain"], shaders["world_of_tanks_shader"], model_matrix, TERRAIN_COLOR_2);
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
