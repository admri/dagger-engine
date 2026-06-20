function(setup_shader_compilation TARGET_NAME)
    # Find glslc
    find_program(GLSLC glslc)

    # Find shadercross
    find_program(SHADERCROSS 
        NAMES shadercross
        HINTS "${CMAKE_SOURCE_DIR}/tools"
    )

    set(SHADER_SOURCES
        "${CMAKE_SOURCE_DIR}/assets/shaders/lighting.frag"
    )

    set(TARGET_ASSET_DIR "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets/shaders")

    if (GLSLC AND SHADERCROSS)
        message(STATUS "Shader tools found! Setting up auto-compilation for target: ${TARGET_NAME}")

        set(COMPILED_SHADERS "")

        set(OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/compiled_shaders")
        file(MAKE_DIRECTORY ${OUT_DIR})

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
            
            set(SPV_FILE "${OUT_DIR}/${BASE_NAME}.spv")
            set(DXIL_FILE "${OUT_DIR}/${BASE_NAME}.dxil")
            set(MSL_FILE "${OUT_DIR}/${BASE_NAME}.msl")

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

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                "${OUT_DIR}"
                "${TARGET_ASSET_DIR}"
            COMMENT "Copying compiled shaders to output directory..."
        )

    else()
        message(WARNING "Shader tools not found! Using PRECOMPILED shaders...")
        
        set(PRECOMPILED_DIR "${CMAKE_SOURCE_DIR}/assets/shaders/precompiled")
        
        if(NOT EXISTS "${PRECOMPILED_DIR}")
            file(MAKE_DIRECTORY "${PRECOMPILED_DIR}")
        endif()

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                "${PRECOMPILED_DIR}"
                "${TARGET_ASSET_DIR}"
            COMMENT "Copying PRECOMPILED shaders for ${TARGET_NAME}..."
        )
    endif()
endfunction()