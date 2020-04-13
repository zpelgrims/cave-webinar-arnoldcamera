// todo: figure out why i can't get rid of /10 if the units are the same


A bit about me

Professionally I'm a surfacing lead. But I'm also a big camera nerd, specifically like to shoot analog medium format. For the past 5 years I've been writing camera shaders on the side.
A lot happens inbetween the sensor and the first object hit. I feel like in pathtracers this often gets criminally neglected.
I find it enjoyable working on something that could eventually elevate everyone's renders to a more realistic level.

We're not going to create anything groundbreaking today, I simply want to give an introduction on how to get started writing this type of shaders.
I often get questions on how to get started with this. I know how overwhelming it can feel to get started with the arnold c++ api as a beginner. 



- camera shaders are ran at the start of the ray tree
- they are pretty simple shaders fundamentally
- we really only need to set the origin and direction of the ray.
- ofcourse how this is determined can get very complex.



- go to arnold website, download sdk:
https://www.arnoldrenderer.com/arnold/download/


- show the arnold boilerplate

- now, how do we know how to use this?
    - show the arnold api docs

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