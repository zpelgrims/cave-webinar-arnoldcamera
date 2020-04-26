A bit about me

Professionally I'm a surfacing lead. But I'm also a big camera nerd, specifically like to shoot analog medium format. For the past 5 years I've been writing camera shaders on the side.

3 sections to model:
    - chemical sensor response
    - lens
    - physics before the light hits the camera

become pretty good at modeling the physics, but we've ignored the lens


A lot happens inbetween the sensor and the first object hit. I feel like in pathtracers this often gets criminally neglected.
I find it enjoyable working on something that could eventually elevate everyone's renders to a more realistic level.

we're used to seeing these imperfections in every single photograph we see.

There's an exciting opportunity to improve the quality of your images by modeling this part of the ray transformation

Any opportunity like this should be grasped today! We're in the age of micro-improvements to the realism of computer graphics, and this is a considerable section that is completely not modeled...


- types of camera shaders
    - thin-lens
    - raytraced naively
    - polynomial optics

We're not going to create anything groundbreaking today, I simply want to give an introduction on how to get started writing this type of shaders.
I often get questions on how to get started with this. I know how overwhelming it can feel to get started with the arnold c++ api as a beginner. 

I'll be skipping technical explanations of c++ concepts to keep this introductory. The goal is to be able to understand the process, after which you can iterate and explore individual concepts in further detail.




- camera shaders are ran at the start of the ray tree
- they are pretty simple shaders fundamentally
- we really only need to set the origin and direction of the ray.
- ofcourse how this is determined can get very complex.





- go to arnold website, download sdk:
https://www.arnoldrenderer.com/arnold/download/


- show the arnold boilerplate

- now, how do we know how to use this?
    - show the arnold api docs: https://docs.arnoldrenderer.com/api/latest
    - show a section of particular use, e.g camera method, shading globals


- show houdini demo of pinhole/thinlens

- write pinhole camera

- write depth of field
    - bit of sampling theory
    - first unit square
    
    then add concentric disk sampling


- always make sure direction vector is normalized, this is assumed



c++ shaders need to be compiled on the platform they will be used on:
    linux -> linux (.so)
    windows -> windows (.dll)



/home/cactus/Arnold-6.0.0.0-linux/bin/kick  tests/head_on_a_platter.ass -dp -l bin/6.0.0.0/


all the code for this can be found at: github page