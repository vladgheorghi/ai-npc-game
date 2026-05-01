#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
// TODO (student): Add the new attributes neccessary for skinning
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

const int MAX_BONES = 200;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
// TODO (student): Declare a new uniform variable array, which will
// receive the bone transformations
uniform mat4 Bones[MAX_BONES];

// Output
out vec2 texture_coord;
// TODO (student): Send the normal to the fragment shader
out vec3 normal;

void main()
{
    // TODO (student): Compute the final bone transformation
    mat4 boneTransform = Bones[BoneIDs[0]] * Weights[0]
                       + Bones[BoneIDs[1]] * Weights[1]
                       + Bones[BoneIDs[2]] * Weights[2]
                       + Bones[BoneIDs[3]] * Weights[3];

    // ====== DEBUG TOGGLES (uncomment ONE at a time) ======

    // (A) Bind pose only — bypass skinning entirely.
    //     If characters look like a clean T-pose standing up, the mesh
    //     geometry is fine and the bug is in bone matrices/animation.
    //     If they still look wrong, the geometry/vertex data itself is bad.
    boneTransform = mat4(1.0);

    // (B) Show bone weight distribution as RGB.
    //     Smooth color gradients = healthy 4-bone skinning.
    //     Sharp patches / pure red-green-blue = effectively 1-bone skinning.
    //     Black regions = vertices with NO bone weights (broken).
    // debug_color = vec3(Weights[0], Weights[1], Weights[2] + Weights[3]);

    // (C) Show whether weights sum to 1 (gray = 1.0, dark = under-weighted).
    // float wsum = Weights[0] + Weights[1] + Weights[2] + Weights[3];
    // debug_color = vec3(wsum);

    // (D) Show dominant bone ID as a hashed color.
    // int id = BoneIDs[0];
    // debug_color = vec3(float((id*37)%255), float((id*73)%255), float((id*113)%255)) / 255.0;

    // ====== END DEBUG ======

    texture_coord = v_texture_coord;
    // TODO (student): Compute the normal
    normal = mat3(boneTransform) * v_normal;

    // TODO (student): Apply the bone transformation on the vertex position
    gl_Position = Projection * View * Model * boneTransform * vec4(v_position, 1.0);
}
