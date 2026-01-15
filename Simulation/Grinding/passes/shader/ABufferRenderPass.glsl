#version 440 core

struct ABufferNode
{
  vec4 color;
  vec4 position;
  unsigned int next;
};

layout(std430, binding = 0) buffer ABufferStorage
{
  ABufferNode nodes[];
}

layout(binding = 0, offset = 0) uniform atomic_uint nodeCounter;
layout(r32ui, binding = 0) uniform uimage2D headPointerImage;

uniform uint maxLayer;
uniform uint maxNodes;

int main()
{
  vec2 pos = vec2(gl_FragCoord.xy);
  uint newNodeIndex = atomicCounterIncrement(nodeCounter);
  if (newNodeIndex >= maxNodes)
  {
    return;
  }

  nodes[newNodeIndex].color = fragOutput0;
  nodes[newNodeIndex].position = gl_FragCoord;
  
  // insert new node in head
  uint prevNodeIndex = imageAtomicExchange(headPointerImage, pos, newNodeIndex);
  nodes[newNodeIndex].next = prevNodeIndex;

}