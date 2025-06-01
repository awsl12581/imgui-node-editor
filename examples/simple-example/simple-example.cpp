# include <imgui.h>
# include <imgui_node_editor.h>
# include <application.h>
#include "implot.h"
#include "implot3d.h"

namespace ed = ax::NodeEditor;

struct Example:
    public Application
{
    using Application::Application;

    void OnStart() override
    {
        ed::Config config;
        config.SettingsFile = "Simple.json";
        m_Context = ed::CreateEditor(&config);
        ImPlot::CreateContext();
        ImPlot3D::CreateContext();
    }

    void OnStop() override
    {
        ed::DestroyEditor(m_Context);
        ImPlot::DestroyContext();
        ImPlot3D::DestroyContext();
    }

    void OnFrame(float deltaTime) override
    {
        auto& io = ImGui::GetIO();

                // Demo windows
        ImGui::ShowDemoWindow();
        ImPlot::ShowDemoWindow();
        ImPlot3D::ShowDemoWindow();


        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();

        ed::SetCurrentEditor(m_Context);
        ed::Begin("My Editor", ImVec2(0.0, 0.0f));
        int uniqueId = 1;
        // Start drawing nodes.
        ed::BeginNode(uniqueId++);
            ImGui::Text("Node A");
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
                ImGui::Text("-> In");
            ed::EndPin();
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
                ImGui::Text("Out ->");
            ed::EndPin();
        ed::EndNode();

        static ed::NodeId contextNodeId; // 必须是函数作用域内的变量
        static bool show_node_panel = false;
        static ed::NodeId current_node_id;
        static bool openContextMenu = false;
        // 检测是否在节点上点击了右键
        if (ed::ShowNodeContextMenu(&contextNodeId))
        {
            openContextMenu = true;  // 标志位，仅设置，不弹出！
        }

        ed::End();

        if (openContextMenu)
        {
            ImGui::OpenPopup("Node Context Menu");
            openContextMenu = false;
        }

        // 弹出菜单
        if (ImGui::BeginPopup("Node Context Menu"))
        {
            ImGui::Text("Node ID: %d", contextNodeId.Get());
            if (ImGui::MenuItem("Show Node"))
            {
                                // 处理编辑逻辑
                current_node_id = contextNodeId;
                show_node_panel = true;
            }
            if (ImGui::MenuItem("Edit Properties"))
            {

            }
            ImGui::EndPopup();
        }

        if (show_node_panel)
        {
            ImGui::Begin("Node Info Panel", &show_node_panel); // 传入指针支持关闭按钮
            ImGui::Text("Node ID: %d", current_node_id.Get());
            
            // 可以显示更多信息
            ImGui::Text("Type: Custom Node");
            ImGui::Text("Position: (x, y)");

            ImGui::End(); // 支持关闭
        }


        
        ed::SetCurrentEditor(nullptr);



	    //ImGui::ShowMetricsWindow();
    }

    ed::EditorContext* m_Context = nullptr;
};

int Main(int argc, char** argv)
{
    Example exampe("Simple", argc, argv);

    if (exampe.Create())
        return exampe.Run();

    return 0;
}