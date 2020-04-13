#include <ai.h>

AI_CAMERA_NODE_EXPORT_METHODS(cave_cameraMethods)


// Improved concentric mapping code by Dave Cline [peter shirley´s blog]
// maps points on the unit square onto the unit disk uniformly
inline void concentric_disk_sample(float ox, float oy, AtVector2 &lens)
{
    if (ox == 0.0 && oy == 0.0){
        lens.x = 0.0;
        lens.y = 0.0;
        return;
    }

    float phi, r;

    // switch coordinate space from [0, 1] to [-1, 1]
    const float a = 2.0 * ox - 1.0;
    const float b = 2.0 * oy - 1.0;

    if ((a * a) > (b * b)){
        r = a;
        phi = 0.78539816339 * (b / a);
    }
    else {
        r = b;
        phi = (AI_PIOVER2) - ((0.78539816339) * (a / b));
    }

    lens.x = r * std::cos(phi);
    lens.y = r * std::sin(phi);
}


struct CameraThinLens
{
    float sensor_width;
    float focal_length;
	float fov;
    float tan_fov;
    float fstop;
    float focus_distance;
    float aperture_radius;
} data;


node_parameters
{
    AiParameterFlt("sensor_width", 36.0); // 35mm film
    AiParameterFlt("focal_length", 35.0); // in mm
    AiParameterFlt("fstop", 1.4);
    AiParameterFlt("focus_distance", 100.0); // in cm
}


// constructor, ran when the node gets created.
// Usually only used to setup the data structures.
node_initialize
{
    AiCameraInitialize(node);
    AiNodeSetLocalData(node, new CameraThinLens());
}

// this sections gets ran every time a node gets updated.
// for example, each time the IPR refreshes.
// important: single threaded!
node_update
{
    AiCameraUpdate(node, false);
    CameraThinLens* data = (CameraThinLens*)AiNodeGetLocalData(node);


    data->sensor_width = AiNodeGetFlt(node, "sensor_width");
    data->focal_length = AiNodeGetFlt(node, "focal_length");
    data->fstop = AiNodeGetFlt(node, "fstop");
    data->focus_distance = AiNodeGetFlt(node, "focus_distance");


    data->aperture_radius = (data->focal_length / (2.0 * data->fstop)) / 10.0;


    // camera reverse
    // data->fov = 2.0 * std::atan(data->sensor_width / (2.0*data->focal_length));
    // data->tan_fov = std::tan(data->fov/2.0);
}


// cleanup
node_finish
{
    CameraThinLens* data = (CameraThinLens*)AiNodeGetLocalData(node);
    delete data;
}



camera_create_ray
{
    CameraThinLens* data = (CameraThinLens*)AiNodeGetLocalData(node);

    // create point on sensor (camera space)
    const AtVector p(input.sx * (data->sensor_width*0.5), // scale by half the sensor width because of [-1, 1] domain of input.sx
                     input.sy * (data->sensor_width*0.5), 
                     -data->focal_length);
        

    // get uniformly distributed points on the unit disk
    AtVector2 unit_disk(0, 0);
    concentric_disk_sample(input.lensx, input.lensy, unit_disk);
        
    AtVector position_on_lens(unit_disk.x * data->aperture_radius, unit_disk.y * data->aperture_radius, 0.0);
    output.origin = position_on_lens;

// then we need to compute where the point would be on the plane of focus

    // how many times does the focal length fit in the focus distance
    float diff = data->focus_distance/data->focal_length;
    
    // point on focus plane, possible because vector is of unit length (1)
    AtVector focusPoint = p * diff;
    
    output.dir = AiV3Normalize(focusPoint - position_on_lens);
}


// reverse mapping, required for screenspace effects like adaptive subdivision to work.
camera_reverse_ray
{
    // const CameraThinLens* data = (CameraThinLens*)AiNodeGetLocalData(node);

    // double coeff = 1.0 / std::max(std::abs(Po.z * data->tan_fov), 1e-3f);
    // Ps.x = Po.x * coeff;
    // Ps.y = Po.y * coeff;

    // return true;
}

node_loader
{
    if (i != 0) return false;
    node->methods = cave_cameraMethods;
    node->output_type = AI_TYPE_UNDEFINED;
    node->name = "cave_camera";
    node->node_type = AI_NODE_CAMERA;
    strcpy(node->version, AI_VERSION);
    return true;
}