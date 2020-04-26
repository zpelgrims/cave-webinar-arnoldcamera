#include <ai.h>

AI_CAMERA_NODE_EXPORT_METHODS(cave_cameraMethods)


struct DataContainer
{
    float some_internal_variable;
} data;


node_parameters
{
    AiParameterFlt("parameter_name", 1.0);
}


// ran when the node gets created.
// Usually only used to setup the data structures.
node_initialize
{
    AiCameraInitialize(node);
    AiNodeSetLocalData(node, new DataContainer());
}

// this sections gets ran every time a node gets updated.
// for example, each time the IPR refreshes.
// important: single threaded!
node_update
{
    DataContainer* data = (DataContainer*)AiNodeGetLocalData(node);
}


// cleanup
node_finish
{
    DataContainer* data = (DataContainer*)AiNodeGetLocalData(node);
    delete data;
}



camera_create_ray
{
    DataContainer* data = (DataContainer*)AiNodeGetLocalData(node);
}


// reverse mapping, required for screenspace effects like adaptive subdivision to work.
camera_reverse_ray
{
    const DataContainer* data = (DataContainer*)AiNodeGetLocalData(node);
}

node_loader
{
    if (i != 0) return false;
    node->methods = cave_cameraMethods;
    node->output_type = AI_TYPE_UNDEFINED;
    node->name = "camera_boilerplate_example";
    node->node_type = AI_NODE_CAMERA;
    strcpy(node->version, AI_VERSION);
    return true;
}