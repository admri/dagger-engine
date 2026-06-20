function(setup_shader_compilation TARGET_NAME)
    # Find glslc
    find_program(GLSLC glslc)

    # Find shadercross
    find_program(SHADERCROSS 
        NAMES shadercross
        HINTS "${CMAKE_SOURCE_DIR}/tools"
    )

    if (GLSLC AND SHADERCROSS)
        message(STATUS "Shader tools found! Setting up auto-compilation for target: ${TARGET_NAME}")
        
        set(SHADER_SOURCES
            "${CMAKE_SOURCE_DIR}/assets/shaders/lighting.frag"
        )

        set(COMPILED_SHADERS "")

        # Compile each shader
        foreach(SHADER_FILE ${SHADER_SOURCES})
            get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
            get_filename_component(BASE_NAME ${SHADER_FILE} NAME_WE)

            # Detect shader stage
            if(FILE_NAME MATCHES "\\.frag$")
                set(SHADER_STAGE "fragment")
            elseif(FILE_NAME MATCHES "\\.vert$")
                set(SHADER_STAGE "vertex")
            elseif(FILE_NAME MATCHES "\\.comp$")
                set(SHADER_STAGE "compute")
            else()
                set(SHADER_STAGE "fragment") # Fallback
            endif()
            
            set(SPV_FILE "${CMAKE_SOURCE_DIR}/assets/shaders/${BASE_NAME}.spv")
            set(DXIL_FILE "${CMAKE_SOURCE_DIR}/assets/shaders/${BASE_NAME}.dxil")
            set(MSL_FILE "${CMAKE_SOURCE_DIR}/assets/shaders/${BASE_NAME}.msl")

            # Compile GLSL shader for Vulkan (SPIR-V) with glslc
            add_custom_command(
                OUTPUT ${SPV_FILE}
                COMMAND ${GLSLC} ${SHADER_FILE} -o ${SPV_FILE}
                DEPENDS ${SHADER_FILE}
                COMMENT "Compiling ${FILE_NAME} to SPIR-V..."
            )

            # Compile SPIR-V shader for DirectX (DXIL) with shadercross
            add_custom_command(
                OUTPUT ${DXIL_FILE}
                COMMAND ${SHADERCROSS} ${SPV_FILE} -o ${DXIL_FILE} -t ${SHADER_STAGE}
                DEPENDS ${SPV_FILE}
                COMMENT "Compiling ${BASE_NAME}.spv to DXIL..."
            )

            # Compile SPIR-V shader for Metal (MSL) with shadercross
            add_custom_command(
                OUTPUT ${MSL_FILE}
                COMMAND ${SHADERCROSS} ${SPV_FILE} -o ${MSL_FILE} -t ${SHADER_STAGE}
                DEPENDS ${SPV_FILE}
                COMMENT "Compiling ${BASE_NAME}.spv to MSL..."
            )

            list(APPEND COMPILED_SHADERS ${SPV_FILE} ${DXIL_FILE} ${MSL_FILE})
        endforeach()

        set(SHADER_TARGET "CompileShaders_${TARGET_NAME}")
        add_custom_target(${SHADER_TARGET} ALL DEPENDS ${COMPILED_SHADERS})
        
        add_dependencies(${TARGET_NAME} ${SHADER_TARGET})

    else()
        message(WARNING "Shader tools not found. Shaders will not be rebuilt. (GLSLC: ${GLSLC}, SHADERCROSS: ${SHADERCROSS})")
    endif()
endfunction()