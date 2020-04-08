#include <ai.h>
#include "lentil_thinlens.h"

AI_CAMERA_NODE_EXPORT_METHODS(lentil_thinlensMethods)



// Improved concentric mapping code by Dave Cline [peter shirley´s blog]
// maps points on the unit square onto the unit disk uniformly
inline void concentricDiskSample(float ox, float oy, AtVector2 &lens)
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


enum
{
    p_sensor_widthTL,
    p_focal_lengthTL,
    p_fstopTL,
    p_focus_distanceTL
};

struct CameraThinLens
{
    float sensor_width;
    float focal_length;
	float fov;
    float tan_fov;
    float fstop;
    float focus_distance;
    float aperture_radius;
} CameraThinLens tl;


node_parameters
{
    AiParameterFlt("sensor_widthTL", 36.0); // 35mm film
    AiParameterFlt("focal_lengthTL", 35.0); // in mm
    AiParameterFlt("fstopTL", 1.4);
    AiParameterFlt("focus_distanceTL", 100.0); // in cm
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
    CameraThinLens* tl = (CameraThinLens*)AiNodeGetLocalData(node);


    tl->sensor_width = AiNodeGetFlt(node, "sensor_widthTL");
    tl->focal_length = AiNodeGetFlt(node, "focal_lengthTL");
    tl->focal_length = clamp_min(tl->focal_length, 0.01);

    tl->fstop = AiNodeGetFlt(node, "fstopTL");
    tl->fstop = clamp_min(tl->fstop, 0.01);

    tl->focus_distance = AiNodeGetFlt(node, "focus_distanceTL");

    tl->fov = 2.0 * std::atan(tl->sensor_width / (2.0*tl->focal_length));
    tl->tan_fov = std::tan(tl->fov/2.0);
    tl->aperture_radius = (tl->focal_length / (2.0 * tl->fstop)) / 10.0;
}

// cleanup
node_finish
{
    CameraThinLens* tl = (CameraThinLens*)AiNodeGetLocalData(node);
    delete tl;
}



camera_create_ray
{
    CameraThinLens* tl = (CameraThinLens*)AiNodeGetLocalData(node);

    
    float r1 = 0.0, r2 = 0.0;
    AtVector origin (0, 0, 0);
    AtVector dir (0, 0, 0);



    // create point on sensor (camera space)
    const AtVector p(input.sx * (tl->sensor_width*0.5), 
                     input.sy * (tl->sensor_width*0.5), 
                     -tl->focal_length);
        

    // calculate direction vector from origin to point on lens
    AtVector dir_from_center = AiV3Normalize(p); // or norm(p-origin)

    // get uniformly distributed points on the unit disk
    Eigen::Vector2d unit_disk(0, 0);
    concentricDiskSample(input.lensx, input.lensy, unit_disk);
        
    AtVector lens(unit_disk(0) * tl->aperture_radius, unit_disk(1) * tl->aperture_radius, 0.0);
    output.origin = lens;

    const float intersection = std::abs(tl->focus_distance / dir_from_center.z);
    const AtVector focusPoint = dir_from_center * intersection;
    output.dir = AiV3Normalize(focusPoint - lens);

}


// reverse mapping, required for screenspace effects like adaptive subdivision to work.
camera_reverse_ray
{
    const CameraThinLens* tl = (CameraThinLens*)AiNodeGetLocalData(node);

    double coeff = 1.0 / std::max(std::abs(Po.z * tl->tan_fov), 1e-3f);
    Ps.x = Po.x * coeff;
    Ps.y = Po.y * coeff;

    return true;
}

node_loader
{
    if (i != 0) return false;
    node->methods = lentil_thinlensMethods;
    node->output_type = AI_TYPE_UNDEFINED;
    node->name = "lentil_thinlens";
    node->node_type = AI_NODE_CAMERA;
    strcpy(node->version, AI_VERSION);
    return true;
}