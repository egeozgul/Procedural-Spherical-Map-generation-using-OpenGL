# OpenGL-based Procedural Spherical Map Generation

A solution for high-resolution, procedural 3D spherical surface rendering, enhanced with noise modulation.

---

## High-Resolution Surface Construction Visualization

<p align="center">
  <img src="patchDemo.gif" width="700" height="500" alt="High-resolution procedural surface construction"/>
</p>

---

## Surface Rendering with a Reference Sphere

<p align="center">
  <img src="video.gif" width="700" height="500" alt="Surface construction with reference sphere"/>
</p>

---

## Project Overview

This OpenGL-based application generates procedural surfaces for 3D spheroids. 

### Surface Composition:
- The surface is constructed using large triangular segments.
- Each segment is subdivided into smaller triangles for finer detail.
- **Perlin noise** is applied to create pseudorandom textures, enabling the generation of complex terrains and elevations.

### Memory and Storage Management:
- Each triangle's coordinates are hashed and stored in a hash table for efficient data retrieval.
- Only triangles near the camera are retained in memory; distant triangles are discarded to optimize RAM usage.
- Discarded triangles are recycled for future use, ensuring efficient memory management.
- This approach allows for the generation of infinitely large planetary surfaces without increasing memory usage.

### Key Features:
- Generates infinitely large, procedurally generated planetary surfaces.
- Memory usage remains constant, regardless of the planet's size, ensuring smooth performance.
