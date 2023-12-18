#pragma once

#include "camera.h"
#include "logic_engine.h"
#include "utils.h"
#include "components/simple_scene.h"

namespace world_of_tanks {
	class RenderingEngine final : public gfxc::SimpleScene {
	private:
		LogicEngine logic_engine_;
		Camera *camera_;
		utils::Viewport viewport_;
    public:
        RenderingEngine() = default;
        
        void Init() override;
		void InitCamera();
		void LoadMeshes();
        void CreateShader();

    private:
        void FrameStart() override;
		void SetViewport();
		void Update(float delta_time_seconds) override;
        void RenderTanks();
		void RenderMap();
		void RenderProjectiles();
		void RenderGameOver();
		void FrameEnd() override;

        void OnInputUpdate(float delta_time, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) override;
        void OnMouseBtnPress(int mouse_x, int mouse_y, int button, int mods) override;
        void OnMouseBtnRelease(int mouse_x, int mouse_y, int button, int mods) override;
        void OnMouseScroll(int mouse_x, int mouse_y, int offset_x, int offset_y) override;
        void OnWindowResize(int width, int height) override;
		void RenderScene();

		void CustomRenderMesh(const ::Mesh *mesh, const ::Shader *shader, const glm::mat4 & model_matrix, const glm::vec3 & color, const int hp) const;
		
	};
}