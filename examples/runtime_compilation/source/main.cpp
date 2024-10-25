#include <3ds.h>
#include <string>
#include <citro3d.h>
#include <sstream>
#include <fstream>
#include <selena/compiler.hpp>

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

typedef struct { float x, y, z; float r, g, b; } vertex;

static const vertex vertices[] = 
{
    {200.f, 200.f, 0.5f, 1.f, 0.f, 0.f},
    {100.f, 40.f,  0.5f, 0.f, 1.f, 0.f},
    {300.f, 40.f,  0.5f, 0.f, 0.f, 1.f},
};

#define vertex_list_count (sizeof(vertices)/sizeof(vertices[0]))

static SelenaInfo info;
static DVLB_s* vshader_dvlb;
static shaderProgram_s program;
static int uLoc_projection;
static int uLoc_modelView;
static C3D_Mtx projection;

static void* vbo_data;

static std::string loadShaderFromFile(const char* path) 
{
    std::ifstream in = std::ifstream(std::string(path));
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

static void sceneInit(void)
{
    // Compile the vertex shader
    std::string source = loadShaderFromFile("romfs:/vshader.vsh");
    //SelenaSetErrorHandler(errorHandler);
    info = SelenaCompileShaderSource(source);
    if(!info.hasCompiled) {
        for(std::string& s : info.errors)
            printf("%s", s.c_str());
       printf("Failed to compile with %d errors.", info.errors.size());
    }

	// Load the vertex shader, create a shader program and bind it
	vshader_dvlb = DVLB_ParseFile(info.shbinData.get(), info.shbinSize);
	shaderProgramInit(&program);
	shaderProgramSetVsh(&program, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&program);

	// Get the location of the uniforms
	uLoc_projection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");
	uLoc_modelView  = shaderInstanceGetUniformLocation(program.vertexShader, "modelView");

	// Configure attributes for use with the vertex shader
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1=color

	// Compute the projection matrix
	Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);

	// Create the VBO (vertex buffer object)
	vbo_data = linearAlloc(sizeof(vertices));
	memcpy(vbo_data, vertices, sizeof(vertices));

	// Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, vbo_data, sizeof(vertex), 1, 0x0);

	// Configure the first fragment shading substage to just pass through the vertex color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

static void sceneRender(void)
{
	// Calculate the modelView matrix
    C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0, 0, 0, true);
	Mtx_Scale(&modelView, 1, 1, 1);

	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView,  &modelView);

	// Draw the VBO
	C3D_DrawArrays(GPU_TRIANGLES, 0, vertex_list_count);
}

static void sceneExit(void)
{
	// Free the VBO
	linearFree(vbo_data);

	// Free the shader program
	shaderProgramFree(&program);
	DVLB_Free(vshader_dvlb);
}

int main() 
{
    // Initialize graphics
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

    // Initialize the render target
    C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    // Initialize the scene
	sceneInit();

    // Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(target);
			sceneRender();
		C3D_FrameEnd(0);
	}

    // Deinitialize the scene
	sceneExit();

    // Deinitialize graphics
    C3D_Fini();
    gfxExit();
    return 0;
}