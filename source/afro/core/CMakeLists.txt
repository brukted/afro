target_sources(
        afro
        PUBLIC
        context.h
        data.h
        function_graph.h
        material_graph.h
        graph_common.h
        image_texture.h
        property.h packed_file.h
        uuid.h
        folder.h
        material_nodes.h
        curve.h
        material_engine.h
        material_processor.h
        material_node_definition.h
        engine_utils.h)

add_subdirectory(intern)