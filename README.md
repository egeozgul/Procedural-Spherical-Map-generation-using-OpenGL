# OpenGL-based Procedural Spherical Map Generation

A solution for high-resolution, procedural 3D spherical surface rendering, augmented with noise modulation.

---

## High-Resolution Surface Construction Visualization

<p align="center">
  <img src="patchDemo.gif" width="700" height="500" alt="High-resolution procedural surface construction"/>
</p>

---

## Surface Rendering alongside a Reference Sphere

<p align="center">
  <img src="video.gif" width="700" height="500" alt="Surface construction with reference sphere"/>
</p>

---

## Project Overview

Utilizing OpenGL, this application is engineered to procedurally fabricate the surface of an arbitrary 3D spheroid.

### Surface Composition:

- The primary structure of the 3D spheroid surface consists of extensive triangular segments.
- Each segment undergoes a meticulous subdivision process, resulting in finer triangular constituents.
- Through the application of Perlin noise, each subdivided triangle acquires a pseudorandom texture, enabling the formation of intricate terrains and elevations.

### Memory and Storage Management:

- For efficient data retrieval and storage, each triangular segment's coordinates are hashed, generating a unique key within a dedicated hash table.
- To ensure optimal memory utilization, only those triangular segments proximate to the camera view are retained. Those segments distant from the focal point are systematically discarded.
- Anticipating extensive planetary scales, such a mechanism negates the need to redundantly store surface data from the unseen portions of the sphere, thereby conserving RAM.
- To further elevate efficiency, discarded triangles undergo a recycling process, ensuring their utility in subsequent renderings.

### Distinctive Features:

This intricate design allows for the generation of infinitely expansive, procedurally curated planetary surfaces. Importantly, memory consumption remains invariant, irrespective of the planet's magnitude, ensuring seamless performance.

