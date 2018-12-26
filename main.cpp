#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#include <math.h>


#include <emscripten.h>
#include <emscripten/html5.h>


GLFWwindow* g_window;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImGuiContext* imgui = 0;
bool show_demo_window = true;
bool show_another_window = false;

void draw()
{
    ImGui::SetCurrentContext(imgui);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically
    // appears in a window called "Debug".
    {
        static float f = 0.0f;
        static int counter = 0;
        ImGui::Text(
            "Hello, world!"); // Display some text (you can use a format string too)
        ImGui::SliderFloat("float", &f, 0.0f,
            1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3(
            "clear color",
            (float*)&clear_color); // Edit 3 floats representing a color

        ImGui::Checkbox(
            "Demo Window",
            &show_demo_window); // Edit bools storing our windows open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most
            // widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    // std::cout << "2nd window" << std::endl;

    // 2. Show another simple window. In most cases you will use an explicit
    // Begin/End pair to name your windows.
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // 3. Show the ImGui demo window. Most of the sample code is in
    // ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    if (show_demo_window) {
        ImGui::SetNextWindowPos(
            ImVec2(650, 20),
            ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call
        // this because positions are saved in .ini
        // file anyway. Here we just want to make the
        // demo initial state a bit more friendly!
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    ImGui::Render();

    int display_w, display_h;
    glfwMakeContextCurrent(g_window);
    glfwGetFramebufferSize(g_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(g_window);
}

void input() {}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    ImGui::SetCurrentContext(imgui);
    ImGuiIO& io = ImGui::GetIO();
    // io.WantSetMousePos = true;
    std::cout << io.MousePos.x << ", " << io.MousePos.y << std::endl;
    io.MousePos = ImVec2((float)xpos * 1.f, (float)ypos * 1.f);
    // std::cout << io.MousePos.x << std::endl;
}

void mouse_button_callback(GLFWwindow* window, int button, int action,
    int mods) {}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

void loop()
{
    input();
    draw();
}

void setWindowSize(int width, int height) 
{
    printf("Changing window size to (%d, %d)\n", width, height);
    glfwSetWindowSize(g_window, width, height);
}

int init()
{
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return 1;
    }

    // glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    // Open a window and create its OpenGL context
    g_window = glfwCreateWindow(640, 480, "The Mayor", NULL, NULL);
    if (g_window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_window); // Initialize GLEW

    // Create game objects
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
    // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
    // Gamepad Controls
    std::cout << "CreateContext()" << std::endl;

    ImGui_ImplGlfw_InitForOpenGL(g_window, false);
    ImGui_ImplOpenGL3_Init();

    // Setup style
    // ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    io.Fonts->AddFontFromFileTTF("data/Roboto-Regular.ttf", 13.0f);
    io.Fonts->AddFontDefault();

    imgui = ImGui::GetCurrentContext();

    // Cursor callbacks
    // glfwSetCursorPosCallback(g_window, cursor_pos_callback);
    glfwSetMouseButtonCallback(g_window, ImGui_ImplGlfw_MouseButtonCallback);
    glfwSetScrollCallback(g_window, ImGui_ImplGlfw_ScrollCallback);
    glfwSetKeyCallback(g_window, ImGui_ImplGlfw_KeyCallback);
    glfwSetCharCallback(g_window, ImGui_ImplGlfw_CharCallback);

    return 0;
}

void quit() { glfwTerminate(); }
void qt_set_canvas_size(const char *targetId, double width, double height)
{
    EM_ASM_({
        var canvasId = Module.Pointer_stringify($0);
        var canvas = document.getElementById(canvasId);
        canvas.width = $1;
        canvas.height = $2;
    }, targetId, width, height);
}
EM_BOOL resize_callback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
    {
        std::cout << "emscripten resize callback" << std::endl;

        double window_dpr = EM_ASM_DOUBLE({
            return window.devicePixelRatio;
        });

        // The HTML canvas has two sizes: the CSS size and the canvas render size.
        // The CSS size is determined according to standard CSS rules, while the
        // render size is set using the "width" and "height" attributes. The render
        // size must be set manually and is not auto-updated on CSS size change.
        // Setting the render size to a value larger than the CSS size enables high-dpi
        // rendering.
        //
        // As a special case, the canvas render size will be used as the CSS size
        // if no CSS size is set.
        //
        // For WebGL canvases the final buffer size may be different than the requested
        // render size due to GPU limitations. Use emscripten_webgl_get_drawing_buffer_size
        // to get the actual size.

        // Set the canvas render size, factoring in window.devicePixelRatio.
        // for (int i = 0; i < g_canvasCount; ++i) {
        //     const char *canvasId = g_canvasIds[i];
            const char *canvasId = "canvas";

            double css_width;
            double css_height;
            emscripten_get_element_css_size(canvasId, &css_width, &css_height);

            // FIXME: the css size returned above might in fact be the canvas render
            // size, if no CSS size is set. This means that setting the canvas render
            // size below will feed back into the next read of the CSS size, which is
            // not what we want.

            double pixel_width = css_width * window_dpr;
            double pixel_height = css_height * window_dpr;


            printf("css(%lf, %lf) dpr(%lf)\n", css_width, css_height, window_dpr);

            std::cout << "set size " << pixel_width << " " << pixel_height << std::endl;

        //     // FIXME: the css size returned above might in fact be the canvas render
        //     // size, if no CSS size is set. This means that setting the canvas render
        //     // size below will feed back into the next read of the CSS size, which is
        //     // not what we want.

        //     double pixel_width = css_width * window_dpr;
        //     double pixel_height = css_height * window_dpr;

        //     cout << "set size " << pixel_width << " " << pixel_height << endl;
        //     cout << "new size " << floor(pixel_width) << " " << floor(pixel_height) << endl;
            // qt_set_canvas_size(canvasId, pixel_width, pixel_height);

            setWindowSize( (css_width), floor(css_height)  );
            draw();
        // }

        // for (int i = 0; i < g_canvasCount; ++i) {
        //     render(i);
        // }
        // printContextInfo(g_canvasIds[0], g_contexts[0]);
        return true;
    }

extern "C" 
int main(int argc, char** argv)
{

// #ifdef __EMSCRIPTEN__
    if (init() != 0)
        return 1;
    void *data = nullptr;
    bool capture = true;
    emscripten_set_resize_callback(0, data, capture, resize_callback);
    

    emscripten_set_main_loop(loop, 0, 1);
// #endif

    quit();

    return 0;
}
