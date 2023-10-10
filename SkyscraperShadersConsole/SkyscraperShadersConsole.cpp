// SkyscraperShadersConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Filesystem.h"
#include "Shaders.h"
#include <assert.h>

// not sure what the point of this is, maybe it's the .h files provided with the shaders?
void AddIncludeFile()
{

}

shaderc_include_result* ResolveInclude(void* user_data, const char* requested_source, int type, const char* requesting_source, size_t include_depth)
{
    char Destination[128] = "./code/";
    strcat_s(Destination, sizeof(Destination), requested_source);

    std::string FileName = GetSourceFileName(Destination);
    std::string FileContents = GetSourceFileText(Destination);

    shaderc_include_result* result = (shaderc_include_result*)malloc(sizeof(shaderc_include_result));
    result->content = FileContents.c_str();
    result->content_length = FileContents.length();
    result->source_name = FileName.c_str();
    result->source_name_length = FileName.length();

    return result;
}

void ReleaseInclude(void* user_data, shaderc_include_result* pInclude)
{
    free(pInclude);
}

shaderc_shader_kind DetermineShaderType(const char* lpacString)
{
    if (strstr(lpacString, "_VERT_"))
    {
        return shaderc_vertex_shader;
    }
    else if (strstr(lpacString, "_FRAG_"))
    {
        return shaderc_fragment_shader;
    }
    else if (strstr(lpacString, "_COMP_"))
    {
        return shaderc_compute_shader;
    }
    else if (strstr(lpacString, "_GEOM_"))
    {
        return shaderc_geometry_shader;
    }
    else if (strstr(lpacString, "_HULL_"))
    {
        return shaderc_tess_control_shader;
    }
    else if (strstr(lpacString, "_DOMN_"))
    {
        return shaderc_tess_evaluation_shader;
    }

    assert("should return shader type?");

    return shaderc_vertex_shader;
}

int main()
{
    const char* filePath = "./code/lightfragment.shader.h";

    shaderc_compiler_t lshaderc_compiler = shaderc_compiler_initialize();
    shaderc_compile_options_t lshaderc_compile_options = shaderc_compile_options_initialize();
    shaderc_compile_options_set_include_callbacks(lshaderc_compile_options, ResolveInclude, ReleaseInclude, 0);
    shaderc_compile_options_set_auto_map_locations(lshaderc_compile_options, true); 
    shaderc_compile_options_set_auto_bind_uniforms(lshaderc_compile_options, true);
    shaderc_compile_options_set_generate_debug_info(lshaderc_compile_options);
    shaderc_compile_options_set_optimization_level(lshaderc_compile_options, shaderc_optimization_level_performance);
    shaderc_compile_options_set_target_env(lshaderc_compile_options, shaderc_target_env_vulkan, 0x400000);

    shaderc_shader_kind lshader_kind = /*DetermineShaderType(filePath);*/ shaderc_fragment_shader;

    std::string FileContent = GetSourceFileText(filePath);
    std::string FileName = GetSourceFileName(filePath);

    shaderc_compilation_result_t result = shaderc_compile_into_spv(lshaderc_compiler, FileContent.c_str(), sizeof(NULL), lshader_kind, FileName.c_str(), "main", lshaderc_compile_options);

    if (shaderc_result_get_num_errors(result))
    {
        assert("shader comp foiled :/");
    }

    std::ofstream Output("./output.spv");

    Output << result;

    Output.close();

    const char* bytes = shaderc_result_get_bytes(result);

    //figure out stuff
    //shaderc_compile_into_spv() 

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
