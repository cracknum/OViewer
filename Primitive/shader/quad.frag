#version 440 core

// uniform float windowWidth;
// uniform float windowLevel;

// uniform sampler3D texture3D;

in vec3 vTexPos;
out vec4 fragColor;

void main()
{
    // float hu = texture(texture3D, vTexPos).r;
    // // minHU + huNormalized * (maxHU - minHU)
    // hu = clamp((hu - windowLevel) / windowWidth + 0.5f, 0.0f, 1.0f);
    // fragColor = vec4(hu, 0.0f, .0f, 1.0f);
    fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}
